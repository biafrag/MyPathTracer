#pragma once

#include <QVector3D>
#include "renderer.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include "intersectionrecord.h"

class RayTracing
{
public:

    RayTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam,
               std::vector<Object *> objects, std::vector<Light> lights, QVector3D backgroundColor = {0, 0, 0});

    QImage generateRayTracingImage();
    QImage generateRayTracingImage2();


    float getTime();

private:

    QVector3D getBaricentricCoordinates(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);
    bool triangleVerification(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);

    bool hasObjectObstacle(Light light, IntersectRecord intersection);

    Object * reflection(Ray ray, float &tCloser, unsigned int &indexObject, unsigned int &vertCloser, unsigned int indMyObject);

    QVector3D getRayPoint(float t, Ray ray);

    QVector3D getColorAt(IntersectRecord intersection, Ray ray, int indObj, int indVert = -1);

    QVector3D getColorAt2(QVector3D point, Ray &ray, Object *object, int indObj, float t, int indVert = -1);

    QVector3D calculateAmbient(IntersectRecord intersection, Light light, int ind1 = -1);

    QVector3D calculatePhongDiffuse(IntersectRecord intersection, Light light, int ind1 = -1);

    QVector3D calculatePhongSpecular(IntersectRecord intersection, Light light);


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

