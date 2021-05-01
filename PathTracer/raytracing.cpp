#include "raytracing.h"
#include <QMatrix4x4>
#include <QImage>
#include "renderer.h"
#include "sphere.h"
#include "trianglemesh.h"

#include <chrono>
#include <iostream>

#define M_PI 3.1415

RayTracing::RayTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam,
                       std::vector<Object *> objects, std::vector<Light> lights, QVector3D backgroundColor)

    : _model(model),
      _camera(cam),
      _objects(objects),
      _lights(lights),
      _backgroundColor(backgroundColor),
      _width(w),
      _height(h)
{
    _Ze = (_camera.eye - cam.center).normalized();
    _Xe = QVector3D::crossProduct(cam.up, _Ze).normalized();
    _Ye = QVector3D::crossProduct(_Ze, _Xe);
}



QVector3D RayTracing::getBaricentricCoordinates(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point)
{
    float A1, A2, A3, At;
    float alfa1, alfa2, alfa3;

    QVector3D normal = QVector3D::crossProduct(p3 - p1, p2 - p1);
    A1 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p1, p2 - p1))/2.0;
    A2 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p2, p3 - p2))/2.0;
    A3 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p3, p1 - p3))/2.0;
    At = QVector3D::dotProduct(normal, QVector3D::crossProduct(p2 - p3, p1 - p3))/2.0;
    alfa3 = A1/At;
    alfa1 = A2/At;
    alfa2 = A3/At;

    return QVector3D(alfa1, alfa2, alfa3);
}



bool RayTracing::triangleVerification(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D point)
{
    float A1, A2, A3, At;
    float alfa1, alfa2, alfa3;

    QVector3D normal = QVector3D::crossProduct(p3 - p1, p2 - p1);
    A1 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p1, p2 - p1))/2.0;
    A2 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p2, p3 - p2))/2.0;
    A3 = QVector3D::dotProduct(normal, QVector3D::crossProduct(point - p3, p1 - p3))/2.0;
    At = QVector3D::dotProduct(normal, QVector3D::crossProduct(p2 - p3, p1 - p3))/2.0;
    alfa3 = A1/At;
    alfa1 = A2/At;
    alfa2 = A3/At;

    //Para estar dentro as coordenadas baricêntricas precisam estar entre 0 e 1
    if(alfa1 < 0 || alfa1 > 1 || alfa2 < 0 || alfa2 > 1 || alfa3 < 0 || alfa3 > 1 )
    {
        return false;
    }
    return true;
}



bool RayTracing::hasObjectObstacle(Light light, QVector3D point, unsigned int index)
{
    Ray pointLightRay;
    pointLightRay.origin = point;
    pointLightRay.direction = (light.position - point).normalized();
    for(unsigned int o = 0; o < _objects.size(); o++)
    {
        if(o != index)
        {
            ObjectType type = _objects[o]->getObjectType();
            if(type == ObjectType::SPHERE)
            {
                Sphere *s = dynamic_cast<Sphere *>(_objects[o]);
                float t = s->intersectsWith(pointLightRay, _model);
                if(t > 0)
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



Object * RayTracing::reflection(Ray ray, float &tCloser, unsigned int &indexObject, unsigned int &vertCloser, unsigned int indMyObject)
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
                if( t < tCloser && t>0 && indTri >= 0)
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



QVector3D RayTracing::getRayPoint(float t, Ray ray)
{
    return ray.origin + (t * ray.direction);
}


QColor RayTracing::getColorAt(QVector3D point, Ray ray, Object *object, int indObj, int indVert)
{
    QColor corF = QColor(_backgroundColor.x() * 255, _backgroundColor.y() * 255, _backgroundColor.z() * 255);

    Material material = object->getMaterial();

    if(ray.timesReflection < 0)
    {
        return corF;
    }
    for(auto light : _lights)
    {
        QVector3D N;
        if(object->getObjectType() == ObjectType::SPHERE)
        {
            N = dynamic_cast<Sphere*>(object)->normalAt(point);
        }
        else
        {
             TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);
             N = mesh->getNormalInsideTriangle(indVert, point, _model);
        }

        if(material.getReflectivity() < 1.0)
        {
            Ray reflection_ray ;
            reflection_ray.origin = point;
            reflection_ray.direction = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);

            float tRef;
            Object * objRef;
            unsigned int indObjRef, indVertRef;
            objRef = reflection(reflection_ray, tRef, indObjRef, indVertRef, indObj);
            QVector3D reflection_intersection_position = point + reflection_ray.direction * tRef;

            if(objRef != nullptr)
            {
                reflection_ray.timesReflection = ray.timesReflection - 1;
                reflection_ray.energy = ray.energy * material.getSpecular();

                QColor cor = getColorAt(reflection_intersection_position, reflection_ray, objRef, indObjRef, indVertRef) ;
                corF = QColor(std::fmin(corF.red() + cor.red(), 255), std::fmin(corF.green() + cor.green(), 255), std::fmin(corF.blue() + cor.blue(), 255));
            }

        }
        bool hasNoEffect = hasObjectObstacle(light, point, indObj);
        if(!hasNoEffect)
        {
            //Normal naquele ponto específico. Fazemos como se fosse uma média entre as normais do triângulo usando as coordenadas baricêntricas
            QVector3D L = (light.position - point).normalized();

            float lambertian = QVector3D::dotProduct(L, N);
            QColor specular(0, 0, 0);
            QColor diffuse(0, 0, 0);
            if(lambertian > 0)
            {
                diffuse = calculatePhongDiffuse(object, light, lambertian, point, indVert); //Adicionar propriedade dos materiais do objeto depois
                specular = calculatePhongSpecular(object, light, point, N);
            }
            //QColor ambient = calculateAmbient(object, light, point, indVert);
            QColor ambient = QColor(0, 0, 0);

            ambient = calculateAmbient(object, light, point, indVert);

            QColor cor(std::fmin(255, diffuse.red() + ambient.red() + specular.red()), std::fmin(255, diffuse.green() + specular.green() + ambient.green()), std::fmin(255, diffuse.blue() + specular.blue() + ambient.blue()));

            corF = QColor(std::fmin((1 - material.getReflectivity()) * corF.red() + cor.red()  * material.getReflectivity() , 255), std::fmin((1 - material.getReflectivity()) * corF.green() + cor.green() * material.getReflectivity(), 255), std::fmin((1 - material.getReflectivity())  * corF.blue() + cor.blue() * material.getReflectivity(), 255));

        }
        else
        {
            QColor ambient = calculateAmbient(object, light, point, indVert);
            corF = QColor(std::fmin(ambient.red(), 255), std::fmin(ambient.green(), 255), std::fmin(ambient.blue(), 255));
        }
    }

    return corF;
}

QColor RayTracing::calculateAmbient(Object *object, Light light, QVector3D point, int ind1)
{
    Material material = object->getMaterial();
    QVector3D color;
    if(material.hasTexture())
    {
        TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);

        QVector3D texInt = mesh->getTexCoordinatesInsideTriangle(ind1, point, _model);

        QImage texture = material.getTexture();
        QColor aux = texture.pixelColor(texInt.x() * texture.width(), (texInt.y() * texture.height()));
        color = QVector3D(aux.red()/255.0, aux.green()/255.0, aux.blue()/255.0);
    }
    else
    {
        color = material.getAlbedo();
    }
    QVector3D corVec = light.ambient * color;
    QColor cor = QColor(std::fmin(corVec.x() * 255, 255), std::fmin(corVec.y() * 255, 255), std::fmin(corVec.z() * 255, 255));

    return cor;
}



QColor RayTracing::calculatePhongDiffuse(Object *object, Light light, float lambertian, QVector3D point, int ind1 )
{
   Material material = object->getMaterial();

    QVector3D color;
    if(material.hasTexture())
    {
        TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);

        QVector3D texInt = mesh->getTexCoordinatesInsideTriangle(ind1, point, _model);

        QImage texture = material.getTexture();
        QColor aux = texture.pixelColor(texInt.x() * texture.width(), texture.height() - (texInt.y() * texture.height()));
        color = QVector3D(aux.red()/255.0, aux.green()/255.0, aux.blue()/255.0);
    }
    else
    {
        color = material.getAlbedo();
    }

    QVector3D corVec = lambertian * light.diffuse * color;
    QColor cor = QColor(std::fmin(corVec.x() * 255, 255), std::fmin(corVec.y() * 255, 255), std::fmin(corVec.z() * 255, 255));

    return cor;
}



QColor RayTracing::calculatePhongSpecular(Object *object, Light light, QVector3D point, QVector3D N)
{
    QVector3D L = (light.position - point).normalized();

    QVector3D V = (_camera.eye - point).normalized();
    QVector3D H = (L+V).normalized();
    float dotP = QVector3D::dotProduct(N,H);
    float ispec;
    Material material = object->getMaterial();

    if(dotP > 0)
    {
        ispec = std::pow(dotP, material.getShi());
    }
    else
    {
        ispec = 0;
    }
    QVector3D specular = material.getSpecular() * light.specular * ispec;

    QColor cor = QColor(std::fmin(specular.x() * 255, 255), std::fmin(specular.y() * 255, 255), std::fmin(specular.z() * 255, 255));

    return cor;
}



QImage RayTracing::generateRayTracingImage()
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
                        QColor cor =  getColorAt(point, cameraPixelRay, objectCloser, indexObject, vertCloser);
                        totalColor = QVector3D(cor.red(), cor.green(), cor.blue());
                    }
                }
            }

            //QColor finalColor = totalColor/aadepth*aadepth;
            image.setPixelColor(x, y, QColor(std::fmin(totalColor.x()/(aadepth*aadepth), 255), std::fmin(totalColor.y()/(aadepth*aadepth), 255), std::fmin(totalColor.z()/(aadepth*aadepth), 255)));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    _time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout<<"Tempo levado: "<<_time<<" s"<<std::endl;
    return image;
}



float RayTracing::getTime()
{
    return _time;
}
