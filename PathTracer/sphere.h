#pragma once
#include "object.h"
#include <QOpenGLVertexArrayObject>

class Sphere : public Object
{
public:
    Sphere();
    virtual void initialize() override;

    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) override;

    virtual void createBuffers() override;
    virtual void updateVertexBuffer() override;
    virtual void updateTexBuffer() override;

    void createVAO();

private:

    QOpenGLVertexArrayObject _vao;
};

