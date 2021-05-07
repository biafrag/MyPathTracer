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
      _backgroundColor(backgroundColor),
      _camera(cam),
      _objects(objects),
      _lights(lights),
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



bool RayTracing::hasObjectObstacle(Light light, IntersectRecord intersection)
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



QVector3D RayTracing::getRayPoint(float t, Ray ray)
{
    return ray.origin + (t * ray.direction);
}


QVector3D RayTracing::getColorAt(IntersectRecord intersection, Ray ray, int indObj, int indVert)
{
    RayHit hit = intersection.hit;
    QVector3D corF = _backgroundColor;
    Object *object = intersection.object;
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
            N = dynamic_cast<Sphere*>(object)->normalAt(hit.position);
        }
        else
        {
             TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);
             N = mesh->getNormalInsideTriangle(indVert, hit.position, _model);
        }
        intersection.hit.normal = N;


        if(material.getReflectivity() < 1.0)
        {
            Ray reflection_ray ;
            reflection_ray.origin = hit.position;
            reflection_ray.direction = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);
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

        }
        bool hasNoEffect = hasObjectObstacle(light, intersection);
        if(!hasNoEffect || material.getReflectivity() < 1 )
        {
            QVector3D diffuse = calculatePhongDiffuse(intersection, light, indVert); //Adicionar propriedade dos materiais do objeto depois
            QVector3D specular = calculatePhongSpecular(intersection, light);
            QVector3D ambient = calculateAmbient(intersection, light, indVert);

            QVector3D cor = ambient + diffuse + specular;
            //cor = {1, 0, 0};
            corF = (1 - material.getReflectivity()) * corF + cor  * material.getReflectivity() ;
        }
        else
        {
            QVector3D ambient = calculateAmbient(intersection, light, indVert);
            corF = material.getReflectivity() * ambient;
        }
    }
    return corF;
}



QVector3D RayTracing::getColorAt2(QVector3D point, Ray &ray, Object *object, int indObj, float t, int indVert)
{
    QVector3D albedo = QVector3D(0.8f, 0.8f, 0.8f);

    if(t < FLT_MAX)
    {
        QVector3D specular = object->getMaterial().getSpecular();

        QVector3D N;
        point = ray.hit(t);

        if(object->getObjectType() == ObjectType::SPHERE)
        {
            N = dynamic_cast<Sphere*>(object)->normalAt(point);
        }
        else
        {
             TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);
             N = mesh->getNormalInsideTriangle(indVert, point, _model);
        }

        ray.origin = point + N * 0.001f;
        //v = i - 2 * n * dot(i n) .
        ray.direction = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);
        ray.energy *= specular;
        IntersectRecord intersection;
        RayHit hit;
        hit.t = t;
        hit.normal = N;
        hit.position = point;
        intersection.hit = hit;
        intersection.object = object;
        bool hasNoEffect = hasObjectObstacle(_lights[0], intersection);
        QVector3D ambient = calculateAmbient(intersection, _lights[0], indVert);

        if(hasNoEffect)
        {
            return ambient;
        }

        //return QVector3D(0.5, 0.6, 0.3) ;


        // Return a diffuse-shaded color
        //return QVector3D::dotProduct(hit.normal, _lights[0].position) /** _lights[0].diffuse*/ * albedo /*+ _lights[0].ambient * albedo*/;
        QVector3D diffuse = calculatePhongDiffuse(intersection, _lights[0], indVert); //Adicionar propriedade dos materiais do objeto depois
        specular = calculatePhongSpecular(intersection, _lights[0]);
        return diffuse + ambient + specular;

    }
    else
    {
        ray.energy = QVector3D(0.0f, 0.0f, 0.0f);

        return _backgroundColor;
    }

    //return (N * 0.5f) + QVector3D(0.5, 0.5, 0.5);
}



QVector3D RayTracing::calculateAmbient(IntersectRecord intersection, Light light, int ind1)
{
    Object *object = intersection.object;
    Material material = intersection.object->getMaterial();
    RayHit hit = intersection.hit;

    QVector3D color(0, 0, 0);
    if(material.hasTexture())
    {
        TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);

        QVector3D texInt = mesh->getTexCoordinatesInsideTriangle(ind1, hit.position, _model);

        QImage texture = material.getTexture();
        QColor aux = texture.pixelColor(texInt.x() * texture.width(), (texInt.y() * texture.height()));
        color = QVector3D(aux.red()/255.0, aux.green()/255.0, aux.blue()/255.0);
    }
    else
    {
        color = material.getAlbedo();
    }
    QVector3D corVec = light.ambient * color;

    return corVec;
}



QVector3D RayTracing::calculatePhongDiffuse(IntersectRecord intersection, Light light, int ind1 )
{
   Object *object = intersection.object;
   Material material = intersection.object->getMaterial();
   RayHit hit = intersection.hit;

   QVector3D L = (light.position - hit.position).normalized();

   float lambertian = QVector3D::dotProduct(L, hit.normal);
    QVector3D color(0, 0, 0);
    if(lambertian > 0)
    {
        if(material.hasTexture())
        {
            TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);

            QVector3D texInt = mesh->getTexCoordinatesInsideTriangle(ind1, hit.position, _model);

            QImage texture = material.getTexture();
            QColor aux = texture.pixelColor(texInt.x() * texture.width(), texture.height() - (texInt.y() * texture.height()));
            color = QVector3D(aux.red()/255.0, aux.green()/255.0, aux.blue()/255.0);
        }
        else
        {
            color = material.getAlbedo();
        }

    }
    QVector3D corVec = lambertian * light.diffuse * color;

    return corVec;
}



QVector3D RayTracing::calculatePhongSpecular(IntersectRecord intersection, Light light)
{
    RayHit hit = intersection.hit;
    Object *object = intersection.object;
    QVector3D L = (light.position - hit.position).normalized();

    QVector3D V = (_camera.eye - hit.position).normalized();
    QVector3D H = (L+V).normalized();
    float dotP = QVector3D::dotProduct(hit.normal, H);
    float ispec;
    Material material = object->getMaterial();

    float lambertian = QVector3D::dotProduct(L, hit.normal);
     QVector3D specular(0, 0, 0);
     if(lambertian > 0)
     {
         if(dotP > 0)
         {
             ispec = std::pow(dotP, material.getShi());
         }
         else
         {
             ispec = 0;
         }
         specular = /*material.getSpecular() * */light.specular * ispec;
     }

    return specular;
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



QImage RayTracing::generateRayTracingImage2()
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
            QVector3D totalColor = QVector3D(0, 0, 0);
            for (unsigned int aax = 0; aax < aadepth; aax++)
            {
                for (unsigned int aay = 0; aay < aadepth; aay++)
                {
                    //Lance um raio
                     Ray cameraPixelRay;
                     cameraPixelRay.origin = _camera.eye;
                     cameraPixelRay.energy = QVector3D(1, 1, 1);
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

                    QVector3D point = getRayPoint(tCloser, cameraPixelRay);

                    for (int i = 0; i < 8; i++)
                    {
                        QVector3D energy = cameraPixelRay.energy;
                        QVector3D cor = getColorAt2(point, cameraPixelRay, objectCloser, indexObject,tCloser, vertCloser);
                        totalColor =  totalColor + energy * cor;
                        if(cameraPixelRay.energy.x() <= 0.01 && cameraPixelRay.energy.y() <= 0.01 && cameraPixelRay.energy.z() <= 0.01)
                        {
                            break;
                        }
                        tCloser = FLT_MAX;
                        objectCloser = reflection(cameraPixelRay, tCloser, indexObject, vertCloser, indexObject);
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



float RayTracing::getTime()
{
    return _time;
}
