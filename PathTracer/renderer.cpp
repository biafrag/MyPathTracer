#include "renderer.h"
#define M_PI 3.14159265358979323846
#include <QMouseEvent>
#include "raytracing.h"
#include "pathtracing.h"

#include "sphere.h"
#include "plane.h"
#include "trianglemesh.h"
#include "readerOBJ.h"
#include "material.h"
#include <ctime>

Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    std::srand(3000); //use current time as seed for random generator

    _camera.center = QVector3D(0.f,0.f,0.f);
    _camera.eye =  QVector3D(0.f,0.f,10.f);
    _camera.up = QVector3D(0.f,2.f,0.f);
    _camera.zNear = 0.01f;
    _camera.zFar  = 1000.f;
    _camera.fov  = 60.f;

    _light.position = {-10, 50, 50};
    _light.ambient = {0.3f, 0.3f, 0.3f};
    _light.diffuse = {1.0f, 1.0f, 1.0f};
    _light.specular = {0.3f,  0.3f, 0.3f};
    _light.shi = 60.0f;

    Light light2;
    light2.position = {50, 50, 50};
    light2.ambient = {0.3f, 0.3f, 0.3f};
    light2.diffuse = {0.8f, 0.8f, 0.8f};
    light2.specular = {0.3f, 0.3f, 0.3f};
    light2.shi = 60.0f;

    Light light3;
    light3.position = {150, -150, -150};
    light3.ambient = {0.3f, 0.3f, 0.3f};
    light3.diffuse = {0.8f, 0.8f, 0.8f};
    light3.specular = {0.3f, 0.3f, 0.3f};
    light3.shi = 60.0f;

    _lights.push_back(_light);

    _lights.push_back(light2);
    //_lights.push_back(light3);

    _pathTracer = new PathTracing();
    _rayTracer = new RayTracing();

}



Renderer::~Renderer()
{
    delete _pathTracer;
    delete _rayTracer;
}



QImage Renderer::getRayTracedImage(float &time)
{
    //_rayTracer = new RayTracing(width(), height(), _model,  _camera, _objects, _lights, QVector3D(0, 0, 0));
    QImage image = _rayTracer->generateRayTracingImage(width(), height(), _model,  _camera, _objects, _lights, QVector3D(0, 0, 0));
    time = _rayTracer->getTime();
    return image;

}



QImage Renderer::getPathTracedImage(float &time)
{
    //_pathTracer = new PathTracing(width(), height(), _model,  _camera, _objects, _lights, QVector3D(0.9, 0.9, 0.9));
    QImage image = _pathTracer->generatePathTracingImage(width(), height(), _model,  _camera, _objects, QVector3D(0.9, 0.9, 0.9));
    time = _pathTracer->getTime();
    return image;
}



void Renderer::setNumberOfRays(unsigned int rays)
{
    _pathTracer->setRayNumber(rays);
}



void Renderer::createScene()
{
    QMatrix4x4 rot, trans, scale;
    scale.scale(QVector3D(100, 100, 100));
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -100, 0));
    Plane *frontScenePlane=  new Plane(trans, rot, scale);
    Material material;
    material.setAlbedo(QVector3D(0.5, 0.5, 1));
    material.setSpecular(QVector3D(0, 0, 0));
    //frontScenePlane->setMaterial(Material::Rubber());
    frontScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    //_objects.push_back(frontScenePlane);
//    material.setAlbedo(QVector3D(0.5, 0, 0));
    //material.color = QVector3D(1, 1, 1);

    // Albedo and specular color
    QVector3D color = randVec();
    float prob = rand0x1();
    float shi = rand30x200();
    bool metal = prob < 0.5f;
    QVector3D albedo, specular;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
//    material.setAlbedo(albedo);
//    material.setSpecular(specular);
//    material.shininess(shi);

//    specular = QVector3D(1,0,0);
//    material.setSpecular(specular);

    Sphere *s =  new Sphere(QVector3D(0, 0, 1));
    s->setMaterial(Material::Gold());
    material.setEmission(QVector3D(0.5, 0.5, 0.5));

   // s->setMaterial(material);

    _objects.push_back(s);

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
    material.shininess(shi);

    Sphere *s2 =  new Sphere(QVector3D(-3, 1, 0.5), 0.5);
    s2->setMaterial(material);
    _objects.push_back(s2);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    //material.setEmission(QVector3D(0, 0, 0));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.5f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.shininess(shi);

    Sphere *s5 =  new Sphere(QVector3D(0, 2, 0.5), 0.5);
    s5->setMaterial(material);
    _objects.push_back(s5);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.5f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.shininess(shi);

    Sphere *s6 =  new Sphere(QVector3D(3, -2, 0.5), 0.5);
    s6->setMaterial(material);
    _objects.push_back(s6);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.5f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.shininess(shi);
    Sphere *s7 =  new Sphere(QVector3D(-3, -2, 0.5), 0.5);
    s7->setMaterial(material);
    _objects.push_back(s7);
//    color = randVec();
//    prob = rand0x1();
//    shi = rand30x200();
//    metal = prob < 0.0f;
//    albedo = metal ? QVector3D(0, 0, 0) : color;
//    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
//    material.setAlbedo(albedo);
//    material.setSpecular(specular);
//    material.shininess(shi);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.5f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.shininess(shi);
    Sphere *s3 =  new Sphere(QVector3D(3, 1, 0.5), 0.5);
    s3->setMaterial(material);
    _objects.push_back(s3);

//    color = randVec();
//    prob = rand0x1();
//    shi = rand30x200();
//    metal = prob < 0.0f;
//    albedo = metal ? QVector3D(0, 0, 0) : color;
//    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
//    material.setAlbedo(albedo);
//    material.setSpecular(specular);
//    material.shininess(shi);

    material.setAlbedo( QVector3D(1, 0.3, 0.7));
    color = randVec();
    prob = rand0x1();
    shi = rand30x200();
    metal = prob < 0.5f;
    albedo = metal ? QVector3D(0, 0, 0) : color;
    specular = metal ? color : QVector3D(0.04f, 0.04f, 0.04f);
    material.setAlbedo(albedo);
    material.setSpecular(specular);
    material.shininess(shi);


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
    //backScenePlane->setMaterial(Material::Rubber());
    backScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    //_objects.push_back(backScenePlane);

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
    groundPlane->setMaterial(material);
    //groundPlane->setMaterial(Material::Mirror());
    //groundPlane->setTexture(":/textures/Texturas/Ground.jpg");
    //groundPlane->setTexture(":/textures/Texturas/Sky4.jpg");

    _objects.push_back(groundPlane);

    rot.setToIdentity();
    rot.rotate(-90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(-100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *leftScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    //leftScenePlane->setMaterial(Material::Rubber());
    leftScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    //_objects.push_back(leftScenePlane);

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *rightScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 0));
    //rightScenePlane->setMaterial(Material::Rubber());
    rightScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

    //_objects.push_back(rightScenePlane);


    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 100));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *upScenePlane =  new Plane(trans, rot, scale);
    material.setAlbedo(QVector3D(1, 1, 1));
    //upScenePlane->setMaterial(Material::Rubber());
    upScenePlane->setTexture(":/textures/Texturas/Sky4.jpg");

   //_objects.push_back(upScenePlane);

    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;

     //material.isReflective = true;
     //material.color = QVector3D(0.5,0.5,0.5);

    readFile("../PathTracer/Malhas/bunny2.obj", points, normals, texCoords, indicesTri,  indicesNormalsTri, indicesTexTri);

    TriangleMesh *t = new TriangleMesh(points, indicesTri, normals);

    if(normals.size() == 0)
    {
        t->computeNormals();
    }
    //material = Material::Gold();
    material.setEmission(QVector3D(1, 1, 1));

    t->setMaterial(material);
   //_objects.push_back(t);
}



QVector3D Renderer::Points_Sphere(QVector3D pointT)
{
    QVector3D pointf;
    double r,s;
    pointf.setX((pointT.x()-(width()/2.0))/_radius);
    pointf.setY((pointT.y()-(height()/2))/_radius);
    r=pointf.x()*pointf.x()+pointf.y()*pointf.y();

    if(r>1.0)
    {
        s=1.0/sqrt(r);
        pointf.setX(s*pointf.x());
        pointf.setY(s*pointf.y());
        pointf.setZ(0);
    }
    else
    {
        pointf.setZ(sqrt(1.0-r));
    }
    return pointf;
}



void Renderer::mousePressEvent(QMouseEvent *event)
{
    if(_mousepress == false && event->button() == Qt::LeftButton)
    {
       _mousepress=true;
       QVector3D point( event->x(), height()-event->y(), 0 ); // Pegando o ponto que está na tela
       point.setZ(0.f);
       _p0 = Points_Sphere(point);
    }
    update();
}



void Renderer::mouseReleaseEvent(QMouseEvent *event)
{
    _mousepress = false;
    update();
}



void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    if(_mousepress==true)
    {

            QVector3D point( event->x(), height()-event->y(), 0 ); // Pegando o ponto que está na tela
            point.setZ(0.f);
            _p1=Points_Sphere(point);
            QVector3D pt0(_p0);
            QVector3D pt1(_p1);
            QQuaternion Q0(0,pt0);
            QQuaternion Q1(0,pt1);
            QQuaternion Qrot= Q1 * Q0.conjugated();
            QMatrix4x4 Matrot;
            Matrot = QMatrix4x4(Qrot.toRotationMatrix());
            _model=Matrot*_model;
            _p0 = _p1;
    }

    update();
}



void Renderer::wheelEvent(QWheelEvent *event)
{
    //Aqui o zoom
      if(event->delta() > 0) //Quer dizer que estou rolando para cima-> zoom in
      {
         _camera.eye = _camera.eye*0.8;
      }
      else if(event->delta() < 0) //Quer dizer que estou rolando para baixo-> zoom out
      {
           _camera.eye = _camera.eye/0.8;
      }
      update();
}



QVector3D Renderer::randVec()
{
    float x = ((float) std::rand() / (RAND_MAX));
    float y = ((float) std::rand() / (RAND_MAX));
    float z = ((float) std::rand() / (RAND_MAX));

    return QVector3D(x, y, z);

}



float Renderer::rand0x1()
{
    return ((float) std::rand() / (RAND_MAX));
}



float Renderer::rand30x200()
{
    return 30 + ( std::rand() % ( 200 - 30 + 1 ) );
}



void Renderer::initializeGL()
{
    initializeOpenGLFunctions();
    makeCurrent();

    createScene();

    for(unsigned int i = 0; i < _objects.size(); i++)
    {
        _objects[i]->initialize();
    }
}



void Renderer::paintGL()
{
    _view.setToIdentity();
    _view.lookAt(_camera.eye, _camera.center, _camera.up);
    _proj.setToIdentity();

    _proj.perspective(_camera.fov, (float) width()/(float)height(), _camera.zNear, _camera.zFar);

    for (unsigned int i = 0; i < _objects.size(); i++)
    {
        _objects[i]->render(_proj, _view, _model, _lights);
    }
}



void Renderer::resizeGL(int w, int h)
{
    //Atualizar a viewport
    glViewport(0, 0, w, h);
    _pathTracer->setDimensions(w, h);
    _radius = ((std::min(h,w)/2.0) - 1);
}
