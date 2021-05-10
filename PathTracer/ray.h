#pragma once
#include <QVector3D>

struct Ray
{
    int timesReflection = 5;
    QVector3D energy = {1, 1, 1};
    QVector3D origin;
    QVector3D direction;

    QVector3D hit(float t)
    {
        return (origin + (t * direction));
    }
};



struct RayHit
{
    QVector3D position;
    float t = FLT_MAX;
    QVector3D normal;
    QVector3D albedo;
    QVector3D specular;
    QVector3D emission;
    float smoothness;
};


