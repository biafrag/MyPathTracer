#include "raytracing.h"
#include <QMatrix4x4>
#include <QImage>
#include "renderer.h"
#define M_PI 3.1415

RayTracing::RayTracing(int w, int h, QMatrix4x4 model, Renderer::Camera cam,
                       std::vector<Object *> objects, std::vector<Light> lights)

    : _model(model),
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



RayTracing::Ray RayTracing::CreateRay(QVector3D origin, QVector3D direction)
{
    //Cria raio com uma origem e uma direção
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}



RayTracing::Ray RayTracing::CreateCameraRay(QVector3D uv, QMatrix4x4 model, QMatrix4x4 proj)
{
    // Transforma a origem da câmera em espaço de mundo
    QVector3D origin = model * QVector3D(0.0f, 0.0f, 0.0f);

    // Invert the perspective projection of the view-space position
    QVector3D direction = model * QVector3D(uv.x(), uv.y(), 0.0f);

    // Transform the direction from camera to world space and normalize
    direction = model * direction;
    direction.normalize();
    return CreateRay(origin, direction);
}



void RayTracing::AddPixels(QVector3D uv, QMatrix4x4 model, QMatrix4x4 proj)
{
    // Get a ray for the UVs
    Ray ray = CreateCameraRay(uv, model, proj);

    // Write some colors
    _result.push_back(QVector3D(ray.direction * 0.5 + QVector3D(0.5f, 0.5f, 0.5f)));
}



RayTracing::RayHit RayTracing::CreateRayHit()
{
    RayHit hit;
    hit.position = QVector3D(0.0f, 0.0f, 0.0f);
//    hit.distance = 1.#INF;
    hit.normal = QVector3D(0.0f, 0.0f, 0.0f);
    return hit;
}


void RayTracing::IntersectGroundPlane(Ray ray, RayHit &bestHit)
{
    // Calculate distance along the ray where the ground plane is intersected
    float t = -ray.origin.y() / ray.direction.y();
    if (t > 0 && t < bestHit.distance)
    {
        bestHit.distance = t;
        bestHit.position = ray.origin + t * ray.direction;
        bestHit.normal = QVector3D(0.0f, 1.0f, 0.0f);
    }
}



RayTracing::RayHit RayTracing::Trace(RayTracing::Ray ray)
{
    RayHit bestHit = CreateRayHit();
    IntersectGroundPlane(ray, bestHit);
    return bestHit;
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

    std::vector<unsigned int> indices;
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    for(unsigned int o = 0; o < _objects.size(); o++)
    {
        if(o != index)
        {
            //Checar se raio atinge outro objeto sem ser esse
            indices = _objects[o]->getIndices();
            vertices = _objects[o]->getVertices(_model);
            normals = _objects[o]->getNormals(_model.transposed().inverted());

            for(unsigned int i = 0; i < indices.size() - 2; i = i + 3)
            {
                int i1 = indices[i];
                int i2 = indices[i+1];
                int i3 = indices[i+2];

                //Vetores formados pelos pontos do triângulo
                QVector3D e2 = vertices[i3] - vertices[i2];
                QVector3D e3 = vertices[i1] - vertices[i3];

                //Normal ao triângulo
                QVector3D normal = QVector3D::crossProduct(e2, e3).normalized();

                //Produto interno entre vetor do raio e a normal não pode ser 0 senão quer dizer que eles são perpendiculares
                // Ou seja, a superfície e o raio são paralelos então o raio nunca atinge aquele lugar da superfície

                float prodDN = QVector3D::dotProduct(pointLightRay.direction, normal);
                if(prodDN != 0)
                {
                    //t é a coordenada paramétrica do ponto no raio
                    float t = (QVector3D::dotProduct(vertices[i1] - point, normal))/prodDN;

                    if(t > 0)
                    {
                        //p é o ponto que raio atinge na superfície
                        QVector3D p = getRayPoint(t, pointLightRay);

                        //Verificar se ponto que raio atinge intersecta o triângulo
                        bool verif = triangleVerification(vertices[i1], vertices[i2], vertices[i3], p);

                        if(verif)
                        {
                            return true;
                        }
                    }
                }
            }
        }

    }
    return false;
}



QColor RayTracing::reflection(std::vector<Light> lights, QVector3D point, QVector3D d, QVector3D n, unsigned int index, std::vector<Object *> objects, QMatrix4x4 model, QVector3D eye)
{
    float thetaRad = acos(QVector3D::dotProduct(d, n)/(d.length() * n.length()));

    float theta = 180 * thetaRad/ M_PI;

    QVector3D cross = QVector3D::crossProduct(d, n);
    QMatrix4x4 rot;
    rot.rotate(theta, cross);

    QVector3D newDirection = rot * n;

    std::vector<unsigned int> indices;
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    float tCloser = FLT_MAX;
    unsigned int vertCloser;
    Object * objectCloser;
    unsigned int indexObject;

    for(unsigned int o; o < objects.size(); o++)
    {
        if(o != index)
        {
            //Checar se raio atinge outro objeto sem ser esse
            indices = objects[o]->getIndices();
            vertices = objects[o]->getVertices(model);
            normals = objects[o]->getNormals(model.transposed().inverted());
            for(unsigned int i = 0; i < indices.size() - 2; i = i + 3)
            {
                int i1 = indices[i];
                int i2 = indices[i+1];
                int i3 = indices[i+2];

                //Vetores formados pelos pontos do triângulo
                QVector3D e2 = vertices[i3] - vertices[i2];
                QVector3D e3 = vertices[i1] - vertices[i3];

                //Normal ao triângulo
                QVector3D normal = QVector3D::crossProduct(e2, e3).normalized();

                //Produto interno entre vetor do raio e a normal não pode ser 0 senão quer dizer que eles são perpendiculares
                // Ou seja, a superfície e o raio são paralelos então o raio nunca atinge aquele lugar da superfície

                float prodDN = QVector3D::dotProduct(d, normal);
                if(prodDN != 0)
                {
                    //t é a coordenada paramétrica do ponto no raio
                    float t = (QVector3D::dotProduct(vertices[i1] - point, normal))/prodDN;

                    if(t > 0)
                    {
                        //p é o ponto que raio atinge na superfície
                        QVector3D p = point + (t * d);

                        //Verificar se ponto que raio atinge intersecta o triângulo
                        bool verif = triangleVerification(vertices[i1], vertices[i2], vertices[i3], p);

                        if(verif)
                        {
                            if(t < tCloser)
                            {
                                tCloser = t;
                                vertCloser = i;
                                objectCloser = objects[o];
                                indexObject = o;

                            }
                        }
                    }
                }
            }
        }
    }
    if(tCloser < FLT_MAX)
    {
        indices = objectCloser->getIndices();
        vertices = objectCloser->getVertices(model);
        normals = objectCloser->getNormals(model.transposed().inverted());

        unsigned int ind1, ind2, ind3;
        ind1 = indices[vertCloser];
        ind2 = indices[vertCloser + 1];
        ind3 = indices[vertCloser + 2];

        //Determinando ponto mais próximo
        QVector3D point = eye + (tCloser * d);
        QVector3D alfas= getBaricentricCoordinates(vertices[ind1],vertices[ind2],vertices[ind3], point);

        //Phong

        QColor corF = QColor(0, 0, 0);
        Object::Material material = objectCloser->getMaterial();

        for(auto light : lights)
        {

            //Normal naquele ponto específico. Fazemos como se fosse uma média entre as normais do triângulo usando as coordenadas baricêntricas
            QVector3D normalInt = alfas.x() * normals[ind1] + alfas.y() * normals[ind2] + alfas.z() * normals[ind3];

            QVector3D N = normalInt.normalized();
            QVector3D L = (light.position - point).normalized();

            float lambertian = QVector3D::dotProduct(L, N);
            QVector3D specular(0, 0, 0);
            QVector3D diffuse(0, 0, 0);
            QVector3D ambient = light.ambient * material.color;
            if(lambertian > 0)
            {
                diffuse = lambertian * light.diffuse * material.color; //Adicionar propriedade dos materiais do objeto depois
                QVector3D V = (eye - point).normalized();
                QVector3D H = (L+V).normalized();
                float dotP = QVector3D::dotProduct(N,H);
                float ispec;

                if(dotP > 0)
                {
                    ispec = std::pow(dotP, light.shi);
                }
                else
                {
                    ispec = 0;
                }
                specular = light.specular * ispec;
                alfas= getBaricentricCoordinates(vertices[ind1],vertices[ind2],vertices[ind3], point);

            }
            QVector3D aux = (diffuse + ambient + specular) * 255;

            QColor cor(std::fmin(255, aux.x()), std::fmin(255, aux.y()), std::fmin(255, aux.z()));

            corF = QColor(std::fmin(corF.red() + cor.red(), 255), std::fmin(corF.green() + cor.green(), 255), std::fmin(corF.blue() + cor.blue(), 255));
        }
    }
    return QColor(20,20,20);
}



QVector3D RayTracing::getRayPoint(float t, RayTracing::Ray ray)
{
    return ray.origin + (t * ray.direction);
}






QImage RayTracing::generateRayTracingImage()
{
    QImage image(_width, _height, QImage::Format_RGB32);

    //a é a altura real
    float a = 2 * _camera.zNear * tan(_camera.fov* (M_PI/180)/2.0);

    //b é a largura real
    float b = (a * _width)/_height;

    #pragma omp parallel for
    for(int y = 0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            //Lance um raio
             Ray cameraPixelRay;
             cameraPixelRay.origin = _camera.eye;

             //d é um vetor que vai do olho até a tela
            QVector3D d = (- _camera.zNear * _Ze) + (a*(((_height-y)/(float)_height) - 0.5) * _Ye) + b * (((float)x/(float)_width)-0.5) * _Xe;

            cameraPixelRay.direction = d;

            //Variável que vai guardar qual é o menor t, ou seja, qual o ponto mais a frente que o raio atinge
            float tCloser = FLT_MAX;
            unsigned int vertCloser;
            Object * objectCloser;
            unsigned int indexObject;
            std::vector<unsigned int> indices;
            std::vector<QVector3D> vertices;
            std::vector<QVector3D> normals;
            for(unsigned int o = 0; o < _objects.size(); o++)
            {
                indices = _objects[o]->getIndices();
                vertices = _objects[o]->getVertices(_model);
                normals = _objects[o]->getNormals(_model.transposed().inverted());

                for(unsigned int i = 0; i < indices.size() - 2; i = i + 3)
                {
                    int i1 = indices[i];
                    int i2 = indices[i+1];
                    int i3 = indices[i+2];

                    //Vetores formados pelos pontos do triângulo
                    QVector3D e2 = vertices[i3] - vertices[i2];
                    QVector3D e3 = vertices[i1] - vertices[i3];

                    //Normal ao triângulo
                    QVector3D normal = QVector3D::crossProduct(e2, e3).normalized();

                    //Produto interno entre vetor do raio e a normal não pode ser 0 senão quer dizer que eles são perpendiculares
                    // Ou seja, a superfície e o raio são paralelos então o raio nunca atinge aquele lugar da superfície

                    float prodDN = QVector3D::dotProduct(d, normal);
                    if(prodDN != 0)
                    {
                        //t é a coordenada paramétrica do ponto no raio
                        float t = (QVector3D::dotProduct(vertices[i1] - _camera.eye, normal))/prodDN;

                        //p é o ponto que raio atinge na superfície
                        QVector3D p = getRayPoint(t, cameraPixelRay);

                        //Verificar se ponto que raio atinge intersecta o triângulo
                        bool verif = triangleVerification(vertices[i1], vertices[i2], vertices[i3], p);

                        if(verif && t < tCloser && t>0)
                        {
                            tCloser = t;
                            vertCloser = i;
                            objectCloser = _objects[o];
                            indexObject = o;

                        }
                    }

                }
            }


            //Pintar ponto mais próximo encontrado
            //Calcular contribuição da luz nesse ponto

            //Se o t que eu encontrei tiver intersectado algum objeto
            if(tCloser < FLT_MAX)
            {
                indices = objectCloser->getIndices();
                vertices = objectCloser->getVertices(_model);
                normals = objectCloser->getNormals(_model.transposed().inverted());

                unsigned int ind1, ind2, ind3;
                ind1 = indices[vertCloser];
                ind2 = indices[vertCloser + 1];
                ind3 = indices[vertCloser + 2];

                //Determinando ponto mais próximo
                QVector3D point = getRayPoint(tCloser, cameraPixelRay);
                QVector3D alfas= getBaricentricCoordinates(vertices[ind1],vertices[ind2],vertices[ind3], point);

                //Phong

                QColor corF = QColor(0, 0, 0);
                Object::Material material = objectCloser->getMaterial();
                if(!material.isReflective)
                {

                    for(auto light : _lights)
                    {
                        //Checa se luz vai cotribuir para o ponto
                            QVector3D ambient = light.ambient * material.color;

                            bool hasNoEffect = hasObjectObstacle(light, point, indexObject);
                            if(!hasNoEffect)
                            {
                                //Normal naquele ponto específico. Fazemos como se fosse uma média entre as normais do triângulo usando as coordenadas baricêntricas
                                QVector3D normalInt = alfas.x() * normals[ind1] + alfas.y() * normals[ind2] + alfas.z() * normals[ind3];

                                QVector3D N = normalInt.normalized();
                                QVector3D L = (light.position - point).normalized();

                                float lambertian = QVector3D::dotProduct(L, N);
                                QVector3D specular(0, 0, 0);
                                QVector3D diffuse(0, 0, 0);
                                if(lambertian > 0)
                                {
                                    diffuse = lambertian * light.diffuse * material.color; //Adicionar propriedade dos materiais do objeto depois
                                    QVector3D V = (_camera.eye - point).normalized();
                                    QVector3D H = (L+V).normalized();
                                    float dotP = QVector3D::dotProduct(N,H);
                                    float ispec;

                                    if(dotP > 0)
                                    {
                                        ispec = std::pow(dotP, light.shi);
                                    }
                                    else
                                    {
                                        ispec = 0;
                                    }
                                    specular = light.specular * ispec;
                                    alfas= getBaricentricCoordinates(vertices[ind1],vertices[ind2],vertices[ind3], point);

                                }
                                QVector3D aux = (diffuse + ambient + specular) * 255;

                                QColor cor(std::fmin(255, aux.x()), std::fmin(255, aux.y()), std::fmin(255, aux.z()));

                                corF = QColor(std::fmin(corF.red() + cor.red(), 255), std::fmin(corF.green() + cor.green(), 255), std::fmin(corF.blue() + cor.blue(), 255));
                            }
                            else
                            {
                                QVector3D aux = ambient * 255;
                                QColor cor(std::fmin(255, aux.x()), std::fmin(255, aux.y()), std::fmin(255, aux.z()));

                                corF = QColor(std::fmin(corF.red() + cor.red(), 255), std::fmin(corF.green() + cor.green(), 255), std::fmin(corF.blue() + cor.blue(), 255));
                            }
                        }
                }
                else
                {
                        //Normal naquele ponto específico. Fazemos como se fosse uma média entre as normais do triângulo usando as coordenadas baricêntricas
                        QVector3D normalInt = alfas.x() * normals[ind1] + alfas.y() * normals[ind2] + alfas.z() * normals[ind3];

                        QVector3D N = normalInt.normalized();
                        corF = reflection(_lights, point, d, N, indexObject, _objects, _model, _camera.eye);
                }

                image.setPixelColor(x, y, corF);
            }
            else
            {
                //Se não intersectou é porque é background
                image.setPixelColor(x, y, QColor(0, 0, 0));
            }



        }
    }

   return image;
}
