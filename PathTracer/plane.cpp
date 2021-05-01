#include "plane.h"

Plane::Plane()
{
    _type = ObjectType::PLANE;

    std::vector<QVector3D> points, normals;
    std::vector<unsigned int> indices;
    points = {{-1,1,0}, {1,1,0},
               {-1,-1,0}, {1,-1,0}};
    QMatrix4x4 rot, trans;

    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -2, 0));
    std::vector<QVector4D> points4D;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        points[i] = rot * points[i];
        points[i] = trans * points[i];

    }

    indices = {0, 2, 1, 1, 2, 3};

    normals = {{0, 0, 1}, {0, 0, 1},
                {0, 0, 1}, {0, 0, 1}};

    _points = points;
    _indices = indices;
    _normals = normals;

}



Plane::Plane(QMatrix4x4 translation, QMatrix4x4 rotation, QMatrix4x4 scale)
{
    _type = ObjectType::PLANE;
    std::vector<QVector3D> points, normals;
    std::vector<unsigned int> indices;
    points = {{-1,1,0}, {1,1,0},
               {-1,-1,0}, {1,-1,0}};

    std::vector<QVector4D> points4D;

    normals = {{0, 0, 1}, {0, 0, 1},
                {0, 0, 1}, {0, 0, 1}};

    QMatrix4x4 planeModel = translation * rotation * scale;

    _texCoords = {{0, 1, 0}, {1, 1, 0},
                 {0, 0, 0}, {1, 0, 0}};
    for(unsigned int i = 0; i < points.size(); i++)
    {

        points[i] = planeModel * points[i];
        normals[i] = /*planeModel.inverted().transposed() **/ normals[i];
    }

    indices = {0, 2, 1, 1, 2, 3};

    _points = points;
    _indices = indices;
    _normals = normals;

}



void Plane::initialize()
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
    _isInitialized = true;
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



void Plane::render(const QMatrix4x4 &projMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights)
{
    if(_isInitialized)
    {
        _program->bind();
        _vao.bind();
        updateVertexBuffer();

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
        glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, 0);
        _vao.release();

    }
}



void Plane::createBuffers()
{
    glGenBuffers(1, &_pointsBuffer);
    glGenBuffers(1, &_normalsBuffer);
    glGenBuffers(1, &_texCoordsBuffer);
    glGenBuffers(1, &_indicesBuffer);

    updateVertexBuffer();
    updateTexBuffer();
}



void Plane::updateVertexBuffer()
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
}



void Plane::updateTexBuffer()
{
    int size = static_cast<int>(_texCoords.size());
    int numberOfBytes = size * static_cast<int>(sizeof(float));

    glBindBuffer(GL_ARRAY_BUFFER, _texCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _texCoords.data(), GL_STATIC_DRAW);
}



void Plane::createVAO()
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
