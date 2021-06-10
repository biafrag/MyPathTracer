#pragma once
#include <QOpenGLVertexArrayObject>
#include "trianglemesh.h"

/**
 * @brief The Plane class
 */
class Plane : public TriangleMesh
{
public:
    /**
     * @brief Plane
     */
    Plane();
    /**
     * @brief Plane
     * @param translation
     * @param rotation
     * @param scale
     */
    Plane(QMatrix4x4 translation, QMatrix4x4 rotation, QMatrix4x4 scale);
};

