#include "material.h"

/**
 * @class Material
 *
 * The Material class is responsible to store properties of a material.
 *
 * @author Bianca Fragoso
 */

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


QVector3D Material::getEmission()
{
    return _emission;
}



float Material::getSmoothness()
{
    return _smoothness;

}



 bool Material::hasTexture() const
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



void Material::setEmission(QVector3D emission)
{
    _emission = emission;
}



void Material::setSmoothness(float smoothness)
{
    _smoothness = smoothness;
}



void Material::setShininess(float shi)
{
    _shininess = shi;
}





void Material::setTexture(QImage texture)
{
    _hasTexture = true;
    _texture = texture;
}



 Material Material::SilverSmooth()
{
     Material m("SilverSmooth", QVector3D(0.3, 0.3, 0.3), QVector3D(0.7f, 0.7f, 0.7f), 60, 0.5, false);
     m.setEmission(QVector3D(0.2, 0.2, 0.2));
     m.setSmoothness(1);
     return m;
 }



 Material Material::SilverNotSmooth()
 {
     Material m("SilverNotSmooth", QVector3D(0.7, 0.7, 0.7), QVector3D(0.7f, 0.7f, 0.7f), 60, 0.8, false);
     m.setEmission(QVector3D(0.2, 0.2, 0.2));
     m.setSmoothness(0.8);
     return m;
 }



 Material Material::Rubber()
{
     return Material("Rubber", QVector3D(0.8, 0.4, 0.4), QVector3D(0, 0, 0), 60, 1, false);
 }



 Material Material::Mirror()
 {
     Material m("Mirror", QVector3D(0.3, 0.3, 0.3), QVector3D(1, 1, 1), 60, 0, false);
     return m;

 }



 Material Material::Light()
 {
     Material m("Light", QVector3D(1, 1, 1), QVector3D(0, 0, 0), 60, 1, false);
     m.setEmission(QVector3D(1, 1, 1));

     return m;
 }
