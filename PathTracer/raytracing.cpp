#include "raytracing.h"

RayTracing::RayTracing()
{

}



RayTracing::Ray RayTracing::CreateRay(QVector3D origin, QVector3D direction)
{
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}

