#pragma once
#include<QVector3D>

/**
 * @brief The Light struct stores the properties of a light like its ambient, diffuse and specular components.
 *
 * @author Bianca Fragoso
 */
struct Light
{
    /**
     * @brief The light position.
     */
    QVector3D position;

    /**
     * @brief The ambient component of the light.
     */
    QVector3D ambient;

    /**
     * @brief The diffuse component of the light.
     */
    QVector3D diffuse;

    /**
     * @brief The specular component of the light.
     */
    QVector3D specular;

    /**
     * @brief The shininess component of the light.
     */
    float shi;
};
