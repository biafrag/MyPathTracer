#pragma once
#include <QVector3D>

struct Ray
{
    QVector3D energy = QVector3D(1.0, 1.0, 1.0);
    QVector3D origin;
    QVector3D direction;
};
