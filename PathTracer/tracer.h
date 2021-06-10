#pragma once

#include <QVector3D>
#include "scene.h"
#include "ray.h"
#include "renderer.h"

class Tracer
{
public:
    Tracer();

    virtual QImage generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam,
                                    Scene scene, QVector3D backgroundColor = {0, 0, 0}) = 0;

    /**
     * @brief getTime
     * @return
     */
    float getTime();

    /**
     * @brief setRayNumber
     * @param number
     */
    void setRayNumber(unsigned int number);

    /**
     * @brief setDimensions
     * @param width
     * @param height
     */
    void setDimensions(int width, int height);

protected:
    /**
     * @brief Trace
     * @param ray
     * @return
     */
    RayHit Trace(Ray ray);

protected:
    /**
     * @brief _model
     */
    QMatrix4x4 _model;

    QVector3D _backgroundColor = {0, 0, 0};

    /**
     * @brief _objects
     */
    std::vector<Object *> _objects;

    /**
     * @brief _width
     */
    int _width, _height;

    /**
     * @brief _time
     */
    float _time;


    /**
     * @brief _rayNumber
     */
    unsigned int _rayNumber = 1;
};

