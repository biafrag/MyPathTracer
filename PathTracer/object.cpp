#include "object.h"
/**
 * @brief Object
 *
 * The Object class its an abstract class that implements an renderable object.
 *
 * @author Bianca Fragoso
 */

Object::Object()
{
}



std::vector<unsigned int> Object::getIndices()
{
    return _indices;
}



std::vector<QVector3D> Object::getVertices(QMatrix4x4 matrix)
{
    std::vector<QVector3D> vertices;
    for(auto vertice : _points)
    {
        vertices.push_back(matrix * vertice);
    }
    return vertices;
}



std::vector<QVector3D> Object::getNormals(QMatrix4x4 matrix)
{
    std::vector<QVector3D> normals;
    for(auto normal : _normals)
    {
        normals.push_back(matrix * normal);
    }
    return normals;
}



std::vector<QVector2D> Object::getTexCoordinates()
{
    return _texCoords;
}



Material Object::getMaterial()
{
    return  _material;
}



void Object::setMaterial(Material material)
{
    _material = material;
}



ObjectType Object::getObjectType()
{
    return _type;
}



void Object::setTexture(QString path)
{
    _material.setTexture(QImage(path));
}



void Object::createTexture()
{

}
