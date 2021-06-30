#include "renderer.h"
#define M_PI 3.14159265358979323846
#include <QMouseEvent>
#include "raytracing.h"
#include "pathtracing.h"
#include <ctime>

/**
 * @brief Renderer
 *
 * The Renderer class it's a class that renders and has other renderers using other render techniques.
 *
 * @author Bianca Fragoso
 */

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
    QImage image = _rayTracer->generateRayTracingImageRecursionApproach(width(), height(), _model,  _camera, _scene);
    time = _rayTracer->getTime();
    return image;

}



QImage Renderer::getPathTracedImage(float &time)
{
    QImage image = _pathTracer->generateImage(width(), height(), _model,  _camera, _scene);
    time = _pathTracer->getTime();
    return image;
}



void Renderer::setNumberOfRays(unsigned int rays)
{
    _pathTracer->setRayNumber(rays);
    _rayTracer->setRayNumber(rays);
}



QVector3D Renderer::Points_Sphere(QVector3D pointT)
{
    QVector3D pointf;
    double r,s;
    pointf.setX((pointT.x() - (width()/2.0))/_radius);
    pointf.setY((pointT.y() - (height()/2))/_radius);
    r = pointf.x()*pointf.x()+pointf.y()*pointf.y();

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



void Renderer::setScene(Scene scene)
{
    _scene = scene;
    if(_isInitialized)
    {
        makeCurrent();

        auto objects = _scene.getObjects();

        for(unsigned int i = 0; i < objects.size(); i++)
        {
            objects[i]->initialize();
        }
    }
    update();
}



void Renderer::setEye(QVector3D eye)
{
    _camera.eye = eye;
}



std::vector<unsigned int> Renderer::getCountVector()
{
    return _rayTracer->getCountVector();
}



void Renderer::initializeGL()
{
    if(!_isInitialized)
    {
        initializeOpenGLFunctions();
        makeCurrent();
        _isInitialized = true;
        auto objects = _scene.getObjects();
        for(unsigned int i = 0; i < objects.size(); i++)
        {
            objects[i]->initialize();
        }
    }
}



void Renderer::paintGL()
{
    QVector3D color = _scene.getBackgroundColor();
    glClearColor(color.x(), color.y(), color.z(), 1.0f);

    _view.setToIdentity();
    _view.lookAt(_camera.eye, _camera.center, _camera.up);
    _proj.setToIdentity();

    _proj.perspective(_camera.fov, (float) width()/(float)height(), _camera.zNear, _camera.zFar);

    auto objects = _scene.getObjects();
    auto lights = _scene.getLights();
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        objects[i]->render(_proj, _view, _model, lights);
    }
}



void Renderer::resizeGL(int w, int h)
{
    //Atualizar a viewport
    glViewport(0, 0, w, h);
    _radius = ((std::min(h,w)/2.0) - 1);
}
