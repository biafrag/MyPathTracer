#include "sphere.h"
#define M_PI 3.14159265358979323846

int getIndex( int i, int j, int n )
{
    return j + i * ( n + 1 );
}

Sphere::Sphere(QVector3D center, float radius)
    :_center(center),
     _radius(radius)
{
    _type = ObjectType::SPHERE;

    const int n = 30;
    const int m = 30;

    const int numTriangles = 2 * n * m;
    const int numVertices = ( n + 1 ) * ( m + 1 );

    std::vector<QVector3D> points;

    for( unsigned int i = 0; i <= n; i++ )
    {
        for( unsigned int j = 0; j <= m; j++ )
        {
            //Atualizar as coordenadas de textura
            float s = (float) i / n;
            float t = (float) j / m;
            //texCoords.push_back(glm::vec2(s,t));

            //Calcula os parâmetros
            double theta = 2 * s * M_PI;
            double phi = t * M_PI;
            double sinTheta = sin( theta );
            double cosTheta = cos( theta );
            double sinPhi = sin( phi );
            double cosPhi = cos( phi );

            //Calcula os vértices == equacao da esfera
            points.push_back( _radius * QVector3D(cosTheta * sinPhi,
                                          cosPhi,
                                          sinTheta * sinPhi) + _center);
        }
    }

    std::vector<unsigned int> indicesS;
    indicesS.resize(numTriangles*3);

    unsigned int startIndex = _points.size();
    _points = points;

    std::vector<QVector3D> normals;
    normals = points;

    _normals = normals;

    //Preenche o vetor com a triangulação
    unsigned int k = 0;
    for( unsigned int i = 0; i < n; i++ )
    {
        for( unsigned int j = 0; j < m; j++ )
        {
            indicesS[ k++ ] = getIndex( i + 1, j, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i + 1, j + 1, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i, j, n ) + startIndex;


            indicesS[ k++ ] = getIndex( i + 1, j + 1, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i, j + 1, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i, j, n ) + startIndex;
        }
    }

    _indices = indicesS;
}



void Sphere::initialize()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0,0,0,1);

    //Create shader program.
    _program = new QOpenGLShaderProgram();
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexShader.glsl");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentShader.glsl");

    //_program->addShaderFromSourceFile(QOpenGLShader::Compute, ":/shaders/computeShader.glsl");

    //Try to link the program.
    _program->link();

    createBuffers();
    createVAO();
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


void Sphere::render(const QMatrix4x4 &projMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights)
{
    _program->bind();
    _vao.bind();
    updateVertexBuffer();



//    _program->setUniformValue("light.position", viewMatrix*QVector3D(5, -5, 5) );
//    _program->setUniformValue("light.ambient", QVector3D(0.3f, 0.3f, 0.3f));
//    _program->setUniformValue("light.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
//    _program->setUniformValue("light.specular", QVector3D(1.0f,  1.0f,1.0f));
//    _program->setUniformValue("light.shininess", 24.0f);

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
    _program->setUniformValue("hasTexture", static_cast<int>(_material.hasTexture()));

    glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, 0);

    _vao.release();
}



void Sphere::createBuffers()
{
    glGenBuffers(1, &_pointsBuffer);
    glGenBuffers(1, &_normalsBuffer);
    glGenBuffers(1, &_texCoordsBuffer);
    glGenBuffers(1, &_indicesBuffer);

    updateVertexBuffer();
    updateTexBuffer();
}



void Sphere::updateVertexBuffer()
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



void Sphere::updateTexBuffer()
{
    int size = static_cast<int>(_texCoords.size());
    int numberOfBytes = size * static_cast<int>(sizeof(float));

    glBindBuffer(GL_ARRAY_BUFFER, _texCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _texCoords.data(), GL_STATIC_DRAW);
}



float Sphere::intersectsWith(Ray ray, QMatrix4x4 model)
{
    _model = model;
    QVector3D center = model * _center;
    float a = QVector3D::dotProduct(ray.direction, ray.direction);
    float b = 2 * QVector3D::dotProduct(ray.direction, ray.origin - center);
    float c = QVector3D::dotProduct(ray.origin - center, ray.origin - center) - (_radius * _radius);

    float discriminant = b*b - 4*a*c;

    if(discriminant < 0)
    {
        return -1;
    }
    else
    {
        float numerator = - b - sqrt(discriminant);

        return numerator / (2.0 * a);

    }
}



QVector3D Sphere::normalAt(QVector3D point)
{
    return (point - _model *_center).normalized();
}



void Sphere::createVAO()
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
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    //Add elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBuffer);
}
