#pragma once

#include "object.h"
#include <QOpenGLVertexArrayObject>

class TriangleMesh : public Object
{
public:
    TriangleMesh() = default;
    TriangleMesh(std::vector<QVector3D> &points, std::vector<unsigned int> &indices, std::vector<QVector3D> &normals);

    virtual void initialize() override;

    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) override;

    virtual void createBuffers() override;
    virtual void updateVertexBuffer() override;
    virtual void updateTexBuffer() override;
    void createVAO();

    bool triangleVerification(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);
    QVector3D getTriangleNormal(unsigned int trianglefirstIndexOfIndice, QMatrix4x4 m);
    QVector3D getTriangleBaricentricCoordinates(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);
    QVector3D getNormalInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);
    QVector3D getTexCoordinatesInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    //Recebe raio e vê se raio intersecta algum triângulo da malha
    int intersectsWithRay(Ray ray, QMatrix4x4 m, float tCloser, int &firstIndTri);
    bool isRayIntersecting(Ray ray, QMatrix4x4 m);
    void computeNormals();
    void normalizeNormals();

protected:
    void createTexture() override;

    unsigned int _textureID = static_cast<unsigned int>(-1);

    QOpenGLVertexArrayObject _vao;
private:

//    QOpenGLVertexArrayObject _vao;
};
