#pragma once

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include "light.h"
#include "ray.h"
#include "ObjectType.h"
#include <QImage>
#include "material.h"

/**
 * @brief The object class implements an object that knows how to
 * render itself (mostly using Opengl). An object usualy is a mesh
 * that has points, normals and texture coordinates. An object also is made of a material.
 *
 * @author Bianca Fragoso
 */
class Object : public QObject, protected QOpenGLExtraFunctions
{
public:

    /**
     * @brief The Object default constructor.
     */
    Object();

    /**
     * @brief Initializes the object structures and the openGl functions.
     */
    virtual void initialize() = 0;

    /**
     * @brief Draws the object on the screen, mostly using openGl functions.
     * @param projMatrix The projection matrix.
     * @param viewMatrix The view matrix.
     * @param modelMatrix The model matrix.
     * @param lights The lights on the scene that the object its included.
     */
    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) = 0;

    /**
     * @brief Creates the points, normals, texture coordinates and indexes buffers
     */
    virtual void createBuffers() = 0;

    /**
     * @brief Updates the points and the normal buffers.
     */
    virtual void updateVertexBuffer() = 0;

    /**
     * @brief Updates the texture coordinates buffer.
     */
    virtual void updateTexBuffer() = 0;

    /**
     * @brief Creates the vertex array object that holds all the buffers created.
     */
    void createVAO();

    /**
     * @brief Gets the object indices.
     * @return A vector with the indices.
     */
    std::vector <unsigned int> getIndices();

    /**
     * @brief Gets the object vertices multiplied by the @p matrix .
     * @param matrix The matrix that is used to make a transformation on the vertices.
     * @return A vector of 3D vectors with the vertices transformated by the given matrix.
     */
    std::vector <QVector3D> getVertices(QMatrix4x4 matrix = QMatrix4x4());

    /**
     * @brief Gets the object's normals multiplied by the @p matrix .
     * @param matrix The matrix that is used to make a transformation on the normals.
     * @return  A vector of 3D vectors with the normals transformated by the given matrix.
     */
    std::vector <QVector3D> getNormals(QMatrix4x4 matrix = QMatrix4x4());

    /**
     * @brief Gets the object's texture coordinates.
     * @return  A vector of 2D vectors with the texture coordinates.
     */
    std::vector<QVector2D> getTexCoordinates();

    /**
     * @brief Gets the object's material.
     * @return A material.
     */
    Material getMaterial();

    /**
     * @brief Sets a new material.
     * @param material The new material.
     */
    void setMaterial(Material material);

    /**
     * @brief Gets the object type.
     * @return An object type: Sphere, Mesh or Plane.
     */
    ObjectType getObjectType();
    
    /**
     * @brief Sets the new texture.
     * @param path The path to the texture image.
     */
    virtual void setTexture(QString path);


protected:

    /**
     * @brief Creates a new texture with OpenGL.
     */
    virtual void createTexture();

    /**
     * @brief OpenGL program used to render meshes.
     */
    QOpenGLShaderProgram *_program {nullptr};

    /**
     * @brief The texture coordinates buffer, used to store the texture coordinates info.
     */
    unsigned int _texCoordsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief The points buffer, used to store the vertices info.
     */
    unsigned int _pointsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief The normals buffer, used to store the normals info.
     */
    unsigned int _normalsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief The indices buffer, used to store the indices info.
     */
    unsigned int _indicesBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief The physic material.
     */
    Material _material;


    /**
     * @brief The points of the object's mesh.
     */
    std::vector<QVector3D> _points;

    /**
     * @brief The indices of the object's mesh.
     */
    std::vector<unsigned int> _indices;

    /**
     * @brief The vector that holds the normals for each vertice.
     */
    std::vector<QVector3D> _normals;

    /**
     * @brief The texture coordinates.
     */
    std::vector<QVector2D> _texCoords;

    /**
     * @brief The object type. Can be Sphere, Plane or Mesh.
     */
    ObjectType _type;
};
