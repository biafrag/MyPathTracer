#pragma once

#include <QVector3D>
#include "renderer.h"
#include "ray.h"
#include "intersectionrecord.h"
#include "scene.h"
#include "tracer.h"

/**
 * @brief The RayTracing class implements the ray tracing algorithm.
 * The algorithm consists in trace rays for each pixel and estimate
 * the color of the pixel. There's 2 options of implementation of the
 * ray tracing algorithm in this class.
 *
 * @author Bianca Fragoso
 */

class RayTracing : public Tracer
{
public:

    /**
     * @brief The Ray Tracing default constructor.
     */
    RayTracing();

    /**
     * @brief This is one of the main functions of this class, because it walks through
     * all the pixels to estimate their colors and generates the final image.This function
     * uses a recursive approach of the ray tracing algorithm.
     * @param w The width of the screen that the image will be draw.
     * @param h The height of the screen that the image will be draw.
     * @param model The model matrix.
     * @param cam The camera properties.
     * @param scene The scene with the objects that will be rendered.
     * @param backgroundColor The background color.
     * @return A final image, result of the ray tracing recursive algorithm.
     */
    QImage generateRayTracingImageRecursionApproach(int w, int h, QMatrix4x4 model, Renderer::Camera cam, Scene scene, QVector3D backgroundColor);

    /**
     * @brief This is one of the main functions of this class, because it walks through
     * all the pixels to estimate their colors and generates the final image. This function
     * uses a iterative approach of the ray tracing algorithm.
     * @param w The width of the screen that the image will be draw.
     * @param h The height of the screen that the image will be draw.
     * @param model The model matrix.
     * @param cam The camera properties.
     * @param scene The scene with the objects that will be rendered.
     * @param backgroundColor The background color.
     * @return A final image, result of the ray tracing recursive algorithm.
     */
    QImage generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam, Scene scene, QVector3D backgroundColor);


private:

    /**
     * @brief Checks if there is any object between the light and the intersection.
     * @param light The light.
     * @param intersection The intersection record.
     * @return A boolean that is True if an object it's between the light and the
     * intersection and False if it's not
     */
    bool hasObjectObstacle(Light light, IntersectRecord intersection);

    /**
     * @brief Receives a reflection ray and discovers wich object was hitted by this ray.
     * @param ray The ray that was reflected.
     * @param tCloser The minor distance that the ray has reached some other object or triangle.
     * @param indexObject The variable used to store the index of the object hitted by the ray that was reflected.
     * @param vertCloser The variable used to store the the index of the first index of the triangle hitted by the ray that was reflected.
     * @param indMyObject The index of the object that reflected the ray.
     * @return The object hitted by the reflected ray
     */
    Object * reflection(Ray ray, float &tCloser, unsigned int &indexObject, unsigned int &vertCloser, unsigned int indMyObject);

    /**
     * @brief Computes the color of the ray intersection using the recursive aproach.
     * @param intersection The informations about where the ray intersected.
     * @param lights The lights used to compute the color in the intersection.
     * @param ray The ray that was sended.
     * @param indObj The index of the object that was hitted by the ray.
     * @param indVert The index of the first index of the triangle intersected
     * by the Ray. Can be -1 if it's a sphere.
     * @return A 3D vector with the final color of the intersection record.
     */
    QVector3D getColorAtRecursive(IntersectRecord intersection, std::vector<Light> lights, Ray ray, int indObj, int indVert = -1);

    /**
     * @brief Computes the color of the ray intersection using the iterative aproach.
     * @param point The point that the ray intersected.
     * @param lights The lights that helps to estimate the color.
     * @param ray The ray traced.
     * @param object The object intersected by the ray.
     * @param indObj The index of the object that was hitted by the ray.
     * @param t The distance from the origin of the ray to the object intersected.
     * @param indVert The index of the first index of the triangle intersected
     * by the Ray. Can be -1 if it's a sphere.
     * @return A 3D vector with the final color of the intersection record.
     */
    QVector3D getColorAt(QVector3D point, std::vector<Light> lights, Ray &ray, Object *object, int indObj, float t, int indVert = -1);

    /**
     * @brief Computes the ambient component of the phong calculation method.
     * It considers the intersection's material ambient properties and the light's
     * ambient properties.
     * @param intersection The information about the intersection.
     * @param light The information about the light.
     * @param ind1 The first index of the indices of the triangle.
     * This is important if the object has texture.
     * @return  A 3D vector with the ambient component.
     */
    QVector3D calculateAmbient(IntersectRecord intersection, Light light, int ind1 = -1);

    /**
     * @brief Computes the diffuse component of the phong calculation method.
     * It considers the intersection's material diffuse properties and the light's
     * diffuse properties.
     * @param intersection The information about the intersection.
     * @param light The information about the light.
     * @param ind1 The first index of the indices of the triangle.
     * This is important if the object has texture.
     * @return A 3D vector with the diffuse component.
     */
    QVector3D calculatePhongDiffuse(IntersectRecord intersection, Light light, int ind1 = -1);

    /**
     * @brief Computes the specular component of the phong calculation method.
     * It considers the intersection's material specular properties and the light's
     * specular properties.
     * @param intersection The information about the intersection.
     * @param light The information about the light.
     * @return A 3D vector with the specular component.
     */
    QVector3D calculatePhongSpecular(IntersectRecord intersection, Light light);

    /**
     * @brief The theorical camera point of view to use
     * as reference to render the image.
     */
    Renderer::Camera _camera;
};

