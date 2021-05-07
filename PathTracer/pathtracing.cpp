#include "pathtracing.h"
#include<cmath>
#include "sphere.h"
#include "trianglemesh.h"
#include <iostream>
#include <chrono>

#define M_PI 3.1415

PathTracing::PathTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam, std::vector<Object *> objects, std::vector<Light> lights, QVector3D backgroundColor)
    : _model(model),
      _backgroundColor(backgroundColor),
      _camera(cam),
      _objects(objects),
      _lights(lights),
      _width(w),
      _height(h)
{
    _seed = std::rand();

    _Ze = (_camera.eye - cam.center).normalized();
    _Xe = QVector3D::crossProduct(cam.up, _Ze).normalized();
    _Ye = QVector3D::crossProduct(_Ze, _Xe);
}



bool PathTracing::hasObjectObstacle(Light light, IntersectRecord intersection)
{
    RayHit hit = intersection.hit;
    Ray pointLightRay;
    pointLightRay.origin = hit.position;
    pointLightRay.direction = (light.position - hit.position).normalized();
    for(unsigned int o = 0; o < _objects.size(); o++)
    {
        if(_objects[o] != intersection.object)
        {
            ObjectType type = _objects[o]->getObjectType();
            if(type == ObjectType::SPHERE)
            {
                Sphere *s = dynamic_cast<Sphere *>(_objects[o]);
                float t = s->intersectsWith(pointLightRay, _model);
                if(t > 0 && t < 1)
                {
                    return true;
                }
            }
            else
            {
                TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(_objects[o]);
                bool verif = mesh->isRayIntersecting(pointLightRay, _model);
                if(verif)
                {
                    return true;
                }
            }
        }

    }
    return false;
}



Object * PathTracing::reflection(Ray ray, float &tCloser, unsigned int &indexObject, unsigned int &vertCloser, unsigned int indMyObject)
{
    //Variável que vai guardar qual é o menor t, ou seja, qual o ponto mais a frente que o raio atinge
    float tAux = FLT_MAX;
    Object * objectCloser = nullptr;

    bool isSphere = false;

    for(unsigned int o = 0; o < _objects.size(); o++)
    {
        if(indMyObject != o)
        {
            ObjectType type = _objects[o]->getObjectType();
            if(type == ObjectType::SPHERE)
            {
                Sphere *s = dynamic_cast<Sphere *>(_objects[o]);
                float t = s->intersectsWith(ray, _model);

                if( t < tAux && t>0)
                {
                    tAux = t;
                    objectCloser = _objects[o];
                    indexObject = o;
                    isSphere = true;
                }
            }
            else
            {
                TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(_objects[o]);
                int indTri;
                float t = mesh->intersectsWithRay(ray, _model, tCloser, indTri);
                if( t < tAux && t>0 && indTri >= 0 )
                {
                    tAux = t;
                    objectCloser = _objects[o];
                    indexObject = o;
                    vertCloser = indTri;
                }
            }
        }
    }
    tCloser = tAux;
    return objectCloser;
}



QVector3D PathTracing::getRayPoint(float t, Ray ray)
{
    return ray.origin + (t * ray.direction);
}


QImage PathTracing::generatePathTracingImage()
{
    QImage image(_width, _height, QImage::Format_RGB32);

    //a é a altura real
    float a = 2 * _camera.zNear * tan(_camera.fov* (M_PI/180)/2.0);

    //b é a largura real
    float b = (a * _width)/_height;

    unsigned int aadepth = 1;

    //Passada do JFA
    auto start  = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for
    for(int y = 0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            QVector3D totalColor = QVector3D(_backgroundColor.x() * 255, _backgroundColor.y() * 255, _backgroundColor.z() * 255);
            for (unsigned int aax = 0; aax < aadepth; aax++)
            {
                for (unsigned int aay = 0; aay < aadepth; aay++)
                {
                    //Lance um raio
                     Ray cameraPixelRay;
                     cameraPixelRay.origin = _camera.eye;

                     float factor = (float)aax/((float)aadepth);
                     if(aadepth == 1)
                     {
                         factor = 0;
                     }
                     else
                     {
                         factor = (float)aax/((float)aadepth - 1);
                     }
                     //d é um vetor que vai do olho até a tela
                     QVector3D d = (- _camera.zNear * _Ze) + (a*((((_height-y) + factor)/(float)_height) - 0.5) * _Ye) + b * ((((float)x + factor)/(float)_width) - 0.5) * _Xe;

                    cameraPixelRay.direction = d;

                    //Variável que vai guardar qual é o menor t, ou seja, qual o ponto mais a frente que o raio atinge
                    float tCloser = FLT_MAX;
                    unsigned int vertCloser;
                    Object * objectCloser;
                    unsigned int indexObject;


                    for(unsigned int o = 0; o < _objects.size(); o++)
                    {
                        ObjectType type = _objects[o]->getObjectType();
                        if(type == ObjectType::SPHERE)
                        {
                            Sphere *s = dynamic_cast<Sphere *>(_objects[o]);
                            float t = s->intersectsWith(cameraPixelRay, _model);

                            if( t < tCloser && t>0)
                            {
                                tCloser = t;
                                objectCloser = _objects[o];
                                indexObject = o;
                            }
                        }
                        else
                        {
                            TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(_objects[o]);
                            int indTri;
                            float t = mesh->intersectsWithRay(cameraPixelRay, _model, tCloser, indTri);
                            if( t < tCloser && t>0 && indTri >= 0)
                            {
                                tCloser = t;
                                objectCloser = _objects[o];
                                indexObject = o;
                                vertCloser = indTri;
                            }
                        }
                    }

                    //Pintar ponto mais próximo encontrado
                    //Calcular contribuição da luz nesse ponto
                    //Se o t que eu encontrei tiver intersectado algum objeto
                    if(tCloser < FLT_MAX)
                    {

                        QVector3D point = getRayPoint(tCloser, cameraPixelRay);

                        RayHit hit;
                        hit.t = tCloser;
                        hit.position = point;
                        IntersectRecord intersection;
                        intersection.hit = hit;
                        intersection.object = objectCloser;
                        QVector3D cor =  getColorAt(intersection, cameraPixelRay, indexObject, vertCloser);
                        totalColor = totalColor + cor;

                    }
                    else
                    {
                        totalColor = _backgroundColor;
                    }
                }
            }

            QVector3D finalColor = totalColor/aadepth*aadepth;
            image.setPixelColor(x, y, QColor(std::fmin(finalColor.x() * 255/(aadepth*aadepth), 255), std::fmin(finalColor.y() * 255/(aadepth*aadepth), 255), std::fmin(finalColor.z() * 255/(aadepth*aadepth), 255)));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    _time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout<<"Tempo levado: "<<_time<<" s"<<std::endl;
    return image;
}

float energy(QVector3D color)
{
    return QVector3D::dotProduct(color, QVector3D(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f));
}


QVector3D PathTracing::getColorAt(IntersectRecord intersection, Ray ray, int indObj, int indVert)
{
    RayHit hit = intersection.hit;
    QVector3D corF = _backgroundColor;
    Object *object = intersection.object;
    Material material = object->getMaterial();

    QVector3D N;
    if(object->getObjectType() == ObjectType::SPHERE)
    {
        N = dynamic_cast<Sphere*>(object)->normalAt(hit.position);
    }
    else
    {
         TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);
         N = mesh->getNormalInsideTriangle(indVert, hit.position, _model);
    }
    intersection.hit.normal = N;
    hit.normal = N;

    Ray reflection_ray;
    reflection_ray.origin = hit.position;
    //reflection_ray.direction = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);
    reflection_ray.direction = SampleHemisphere(intersection.hit.normal, hit.position);

    Object * objRef;
    RayHit newHit;
    IntersectRecord newIntersection;

    unsigned int indObjRef, indVertRef;
    objRef = reflection(reflection_ray, newHit.t, indObjRef, indVertRef, indObj);
    newHit.position = hit.position + reflection_ray.direction * newHit.t;

    if(objRef != nullptr)
    {
        newIntersection.hit = newHit;
        newIntersection.object = objRef;
        reflection_ray.timesReflection = ray.timesReflection - 1;
        reflection_ray.energy = ray.energy * material.getSpecular();
        QVector3D cor = getColorAt(newIntersection, reflection_ray, indObjRef, indVertRef) ;
        corF += cor;
    }
    return corF;
}



float PathTracing::rand(QVector3D pixel)
{
    float intPart;
    float result = std::modf(sin(_seed / 100.0f * QVector3D::dotProduct(pixel, QVector3D(12.9898f, 78.233f, 0.0f))) * 43758.5453f, &intPart);
    _seed += 1.0f;
    return result;
}



QVector3D PathTracing::SampleHemisphere(QVector3D normal, QVector3D pixel)
{
    // Uniformly sample hemisphere direction
    float cosTheta = rand(pixel);
    float sinTheta = sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
    float phi = 2 * M_PI * rand(pixel);
    QVector3D tangentSpaceDir = QVector3D(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    QMatrix4x4 tangentSpaceMatrix =  GetTangentSpace(normal);
    // Transform direction to world space
    return (tangentSpaceDir * GetTangentSpace(normal));
}



QMatrix4x4 PathTracing::GetTangentSpace(QVector3D normal)
{
    // Choose a helper vector for the cross product
    QVector3D helper = QVector3D(1, 0, 0);
    if (abs(normal.x()) > 0.99f)
        helper = QVector3D(0, 0, 1);
    // Generate vectors
    QVector3D tangent = QVector3D::crossProduct(normal, helper).normalized();
    QVector3D binormal = QVector3D::crossProduct(normal, tangent).normalized();
    return QMatrix4x4(tangent.x(), tangent.y(), tangent.z(), 0, binormal.x(), binormal.y(), binormal.z(), 0, normal.x(), normal.y(), normal.z(), 0, 0, 0, 0, 0);
}

