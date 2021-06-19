#pragma once
#include "object.h"
#include <vector>
#include "light.h"

/**
 * @brief The Scene class implements a scene with objects and lights.
 * A default scene is created and a lot of things like objects materials
 * are chosen randomly.
 *
 * @author Bianca Fragoso
 */
class Scene
{
public:
    /**
     * @brief The Scene's default constructor.
     */
    Scene();

    /**
     * @brief The Scene's desctructor.
     */
    ~Scene();

    /**
     * @brief Gets all the lights that are included on the scene.
     * @return A vector with all the lights.
     */
    std::vector<Light>getLights();

    /**
     * @brief Gets all the objects that are included on the scene.
     * @return A vector with all the objects.
     */
    std::vector<Object *> getObjects();

private:
    /**
     * @brief Creates a default scene with some spheres and planes.
     */
    void createDefaultScene();

    /**
     * @brief Creates 2 lights for the scene.
     */
    void createLights();

    /**
     * @brief Generates a random 3D vector with coordinates between 0 and 1.
     * @return A 3D vector.
     */
    QVector3D randVec();

    /**
     * @brief Generates a random number with coordinates between 0 and 1.
     * @return A random number.
     */
    float rand0x1();

    /**
     * @brief Generates a random number with coordinates between 30 and 200.
     * @return A random number between 30 and 200.
     */
    float rand30x200();

    /**
     * @brief Generates a random material. The material atributtes like specular, albedo, smoothness etc are generated randomly.
     * @return A random material.
     */
    Material randomMaterial();

private:
    /**
     * @brief The vector that stores the lights.
     */
    std::vector<Light> _lights;

    /**
     * @brief The vector that stores pointers for the objects.
     */
    std::vector<Object *> _objects;
};

