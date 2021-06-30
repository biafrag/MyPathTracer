#pragma once

#include <QVector3D>
#include "scene.h"
#include "ray.h"
#include "renderer.h"

/**
 * @brief The Tracer class implements some functions to trace rays and generate images using them.
 *
 * @author Bianca Fragoso
 */
class Tracer
{
public:

    /**
     * @brief The tracer default constructor.
     */
    Tracer();

    /**
     * @brief This is one of the main functions of this class, because it walks through
     * all the pixels to estimate their colors and generates the final image. It's implemented
     * according to the child class.
     * @param w The width of the screen that the image will be draw.
     * @param h The height of the screen that the image will be draw.
     * @param model The model matrix.
     * @param cam The camera properties.
     * @param scene The scene with the objects that will be rendered.
     * @return A final image, result of the ray tracing recursive algorithm.
     */
    virtual QImage generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam,
                                    Scene scene) = 0;

    /**
     * @brief Gets the time that it taked to generate the last image.
     * @return The time in seconds.
     */
    float getTime();

    /**
     * @brief Sets the number of rays that will be traced by pixel.
     * @param number The number of rays.
     */
    void setRayNumber(unsigned int number);

protected:
    /**
     * @brief Traces a ray and finds out where it hits.
     * @param ray The ray.
     * @return A RayHit with the informations about where the ray hitted.
     */
    RayHit Trace(Ray ray);

protected:
    /**
     * @brief The model matrix.
     */
    QMatrix4x4 _model;

    /**
     * @brief The background color.
     */
    QVector3D _backgroundColor = {0, 0, 0};

    /**
     * @brief The vector with the objects of the scene.
     */
    std::vector<Object *> _objects;

    /**
     * @brief The width of the screen that the image will be draw.
     */
    int _width;

    /**
     * @brief The height of the screen that the image will be draw.
     */
    int _height;

    /**
     * @brief The time spended to draw a image.
     */
    float _time;


    /**
     * @brief _rayNumber
     */
    unsigned int _rayNumber = 1;
};

