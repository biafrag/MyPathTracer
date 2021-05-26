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

class Object : public QObject, protected QOpenGLExtraFunctions
{
public:

    Object();

    virtual void initialize() = 0;

    virtual void render(const QMatrix4x4 &projMatrix,
                        const QMatrix4x4 &viewMatrix, const QMatrix4x4 &modelMatrix, const std::vector<Light> &lights) = 0;

    virtual void createBuffers() = 0;
    virtual void updateVertexBuffer() = 0;
    virtual void updateTexBuffer() = 0;

    void createVAO();

    std::vector <unsigned int> getIndices();
    std::vector <QVector3D> getVertices(QMatrix4x4 matrix = QMatrix4x4());
    std::vector <QVector3D> getNormals(QMatrix4x4 matrix = QMatrix4x4());
    std::vector<QVector2D> getTexCoordinates();

    Material getMaterial();

    void setMaterial(Material material);

    ObjectType getObjectType();
    
    virtual void setTexture(QString path);


protected:
    virtual void createTexture();

    /**
     * @brief OpenGL program used to render meshes without shading.
     */
    QOpenGLShaderProgram *_program {nullptr};

    unsigned int _texCoordsBuffer = static_cast<unsigned int>(-1);
    unsigned int _pointsBuffer = static_cast<unsigned int>(-1);
    unsigned int _normalsBuffer = static_cast<unsigned int>(-1);
    unsigned int _indicesBuffer = static_cast<unsigned int>(-1);

    Material _material;


    std::vector<QVector3D> _points;
    std::vector<unsigned int> _indices;
    std::vector<QVector3D> _normals;
    std::vector<QVector2D> _texCoords;

    ObjectType _type;
};
