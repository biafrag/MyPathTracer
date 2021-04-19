#pragma once
#include "object.h"
#include <QOpenGLVertexArrayObject>

class Sphere : public Object
{
public:
    Sphere(QVector3D center = QVector3D(0, 0, 0), float radius = 1.0);
    virtual void initialize() override;

    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) override;

    virtual void createBuffers() override;
    virtual void updateVertexBuffer() override;
    virtual void updateTexBuffer() override;

    float intersectsWith(Ray ray, QMatrix4x4 model);

    QVector3D normalAt(QVector3D point);
    void createVAO();

private:

    QOpenGLVertexArrayObject _vao;

    QVector3D _center;

    float _radius;
};

