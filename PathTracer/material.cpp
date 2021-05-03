
#include "material.h"

Material::Material()
{

}

Material::Material(QString name, QVector3D albedo, QVector3D specular, float shininess, float reflectivity, bool hasTexture)
    : _name(name),
      _albedo(albedo),
      _specular(specular),
      _reflectivity(reflectivity),
      _shininess(shininess),
      _hasTexture(hasTexture)
{

}



const QString &Material::getName()
{
    return _name;
}



const QVector3D Material::getAlbedo()
{
    return _albedo;
}



const QVector3D Material::getSpecular()
{
    return _specular;
}



float Material::getShi()
{
    return _shininess;
}



float Material::getReflectivity()
{
    return _reflectivity;
}



bool Material::hasTexture()
{
    return _hasTexture;
}



QImage Material::getTexture()
{
    return _texture;
}



void Material::setName(QString name)
{
    _name = name;
}



void Material::setAlbedo(QVector3D albedo)
{
    _albedo = albedo;
}



void Material::setSpecular(QVector3D specular)
{
    _specular = specular;
}



void Material::setReflectivity(float reflectivity)
{
    _reflectivity = reflectivity;
}



void Material::setTexture(QImage texture)
{
    _hasTexture = true;
    _texture = texture;
}



 Material Material::Gold()
{
    //return Material("Gold", QVector3D(0, 0, 0), QVector3D(1.0f, 0.78f, 0.34f), 51.2, 0, false);
     return Material("Gold", QVector3D(1, 0.88, 0.43), QVector3D(1.0f, 0.78f, 0.34f), 51.2, 0.6, false);

}



 Material Material::Rubber()
{
     return Material("Rubber", QVector3D(0.4, 0, 0), QVector3D(1, 1, 1), 60, 1, false);
 }



 Material Material::Mirror()
 {
     return Material("Mirror", QVector3D(0, 0, 0), QVector3D(1, 1, 1), 60, 0, false);

 }
