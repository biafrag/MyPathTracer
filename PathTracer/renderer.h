#pragma once

#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "object.h"
#include <vector>
#include "light.h"

class RayTracing;
class PathTracing;

class Renderer  : public QOpenGLWidget, protected QOpenGLExtraFunctions
{

public:
    Renderer(QWidget* parent);
    ~Renderer();

    struct Camera
    {
        QVector3D eye;

        QVector3D center;

        QVector3D up;

        float fov;

        float zNear;

        float zFar;

    };


    QImage getRayTracedImage(float &time);

    QImage getPathTracedImage(float &time);

    void setNumberOfRays(unsigned int rays);


private:
    void createMaterialLib();
    void createScene();
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    //Parte do Arcball
    QVector3D _p0, _p1; //pontos para fazer rotação
    double _radius; //Sphere Radius
    QVector3D Points_Sphere(QVector3D pointT);

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    QVector3D randVec();

    float rand0x1();

    float rand30x200();



private:

    /**
     * @brief
     */
    GLuint _texture = static_cast<GLuint>(-1);

    QMatrix4x4 _model;
    QMatrix4x4 _proj;
    QMatrix4x4 _view;


    Camera _camera;

    //Mouse
    float _percentzoom;
    bool _mousepress = false;

    Light _light;

    std::vector<Light> _lights;

    std::vector<Object *> _objects;

    RayTracing *_rayTracer;

    PathTracing * _pathTracer;
};

