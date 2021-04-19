#pragma once
#include <QVector3D>

struct Ray
{
    int energy = 5;
    QVector3D origin;
    QVector3D direction;
};
