#pragma once

#include <QVector3D>
#include "renderer.h"
#include "object.h"
#include "light.h"
#include "ray.h"

class RayTracing
{
public:
    struct RayHit
    {
        QVector3D position;
        float distance;
        QVector3D normal;
    };


    RayTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam,
               std::vector<Object *> objects, std::vector<Light> lights);

    QImage generateRayTracingImage();

    float getTime();

private:

    Ray CreateRay(QVector3D origin, QVector3D direction);
    Ray CreateCameraRay(QVector3D uv, QMatrix4x4 model, QMatrix4x4 proj);

    void AddPixels(QVector3D uv, QMatrix4x4 model, QMatrix4x4 proj);
    RayHit CreateRayHit();

    void IntersectGroundPlane(Ray ray, RayHit &bestHit);
    RayHit Trace(Ray ray);

    QVector3D getBaricentricCoordinates(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);
    bool triangleVerification(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);

    bool hasObjectObstacle(Light light, QVector3D point, unsigned int index);
    std::vector<QVector3D> _result;

    QColor reflection(std::vector<Light> lights, QVector3D point, QVector3D d, QVector3D n, unsigned int index, std::vector<Object *> objects, QMatrix4x4 model, QVector3D eye);

    QVector3D getRayPoint(float t, Ray ray);

    QColor getColorAt(QVector3D point, Ray ray, float t, Object *object, int indObj, int indVert);

private:

    QMatrix4x4 _model;

    QVector3D _Xe, _Ye, _Ze;

    Renderer::Camera _camera;

    std::vector<Object *> _objects;

    std::vector<Light> _lights;

    int _width, _height;

    float _time;

};

