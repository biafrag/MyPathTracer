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
    _camera.eye =  QVector3D(0.f,0.f,5.f);
    _camera.up = QVector3D(0.f,2.f,0.f);
    _camera.zNear = 0.1f;
    _camera.zFar  = 100.f;
    _camera.fov  = 60.f;

    _light.position = {-50, 50, 50};
    _light.ambient = {0.3f, 0.3f, 0.3f};
    _light.diffuse = {1.0f, 1.0f, 1.0f};
    _light.specular = {1.0f,  1.0f, 1.0f};
    _light.shi = 100.0f;

//    Light light2;
//    light2.position = {5, -5, 5};
//    light2.ambient = {0.3f, 0.3f, 0.3f};
//    light2.diffuse = {1.0f, 1.0f, 1.0f};
//    light2.specular = {1.0f,  1.0f,1.0f};
//    light2.shi = 0.3f;

    _lights.push_back(_light);

    //_lights.push_back(light2);


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



//int getIndex( int i, int j, int n )
//{
//    return j + i * ( n + 1 );
//}

//void Renderer::createSphere()
//{
//    const int n = 10;
//    const int m = 10;

//    const int numTriangles = 2 * n * m;
//    const int numVertices = ( n + 1 ) * ( m + 1 );

//    std::vector<QVector3D> points;
//    for( unsigned int i = 0; i <= n; i++ )
//    {
//        for( unsigned int j = 0; j <= m; j++ )
//        {
//            //Atualizar as coordenadas de textura
//            float s = (float) i / n;
//            float t = (float) j / m;
//            //texCoords.push_back(glm::vec2(s,t));

//            //Calcula os parâmetros
//            double theta = 2 * s * M_PI;
//            double phi = t * M_PI;
//            double sinTheta = sin( theta );
//            double cosTheta = cos( theta );
//            double sinPhi = sin( phi );
//            double cosPhi = cos( phi );

//            //Calcula os vértices == equacao da esfera
//            points.push_back( QVector3D(cosTheta * sinPhi,
//                                          cosPhi,
//                                          sinTheta * sinPhi) );
//        }
//    }

//    std::vector<unsigned int> indicesS;
//    indicesS.resize(numTriangles*3);

//    unsigned int startIndex = _points.size();
//    _points.insert(_points.end(),points.begin(),points.end());

//    std::vector<QVector3D> normals;
//    normals = points;

//    _normals.insert(_normals.end(),normals.begin(),normals.end());

//    //Preenche o vetor com a triangulação
//    unsigned int k = 0;
//    for( unsigned int i = 0; i < n; i++ )
//    {
//        for( unsigned int j = 0; j < m; j++ )
//        {
//            indicesS[ k++ ] = getIndex( i + 1, j, n ) + startIndex;
//            indicesS[ k++ ] = getIndex( i + 1, j + 1, n ) + startIndex;
//            indicesS[ k++ ] = getIndex( i, j, n ) + startIndex;


//            indicesS[ k++ ] = getIndex( i + 1, j + 1, n ) + startIndex;
//            indicesS[ k++ ] = getIndex( i, j + 1, n ) + startIndex;
//            indicesS[ k++ ] = getIndex( i, j, n ) + startIndex;
//        }
//    }

//    _indices.insert(_indices.end(),indicesS.begin(),indicesS.end());
//}


void Renderer::initializeGL()
{
    initializeOpenGLFunctions();
    makeCurrent();

    QMatrix4x4 rot, trans, scale;
    scale.scale(QVector3D(100, 100, 100));
    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -1, 0));
    Plane *p =  new Plane(trans, rot, scale);
    Object::Material material;
    material.color = QVector3D(1, 1, 0);
    //material.isReflective = true;
    p->setMaterial(material);
    _objects.push_back(p);
    material.color = QVector3D(1, 0, 0);
    //material.isReflective = true;

    Sphere *s =  new Sphere();
    s->setMaterial(material);
    _objects.push_back(s);

    material.isReflective = false;
    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 1 , 0));
    trans.setToIdentity();
    trans.translate(QVector3D(1, -1, 0));
    scale.setToIdentity();
    scale.scale(QVector3D(30, 30, 30));


    Plane *p2 =  new Plane(trans, rot, scale);
    material.color = QVector3D(1,0.5,0.1);
    material.isReflective = true;

    p2->setMaterial(material);
    _objects.push_back(p2);

    rot.setToIdentity();
    rot.rotate(90, QVector3D(0, 0 , 1));
    trans.setToIdentity();
    trans.translate(QVector3D(0, -1, -3));
    scale.setToIdentity();
    scale.scale(QVector3D(30, 30, 30));

    Plane *p3 =  new Plane(trans, rot, scale);
    material.color = QVector3D(0.4,0.8,0.5);
    //material.isReflective = true;
    p3->setMaterial(material);
    //_objects.push_back(p3);

    std::vector<QVector3D> points;
    std::vector<unsigned int> indicesTri;
    std::vector<unsigned int> indicesQuad;
    std::vector<unsigned int> indicesNormalsTri;
    std::vector<unsigned int> indicesNormalsQuad;
    std::vector<unsigned int> indicesTexTri;
    std::vector<unsigned int> indicesTexQuad;

    std::vector<QVector2D> texCoords;
    std::vector<QVector3D> normals;

    readFile("../PathTracer/Malhas/teapot.obj", points, normals, texCoords, indicesTri,  indicesNormalsTri, indicesTexTri);
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
