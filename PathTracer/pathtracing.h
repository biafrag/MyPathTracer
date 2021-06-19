#pragma once

#include <QVector3D>
#include "renderer.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include "intersectionrecord.h"
#include "tracer.h"

/**
 * @brief The PathTracing class implements the path tracing algorithm.
 * The algorithm it's a variant of the ray tracing algorithm and also
 * traces some rays for each pixel and estimate the color of
 * the pixel, but it uses the rendering equation to do that.
 *
 * @author Bianca Fragoso
 */
class PathTracing : public Tracer
{
public:

    /**
     * @brief The Path Tracing default constructor.
     */
    PathTracing();

    /**
     * @brief This is the main function of this class, because it walks through
     * all the pixels to estimate their colors and generates the final image.
     * @param w The width of the screen that the image will be draw.
     * @param h The height of the screen that the image will be draw.
     * @param model The model matrix.
     * @param cam The camera properties.
     * @param scene The scene with the objects that will be rendered.
     * @param backgroundColor The background color.
     * @return A final image, result of the path tracing algorithm.
     */
    QImage generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam,
                                    Scene scene, QVector3D backgroundColor = {0, 0, 0});


private:

    /**
     * @brief Gives the final color for a ray hit.
     * @param hit The ray hit.
     * @param ray The ray that hit some surface.
     * @return A 3D vector with a color.
     */
    QVector3D getColorFinalAt(RayHit &hit, Ray &ray);

    /**
     * @brief Gives the diffuse color for a ray hit.
     * @param hit The ray hit.
     * @param ray The ray that hit some surface.
     * @return A 3D vector with a color.
     */
    QVector3D getColorDiffuseAt(RayHit &hit, Ray &ray);

    /**
     * @brief Gets the albedo component of the given hit.
     * It considers that the object that was hitted can have a texture.
     * @param hit The surface hit info.
     * @param object The object that was hitted.
     * @param indVert The index of the hitted vertex of the object that was hitted.
     * @return A 3D vector with the albedo value.
     */
    QVector3D getAlbedoPoint(RayHit hit, Object * object, int indVert);

    /**
     * @brief Generates a random number.
     * @return A float random number.
     */
    float rand();

    /**
     * @brief Gives a random point/direction of the hemisphere centered on the @p normal.
     * @param normal The point that the hemisphere is centered.
     * @param alpha The number that determines the power of the cosine sampling.
     * @return A 3D vector with a random direction in the hemisphere.
     */
    QVector3D SampleHemisphere(QVector3D normal, float alpha = 0);

    /**
     * @brief Creates a matrix to transform to the tangent space (TBN).
     * @param normal The normal used to calculate the tangent and the bitangent.
     * @return The TBN matrix.
     */
    QMatrix4x4 GetTangentSpace(QVector3D normal);

    /**
     * @brief Traces a ray and finds out where it hits.
     * @param ray The ray.
     * @return A RayHit with the informations about where the ray hitted.
     */
    RayHit Trace(Ray ray);

    /**
     * @brief Clamps a number to the interval [low, high].
     * @param number The number to be clamped.
     * @param low The lower number of the interval.
     * @param high The greater number of the interval.
     * @return The result of the number clamped on the interval.
     */
    float clamp(float number, float low, float high);

private:

    /**
     * @brief The first seed used as seed for the std::rand.
     */
    int _seed = 12000;

    /**
     * @brief The number used as a complement to the random number
     * to be sure that the random number will never be the same.
     */
    float _randComplement;

};

