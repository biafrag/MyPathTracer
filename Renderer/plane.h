#pragma once
#include <QOpenGLVertexArrayObject>
#include "trianglemesh.h"

/**
 * @brief The plane class implements a simple plane object.
 * The plane is an object that has 4 points and 2 triangles.
 * The simple plane its implemented as a square with size 2x2
 * and center in (0,0), but the plane can also be translated,
 * rotated and scaled using the  appropriate constructor.
 *
 * @author Bianca Fragoso
 */
class Plane : public TriangleMesh
{
public:
    /**
     * @brief The Plane default constructor.
     */
    Plane();

    /**
     * @brief The Plane constructor that receives all the transformation that the plane should have.
     * @param translation The translation matrix.
     * @param rotation The rotation matrix.
     * @param scale The scale matrix.
     */
    Plane(QMatrix4x4 translation, QMatrix4x4 rotation, QMatrix4x4 scale);
};

