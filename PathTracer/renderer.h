#pragma once

#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "object.h"
#include <vector>
#include "light.h"
#include "scene.h"

class RayTracing;
class PathTracing;

class Renderer  : public QOpenGLWidget, protected QOpenGLExtraFunctions
{

public:

    /**
     * @brief Renderer
     * @param parent
     */
    Renderer(QWidget* parent);

    /**
     * @brief ~Renderer
     * @param parent
     */
    ~Renderer();

    /**
     * @brief The Camera struct
     */
    struct Camera
    {
        QVector3D eye;

        QVector3D center;

        QVector3D up;

        float fov;

        float zNear;

        float zFar;

    };

    /**
     * @brief getRayTracedImage
     * @param time
     * @return
     */
    QImage getRayTracedImage(float &time);

    /**
     * @brief getPathTracedImage
     * @param time
     * @return
     */
    QImage getPathTracedImage(float &time);

    /**
     * @brief setNumberOfRays
     * @param rays
     */
    void setNumberOfRays(unsigned int rays);

    void setScene(Scene scene);

private:
    /**
     * @brief createMaterialLib
     */
    void createMaterialLib();

    /**
     * @brief initializeGL
     */
    virtual void initializeGL() override;

    /**
     * @brief paintGL
     */
    virtual void paintGL() override;

    /**
     * @brief resizeGL
     * @param w
     * @param h
     */
    virtual void resizeGL(int w, int h) override;

    //Parte do Arcball

    /**
     * @brief _p0
     */
    QVector3D _p0, _p1; //pontos para fazer rotação

    /**
     * @brief _radius
     */
    double _radius; //Sphere Radius
    QVector3D Points_Sphere(QVector3D pointT);

    /**
     * @brief mousePressEvent
     * @param event
     */
    virtual void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief mouseMoveEvent
     * @param event
     */
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief wheelEvent
     * @param event
     */
    virtual void wheelEvent(QWheelEvent *event) override;

private:

    /**
     * @brief _texture
     */
    GLuint _texture = static_cast<GLuint>(-1);

    /**
     * @brief _model
     */
    QMatrix4x4 _model;

    /**
     * @brief _proj
     */
    QMatrix4x4 _proj;

    /**
     * @brief _view
     */
    QMatrix4x4 _view;


    /**
     * @brief _camera
     */
    Camera _camera;


    /**
     * @brief _percentzoom
     */
    float _percentzoom;

    /**
     * @brief _mousepress
     */
    bool _mousepress = false;

//    /**
//     * @brief _light
//     */
//    Light _light;

//    /**
//     * @brief _lights
//     */
//    std::vector<Light> _lights;

//    /**
//     * @brief _objects
//     */
//    std::vector<Object *> _objects;

    Scene _scene;

    /**
     * @brief _rayTracer
     */
    RayTracing *_rayTracer;

    /**
     * @brief _pathTracer
     */
    PathTracing * _pathTracer;
};

