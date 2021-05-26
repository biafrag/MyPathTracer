#include "pathtracergpu.h"
#define M_PI 3.14159265358979323846
#include <QMouseEvent>

#include "sphere.h"
#include "plane.h"
#include "trianglemesh.h"
#include "material.h"
#include <ctime>

PathTracerGPU::PathTracerGPU(QWidget *parent)
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

}



PathTracerGPU::~PathTracerGPU()
{
}



void PathTracerGPU::createScene()
{
    QMatrix4x4 rot, trans, scale;

    //rot.rotate(180, QVector3D(1, 0 , 0));

   scale.scale(QVector3D(0, height()/2, 0));

    Plane *backScenePlane =  new Plane(trans, rot, scale);
    backScenePlane->setTexture(":/textures/Texturas/Ceu.jpg");
    _objects.push_back(backScenePlane);
}



QVector3D PathTracerGPU::Points_Sphere(QVector3D pointT)
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



void PathTracerGPU::mousePressEvent(QMouseEvent *event)
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



void PathTracerGPU::mouseReleaseEvent(QMouseEvent *event)
{
    _mousepress = false;
    update();
}



void PathTracerGPU::mouseMoveEvent(QMouseEvent *event)
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



void PathTracerGPU::wheelEvent(QWheelEvent *event)
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



QVector3D PathTracerGPU::randVec()
{
    float x = ((float) std::rand() / (RAND_MAX));
    float y = ((float) std::rand() / (RAND_MAX));
    float z = ((float) std::rand() / (RAND_MAX));

    return QVector3D(x, y, z);

}



float PathTracerGPU::rand0x1()
{
    return ((float) std::rand() / (RAND_MAX));
}



float PathTracerGPU::rand30x200()
{
    return 30 + ( std::rand() % ( 200 - 30 + 1 ) );
}



void PathTracerGPU::initializeGL()
{
    initializeOpenGLFunctions();
    makeCurrent();

    createScene();

    for(unsigned int i = 0; i < _objects.size(); i++)
    {
        _objects[i]->initialize();
    }
}



void PathTracerGPU::paintGL()
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



void PathTracerGPU::resizeGL(int w, int h)
{
    //Atualizar a viewport
    glViewport(0, 0, w, h);
    _radius = ((std::min(h,w)/2.0) - 1);
}
