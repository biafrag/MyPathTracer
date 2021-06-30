#include "sphere.h"
#define M_PI 3.14159265358979323846

/**
 * @brief Sphere
 *
 * The Sphere class implements a sphere object.
 *
 * @author Bianca Fragoso
 */

int getIndex( int i, int j, int n )
{
    return j + i * ( n + 1 );
}



Sphere::Sphere(QVector3D center, float radius)
    :_center(center),
     _radius(radius)
{
    _type = ObjectType::SPHERE;

    const int n = 30;
    const int m = 30;

    const int numTriangles = 2 * n * m;
    const int numVertices = ( n + 1 ) * ( m + 1 );

    std::vector<QVector3D> points;

    for( unsigned int i = 0; i <= n; i++ )
    {
        for( unsigned int j = 0; j <= m; j++ )
        {
            //Atualizar as coordenadas de textura
            float s = (float) i / n;
            float t = (float) j / m;
            //texCoords.push_back(glm::vec2(s,t));

            //Calcula os parâmetros
            double theta = 2 * s * M_PI;
            double phi = t * M_PI;
            double sinTheta = sin( theta );
            double cosTheta = cos( theta );
            double sinPhi = sin( phi );
            double cosPhi = cos( phi );

            //Calcula os vértices == equacao da esfera
            points.push_back( _radius * QVector3D(cosTheta * sinPhi,
                                          cosPhi,
                                          sinTheta * sinPhi) + _center);
        }
    }

    std::vector<unsigned int> indicesS;
    indicesS.resize(numTriangles*3);

    unsigned int startIndex = _points.size();
    _points = points;

    std::vector<QVector3D> normals;
    normals = points;

    _normals = normals;

    //Preenche o vetor com a triangulação
    unsigned int k = 0;
    for( unsigned int i = 0; i < n; i++ )
    {
        for( unsigned int j = 0; j < m; j++ )
        {
            indicesS[ k++ ] = getIndex( i + 1, j, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i + 1, j + 1, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i, j, n ) + startIndex;


            indicesS[ k++ ] = getIndex( i + 1, j + 1, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i, j + 1, n ) + startIndex;
            indicesS[ k++ ] = getIndex( i, j, n ) + startIndex;
        }
    }

    _indices = indicesS;
}



template<class T> void setUniformArrayValue(QOpenGLShaderProgram *program,
                                       const QString& arrayName,
                                       const QString& varName,
                                       int index,
                                       const T& value)
{
    QString firstName = QString("%1[%2].%3")
            .arg(arrayName)
            .arg(index)
            .arg(varName);
    std::string nameStd = firstName.toStdString();
    const char* name = nameStd.c_str();
    program->setUniformValue(name, value);
}



float Sphere::intersectsWith(Ray ray, QMatrix4x4 model)
{
    _model = model;
    QVector3D center = model * _center;
    float a = QVector3D::dotProduct(ray.direction, ray.direction);
    float b = 2 * QVector3D::dotProduct(ray.direction, ray.origin - center);
    float c = QVector3D::dotProduct(ray.origin - center, ray.origin - center) - (_radius * _radius);

    float discriminant = b*b - 4*a*c;

    if(discriminant < 0)
    {
        return -1;
    }
    else
    {
        float numerator = - b - sqrt(discriminant);

        return numerator / (2.0 * a);

    }
}



QVector3D Sphere::normalAt(QVector3D point)
{
    return (point - _model *_center).normalized();
}
