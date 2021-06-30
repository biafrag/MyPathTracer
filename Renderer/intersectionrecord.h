#pragma once
#include <QVector3D>
#include "ray.h"
#include "object.h"
/**
 * @brief The IntersectRecord struct stores the information about a ray intersection.
 *  It stores the RayHit (geometric and material informations) and the object that was hitted by the ray.
 *
 * @author Bianca Fragoso
 */

struct IntersectRecord
{
    /**
     * @brief The geometric information and the material information about the point that the ray intersected.
     */
    RayHit hit;

    /**
     * @brief The object that was intersected.
     */
    Object *object;
};
