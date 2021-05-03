#pragma once
#include <QVector3D>
#include "ray.h"
#include "object.h"

struct IntersectRecord
{
    RayHit hit;

    Object *object;
};
