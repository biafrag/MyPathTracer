#pragma once

#include "object.h"
#include <QOpenGLVertexArrayObject>

/**
 * @brief The TriangleMesh class
 */
class TriangleMesh : public Object
{
public:
    TriangleMesh() = default;

    /**
     * @brief TriangleMesh
     * @param points
     * @param indices
     * @param normals
     */
    TriangleMesh(std::vector<QVector3D> &points, std::vector<unsigned int> &indices, std::vector<QVector3D> &normals);

    /**
     * @brief initialize
     */
    virtual void initialize() override;

    /**
     * @brief render
     * @param projMatrix
     * @param viewMatrix
     * @param modelMatrix
     * @param lights
     */
    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) override;


    /**
     * @brief triangleVerification
     * @param trianglefirstIndexOfIndice
     * @param point
     * @param m
     * @return
     */
    bool triangleVerification(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief getTriangleNormal
     * @param trianglefirstIndexOfIndice
     * @param m
     * @return
     */
    QVector3D getTriangleNormal(unsigned int trianglefirstIndexOfIndice, QMatrix4x4 m);

    /**
     * @brief getTriangleBaricentricCoordinates
     * @param trianglefirstIndexOfIndice
     * @param point
     * @param m
     * @return
     */
    QVector3D getTriangleBaricentricCoordinates(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief getNormalInsideTriangle
     * @param trianglefirstIndexOfIndice
     * @param point
     * @param m
     * @return
     */
    QVector3D getNormalInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief getTexCoordinatesInsideTriangle
     * @param trianglefirstIndexOfIndice
     * @param point
     * @param m
     * @return
     */
    QVector3D getTexCoordinatesInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    //Recebe raio e vê se raio intersecta algum triângulo da malha
    /**
     * @brief intersectsWithRay
     * @param ray
     * @param m
     * @param tCloser
     * @param firstIndTri
     * @return
     */
    int intersectsWithRay(Ray ray, QMatrix4x4 m, float tCloser, int &firstIndTri);

    /**
     * @brief isRayIntersecting
     * @param ray
     * @param m
     * @return
     */
    bool isRayIntersecting(Ray ray, QMatrix4x4 m);

    /**
     * @brief computeNormals
     */
    void computeNormals();

    /**
     * @brief normalizeNormals
     */
    void normalizeNormals();

protected:
    /**
     * @brief createBuffers
     */
    virtual void createBuffers() override;

    /**
     * @brief updateVertexBuffer
     */
    virtual void updateVertexBuffer() override;

    /**
     * @brief updateTexBuffer
     */
    virtual void updateTexBuffer() override;

    /**
     * @brief createVAO
     */
    void createVAO();


protected:
    /**
     * @brief createTexture
     */
    void createTexture() override;

    /**
     * @brief _textureID
     */
    unsigned int _textureID = static_cast<unsigned int>(-1);

    /**
     * @brief _vao
     */
    QOpenGLVertexArrayObject _vao;
};
