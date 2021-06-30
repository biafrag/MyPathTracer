#include "trianglemesh.h"
#include <QGLWidget>

/**
 * @brief Triangle Mesh
 *
 * The Triangle Mesh class implements a mesh composed of triangles.
 *
 * @author Bianca Fragoso
 */

TriangleMesh::TriangleMesh(std::vector<QVector3D> &points, std::vector<unsigned int> &indices, std::vector<QVector3D> &normals)
{
    _type = ObjectType::MESH;
    _points = points;
    _indices = indices;
    _normals = normals;
    normalizeNormals();
}



void TriangleMesh::initialize()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0,0,0,1);

    //Create shader program.
    _program = new QOpenGLShaderProgram();
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexShader.glsl");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentShader.glsl");


    //Try to link the program.
    _program->link();

    createBuffers();
    createVAO();

    if(_material.hasTexture())
    {
        createTexture();
        updateTexBuffer();
    }
}

template<class T> void setUniformArrayValue(QOpenGLShaderProgram *program,
                                       const QString& arrayName,
                                       const QString& varName,
                                       int index,
                                       const T& value)
{
    QString firstName = QString("%1[%2].%3")
            .arg(arrayName)
            .arg(index)
            .arg(varName);
    std::string nameStd = firstName.toStdString();
    const char* name = nameStd.c_str();
    program->setUniformValue(name, value);
}

void TriangleMesh::render(const QMatrix4x4 &projMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights)
{
    _program->bind();
    _vao.bind();
    updateVertexBuffer();
    updateTexBuffer();

    for (unsigned int i = 0; i < lights.size(); i++)
    {
        setUniformArrayValue<QVector3D>(_program, "lights", "position", i, viewMatrix * lights[i].position);
        setUniformArrayValue<QVector3D>(_program, "lights", "ambient", i, lights[i].ambient);
        setUniformArrayValue<QVector3D>(_program, "lights", "diffuse", i, lights[i].diffuse);
        setUniformArrayValue<QVector3D>(_program, "lights", "specular", i, lights[i].specular);
        setUniformArrayValue<float>(_program, "lights", "shininess", i, lights[i].shi);
    }

    _program->setUniformValue("material.color", _material.getAlbedo());


    QMatrix4x4 mvp = projMatrix * viewMatrix * modelMatrix;
    QMatrix4x4 mv = viewMatrix * modelMatrix;

    _program->setUniformValue("mvp", mvp);
    _program->setUniformValue("mv_ti", mv.inverted().transposed());
    _program->setUniformValue("mv", mv);

    if(_material.hasTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textureID);
        int location = glGetUniformLocation(_program->programId(), "sampler");
        glUniform1i(location, 0);
        _program->setUniformValue("hasTexture", 1);
    }
    else
    {
        _program->setUniformValue("hasTexture", 0);

    }

    glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, 0);

    _vao.release();
}



void TriangleMesh::createBuffers()
{
    glGenBuffers(1, &_pointsBuffer);
    glGenBuffers(1, &_normalsBuffer);
    glGenBuffers(1, &_texCoordsBuffer);
    glGenBuffers(1, &_indicesBuffer);

    updateVertexBuffer();
    updateTexBuffer();
}



void TriangleMesh::updateVertexBuffer()
{
    //Transfer new data to buffer.
    int size = static_cast<int>(_points.size());
    int numberOfBytes = size * static_cast<int>(sizeof(QVector3D));

    glBindBuffer(GL_ARRAY_BUFFER, _pointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _points.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _normals.data(), GL_STATIC_DRAW);

    size = static_cast<int>(_indices.size());
    numberOfBytes = size * static_cast<int>(sizeof(unsigned int));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfBytes, _indices.data(), GL_STATIC_DRAW);

    _texCoords.resize(_points.size());
}



void TriangleMesh::updateTexBuffer()
{
    int size = static_cast<int>(_texCoords.size());
    int numberOfBytes = size * static_cast<int>(sizeof(QVector2D));

    glBindBuffer(GL_ARRAY_BUFFER, _texCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _texCoords.data(), GL_STATIC_DRAW);
}



bool TriangleMesh::triangleVerification(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m)
{
    QVector3D p1 = m * _points[_indices[trianglefirstIndexOfIndice]];
    QVector3D p2 = m * _points[_indices[trianglefirstIndexOfIndice + 1]];
    QVector3D p3 = m * _points[_indices[trianglefirstIndexOfIndice + 2]];

    float A1, A2, A3, At;
    float alfa1, alfa2, alfa3;

    QVector3D normal = QVector3D::crossProduct(p3 - p1, p2 - p1);
    A1 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p1, p2 - p1))/2.0;
    A2 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p2, p3 - p2))/2.0;
    A3 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p3, p1 - p3))/2.0;
    At = QVector3D::dotProduct(normal, QVector3D::crossProduct(p2 - p3, p1 - p3))/2.0;
    alfa3 = A1/At;
    alfa1 = A2/At;
    alfa2 = A3/At;

    //Para estar dentro as coordenadas baricêntricas precisam estar entre 0 e 1
    if(alfa1 < 0 || alfa1 > 1 || alfa2 < 0 || alfa2 > 1 || alfa3 < 0 || alfa3 > 1 )
    {
        return false;
    }
    return true;
}



QVector3D TriangleMesh::getTriangleNormal(unsigned int trianglefirstIndexOfIndice, QMatrix4x4 m)
{
    int i1 = _indices[trianglefirstIndexOfIndice];
    int i2 = _indices[trianglefirstIndexOfIndice + 1];
    int i3 = _indices[trianglefirstIndexOfIndice + 2];

    //Vetores formados pelos pontos do triângulo
    QVector3D e2 = m * _points[i3] - m * _points[i2];
    QVector3D e3 = m * _points[i1] - m * _points[i3];

    //Normal ao triângulo
    return QVector3D::crossProduct(e2, e3).normalized();
}



QVector3D TriangleMesh::getTriangleBaricentricCoordinates(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m)
{
    QVector3D p1 = m * _points[_indices[trianglefirstIndexOfIndice]];
    QVector3D p2 = m * _points[_indices[trianglefirstIndexOfIndice + 1]];
    QVector3D p3 = m * _points[_indices[trianglefirstIndexOfIndice + 2]];

    float A1, A2, A3, At;
    float alfa1, alfa2, alfa3;

    QVector3D normal = QVector3D::crossProduct(p3 - p1, p2 - p1);
    A1 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p1, p2 - p1))/2.0;
    A2 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p2, p3 - p2))/2.0;
    A3 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p3, p1 - p3))/2.0;
    At = QVector3D::dotProduct(normal, QVector3D::crossProduct(p2 - p3, p1 - p3))/2.0;

    alfa3 = A1/At;
    alfa1 = A2/At;
    alfa2 = A3/At;

    return QVector3D(alfa1, alfa2, alfa3);
}



QVector3D TriangleMesh::getNormalInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m)
{
    unsigned int ind1, ind2, ind3;
    ind1 = _indices[trianglefirstIndexOfIndice];
    ind2 = _indices[trianglefirstIndexOfIndice + 1];
    ind3 = _indices[trianglefirstIndexOfIndice + 2];

    QMatrix4x4 normalMatrix = m.transposed().inverted();
    QVector3D alfas= getTriangleBaricentricCoordinates(trianglefirstIndexOfIndice, point, m);
    QVector3D normal = alfas.x() * (normalMatrix *_normals[ind1]) + alfas.y() * (normalMatrix * _normals[ind2]) + alfas.z() * (normalMatrix * _normals[ind3]);
    return normal;
}



QVector3D TriangleMesh::getTexCoordinatesInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m)
{
    QVector3D texInt;
    if(_material.hasTexture())
    {
        unsigned int i1 = _indices[trianglefirstIndexOfIndice];
        unsigned int i2 = _indices[trianglefirstIndexOfIndice + 1];
        unsigned int i3 = _indices[trianglefirstIndexOfIndice + 2];

        QVector3D alfas= getTriangleBaricentricCoordinates(trianglefirstIndexOfIndice, point, m);

        texInt = alfas.x() * _texCoords[i1] + alfas.y() * _texCoords[i2] + alfas.z() * _texCoords[i3];
    }
    return texInt;
}



int TriangleMesh::intersectsWithRay(Ray ray, QMatrix4x4 m, float tCloser, int &firstIndTri)
{
    firstIndTri = -1;
    for(unsigned int i = 0; i < _indices.size() - 2; i = i + 3)
    {
        int i1 = _indices[i];

        //Normal ao triângulo
        QVector3D normal = getTriangleNormal(i, m);

        //Produto interno entre vetor do raio e a normal não pode ser 0 senão quer dizer que eles são perpendiculares
        // Ou seja, a superfície e o raio são paralelos então o raio nunca atinge aquele lugar da superfície

        float prodDN = QVector3D::dotProduct(ray.direction, normal);
        if(prodDN != 0)
        {
            //t é a coordenada paramétrica do ponto no raio
            float t = (QVector3D::dotProduct(m * _points[i1] - ray.origin, normal))/prodDN;

            //p é o ponto que raio atinge na superfície
            QVector3D p = ray.hit(t);

            //Verificar se ponto que raio atinge intersecta o triângulo
            bool verif = triangleVerification(i, p, m);

            if(verif && t < tCloser && t>0)
            {
                tCloser = t;
                firstIndTri = i;
            }
        }

    }
    return tCloser;
}



bool TriangleMesh::isRayIntersecting(Ray ray, QMatrix4x4 m)
{
    for(unsigned int i = 0; i < _indices.size() - 2; i = i + 3)
    {
        int i1 = _indices[i];

        //Normal ao triângulo
        QVector3D normal = getTriangleNormal(i, m);

        //Produto interno entre vetor do raio e a normal não pode ser 0 senão quer dizer que eles são perpendiculares
        // Ou seja, a superfície e o raio são paralelos então o raio nunca atinge aquele lugar da superfície

        float prodDN = QVector3D::dotProduct(ray.direction, normal);
        if(prodDN != 0)
        {
            QVector3D pointWorld = m * _points[i1];
            //t é a coordenada paramétrica do ponto no raio
            float t = (QVector3D::dotProduct(pointWorld - ray.origin, normal))/prodDN;

            if(t > 0 && t < 1)
            {
                //p é o ponto que raio atinge na superfície
                QVector3D p = ray.hit(t);

                //Verificar se ponto que raio atinge intersecta o triângulo
                bool verif = triangleVerification(i, p, m);

                if(verif)
                {
                    return true;
                }
            }
        }
    }
    return false;
}



void TriangleMesh::computeNormals()
{
    _normals.resize(_points.size(), QVector3D(0, 0, 0));
    for (unsigned int t = 0; t < _indices.size() / 3; t++)
    {
       //get the triangle vertices
       unsigned int v0 = _indices[3 * t + 0];
       unsigned int v1 = _indices[3 * t + 1];
       unsigned int v2 = _indices[3 * t + 2];
       const QVector3D &p0 = _points[v0];
       const QVector3D &p1 = _points[v1];
       const QVector3D &p2 = _points[v2];
       QVector3D n = QVector3D::crossProduct(p2 - p0, p1 - p0);
       _normals[v0] += n;
       _normals[v1] += n;
       _normals[v2] += n;
    }
    //Normalize each normal
    for (auto &normal : _normals)
    {
       normal = normal.normalized();
    }
}



void TriangleMesh::normalizeNormals()
{
    for (auto &normal : _normals)
    {
       normal = normal.normalized();
    }

}



void TriangleMesh::createTexture()
{
    //Criar a textura
    glGenTextures(1, &_textureID);

    //Linkar (bind) a textura criada
    glBindTexture(GL_TEXTURE_2D, _textureID);

    //Abrir arquivo de imagem com o Qt
    QImage texImage = QGLWidget::convertToGLFormat(_material.getTexture());

    //Enviar a imagem para o OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA,
                 texImage.width(), texImage.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}



void TriangleMesh::createVAO()
{
    _vao.create();
    _vao.bind();

    //Add vertex.
    glBindBuffer(GL_ARRAY_BUFFER, _pointsBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Add normals.
    glBindBuffer(GL_ARRAY_BUFFER, _normalsBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    //Add tex coords.
    glBindBuffer(GL_ARRAY_BUFFER, _texCoordsBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    //Add elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBuffer);
}
