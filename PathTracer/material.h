#pragma once
#include <QVector3D>
#include <QImage>
#include <QString>

/**
 * @brief The material class implements an object material. A material has a name, an albedo, and
 * other important properties that describe a physic material. The material can also have
 * a texture. If it happens, a image with the texture is stored.
 * The class implements some pre factored materials like gold, mirror, rubber etc.
 *
 * @author Bianca Fragoso
 */

class Material
{
public:
    /**
     * @brief The material default constructor.
     */
    Material();

    /**
     * @brief The material constructor.
     * @param name The name of the material.
     * @param albedo The albedo of the material.
     * @param specular The specular of the material.
     * @param shininess The shininess of the material.
     * @param reflectivity The reflectivity of the material.
     * @param hasTexture True if the material has texture and false if its not.
     */
    Material(QString name, QVector3D albedo, QVector3D specular, float shininess, float reflectivity, bool hasTexture = false);

    /**
     * @brief Gets the material's name.
     * @return A string with the name.
     */
    const QString &getName();

    /**
     * @brief Gets the material's albedo.
     * @return A 3D vector with the albedo value.
     */
    const QVector3D getAlbedo();

    /**
     * @brief Gets the material's specular.
     * @return A 3D vector with the specular value.
     */
    const QVector3D getSpecular();

    /**
     * @brief Gets the material's shininess.
     * @return A float with the shininess value.
     */
    float getShi();

    /**
     * @brief Gets the material's reflectivity.
     * @return A float with a number between 0 and 1.
     */
    float getReflectivity();

    /**
     * @brief Gets the material's emission.
     * @return A 3D vector with the emission value.
     */
    QVector3D getEmission();

    /**
     * @brief Gets the material's smoothness.
     * @return A float with the smoothness value.
     */
    float getSmoothness();

    /**
     * @brief Returns a boolean indicating if the material has texture or not.
     * @return True if the material has texture and false if its not.
     */
    bool hasTexture() const;

    /**
     * @brief Gets the material's texture if it has one.
     * @return A image with the texture.
     */
    QImage getTexture();

    /**
     * @brief Sets the material's name.
     * @param name The new name.
     */
    void setName(QString name);

    /**
     * @brief Sets the material's albedo.
     * @param albedo The new albedo value.
     */
    void setAlbedo(QVector3D albedo);

    /**
     * @brief Sets the material's specular.
     * @param specular Thew new specular value.
     */
    void setSpecular(QVector3D specular);

    /**
     * @brief Sets the material's reflectivity.
     * @param reflectivity Thew new reflectivity value.
     */
    void setReflectivity(float reflectivity);

    /**
     * @brief Sets the material's emission.
     * @param emission Thew new emission value.
     */
    void setEmission(QVector3D emission);

    /**
     * @brief Sets the material's smoothness.
     * @param smoothness Thew new smoothness value.
     */
    void setSmoothness(float smoothness);

    /**
     * @brief Sets the material's shininess.
     * @param shi Thew new shininess value.
     */
    void setShininess(float shi);

    /**
     * @brief Sets the material's texture.
     * @param texture Thew new texture image.
     */
    void setTexture(QImage texture);

    /**
     * @brief Creates a material that has the exact properties to simulate Gold.
     * @return A static gold material.
     */
    static Material Gold();

    /**
     * @brief Creates a material that has the exact properties to simulate Rubber.
     * @return A static rubber material.
     */
    static Material Rubber();

    /**
     * @brief Creates a material that has the exact properties to simulate Mirror.
     * @return A static mirror material.
     */
    static Material Mirror();



private:
    /**
     * @brief The material's name.
     */
    QString _name;

    /**
     * @brief Albedo is the color of the material.
     */
    QVector3D _albedo = {1, 1, 1};

    /**
     * @brief Specular is the reflection color.
     */
    QVector3D _specular = {0, 0, 0};

    /**
     * @brief It's the indicator that material is reflective or not.
     */
    float _reflectivity = 1;

    /**
     * @brief The material property that determines the size and sharpness of specular highlights
     */
    float _shininess = 60.0f;

    /**
     * @brief _shininess
     */
    float _smoothness = 1.0f;

    /**
     * @brief A boolean that indicates if a material has texture or not.
     */
    bool _hasTexture = false;

    /**
     * @brief A image with the material's texture if it has texture.
     */
    QImage _texture;

    /**
     * @brief If the emission is 0, the material don't emit light at all.
     * If all the coordinates are 1 the material emits white light with the maximum intensity.
     */
    QVector3D _emission = {0, 0, 0};
};

