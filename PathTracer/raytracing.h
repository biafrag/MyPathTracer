#pragma once

#include <QVector3D>
#include "renderer.h"
#include "ray.h"
#include "intersectionrecord.h"
#include "scene.h"
#include "tracer.h"

class RayTracing : public Tracer
{
public:

    /**
     * @brief RayTracing
     */
    RayTracing();

    QImage generateRayTracingImageRecursionApproach(int w, int h, QMatrix4x4 model, Renderer::Camera cam, Scene scene, QVector3D backgroundColor);
    QImage generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam, Scene scene, QVector3D backgroundColor);


private:

    QVector3D getBaricentricCoordinates(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);
    bool triangleVerification(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point);

    bool hasObjectObstacle(Light light, IntersectRecord intersection);

    Object * reflection(Ray ray, float &tCloser, unsigned int &indexObject, unsigned int &vertCloser, unsigned int indMyObject);

    QVector3D getRayPoint(float t, Ray ray);

    QVector3D getColorAtRecursive(IntersectRecord intersection, std::vector<Light> lights, Ray ray, int indObj, int indVert = -1);

    QVector3D getColorAt(QVector3D point, std::vector<Light> lights, Ray &ray, Object *object, int indObj, float t, int indVert = -1);

    QVector3D calculateAmbient(IntersectRecord intersection, Light light, int ind1 = -1);

    QVector3D calculatePhongDiffuse(IntersectRecord intersection, Light light, int ind1 = -1);

    QVector3D calculatePhongSpecular(IntersectRecord intersection, Light light);

    /**
     * @brief _camera
     */
    Renderer::Camera _camera;
};

