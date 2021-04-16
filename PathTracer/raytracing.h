#pragma once

#include <QVector3D>
#include "renderer.h"
#include "object.h"
#include "light.h"

class RayTracing
{
public:
    struct Ray
    {
        QVector3D origin;
        QVector3D direction;
    };
    struct RayHit
    {
        QVector3D position;
        float distance;
        QVector3D normal;
    };
    RayTracing();
    QImage generateRayTracingImage(int w, int h, QMatrix4x4 model, QMatrix4x4 proj, Renderer::Camera cam,
                                   std::vector<Object *> objects, Light light);
private:

    Ray CreateRay(QVector3D origin, QVector3D direction);
    Ray CreateCameraRay(QVector3D uv, QMatrix4x4 model, QMatrix4x4 proj);

    void AddPixels(QVector3D uv, QMatrix4x4 model, QMatrix4x4 proj);
    RayHit CreateRayHit();

    void IntersectGroundPlane(Ray ray, RayHit &bestHit);
    RayHit Trace(Ray ray);

    QVector3D getBaricentricCoordinates(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);
    bool triangleVerification(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);


    std::vector<QVector3D> _result;

};

