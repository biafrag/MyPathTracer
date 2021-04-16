#include "renderer.h"
#define M_PI 3.14159265358979323846
#include <QMouseEvent>
#include "raytracing.h"
#include "sphere.h"
#include "plane.h"

Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    _camera.center = QVector3D(0.f,0.f,0.f);
    _camera.eye =  QVector3D(0.f,0.f,5.f);
    _camera.up = QVector3D(0.f,2.f,0.f);
    _camera.zNear = 0.1f;
    _camera.zFar  = 100.f;
    _camera.fov  = 60.f;

    _light.position = {-5, 5, 5};
    _light.ambient = {0.3f, 0.3f, 0.3f};
    _light.diffuse = {1.0f, 1.0f, 1.0f};
    _light.specular = {1.0f,  1.0f,1.0f};
    _light.shi = 24.0f;

    Light light2;
    light2.position = {5, -5, 5};
    light2.ambient = {0.3f, 0.3f, 0.3f};
    light2.diffuse = {1.0f, 1.0f, 1.0f};
    light2.specular = {1.0f,  1.0f,1.0f};
    light2.shi = 24.0f;

    _lights.push_back(_light);

    _lights.push_back(light2);


}



QImage Renderer::getRayTracedImage()
{
    RayTracing r;

    //Levando vértices e normais pro espaço do modelo
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;

//    int i = 0;
//    for(auto vertice : _points)
//    {
//        vertices.push_back(_model * vertice);
//        normals.push_back(_model.transposed().inverted()* _normals[i]);
//        i++;
//    }


    return r.generateRayTracingImage(width(), height(), _model, _proj, _camera, _objects, _light);

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

    Plane *p =  new Plane();
    _objects.push_back(p);

    Sphere *s =  new Sphere();
    _objects.push_back(s);


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
