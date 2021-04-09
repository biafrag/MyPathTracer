#pragma once

#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class Renderer  : public QOpenGLWidget, protected QOpenGLExtraFunctions
{

public:
    Renderer(QWidget* parent);

    struct Camera
    {
        QVector3D eye;

        QVector3D center;

        QVector3D up;

        float fov;

        float zNear;

        float zFar;

    };

    struct Sphere
    {
        QVector3D position;
        float radius;

    };

    struct Material
    {
        QVector3D albedo;
        QVector3D specular;
        QVector3D color;
    };

    struct Light
    {
        QVector3D position;
        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
        float shi;
    };

private:
    void createScene();
    void createSimpleScene();
    void createSphere();
    void drawZBuffer();
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    void createBuffers();
    void updateVertexBuffer();
    void updateTexBuffer();

    void createVAO();


private:
    /**
     * @brief
     */
    unsigned int _texCoordsBuffer = static_cast<unsigned int>(-1);
    unsigned int _pointsBuffer = static_cast<unsigned int>(-1);
    unsigned int _normalsBuffer = static_cast<unsigned int>(-1);
    unsigned int _indicesBuffer = static_cast<unsigned int>(-1);


    /**
     * @brief
     */
    GLuint _texture = static_cast<GLuint>(-1);

    QMatrix4x4 _model;
    QMatrix4x4 _proj;
    QMatrix4x4 _view;

    /**
     * @brief OpenGL program used to render meshes without shading.
     */
    QOpenGLShaderProgram *_program {nullptr};

    std::vector<QVector3D> _points;
    std::vector<unsigned int> _indices;
    std::vector<QVector3D> _normals;
    std::vector<QVector3D> _texCoords;

    QOpenGLVertexArrayObject _vao;

    Camera _camera;


};

