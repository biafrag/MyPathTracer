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

    PathTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam,
               std::vector<Object *> objects, std::vector<Light> lights, QVector3D backgroundColor = {0, 0, 0});

    QImage generatePathTracingImage();

    float getTime();

private:

    bool hasObjectObstacle(Light light, IntersectRecord intersection);

    Object * reflection(Ray ray, float &tCloser, int &indexObject, int &vertCloser, int indMyObject);

    QVector3D getRayPoint(float t, Ray ray);

    QVector3D getColorAt(RayHit &hit, Ray &ray);
    QVector3D getColorFinalAt(RayHit &hit, Ray &ray);

    QVector3D getColorDiffuseAt(RayHit &hit, Ray &ray);


    QVector3D calculateAmbient(IntersectRecord intersection, Light light, int ind1 = -1);

    QVector3D calculatePhongDiffuse(IntersectRecord intersection, Light light, int ind1 = -1);

    QVector3D calculatePhongSpecular(IntersectRecord intersection, Light light);

    QVector3D getAlbedoPoint(RayHit hit, Object * object, int indVert);

    float rand();

    float rand2(QVector3D point);

    QVector3D SampleHemisphere(QVector3D normal, float alpha = 0);

    QMatrix4x4 GetTangentSpace(QVector3D normal);

    RayHit Trace(Ray ray);

private:
    float clamp(float number, float low, float high);
    QMatrix4x4 _model;

    QVector3D _Xe, _Ye, _Ze;

    QVector3D _backgroundColor = {0, 0, 0};

    Renderer::Camera _camera;

    std::vector<Object *> _objects;

    std::vector<Light> _lights;

    int _width, _height;

    float _time;

    int _sphereSeed = 1223832719;
    float _seed;

};

