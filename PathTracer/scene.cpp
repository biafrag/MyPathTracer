#include "scene.h"
#include "sphere.h"
#include "trianglemesh.h"
#include "plane.h"
#include "material.h"
#include "readerOBJ.h"

/**
 * @brief Scene
 *
 * The Scene class implements a scene.
 *
 * @author Bianca Fragoso
 */

Scene::Scene()
{
    createLights();
}



Scene::~Scene()
{

}



std::vector<Light> Scene::getLights()
{
    return _lights;
}



std::vector<Object *> Scene::getObjects()
{
    return _objects;
}



QVector3D Scene::getBackgroundColor()
{
    return _backgroundColor;
}



void Scene::createDefaultScene()
{
    _objects.clear();

    QMatrix4x4 rot, trans, scale;
    scale.scale(QVector3D(100, 100, 100));
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -100, 0));
    Plane *frontScenePlane=  new Plane(trans, rot, scale);
    Material material;
    material.setAlbedo(QVector3D(0.5, 0.5, 1));
    material.setSpecular(QVector3D(0, 0, 0));
    frontScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    // Albedo and specular color
    QVector3D color = randVec();
    float prob = rand0x1();
    float shi = rand30x200();
    bool metal = false;
    QVector3D albedo, specular;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);

    Sphere *s =  new Sphere(QVector3D(0, 0, 1));
    Material m;
    m.setAlbedo(QVector3D(0.7, 0.7, 0));
    s->setMaterial(m);
    _objects.push_back(s);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    material.setEmission(QVector3D(0, 0, 0));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s2 =  new Sphere(QVector3D(-3, 1, 0.5), 0.5);
    s2->setMaterial(material);
    _objects.push_back(s2);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s5 =  new Sphere(QVector3D(0, 2, 0.5), 0.5);
    s5->setMaterial(material);
    _objects.push_back(s5);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s6 =  new Sphere(QVector3D(3, -2, 0.5), 0.5);
    s6->setMaterial(material);
    _objects.push_back(s6);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    Sphere *s7 =  new Sphere(QVector3D(-3, -2, 0.5), 0.5);
    s7->setMaterial(material);
    _objects.push_back(s7);
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.0f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    Sphere *s3 =  new Sphere(QVector3D(3, 1, 0.5), 0.5);
    s3->setMaterial(material);
    _objects.push_back(s3);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);


    Sphere *s4 =  new Sphere(QVector3D(0, -3, 0.5), 0.5);
    s4->setMaterial(material);
    _objects.push_back(s4);

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(1, 0 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 100, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));


    Plane *backScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    backScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");


    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    rot.rotate(-90, QVector3D(0, 0 , 1));
    rot.rotate(90, QVector3D(1, 0 , 0));

    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(5, 5, 5));

    Plane *groundPlane =  new Plane(trans, rot, scale);
    Material groundMaterial;

    groundPlane->setMaterial(Material::Rubber());

    _objects.push_back(groundPlane);

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(-100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *leftScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    leftScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *rightScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    rightScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 100));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *upScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 1));
    upScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;

    rot.setToIdentity();
    rot.rotate(45, QVector3D(0, 1 , 0));

    trans.setToIdentity();
    trans.translate(QVector3D(0, -1, -2.5));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *meshPlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(0.6, 0.6, 0.6));
    meshPlane->setMaterial(material);

    TriangleMesh *t = new TriangleMesh(points, indicesTri, normals);

    if(normals.size() == 0)
    {
        t->computeNormals();
    }
    material.setAlbedo(QVector3D(0.6, 0.6, 0.9));
    t->setMaterial(material);
}



void Scene::createDefaultScene2()
{
    QMatrix4x4 rot, trans, scale;
    scale.scale(QVector3D(100, 100, 100));
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -100, 0));
    Plane *frontScenePlane=  new Plane(trans, rot, scale);
    Material material;
    material.setAlbedo(QVector3D(0.5, 0.5, 1));
    material.setSpecular(QVector3D(0, 0, 0));
    frontScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    // Albedo and specular color
    QVector3D color = randVec();
    float prob = rand0x1();
    float shi = rand30x200();
    bool metal = false;
    QVector3D albedo, specular;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);

    Sphere *s =  new Sphere(QVector3D(0, 0, 1));
    s->setMaterial(Material::SilverSmooth());
    material.setEmission(QVector3D(0.5, 0.5, 0.5));
    _objects.push_back(s);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s2 =  new Sphere(QVector3D(-3, 1, 0.5), 0.5);
    s2->setMaterial(material);
    _objects.push_back(s2);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s5 =  new Sphere(QVector3D(0, 2, 0.5), 0.5);
    s5->setMaterial(material);
    _objects.push_back(s5);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s6 =  new Sphere(QVector3D(3, -2, 0.5), 0.5);
    s6->setMaterial(material);
    _objects.push_back(s6);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    Sphere *s7 =  new Sphere(QVector3D(-3, -2, 0.5), 0.5);
    s7->setMaterial(material);
    _objects.push_back(s7);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    Sphere *s3 =  new Sphere(QVector3D(3, 1, 0.5), 0.5);
    s3->setMaterial(material);
    _objects.push_back(s3);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = true;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);


    Sphere *s4 =  new Sphere(QVector3D(0, -3, 0.5), 0.5);
    s4->setMaterial(material);
    _objects.push_back(s4);


    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    rot.rotate(-90, QVector3D(0, 0 , 1));
    rot.rotate(90, QVector3D(1, 0 , 0));

    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(5, 5, 5));

    Plane *groundPlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(0.5, 0.3, 0.3));
    groundPlane->setMaterial(Material::Rubber());

    _objects.push_back(groundPlane);

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(-100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *leftScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    leftScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *rightScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    rightScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 100));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *upScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 1));
    upScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;

    readFile("../PathTracer/Malhas/bunny2.obj", points, normals, texCoords, indicesTri,  indicesNormalsTri, indicesTexTri);

    TriangleMesh *t = new TriangleMesh(points, indicesTri, normals);

    if(normals.size() == 0)
    {
        t->computeNormals();
    }
    material.setEmission(QVector3D(1, 1, 1));

    t->setMaterial(material);
}



void Scene::createDefaultScene3()
{
    _backgroundColor = QVector3D(0.5, 0.5, 0.8);
    QMatrix4x4 rot, trans, scale;
    scale.scale(QVector3D(100, 100, 100));
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -100, 0));
    Material material;
    material.setAlbedo(QVector3D(0.5, 0.5, 1));
    material.setSpecular(QVector3D(0, 0, 0));


    // Albedo and specular color
    QVector3D color = randVec();
    float prob = rand0x1();
    float shi = rand30x200();
    bool metal = prob < 0.5f;
    QVector3D albedo, specular;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    material.setEmission(QVector3D(0, 0, 0));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.5f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s2 =  new Sphere(QVector3D(-3, 1, 0.5), 0.5);
    s2->setMaterial(Material::Light());
    _objects.push_back(s2);

    Sphere *s5 =  new Sphere(QVector3D(0, 2, 0.5), 0.5);
    s5->setMaterial(Material::Light());
    _objects.push_back(s5);

    Sphere *s6 =  new Sphere(QVector3D(3, -2, 0.5), 0.5);
    s6->setMaterial(Material::Light());
    _objects.push_back(s6);

    Sphere *s7 =  new Sphere(QVector3D(-3, -2, 0.5), 0.5);
    s7->setMaterial(Material::Light());
    _objects.push_back(s7);


    Sphere *s4 =  new Sphere(QVector3D(0, -3, 0.5), 0.5);
    s4->setMaterial(Material::Light());
    _objects.push_back(s4);

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(1, 0 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 100, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    rot.rotate(-90, QVector3D(0, 0 , 1));
    rot.rotate(90, QVector3D(1, 0 , 0));

    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(5, 5, 5));

    Plane *groundPlane =  new Plane(trans, rot, scale);
    groundPlane->setTexture(":/textures/Texturas/Ground.jpg");
    _objects.push_back(groundPlane);


    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;

    readFile("../PathTracer/Malhas/bunny2.obj", points, normals, texCoords, indicesTri,  indicesNormalsTri, indicesTexTri);

    TriangleMesh *t = new TriangleMesh(points, indicesTri, normals);

    if(normals.size() == 0)
    {
        t->computeNormals();
    }
    t->setMaterial(Material::SilverNotSmooth());
   _objects.push_back(t);
}



void Scene::createOnlyPlane()
{
    _objects.clear();

    QMatrix4x4 rot, trans, scale;
    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *frontScenePlane =  new Plane(trans, rot, scale);
    Material material;
    material.setAlbedo(QVector3D(0.5, 0.5, 1));
    material.setSpecular(QVector3D(0, 0, 0));
    frontScenePlane->setMaterial(Material::Rubber());

    _objects.push_back(frontScenePlane);

}



void Scene::createTriangleMeshAndPlaneAndSpheres()
{
    _objects.clear();

    QMatrix4x4 rot, trans, scale;

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    rot.rotate(-90, QVector3D(0, 0 , 1));
    rot.rotate(90, QVector3D(1, 0 , 0));

    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(5, 5, 5));

    Plane *groundPlane =  new Plane(trans, rot, scale);
    Material groundMaterial;
    groundMaterial.setAlbedo(QVector3D(0.7, 0.7, 0.7));
    groundPlane->setMaterial(Material::Rubber());

    _objects.push_back(groundPlane);

    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;
    readFile("../PathTracer/Malhas/bunny2.obj", points, normals, texCoords, indicesTri,  indicesNormalsTri, indicesTexTri);


    TriangleMesh *t = new TriangleMesh(points, indicesTri, normals);

    if(normals.size() == 0)
    {
        t->computeNormals();
    }
    Material material;
    material.setAlbedo(QVector3D(0.6, 0.6, 0.9));
    t->setMaterial(material);
    _objects.push_back(t);

    Sphere *s =  new Sphere(QVector3D(0, 0, 1));
    Material m;
    m.setAlbedo(QVector3D(0.7, 0.7, 0));

    s->setMaterial(m);
    _objects.push_back(s);

    QVector3D color = randVec();
    float prob = rand0x1();
    float shi = rand30x200();
    bool metal = false;
    QVector3D albedo, specular;
    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    material.setEmission(QVector3D(0, 0, 0));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s2 =  new Sphere(QVector3D(-3, 1, 0.5), 0.5);
    s2->setMaterial(material);
    _objects.push_back(s2);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    //material.setEmission(QVector3D(0, 0, 0));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s5 =  new Sphere(QVector3D(0, 2, 0.5), 0.5);
    s5->setMaterial(material);
    _objects.push_back(s5);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    Sphere *s6 =  new Sphere(QVector3D(3, -2, 0.5), 0.5);
    s6->setMaterial(material);
    _objects.push_back(s6);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    Sphere *s7 =  new Sphere(QVector3D(-3, -2, 0.5), 0.5);
    s7->setMaterial(material);
    _objects.push_back(s7);
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.0f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    Sphere *s3 =  new Sphere(QVector3D(3, 1, 0.5), 0.5);
    s3->setMaterial(material);
    _objects.push_back(s3);


    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = false;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);


    Sphere *s4 =  new Sphere(QVector3D(0, -3, 0.5), 0.5);
    s4->setMaterial(material);
    _objects.push_back(s4);

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(1, 0 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 100, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

}



void Scene::createEmptyScene()
{
    _objects.clear();

}



void Scene::createLights()
{
    Light light1;

    light1.position = {-10, 50, 50};
    light1.ambient = {0.3f, 0.3f, 0.3f};
    light1.diffuse = {1.0f, 1.0f, 1.0f};
    light1.specular = {0.3f,  0.3f, 0.3f};
    light1.shi = 60.0f;

    Light light2;
    light2.position = {50, 50, 50};
    light2.ambient = {0.3f, 0.3f, 0.3f};
    light2.diffuse = {0.8f, 0.8f, 0.8f};
    light2.specular = {0.3f, 0.3f, 0.3f};
    light2.shi = 60.0f;


    _lights.push_back(light1);

    _lights.push_back(light2);
}



QVector3D Scene::randVec()
{
    float x = ((float) std::rand() / (RAND_MAX));
    float y = ((float) std::rand() / (RAND_MAX));
    float z = ((float) std::rand() / (RAND_MAX));

    return QVector3D(x, y, z);

}



float Scene::rand0x1()
{
    return ((float) std::rand() / (RAND_MAX));
}



float Scene::rand30x200()
{
    return 30 + ( std::rand() % ( 200 - 30 + 1 ) );
}



Material Scene::randomMaterial()
{
    Material material;
    QVector3D color = randVec();
    float prob = rand0x1();
    float shi = rand30x200();
    float emission = rand0x1();
    bool metal = prob < 0.5;
    QVector3D albedo, specular;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.setShininess(shi);
    material.setEmission(QVector3D(emission, emission, emission));

    return material;
}
