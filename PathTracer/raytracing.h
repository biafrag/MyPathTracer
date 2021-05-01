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
               std::vector<Object *> objects, std::vector<Light> lights, QVector3D backgroundColor = {0, 0, 0});

    QImage generateRayTracingImage();

    float getTime();

private:

    void IntersectGroundPlane(Ray ray, RayHit &bestHit);
    RayHit Trace(Ray ray);

    QVector3D getBaricentricCoordinates(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);
    bool triangleVerification(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);

    bool hasObjectObstacle(Light light, QVector3D point, unsigned int index);
    std::vector<QVector3D> _result;

    Object * reflection(Ray ray, float &tCloser, unsigned int &indexObject, unsigned int &vertCloser, unsigned int indMyObject);

    QVector3D getRayPoint(float t, Ray ray);

    QColor getColorAt(QVector3D point, Ray ray, float t, Object *object, int indObj, int indVert = -1);
    QColor getColorAt2(QVector3D point, Ray ray, float t, Object *object, int indObj, int indVert = -1);

    QVector3D color(QVector3D point, Ray ray, float t, Object *object, int indObj, int indVert = -1);

    QColor calculateAmbient(Object *object, Light light, QVector3D point, int ind1 = -1);

    QColor calculatePhongDiffuse(Object *object, Light light, float lambertian, QVector3D point, int ind1 = -1);

    QColor calculatePhongSpecular(Object *object, Light light, QVector3D point, QVector3D N, int ind1 = -1);


private:

    QMatrix4x4 _model;

    QVector3D _Xe, _Ye, _Ze;

    QVector3D _backgroundColor = {0, 0, 0};

    Renderer::Camera _camera;

    std::vector<Object *> _objects;

    std::vector<Light> _lights;

    int _width, _height;

    float _time;

};

