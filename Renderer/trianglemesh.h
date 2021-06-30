#pragma once

#include "object.h"
#include <QOpenGLVertexArrayObject>

/**
 * @brief The TriangleMesh class knows how to render all kinds of triangle meshes.
 * This class also has functions that gives some mesh informations, like if a ray hits some of it triangles.
 *
 * @author Bianca Fragoso
 */
class TriangleMesh : public Object
{
public:
    /**
     * @brief The TriangleMesh default constructor.
     */
    TriangleMesh() = default;

    /**
     * @brief The constructor that creates a TriangleMesh with the given points indices and normals.
     * @param points The points of the new mesh.
     * @param indices The indices of the new mesh.
     * @param normals The normals of the new mesh.
     */
    TriangleMesh(std::vector<QVector3D> &points, std::vector<unsigned int> &indices, std::vector<QVector3D> &normals);

    /**
     * @brief Initializes the object structures and the openGl functions.
     */
    virtual void initialize() override;

    /**
     * @brief Draws the object on the screen, mostly using openGl functions.
     * @param projMatrix The projection matrix.
     * @param viewMatrix The view matrix.
     * @param modelMatrix The model matrix.
     * @param lights The lights on the scene that the object its included.
     */
    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) override;


    /**
     * @brief Checks if a point its in a given triangle of the mesh.
     * @param trianglefirstIndexOfIndice The index of the first index of the triangle in focus.
     * @param point The point that its checked.
     * @param m The transformation matrix.
     * @return True if the point its inside the triangle and false if its not.
     */
    bool triangleVerification(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief Gets the normal of the given triangle.
     * @param trianglefirstIndexOfIndice The index of the first index of the triangle in focus.
     * @param m The transformation matrix
     * @return A 3D vector with the normal of the triangle.
     */
    QVector3D getTriangleNormal(unsigned int trianglefirstIndexOfIndice, QMatrix4x4 m);

    /**
     * @brief Gets the baricentric coordinates of the given triangle.
     * @param trianglefirstIndexOfIndice The index of the first index of the triangle in focus.
     * @param point The point that is used to compute the coordinates.
     * @param m The transformation matrix.
     * @return A 3D vector with the baricentric coordinates.
     */
    QVector3D getTriangleBaricentricCoordinates(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief Gets the normal on some point inside a triangle of the mesh.
     * @param trianglefirstIndexOfIndice The index of the first index of the triangle in focus.
     * @param point The point that the normal is computed.
     * @param m The transformation matrix.
     * @return A 3D vector with the normal.
     */
    QVector3D getNormalInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief Gets the the texture coordinate of a point inside of a triangle of the mesh.
     * @param trianglefirstIndexOfIndice The index of the first index of the triangle in focus.
     * @param point The point that the texture coordinates is computed.
     * @param m The transformation matrix.
     * @return A 3D vector with the texture coordinates.
     */
    QVector3D getTexCoordinatesInsideTriangle(unsigned int trianglefirstIndexOfIndice, QVector3D point, QMatrix4x4 m);

    /**
     * @brief Receives a ray and checks if the ray intersects with a triangle of the mesh.
     * @param ray The ray to be checked.
     * @param m The transformation matrix.
     * @param tCloser The minor distance that the ray has reached some other object or triangle.
     * @param firstIndTri The index of the first index of the triangle in focus.
     * @return The new minor distance. If the ray intersects with the triangle in focus with a minor
     * distance, that distance is returned. If the ray doesn't intersects with the triangle or intersects
     * with it but the distance is greater that @p tCloser, tCloser is returned.
     */
    int intersectsWithRay(Ray ray, QMatrix4x4 m, float tCloser, int &firstIndTri);

    /**
     * @brief Checks if a ray intersects with the mesh.
     * @param ray The ray that will be checked.
     * @param m The transformation matrix.
     * @return Returns True if the ray intersects the mesh and false if its not.
     */
    bool isRayIntersecting(Ray ray, QMatrix4x4 m);

    /**
     * @brief Computes the normals for each vertex of the mesh.
     */
    void computeNormals();

    /**
     * @brief Normalizes the normals vector.
     */
    void normalizeNormals();

protected:
    /**
     * @brief Creates the points, normals, texture coordinates and indexes buffers
     */
    virtual void createBuffers() override;

    /**
     * @brief Updates the points and the normal buffers.
     */
    virtual void updateVertexBuffer() override;

    /**
     * @brief Updates the texture coordinates buffer.
     */
    virtual void updateTexBuffer() override;

    /**
     * @brief Creates the vertex array object that holds all the buffers created.
     */
    void createVAO();


protected:
    /**
     * @brief Creates a new texture with OpenGL.
     */
    void createTexture() override;

    /**
     * @brief The texture ID.
     */
    unsigned int _textureID = static_cast<unsigned int>(-1);

    /**
     * @brief The vertex array object.
     */
    QOpenGLVertexArrayObject _vao;
};
