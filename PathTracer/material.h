#pragma once
#include <QVector3D>
#include <QImage>
#include <QString>

class Material
{
public:
    Material();

    Material(QString name, QVector3D albedo, QVector3D specular, float shininess, float reflectivity, bool hasTexture = false);

    const QString &getName();
    const QVector3D getAlbedo();
    const QVector3D getSpecular();
    float getShi();
    float getReflectivity();

    bool hasTexture();
    QImage getTexture();

    void setName(QString name);
    void setAlbedo(QVector3D albedo);
    void setSpecular(QVector3D specular);
    void setReflectivity(float reflectivity);

    void shininess(float shi);
    void setTexture(QImage texture);

    static Material Gold();
    static Material Rubber();


private:
    QString _name;
    QVector3D _albedo = {1, 1, 1};
    QVector3D _specular = {1, 1, 1};
    float _reflectivity = 1;
    float _shininess = 60.0f;
    bool _hasTexture = false;
    QImage _texture;
};

