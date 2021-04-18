#pragma once

#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "object.h"
#include <vector>
#include "light.h"

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

//    struct Sphere
//    {
//        QVector3D position;
//        float radius;

//    };

//    struct Material
//    {
//        QVector3D albedo;
//        QVector3D specular;
//        QVector3D color;
//    };

    QImage getRayTracedImage(float &time);

private:
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
    //Material _material;

    std::vector<Object *> _objects;
};

