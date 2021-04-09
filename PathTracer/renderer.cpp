#include "renderer.h"
#define M_PI 3.14159265358979323846

Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    _camera.center = QVector3D(0.f,0.f,0.f);
    _camera.eye =  QVector3D(0.f,0.f,10.f);
    _camera.up = QVector3D(0.f,2.f,0.f);
    _camera.zNear = 0.1f;
    _camera.zFar  = 100.f;
    _camera.fov  = 60.f;

    _view.lookAt(_camera.eye, _camera.center, _camera.up);
    _proj.perspective(_camera.fov, width()/height(), _camera.zNear, _camera.zFar);
}



void Renderer::createScene()
{
    unsigned int spheresMax = 100;
    QVector3D sphereRadius(3.0f, 8.0f, 0.0f);
    float spherePlacementRadius = 100.0f;

    //Cria cena boba com esferas
    std::vector<Sphere> spheres;
    for(unsigned int i = 0; i < spheresMax; i++)
    {
        Sphere sphere;

        // Radius and radius
        float rand = (((float) std::rand() / (RAND_MAX)) + 1);
        sphere.radius = sphereRadius.x() + rand * (sphereRadius.y() - sphereRadius.x());

        float randX = (((float) std::rand() / (RAND_MAX)) + 1);
        float randY = (((float) std::rand() / (RAND_MAX)) + 1);

        QVector3D randomPos = QVector3D(randX, randY, 0) * spherePlacementRadius;
        sphere.position = QVector3D(randomPos.x(), sphere.radius, randomPos.y());
        for(auto s : spheres)
        {
            float minDist = sphere.radius + s.radius;

            int randR = 0 + ( std::rand() % ( 255 - 0 + 1 ));
            int randG =  0 + ( std::rand() % ( 255 - 0 + 1 ));
            int randB =  0 + ( std::rand() % ( 255 - 0 + 1 ));

            // Albedo and specular color
            QColor color(randR, randG, randB);
            bool metal = rand < 0.5f;
            //sphere.albedo = metal ? QVector3D(0,0,0) : QVector3D(color.red(), color.green(), color.blue());
            //sphere.specular = metal ? QVector3D(color.red(), color.green(), color.blue()) : QVector3D(0.04f, 0.04f, 0.04f);

            if (std::sqrt((sphere.position - s.position).length()) > minDist * minDist)
            {
                spheres.push_back(sphere);
            }
        }

        // Assign to compute buffer
        if (spheres.size() > 0)
        {
            //Quando eu tiver um Compute Shader
//            _sphereBuffer = new ComputeBuffer(spheres.Count, 40);
//            _sphereBuffer.SetData(spheres);
        }
    }
}



void Renderer::createSimpleScene()
{
    createSphere();
}



void Renderer::createBuffers()
{
    glGenBuffers(1, &_pointsBuffer);
    glGenBuffers(1, &_normalsBuffer);
    glGenBuffers(1, &_texCoordsBuffer);
    glGenBuffers(1, &_indicesBuffer);

    updateVertexBuffer();
    updateTexBuffer();

}



void Renderer::updateVertexBuffer()
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



void Renderer::updateTexBuffer()
{
    int size = static_cast<int>(_texCoords.size());
    int numberOfBytes = size * static_cast<int>(sizeof(float));

    glBindBuffer(GL_ARRAY_BUFFER, _texCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _texCoords.data(), GL_STATIC_DRAW);
}



void Renderer::createVAO()
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


int getIndex( int i, int j, int n )
{
    return j + i * ( n + 1 );
}

void Renderer::createSphere()
{
    const int n = 100;
    const int m = 100;

    const int numTriangles = 2 * n * m;
    const int numVertices = ( n + 1 ) * ( m + 1 );

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
            _points.push_back( QVector3D(cosTheta * sinPhi,
                                          cosPhi,
                                          sinTheta * sinPhi) );
        }
    }

    _normals = _points;

    _indices.resize(numTriangles*3);

    //Preenche o vetor com a triangulação
    unsigned int k = 0;
    for( unsigned int i = 0; i < n; i++ )
    {
        for( unsigned int j = 0; j < m; j++ )
        {
            _indices[ k++ ] = getIndex( i + 1, j, n );
            _indices[ k++ ] = getIndex( i + 1, j + 1, n );
            _indices[ k++ ] = getIndex( i, j, n );


            _indices[ k++ ] = getIndex( i + 1, j + 1, n );
            _indices[ k++ ] = getIndex( i, j + 1, n );
            _indices[ k++ ] = getIndex( i, j, n );
        }
    }
}




void Renderer::initializeGL()
{
    initializeOpenGLFunctions();
    makeCurrent();

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

    createSimpleScene();
    createBuffers();
    createVAO();

}



void Renderer::paintGL()
{
    _program->bind();
    _vao.bind();

    QMatrix4x4 mvp = _proj * _view * _model;
    QMatrix4x4 mv = _view * _model;

    _program->setUniformValue("mvp", mvp);
    _program->setUniformValue("mv_ti", mv.inverted().transposed());
    _program->setUniformValue("mv", mv);

    _program->setUniformValue("light.position", QVector3D(5, 5, -5) );
    _program->setUniformValue("light.ambient", QVector3D(0.3f, 0.3f, 0.3f));
    _program->setUniformValue("light.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
    _program->setUniformValue("light.specular", QVector3D(1.0f,  1.0f,1.0f));
    _program->setUniformValue("light.shininess", 24.0f);
    _program->setUniformValue("material.color", QVector3D(1, 0, 0));

    glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, 0);

}



void Renderer::resizeGL(int w, int h)
{

}
