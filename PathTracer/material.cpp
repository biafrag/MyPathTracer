
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


QVector3D Material::getEmission()
{
    return _emission;
}



float Material::getSmoothness()
{
    return _smoothness;

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



void Material::setEmission(QVector3D emission)
{
    _emission = emission;
}



void Material::setSmoothness(float smoothness)
{
    _smoothness = smoothness;
}



void Material::shininess(float shi)
{
    _shininess = shi;
}





void Material::setTexture(QImage texture)
{
    _hasTexture = true;
    _texture = texture;
}



 Material Material::Gold()
{
     Material m("Gold", QVector3D(0.2, 0.2, 0.2), QVector3D(1.0f, 0.78f, 0.34f), 51.2, 0.6, false);
     //m.setEmission(Q0.7,0.7,0.7});
     m.setSmoothness(1);
     return m;
}



 Material Material::Rubber()
{
     return Material("Rubber", QVector3D(0.4, 0, 0), QVector3D(1, 1, 1), 60, 1, false);
 }



 Material Material::Mirror()
 {
     Material m("Mirror", QVector3D(0, 0, 0), QVector3D(1, 1, 1), 70, 0, false);
     m.setSmoothness(0);

     return m;

 }
