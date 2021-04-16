#pragma once
#include<QVector3D>

struct Light
{
    QVector3D position;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shi;
};
