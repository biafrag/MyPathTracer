#include "tracer.h"
#include "sphere.h"
#include "trianglemesh.h"

Tracer::Tracer()
{

}



float Tracer::getTime()
{
    return _time;
}



void Tracer::setRayNumber(unsigned int number)
{
    _rayNumber = number;
}



void Tracer::setDimensions(int width, int height)
{
    _width = width;

    _height = height;
}



RayHit Tracer::Trace(Ray ray)
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

        //hit.albedo = getAlbedoPoint(hit, objectCloser, vertCloser);
        hit.specular = objectCloser->getMaterial().getSpecular();
        hit.emission = objectCloser->getMaterial().getEmission();
        hit.smoothness = objectCloser->getMaterial().getSmoothness();
    }

    return hit;
}
