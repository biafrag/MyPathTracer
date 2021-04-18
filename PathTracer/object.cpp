#include "object.h"

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



Object::Material Object::getMaterial()
{
    return  _material;
}



void Object::setMaterial(Object::Material material)
{
    _material = material;
}



ObjectType Object::getObjectType()
{
    return _type;
}
