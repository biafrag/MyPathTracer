#pragma once
#include "object.h"
#include <vector>
#include "light.h"

class Scene
{
public:
    Scene();
    ~Scene();
    std::vector<Light>getLights();
    std::vector<Object *> getObjects();




private:
    void createDefaultScene();
    void createLights();

    /**
     * @brief randVec
     * @return
     */
    QVector3D randVec();

    /**
     * @brief rand0x1
     * @return
     */
    float rand0x1();

    /**
     * @brief rand30x200
     * @return
     */
    float rand30x200();

    Material randomMaterial();

private:
    std::vector<Light> _lights;
    std::vector<Object *> _objects;
};

