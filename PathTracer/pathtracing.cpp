#include "pathtracing.h"
#include<cmath>
#define M_PI 3.1415

PathTracing::PathTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam, std::vector<Object *> objects, std::vector<Light> lights, QVector3D backgroundColor)
    : _model(model),
      _backgroundColor(backgroundColor),
      _camera(cam),
      _objects(objects),
      _lights(lights),
      _width(w),
      _height(h)
{
    _seed = std::rand();

}



float PathTracing::rand(QVector3D pixel)
{
    float intPart;
    float result = std::modf(sin(_seed / 100.0f * QVector3D::dotProduct(pixel, QVector3D(12.9898f, 78.233f, 0.0f))) * 43758.5453f, &intPart);
    _seed += 1.0f;
    return result;
}



QVector3D PathTracing::SampleHemisphere(QVector3D normal, QVector3D pixel)
{
    // Uniformly sample hemisphere direction
    float cosTheta = rand(pixel);
    float sinTheta = sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
    float phi = 2 * M_PI * rand(pixel);
    QVector3D tangentSpaceDir = QVector3D(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    // Transform direction to world space
    return (tangentSpaceDir * GetTangentSpace(normal));
}



QMatrix4x4 PathTracing::GetTangentSpace(QVector3D normal)
{
    // Choose a helper vector for the cross product
    QVector3D helper = QVector3D(1, 0, 0);
    if (abs(normal.x()) > 0.99f)
        helper = QVector3D(0, 0, 1);
    // Generate vectors
    QVector3D tangent = QVector3D::crossProduct(normal, helper).normalized();
    QVector3D binormal = QVector3D::crossProduct(normal, tangent).normalized();
    return QMatrix4x4(tangent.x(), tangent.y(), tangent.z(), 0, binormal.x(), binormal.y(), binormal.z(), 0, normal.x(), normal.y(), normal.z(), 0, 0, 0, 0, 0);
}
