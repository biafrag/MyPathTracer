#include "raytracing.h"
#include <QMatrix4x4>
#include <QImage>
#include "renderer.h"
#include "sphere.h"
#include "trianglemesh.h"

#include <chrono>
#include <iostream>

#define M_PI 3.1415

/**
 * @brief RayTracing
 *
 * The RayTracing class has some functions that helps the implementation of the ray tracing algorithm.
 *
 * @author Bianca Fragoso
 */
RayTracing::RayTracing()
{
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



QVector3D RayTracing::getColorAtRecursive(IntersectRecord intersection, std::vector<Light> lights, Ray ray, int indObj, int indVert)
{
        RayHit hit = intersection.hit;
      QVector3D corF = _backgroundColor;
      Object *object = intersection.object;
      Material material = object->getMaterial();

      if(ray.timesReflection < 0)
      {
          return corF;
      }
      for(auto light : lights)
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
                  QVector3D cor = getColorAtRecursive(newIntersection, lights, reflection_ray, indObjRef, indVertRef) ;
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



QVector3D RayTracing::getColorAt(QVector3D point, std::vector<Light> lights, Ray &ray, Object *object, int indObj, float t, int indVert)
{
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
        ray.direction = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);
        ray.energy *= specular;
        IntersectRecord intersection;
        RayHit hit;
        hit.t = t;
        hit.normal = N;
        hit.position = point;
        intersection.hit = hit;
        intersection.object = object;
        bool hasNoEffect = hasObjectObstacle(lights[0], intersection);
        QVector3D ambient = calculateAmbient(intersection, lights[0], indVert);

        if(hasNoEffect)
        {
            return ambient;
        }

        // Return a diffuse-shaded color
        QVector3D diffuse = calculatePhongDiffuse(intersection, lights[0], indVert); //Adicionar propriedade dos materiais do objeto depois
        specular = calculatePhongSpecular(intersection, lights[0]);
        return diffuse + ambient /*+ specular*/;

    }
    else
    {
        ray.energy = QVector3D(0.0f, 0.0f, 0.0f);

        return _backgroundColor;
    }
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

   float lambertian = QVector3D::dotProduct(L, hit.normal.normalized());
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
         specular = /*material.getSpecular() **/ light.specular * ispec;
     }

    return specular;
}



QImage RayTracing::generateRayTracingImageRecursionApproach(int w, int h, QMatrix4x4 model, Renderer::Camera cam,
                                           Scene scene, QVector3D backgroundColor)
{
    _model = model;
    _backgroundColor = backgroundColor,
    _objects = scene.getObjects();
    std::vector<Light> lights = scene.getLights();
    _width = w;
    _height = h;
    _camera = cam;
    QImage image(_width, _height, QImage::Format_RGB32);

    QVector3D Ze = (cam.eye - cam.center).normalized();
    QVector3D Xe = QVector3D::crossProduct(cam.up, Ze).normalized();
    QVector3D Ye = QVector3D::crossProduct(Ze, Xe);

    //a é a altura real
    float a = 2 * cam.zNear * tan(cam.fov* (M_PI/180)/2.0);

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
            unsigned int xAmount = sqrt(_rayNumber);
            unsigned int yAmount = xAmount;
            for (unsigned int aax = 0; aax < xAmount; aax++)
            {
                for (unsigned int aay = 0; aay < yAmount; aay++)
                {
                    //Lance um raio
                     Ray cameraPixelRay;
                     cameraPixelRay.origin = cam.eye;
                     float factorY = (float)aay/((float)yAmount);
                     float factorX = (float)aax/((float)xAmount);

                     if(yAmount == 1)
                     {
                         factorY = 0;
                     }
                     else
                     {
                         factorY = (float)aay/((float)yAmount - 1);
                     }

                     if(xAmount == 1)
                     {
                         factorX = 0;
                     }
                     else
                     {
                         factorX = (float)aax/((float)xAmount - 1);
                     }
                     //d é um vetor que vai do olho até a tela
                     QVector3D d = (- cam.zNear * Ze) + (a*((((_height-y) + factorY)/(float)_height) - 0.5) * Ye) + b * ((((float)x + factorX)/(float)_width) - 0.5) * Xe;

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

                        QVector3D point = cameraPixelRay.hit(tCloser);

                        RayHit hit;
                        hit.t = tCloser;
                        hit.position = point;
                        IntersectRecord intersection;
                        intersection.hit = hit;
                        intersection.object = objectCloser;
                        QVector3D cor =  getColorAtRecursive(intersection, lights, cameraPixelRay, indexObject, vertCloser);
                        totalColor = totalColor + cor;

                    }
                    else
                    {
                        totalColor += _backgroundColor;
                    }
                }
            }

            QVector3D finalColor = totalColor/_rayNumber;
            image.setPixelColor(x, y, QColor(std::fmin(finalColor.x() * 255/(aadepth*aadepth), 255), std::fmin(finalColor.y() * 255/(aadepth*aadepth), 255), std::fmin(finalColor.z() * 255/(aadepth*aadepth), 255)));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    _time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout<<"Tempo levado: "<<_time<<" s"<<std::endl;
    return image;
}



QImage RayTracing::generateImage(int w, int h, QMatrix4x4 &model, Renderer::Camera &cam,
                                            Scene scene, QVector3D backgroundColor)
{
    _model = model;
    _backgroundColor = backgroundColor,
    _objects = scene.getObjects();
    std::vector<Light> lights = scene.getLights();
    _width = w;
    _height = h;

    QVector3D Ze = (cam.eye - cam.center).normalized();
    QVector3D Xe = QVector3D::crossProduct(cam.up, Ze).normalized();
    QVector3D Ye = QVector3D::crossProduct(Ze, Xe);

    QImage image(_width, _height, QImage::Format_RGB32);

    //a é a altura real
    float a = 2 * cam.zNear * tan(cam.fov* (M_PI/180)/2.0);

    //b é a largura real
    float b = (a * _width)/_height;

    //Passada do JFA
    auto start  = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for
    for(int y = 0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            QVector3D totalColor = QVector3D(0, 0, 0);

            #pragma omp parallel for
            unsigned int xAmount = sqrt(_rayNumber);
            unsigned int yAmount = xAmount;
            for (unsigned int aax = 0; aax < xAmount; aax++)
            {
                for (unsigned int aay = 0; aay < yAmount; aay++)
                {
                    //Lance um raio
                     Ray cameraPixelRay;
                     cameraPixelRay.origin = cam.eye;
                     cameraPixelRay.energy = QVector3D(1, 1, 1);
                     float factorY = (float)aay/((float)yAmount);
                     float factorX = (float)aax/((float)xAmount);

                     if(yAmount == 1)
                     {
                         factorY = 0;
                     }
                     else
                     {
                         factorY = (float)aay/((float)yAmount - 1);
                     }

                     if(xAmount == 1)
                     {
                         factorX = 0;
                     }
                     else
                     {
                         factorX = (float)aax/((float)xAmount - 1);
                     }

                     //d é um vetor que vai do olho até a tela
                     QVector3D d = (- cam.zNear * Ze) + (a*((((_height-y) + factorY)/(float)_height) - 0.5) * Ye) + b * ((((float)x + factorX)/(float)_width) - 0.5) * Xe;

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

                    QVector3D point = cameraPixelRay.hit(tCloser);

                    for (int i = 0; i < 8; i++)
                    {
                        QVector3D energy = cameraPixelRay.energy;
                        QVector3D cor = getColorAt(point, lights, cameraPixelRay, objectCloser, indexObject,tCloser, vertCloser);
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

            QVector3D finalColor = totalColor/ _rayNumber;

            image.setPixelColor(x, y, QColor(std::fmin(finalColor.x() *  255, 255), std::fmin(finalColor.y() * 255, 255), std::fmin(finalColor.z() * 255, 255)));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    _time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout<<"Tempo levado: "<<_time<<" s"<<std::endl;
    return image;
}

