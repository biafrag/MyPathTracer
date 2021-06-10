#pragma once

#include <QVector3D>
#include "renderer.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include "intersectionrecord.h"
#include "tracer.h"

class PathTracing : public Tracer
{
public:

    PathTracing();

    /**
     * @brief generatePathTracingImage
     * @return
     */
    QImage generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam,
                                    Scene scene, QVector3D backgroundColor = {0, 0, 0});


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
     * @brief _sphereSeed
     */
    int _sphereSeed = 12000;

    /**
     * @brief _seed
     */
    float _seed;

};

