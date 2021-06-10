#include "plane.h"

Plane::Plane()
{
    _type = ObjectType::PLANE;

    std::vector<QVector3D> points, normals;
    std::vector<unsigned int> indices;
    points = {{-1,1,0}, {1,1,0},
               {-1,-1,0}, {1,-1,0}};
    QMatrix4x4 rot, trans;

    rot.rotate(90, QVector3D(1, 0 , 0));
    trans.translate(QVector3D(0, -2, 0));
    std::vector<QVector4D> points4D;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        points[i] = rot * points[i];
        points[i] = trans * points[i];

    }

    indices = {0, 2, 1, 1, 2, 3};

    normals = {{0, 0, 1}, {0, 0, 1},
                {0, 0, 1}, {0, 0, 1}};

    _points = points;
    _indices = indices;
    _normals = normals;

}



Plane::Plane(QMatrix4x4 translation, QMatrix4x4 rotation, QMatrix4x4 scale)
{
    _type = ObjectType::PLANE;
    std::vector<QVector3D> points, normals;
    std::vector<QVector2D> texCoords;
    std::vector<unsigned int> indices;
    points = {{-1,1,0}, {1,1,0},
               {-1,-1,0}, {1,-1,0}};

    std::vector<QVector4D> points4D;

    normals = {{0, 0, 1}, {0, 0, 1},
                {0, 0, 1}, {0, 0, 1}};

    QMatrix4x4 planeModel = translation * rotation * scale;

    texCoords = {{0, 1}, {1, 1},
                 {0, 0}, {1, 0}};
    for(unsigned int i = 0; i < points.size(); i++)
    {

        points[i] = planeModel * points[i];
    }

    indices = {0, 2, 1, 1, 2, 3};

    _points = points;
    _indices = indices;
    computeNormals();

    _texCoords = texCoords;


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
