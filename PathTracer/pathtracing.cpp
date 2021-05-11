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
    std::srand(12000); //use current time as seed for random generator

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



Object * PathTracing::reflection(Ray ray, float &tCloser, int &indexObject, int &vertCloser, int indMyObject)
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

    unsigned int aadepth = 40;

    //Passada do JFA
    auto start  = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for(int y = 0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            QVector3D totalColor = QVector3D(0, 0, 0);

            #pragma omp parallel for
            for (unsigned int aax = 0; aax < aadepth; aax++)
            {
                for (unsigned int aay = 0; aay < aadepth; aay++)
                {
                    QVector3D color = QVector3D(0, 0, 0);

                    //Lance um raio
                     Ray ray;
                     ray.origin = _camera.eye;
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
                     ray.direction = d;
                    RayHit bestHit;
                    QVector3D energy;
                    QVector3D cor;
                    for (int i = 0; i < 8; i++)
                    {
                        bestHit = Trace(ray);
                        energy = ray.energy;
                        cor =  getColorFinalAt(bestHit, ray);

                        color =  color + (energy * cor);
                        if(ray.energy.x() <= 0 && ray.energy.y() <= 0 && ray.energy.z() <= 0)
                        {
                            break;
                        }
                    }
                    totalColor = totalColor + color;

                }
            }

            int samples = aadepth*aadepth;
            QVector3D finalColor = totalColor/ samples;

            if(finalColor.x() < 0)
            {
                finalColor.setX(0);
            }
            if(finalColor.y() < 0)
            {
                finalColor.setY(0);
            }
            if(finalColor.z() < 0)
            {
                finalColor.setZ(0);
            }
            image.setPixelColor(x, y, QColor(std::fmin(finalColor.x() * 255, 255), std::fmin(finalColor.y() * 255, 255), std::fmin(finalColor.z() * 255, 255)));
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


QVector3D PathTracing::getColorAt(RayHit &hit, Ray &ray)
{
    if(hit.t < FLT_MAX)
    {

        QVector3D aux = QVector3D(1, 1, 1) - hit.specular;
        QVector3D min = QVector3D(std::min(aux.x(), hit.albedo.x()), std::min(aux.y(), hit.albedo.y()), std::min(aux.z(), hit.albedo.z()));
        QVector3D diffuse = 2 * min;

        float specChance = energy(hit.specular);
        float diffChance = energy( min);
        float sum = specChance + diffChance;
        specChance /= sum;
        diffChance /= sum;
        // Roulette-select the ray's path
        float roulette = rand();

        if (roulette < specChance)
        {
            // Specular reflection
            ray.origin = hit.position + hit.normal * 0.001f;

            QVector3D reflect = ray.direction - 2 * hit.normal * QVector3D::dotProduct(ray.direction, hit.normal);
            ray.direction = reflect;
            float dot = clamp(QVector3D::dotProduct(hit.normal.normalized(), ray.direction.normalized() ), 0, 1);

            ray.energy *= (1.0f / specChance) * hit.specular * dot;
        }
        else
        {
            // Diffuse reflection
            //ray.origin = hit.position + hit.normal * 0.001f;
            ray.direction = SampleHemisphere(hit.normal);
            float dot = clamp(QVector3D::dotProduct(hit.normal, ray.direction), 0, 1);

            ray.energy *= (1.0f / diffChance) * diffuse * dot;
        }
        return QVector3D(0, 0, 0);
    }
    else
    {
        ray.energy = QVector3D(0.0f, 0.0f, 0.0f);

        return _backgroundColor;
    }
}


float SmoothnessToPhongAlpha(float s)
{
    return pow(1000.0f, s * s);
}

QVector3D PathTracing::getColorFinalAt(RayHit &hit, Ray &ray)
{
    if(hit.t < FLT_MAX)
    {

         //Calculate chances of diffuse and specular reflection
        QVector3D aux = QVector3D(1, 1, 1) - hit.specular;
        QVector3D min = QVector3D(std::min(aux.x(), hit.albedo.x()), std::min(aux.y(), hit.albedo.y()), std::min(aux.z(), hit.albedo.z()));
        QVector3D diffuse = 2 * min;

        float specChance = energy(hit.specular);
        float diffChance = energy(min);

        // Roulette-select the ray's path
        float roulette = rand();
        if (roulette < specChance)
        {
            // Specular reflection
            ray.origin = hit.position + hit.normal * 0.001f;
            float alpha = SmoothnessToPhongAlpha(hit.smoothness);
            QVector3D reflect = ray.direction - 2 * hit.normal * QVector3D::dotProduct(ray.direction, hit.normal);
            ray.direction = SampleHemisphere(reflect.normalized(), alpha);
            float f = (alpha + 2) / (alpha + 1);
            float dot = clamp(QVector3D::dotProduct(hit.normal.normalized(), ray.direction.normalized()) * f, 0, 1);

            ray.energy *= (1.0f / specChance) * hit.specular * dot;
        }
        else if (diffChance > 0 && roulette < specChance + diffChance)
        {
            // Diffuse reflection
            ray.origin = hit.position + hit.normal * 0.001f;
            ray.direction = SampleHemisphere(hit.normal, 1.0f);
            ray.energy *= (1.0f / diffChance) * min;
        }
        else
        {
            // Terminate ray
            ray.energy = QVector3D(0, 0, 0);
        }

        return hit.emission;
    }
    else
    {
        ray.energy = QVector3D(0.0f, 0.0f, 0.0f);

        return _backgroundColor;
    }
}



QVector3D PathTracing::getColorDiffuseAt(RayHit &hit, Ray &ray)
{
    if(hit.t < FLT_MAX)
    {
        ray.origin = hit.position + hit.normal * 0.001f;
        ray.direction = SampleHemisphere(hit.normal);

        QVector3D diffuse = 2 * hit.albedo;

        float dot = clamp(QVector3D::dotProduct(hit.normal, ray.direction), 0, 1);

        ray.energy *= (diffuse) * dot;
        return QVector3D(0, 0, 0);
    }

    else
    {
        ray.energy = QVector3D(0.0f, 0.0f, 0.0f);
        return _backgroundColor;
    }
}



QVector3D PathTracing::calculateAmbient(IntersectRecord intersection, Light light, int ind1)
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



QVector3D PathTracing::calculatePhongDiffuse(IntersectRecord intersection, Light light, int ind1 )
{
   Object *object = intersection.object;
   Material material = intersection.object->getMaterial();
   RayHit hit = intersection.hit;

   QVector3D L = (light.position - hit.position).normalized();

   float lambertian = QVector3D::dotProduct(L, hit.normal);
    QVector3D color(0, 0, 0);
   // if(lambertian > 0)
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
    QVector3D corVec = /*lambertian * light.diffuse * */color;

    return corVec;
}



QVector3D PathTracing::calculatePhongSpecular(IntersectRecord intersection, Light light)
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



QVector3D PathTracing::getAlbedoPoint(RayHit hit, Object *object, int indVert)
{
     Material material = object->getMaterial();
     QVector3D color = {0, 0, 0};
     if(material.hasTexture())
     {
         TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(object);

         QVector3D texInt = mesh->getTexCoordinatesInsideTriangle(indVert, hit.position, _model);

         QImage texture = material.getTexture();
         QColor aux = texture.pixelColor(texInt.x() * texture.width(), texture.height() - (texInt.y() * texture.height()));
         color = QVector3D(aux.red()/255.0, aux.green()/255.0, aux.blue()/255.0);
     }
     else
     {
         color = material.getAlbedo();
     }


     QVector3D corVec = color;

     return corVec;
}



float PathTracing::rand()
{
    float rand = std::rand();
    float number;
    double randMax = RAND_MAX;
    if(rand + _seed <= RAND_MAX)
    {
        number = ((float) (rand + _seed) / (RAND_MAX ));
        if(number > 1 || number < 0)
        {

        }
        _seed++;
        return number;
    }
    number = 1;
    _seed = 0;
    return 1;

}



float PathTracing::rand2(QVector3D point)
{
    float intPart;
    float result = std::modf(sin(_seed / 100.0f * QVector3D::dotProduct(point, QVector3D(12.9898f, 78.233f, 0))) * 43758.5453f, &intPart);
    _seed += 1.0f;
    return result;
}



QVector3D PathTracing::SampleHemisphere(QVector3D normal, float alpha)
{
    // Uniformly sample hemisphere direction
    float cosTheta = std::pow(rand(), 1.0/(alpha + 1));
    float sinTheta = sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
    float phi = 2 * M_PI * rand();
    QVector3D tangentSpaceDir = QVector3D(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    QMatrix4x4 tangentSpaceMatrix =  GetTangentSpace(normal);
    // Transform direction to world space
    QVector3D result = (tangentSpaceDir * tangentSpaceMatrix);
    return result;

//    // Uniformly sample hemisphere direction
//    float cosTheta = rand();
//    float sinTheta = sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
//    float phi = 2 * M_PI * rand();
//    QVector3D tangentSpaceDir = QVector3D(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
//    QMatrix4x4 tangentSpaceMatrix =  GetTangentSpace(normal);
//    QVector3D result = (tangentSpaceDir * tangentSpaceMatrix);

//    // Transform direction to world space
//    return result;
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
    return QMatrix4x4(tangent.x(), tangent.y(), tangent.z(), 1, binormal.x(), binormal.y(), binormal.z(), 1, normal.x(), normal.y(), normal.z(), 1, 0, 0, 0, 1);
}



RayHit PathTracing::Trace(Ray ray)
{
    RayHit hit;

    //Variável que vai guardar qual é o menor t, ou seja, qual o ponto mais a frente que o raio atinge
    hit.t = FLT_MAX;
    int vertCloser;
    Object *objectCloser;

    for(unsigned int o = 0; o < _objects.size(); o++)
    {
        ObjectType type = _objects[o]->getObjectType();
        if(type == ObjectType::SPHERE)
        {
            Sphere *s = dynamic_cast<Sphere *>(_objects[o]);
            float t = s->intersectsWith(ray, _model);

            if( t < hit.t && t>0)
            {
                hit.t = t;
                objectCloser = _objects[o];
                //indexObject = o;
            }
        }
        else
        {
            TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(_objects[o]);
            int indTri;
            float t = mesh->intersectsWithRay(ray, _model, hit.t, indTri);
            if( t < hit.t && t>0 && indTri >= 0)
            {
                hit.t = t;
                objectCloser = _objects[o];
                //indexObject = o;
                vertCloser = indTri;
            }
        }
    }

    if(hit.t < FLT_MAX)
    {
        hit.position = ray.hit(hit.t);

        if(objectCloser->getObjectType() == ObjectType::SPHERE)
        {
            hit.normal = dynamic_cast<Sphere*>(objectCloser)->normalAt(hit.position);
            hit.normal.normalize();
        }
        else
        {
             TriangleMesh *mesh = dynamic_cast<TriangleMesh *>(objectCloser);
             hit.normal = mesh->getNormalInsideTriangle(vertCloser, hit.position, _model);
             hit.normal.normalize();
        }

        hit.albedo = getAlbedoPoint(hit, objectCloser, vertCloser);
        hit.specular = objectCloser->getMaterial().getSpecular();
        hit.emission = objectCloser->getMaterial().getEmission();
        hit.smoothness = objectCloser->getMaterial().getSmoothness();
    }

    return hit;

}



float PathTracing::clamp(float number, float low, float high)
{
    if(number <= low)
    {
        return low;
    }
    else if (number >= high)
    {
        return high;
    }
    return number;
}

