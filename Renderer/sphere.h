#pragma once
#include "object.h"
#include <QOpenGLVertexArrayObject>
#include<trianglemesh.h>

/**
 * @brief The Sphere class implements a sphere.
 * The sphere inherits from a TriangleMesh, but
 * has some functions that improve the intersection calculus.
 * So, there's no need to go through all the triangles testing
 * if the ray intersected one of them. For a sphere a simpler test it's made.
 *
 * @author Bianca Fragoso
 */
class Sphere : public TriangleMesh
{
public:
    /**
     * @brief Creates a sphere mesh with the given center and radius.
     * @param center The center of the sphere.
     * @param radius The radius of the sphere.
     */
    Sphere(QVector3D center = QVector3D(0, 0, 0), float radius = 1.0);

    /**
     * @brief Checks if ray intercepts with the sphere.
     * @param ray The ray that needs to be checked.
     * @param model The model matrix.
     * @return The distance between the begin of the ray and the sphere.
     * If the ray doesn't intercept the sphere the returned value is -1.
     */
    float intersectsWith(Ray ray, QMatrix4x4 model);

    /**
     * @brief Computes the normal vector of the sphere at a given point.
     * @param point The point to compute the normal at.
     * @return The normal at the given point.
     */
    QVector3D normalAt(QVector3D point);

private:

    /**
     * @brief The sphere's center.
     */
    QVector3D _center;

    /**
     * @brief The sphere's radius.
     */
    float _radius;

    /**
     * @brief The model matrix of the sphere.
     */
    QMatrix4x4 _model;
};

