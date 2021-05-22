#pragma once

#include <QVector3D>
#include "renderer.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include "intersectionrecord.h"
class PathTracing
{
public:

    PathTracing();

    /**
     * @brief generatePathTracingImage
     * @return
     */
    QImage generatePathTracingImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam,
                                    std::vector<Object *> &objects, QVector3D backgroundColor = {0, 0, 0});

    /**
     * @brief getTime
     * @return
     */
    float getTime();

    void setRayNumber(unsigned int number);

    void setDimensions(int width, int height);


private:

    /**
     * @brief getColorAt
     * @param hit
     * @param ray
     * @return
     */
    QVector3D getColorAt(RayHit &hit, Ray &ray);

    /**
     * @brief getColorFinalAt
     * @param hit
     * @param ray
     * @return
     */
    QVector3D getColorFinalAt(RayHit &hit, Ray &ray);

    /**
     * @brief getColorDiffuseAt
     * @param hit
     * @param ray
     * @return
     */
    QVector3D getColorDiffuseAt(RayHit &hit, Ray &ray);

    /**
     * @brief getAlbedoPoint
     * @param hit
     * @param object
     * @param indVert
     * @return
     */
    QVector3D getAlbedoPoint(RayHit hit, Object * object, int indVert);

    /**
     * @brief rand
     * @return
     */
    float rand();

    /**
     * @brief SampleHemisphere
     * @param normal
     * @param alpha
     * @return
     */
    QVector3D SampleHemisphere(QVector3D normal, float alpha = 0);

    /**
     * @brief GetTangentSpace
     * @param normal
     * @return
     */
    QMatrix4x4 GetTangentSpace(QVector3D normal);

    /**
     * @brief Trace
     * @param ray
     * @return
     */
    RayHit Trace(Ray ray);

    /**
     * @brief clamp
     * @param number
     * @param low
     * @param high
     * @return
     */
    float clamp(float number, float low, float high);

private:
    /**
     * @brief _model
     */
    QMatrix4x4 _model;

    /**
     * @brief _Xe
     */
    QVector3D _Xe;

    /**
     * @brief _Ye
     */
    QVector3D _Ye;

    /**
     * @brief _Ze
     */
    QVector3D _Ze;

    QVector3D _backgroundColor = {0, 0, 0};

    /**
     * @brief _camera
     */
    Renderer::Camera _camera;

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
     * @brief _sphereSeed
     */
    int _sphereSeed = 12000;

    /**
     * @brief _seed
     */
    float _seed;

    /**
     * @brief _rayNumber
     */
    unsigned int _rayNumber = 1;


};

