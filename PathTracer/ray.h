#pragma once
#include <QVector3D>

struct Ray
{
    int timesReflection = 5;
    QVector3D energy = {1, 1, 1};
    QVector3D origin;
    QVector3D direction;
};
