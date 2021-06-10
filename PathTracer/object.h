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
 * @brief The Object class
 */
class Object : public QObject, protected QOpenGLExtraFunctions
{
public:

    /**
     * @brief Object
     */
    Object();

    /**
     * @brief initialize
     */
    virtual void initialize() = 0;

    /**
     * @brief render
     * @param projMatrix
     * @param viewMatrix
     * @param modelMatrix
     * @param lights
     */
    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) = 0;

    /**
     * @brief createBuffers
     */
    virtual void createBuffers() = 0;

    /**
     * @brief updateVertexBuffer
     */
    virtual void updateVertexBuffer() = 0;

    /**
     * @brief updateTexBuffer
     */
    virtual void updateTexBuffer() = 0;

    /**
     * @brief createVAO
     */
    void createVAO();

    /**
     * @brief getIndices
     * @return
     */
    std::vector <unsigned int> getIndices();

    /**
     * @brief getVertices
     * @param matrix
     * @return
     */
    std::vector <QVector3D> getVertices(QMatrix4x4 matrix = QMatrix4x4());

    /**
     * @brief getNormals
     * @param matrix
     * @return
     */
    std::vector <QVector3D> getNormals(QMatrix4x4 matrix = QMatrix4x4());

    /**
     * @brief getTexCoordinates
     * @return
     */
    std::vector<QVector2D> getTexCoordinates();

    /**
     * @brief getMaterial
     * @return
     */
    Material getMaterial();

    /**
     * @brief setMaterial
     * @param material
     */
    void setMaterial(Material material);

    /**
     * @brief getObjectType
     * @return
     */
    ObjectType getObjectType();
    
    /**
     * @brief setTexture
     * @param path
     */
    virtual void setTexture(QString path);


protected:

    /**
     * @brief createTexture
     */
    virtual void createTexture();

    /**
     * @brief OpenGL program used to render meshes without shading.
     */
    QOpenGLShaderProgram *_program {nullptr};

    /**
     * @brief _texCoordsBuffer
     */
    unsigned int _texCoordsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _pointsBuffer
     */
    unsigned int _pointsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _normalsBuffer
     */
    unsigned int _normalsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _indicesBuffer
     */
    unsigned int _indicesBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _material
     */
    Material _material;


    /**
     * @brief _points
     */
    std::vector<QVector3D> _points;

    /**
     * @brief _indices
     */
    std::vector<unsigned int> _indices;

    /**
     * @brief _normals
     */
    std::vector<QVector3D> _normals;

    /**
     * @brief _texCoords
     */
    std::vector<QVector2D> _texCoords;

    /**
     * @brief _type
     */
    ObjectType _type;
};
