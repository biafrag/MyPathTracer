#pragma once

#include <QVector3D>

class RayTracing
{
public:
    RayTracing();

private:
    struct Ray
    {
        QVector3D origin;
        QVector3D direction;
    };

    Ray CreateRay(QVector3D origin, QVector3D direction);
    Ray CreateCameraRay(QVector3D uv);


};

