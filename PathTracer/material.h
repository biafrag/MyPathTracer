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
    QVector3D getEmission();
    float getSmoothness();

    const bool hasTexture();
    QImage getTexture();

    void setName(QString name);
    void setAlbedo(QVector3D albedo);
    void setSpecular(QVector3D specular);
    void setReflectivity(float reflectivity);
    void setEmission(QVector3D emission);
    void setSmoothness(float smoothness);

    void shininess(float shi);
    void setTexture(QImage texture);

    static Material Gold();
    static Material Rubber();
    static Material Mirror();



private:
    QString _name;
    QVector3D _albedo = {1, 1, 1};
    QVector3D _specular = {0, 0, 0};
    float _reflectivity = 1;
    float _shininess = 60.0f;
    float _smoothness = 1.0f;

    bool _hasTexture = false;
    QImage _texture;
    QVector3D _emission = {0, 0, 0};
};

