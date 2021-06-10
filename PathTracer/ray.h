#pragma once
#include <QVector3D>
/**
 * @brief The Ray struct
 */
struct Ray
{
    /**
     * @brief timesReflection
     */
    int timesReflection = 5;

    /**
     * @brief energy
     */
    QVector3D energy = {1, 1, 1};

    /**
     * @brief origin
     */
    QVector3D origin;

    /**
     * @brief direction
     */
    QVector3D direction;

    /**
     * @brief hit
     * @param t
     * @return
     */
    QVector3D hit(float t)
    {
        return (origin + (t * direction));
    }
};



struct RayHit
{
    /**
     * @brief position
     */
    QVector3D position;

    /**
     * @brief t
     */
    float t = FLT_MAX;

    /**
     * @brief normal
     */
    QVector3D normal;

    /**
     * @brief albedo
     */
    QVector3D albedo;

    /**
     * @brief specular
     */
    QVector3D specular;

    /**
     * @brief emission
     */
    QVector3D emission;

    /**
     * @brief smoothness
     */
    float smoothness;
};


