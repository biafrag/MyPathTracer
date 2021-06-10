#pragma once
#include <QVector3D>
#include <QImage>
#include <QString>

class Material
{
public:
    /**
     * @brief Material
     */
    Material();

    /**
     * @brief Material
     * @param name
     * @param albedo
     * @param specular
     * @param shininess
     * @param reflectivity
     * @param hasTexture
     */
    Material(QString name, QVector3D albedo, QVector3D specular, float shininess, float reflectivity, bool hasTexture = false);

    /**
     * @brief getName
     * @return
     */
    const QString &getName();

    /**
     * @brief getAlbedo
     * @return
     */
    const QVector3D getAlbedo();

    /**
     * @brief getSpecular
     * @return
     */
    const QVector3D getSpecular();

    /**
     * @brief getShi
     * @return
     */
    float getShi();

    /**
     * @brief getReflectivity
     * @return
     */
    float getReflectivity();

    /**
     * @brief getEmission
     * @return
     */
    QVector3D getEmission();

    /**
     * @brief getSmoothness
     * @return
     */
    float getSmoothness();

    /**
     * @brief hasTexture
     * @return
     */
    bool hasTexture() const;

    /**
     * @brief getTexture
     * @return
     */
    QImage getTexture();

    /**
     * @brief setName
     * @param name
     */
    void setName(QString name);

    /**
     * @brief setAlbedo
     * @param albedo
     */
    void setAlbedo(QVector3D albedo);

    /**
     * @brief setSpecular
     * @param specular
     */
    void setSpecular(QVector3D specular);

    /**
     * @brief setReflectivity
     * @param reflectivity
     */
    void setReflectivity(float reflectivity);

    /**
     * @brief setEmission
     * @param emission
     */
    void setEmission(QVector3D emission);

    /**
     * @brief setSmoothness
     * @param smoothness
     */
    void setSmoothness(float smoothness);

    /**
     * @brief shininess
     * @param shi
     */
    void shininess(float shi);

    /**
     * @brief setTexture
     * @param texture
     */
    void setTexture(QImage texture);

    /**
     * @brief Gold
     * @return
     */
    static Material Gold();

    /**
     * @brief Rubber
     * @return
     */
    static Material Rubber();

    /**
     * @brief Mirror
     * @return
     */
    static Material Mirror();



private:
    /**
     * @brief _name
     */
    QString _name;

    /**
     * @brief _albedo
     */
    QVector3D _albedo = {1, 1, 1};

    /**
     * @brief _specular
     */
    QVector3D _specular = {0, 0, 0};

    /**
     * @brief _reflectivity
     */
    float _reflectivity = 1;

    /**
     * @brief _shininess
     */
    float _shininess = 60.0f;

    /**
     * @brief _shininess
     */
    float _smoothness = 1.0f;

    /**
     * @brief _hasTexture
     */
    bool _hasTexture = false;

    /**
     * @brief _texture
     */
    QImage _texture;

    /**
     * @brief _emission
     */
    QVector3D _emission = {0, 0, 0};
};

