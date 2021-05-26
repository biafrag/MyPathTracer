#pragma once
#include <QOpenGLVertexArrayObject>
#include "trianglemesh.h"

class Plane : public TriangleMesh
{
public:
    Plane();
    Plane(QMatrix4x4 translation, QMatrix4x4 rotation, QMatrix4x4 scale);
    void initialize() override;

    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) override;

//    virtual void createBuffers() override;
//    virtual void updateVertexBuffer() override;
//    virtual void updateTexBuffer() override;

    //void createVAO();

    void setTexture(QString path) override;

private:
    //QOpenGLVertexArrayObject _vao;

    bool _isInitialized = false;
};

