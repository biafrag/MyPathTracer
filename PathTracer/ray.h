#pragma once
#include <QVector3D>
/**
 * @brief The file holds the Ray struct that has all the info that its needed for a Ray.
 * This file also has the RayHit struct that holds the geometric info of a ray hit.
 *
 * @author Bianca Fragoso
 */
struct Ray
{
    /**
     * @brief Times that a ray can bounce on surfaces.
     */
    int timesReflection = 5;

    /**
     * @brief The light ray energy dictates if the ray will continuing to bounce on surfaces. If the energy
     * its (0,0,0) it means that the light ray doesn't have energy anymore to keep going bouncing on other surfaces.
     */
    QVector3D energy = {1, 1, 1};

    /**
     * @brief The origin of the ray. Can be the camera position or other surface position.
     */
    QVector3D origin;

    /**
     * @brief The direction of the ray.
     */
    QVector3D direction;

    /**
     * @brief Computes a hit for a given @p t distance.
     * @param t The distance from the origin of the ray to the hit point.
     * @return The hit position.
     */
    QVector3D hit(float t)
    {
        return (origin + (t * direction));
    }
};



struct RayHit
{
    /**
     * @brief The hit position.
     */
    QVector3D position;

    /**
     * @brief The distance from the origin of the ray to the hit point.
     */
    float t = FLT_MAX;

    /**
     * @brief The normal of the hit point.
     */
    QVector3D normal;

    /**
     * @brief The albedo component of the hit point.
     */
    QVector3D albedo;

    /**
     * @brief The specular component of the hit point.
     */
    QVector3D specular;

    /**
     * @brief The emission component of the hit point.
     */
    QVector3D emission;

    /**
     * @brief The smoothness component of the hit point.
     */
    float smoothness;
};


