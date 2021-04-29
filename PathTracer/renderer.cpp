#include "renderer.h"
#define M_PI 3.14159265358979323846
#include <QMouseEvent>
#include "raytracing.h"
#include "sphere.h"
#include "plane.h"
#include "trianglemesh.h"
#include "readerOBJ.h"

Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    _camera.center = QVector3D(0.f,0.f,0.f);
    _camera.eye =  QVector3D(0.f,0.f,10.f);
    _camera.up = QVector3D(0.f,2.f,0.f);
    _camera.zNear = 0.01f;
    _camera.zFar  = 1000.f;
    _camera.fov  = 60.f;

    _light.position = {-50, 50, 50};
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
    light3.position = {50, -50, -50};
    light3.ambient = {0.3f, 0.3f, 0.3f};
    light3.diffuse = {0.8f, 0.8f, 0.8f};
    light3.specular = {0.3f, 0.3f, 0.3f};
    light3.shi = 60.0f;

    _lights.push_back(_light);

    _lights.push_back(light2);
    //_lights.push_back(light3);


}



QImage Renderer::getRayTracedImage(float &time)
{
    RayTracing r(width(), height(), _model,  _camera, _objects, _lights);

    //Levando vértices e normais pro espaço do modelo
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    QImage image = r.generateRayTracingImage();

    time = r.getTime();
    return r.generateRayTracingImage();

}



void Renderer::createScene()
{

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



void Renderer::initializeGL()
{
    initializeOpenGLFunctions();
    makeCurrent();

    QMatrix4x4 rot, trans, scale;
    scale.scale(QVector3D(5, 5, 5));
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -1, 0));
    Plane *p =  new Plane(trans, rot, scale);
    Object::Material material;
    material.color = QVector3D(1, 1, 0);
    //material.isReflective = true;
    p->setMaterial(material);
    p->setTexture(":/textures/Texturas/Ground.jpg");
    //_objects.push_back(p);
    material.color = QVector3D(0.5, 0, 0);
    //material.color = QVector3D(1, 1, 1);

    //material.isReflective = true;

    Sphere *s =  new Sphere();
    s->setMaterial(material);
    //_objects.push_back(s);

    //material.isReflective = false;
    material.color = QVector3D(1, 0.3, 0.7);

    Sphere *s2 =  new Sphere(QVector3D(-3, 1, 1), 0.5);
    s2->setMaterial(material);
    _objects.push_back(s2);

    Sphere *s3 =  new Sphere(QVector3D(3, 1, 1), 0.5);
    s3->setMaterial(material);
    _objects.push_back(s3);

    Sphere *s4 =  new Sphere(QVector3D(0, -3, 1), 0.5);
    s4->setMaterial(material);
    _objects.push_back(s4);

    material.isReflective = false;
    rot.setToIdentity();
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 100, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));


    Plane *p2 =  new Plane(trans, rot, scale);
    material.color = QVector3D(1,0.5,0.1);
    //material.isReflective = true;
    p2->setMaterial(material);

    p2->setTexture(":/textures/Texturas/Ceu2.jpg");
    //_objects.push_back(p2);

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(5, 5, 5));

    Plane *p3 =  new Plane(trans, rot, scale);
    material.color = QVector3D(0.4,0.8,0.5);
   // material.isReflective = true;
    p3->setMaterial(material);
    //p3->setTexture(":/textures/Texturas/Ceu2.jpg");
    p3->setTexture(":/textures/Texturas/Ground.jpg");

    _objects.push_back(p3);

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(-100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *p4 =  new Plane(trans, rot, scale);
    material.color = QVector3D(0.4,0.8,0.5);
   // material.isReflective = true;
    p4->setMaterial(material);
    p4->setTexture(":/textures/Texturas/Ceu2.jpg");

    //_objects.push_back(p4);

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(100, 0, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *p5 =  new Plane(trans, rot, scale);
    material.color = QVector3D(0.4,0.8,0.5);
   // material.isReflective = true;
    p5->setMaterial(material);
    p5->setTexture(":/textures/Texturas/Ceu2.jpg");


    //_objects.push_back(p5);


    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, 0, 100));
    scale.setToIdentity();
    scale.scale(QVector3D(100, 100, 100));

    Plane *p6 =  new Plane(trans, rot, scale);
    material.color = QVector3D(0.4,0.8,0.5);
   // material.isReflective = true;
    p6->setMaterial(material);
    p6->setTexture(":/textures/Texturas/Ceu2.jpg");
    //_objects.push_back(p6);



    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;

     material.isReflective = true;
     material.color = QVector3D(0.5,0.5,0.5);

    readFile("../PathTracer/Malhas/bunny2.obj", points, normals, texCoords, indicesTri,  indicesNormalsTri, indicesTexTri);
    if(normals.size() == 0)
    {
        //normals = points;
    }
    TriangleMesh *t = new TriangleMesh(points, indicesTri, normals);
    t->setMaterial(material);
    //_objects.push_back(t);
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

    _radius = ((std::min(h,w)/2.0) - 1);
}
