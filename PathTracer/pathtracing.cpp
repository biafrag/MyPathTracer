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

    unsigned int aadepth = 3;

    //Passada do JFA
    auto start  = std::chrono::high_resolution_clock::now();
    Object * objectCloserFocus ;

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
                    int vertCloser;
                    Object * objectCloser;
                    int indexObject = 0;


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


                    if(tCloser < FLT_MAX)
                    {
                        float tCloserFocus = tCloser;
                        objectCloserFocus = objectCloser;
                        int indexObjectFocus = indexObject;
                        int vertCloserFocus = vertCloser;
                        QVector3D point = getRayPoint(tCloser, cameraPixelRay);
                        QVector3D color(0, 0, 0);
                        int sample = 1;
                        for(int j = 0; j < sample; j++)
                        {
                            tCloser = tCloserFocus;
                            objectCloser = objectCloserFocus;
                            indexObject = indexObjectFocus;
                            vertCloserFocus = vertCloser;
                            cameraPixelRay.origin = _camera.eye;
                            cameraPixelRay.energy = QVector3D(1, 1, 1);
                            cameraPixelRay.direction = d;
                            point = getRayPoint(tCloser, cameraPixelRay);

                            RayHit hit;
                            hit.t = tCloser;
                            hit.position = point;
                            IntersectRecord intersection;
                            intersection.hit = hit;
                            intersection.object =  _objects[indexObject];

                            for (int i = 0; i < 8; i++)
                            {

                                QVector3D energy = cameraPixelRay.energy;
                                QVector3D cor =  getColorAt(intersection, cameraPixelRay, indexObject, vertCloser);
                                if(cor.x() >= 1 && cor.y() >= 1 && cor.x() >= 1)
                                {
                                    //printf("Deu maior\n");
                                }

                                totalColor =  totalColor + energy * cor;
                                if(energy.x() <= 0 && energy.y() <= 0 && energy.z() <= 0)
                                {
                                    break;
                                }
                                tCloser = FLT_MAX;
                                objectCloser = reflection(cameraPixelRay, tCloser, indexObject, vertCloser, indexObject);
                                hit.t = tCloser;
                                hit.position =  getRayPoint(tCloser, cameraPixelRay);
                                intersection.hit = hit;
                                intersection.object =  _objects[indexObject];
                            }
                        }

                    }
                    else
                    {
                        totalColor = QVector3D(0.7, 0.7, 0.7);
                    }
                }
            }


            QVector3D finalColor = totalColor/aadepth*aadepth;

            if(finalColor.x() > 1 && finalColor.y() > 1 && finalColor.x() > 1)
            {
                printf("Deu maior\n");
            }
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


QVector3D PathTracing::getColorAt(IntersectRecord &intersection, Ray &ray, int indObj, int indVert)
{
    RayHit hit = intersection.hit;
    Object *object = intersection.object;
    Material material = object->getMaterial();

    if(hit.t < FLT_MAX)
    {
        QVector3D specular = object->getMaterial().getSpecular();
        //QVector3D specular = QVector3D(0.6f, 0.6f, 0.6f);

        QVector3D N;
        hit.position = ray.hit(hit.t);

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

       // ray.origin = hit.position + hit.normal * 0.001f;
        QVector3D reflect = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);
        ray.direction = SampleHemisphere(hit.normal);

        QVector3D albedo = calculatePhongDiffuse(intersection, Light(), indVert);
        QVector3D aux = QVector3D(1, 1, 1) - specular;
        QVector3D min = QVector3D(std::min(aux.x(), albedo.x()), std::min(aux.y(), albedo.y()), std::min(aux.z(), albedo.z()));
        QVector3D diffuse = 2 * albedo;

        float alpha = 15.0f;
        float dot2 = clamp(/*std::fabs*/(QVector3D::dotProduct(hit.normal, ray.direction)), 0, 1);
        float dot1 = clamp(/*std::fabs*/(QVector3D::dotProduct(ray.direction, reflect)), 0, 1);

        specular = material.getSpecular() * (alpha + 2) /** pow(dot1, alpha)*/;

        ray.energy *= (diffuse /*+ specular*/) * dot2;
        return QVector3D(0, 0, 0);
    }
//        // Calculate chances of diffuse and specular reflection
//        QVector3D albedo = calculatePhongDiffuse(intersection, Light(), indVert);
//        QVector3D aux = QVector3D(1, 1, 1) - specular;
//        QVector3D min = QVector3D(std::min(aux.x(), albedo.x()), std::min(aux.y(), albedo.y()), std::min(aux.z(), albedo.z()));
//        QVector3D diffuse = 2 * min;

//        float specChance = energy(material.getSpecular());
//        float diffChance = energy(min);
//        float sum = specChance + diffChance;
//        specChance /= sum;
//        diffChance /= sum;
//        // Roulette-select the ray's path
//        float roulette = rand();

//        if (roulette < specChance)
//        {
//            // Specular reflection
//            //ray.origin = hit.position /*+ hit.normal * 0.001f*/;
//            QVector3D reflect = ray.direction - 2 * N * QVector3D::dotProduct(ray.direction, N);
//            ray.direction = reflect;
//            float dot = clamp(QVector3D::dotProduct(hit.normal, ray.direction), 0, 1);

//            ray.energy *= (1.0f / specChance) * material.getSpecular() * dot;
//        }
//        else
//        {
//            // Diffuse reflection
//            //ray.origin = hit.position + hit.normal * 0.001f;
//            ray.direction = SampleHemisphere(hit.normal);
//            float dot = clamp(QVector3D::dotProduct(hit.normal, ray.direction), 0, 1);

//            ray.energy *= (1.0f / diffChance) * diffuse * dot;
//        }
//        return QVector3D(0, 0, 0);
//    }
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

