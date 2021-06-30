#pragma once

#pragma once
#include<iostream>
#include<vector>
#include <fstream>
#include<string>
#include <QVector3D>
#include <QVector2D>
#include <sstream>

/**
* This ReaderOBJ header implements a reader os .OBJ files.
* The function expects only triangle meshes as entrances.
*
*@author Bianca Fragoso
*/

/**
 * @brief Reads .OBJ files and fill all the vectors with the points, normals, texture coordinates and indices.
 * @param fileName The path of the file.
 * @param points The vector that will be filled with 3D points.
 * @param normals The vector that will be filled with the normals.
 * @param texCoords The vector that will be filled with the texture coordinates.
 * @param indexPointsTriangles The vector that will be filled with the points indices.
 * @param indexNormalsTriangles The vector that will be filled with the normals indices.
 * @param indexTexTriangles The vector that will be filled with the texture coordinate indices.
 */
void readFile(std::string fileName, std::vector<QVector3D>& points, std::vector<QVector3D>& normals, std::vector<QVector2D>& texCoords, std::vector<unsigned int>& indexPointsTriangles,std::vector<unsigned int>& indexNormalsTriangles, std::vector<unsigned int>& indexTexTriangles)
{
    std::string line;
    std::string type = " ";
    std::ifstream myfile (fileName);

    while (1)
    {
      if(myfile.eof())
      {
          break;
      }

      else if(type != "v" && type != "vn" && type != "vt" && type != "f")
      {
          myfile >> type;
      }
      else if(type == "v")
      {
          QVector3D point;
          float number;
          myfile >> number;
          point.setX(number);
          myfile >> number;
          point.setY(number);
          myfile >> number;
          point.setZ(number);
          points.push_back(point);

          myfile >> type;
      }
      else if(type == "vt")
      {
          QVector2D tex;
          float number;
          myfile >> number;
          tex.setX(number);
          myfile >> number;
          tex.setY(number);
          texCoords.push_back(tex);
          myfile >> type;
      }
      else if(type == "vn")
      {
          QVector3D normal;
          float number;
          myfile >> number;
          normal.setX(number);
          myfile >> number;
          normal.setY(number);
          myfile >> number;
          normal.setZ(number);
          normals.push_back(normal);
          myfile >> type;
      }
      else if(type == "f" )
      {
          std::string s;
          std::string aux;
          std::vector<unsigned int> v;
          v.resize(300);
          std::string auxtype ;
          myfile >> auxtype;
          unsigned int cont = 0;
          while(auxtype != "f" && auxtype != "v" && auxtype != "vn" && auxtype != "vt")
          {
              if(myfile.eof())
              {
                  break;
              }
              std::istringstream f(auxtype);
              while (getline(f, s, '/'))
              {
                      v[cont] = static_cast<unsigned int>(std::atoi(s.c_str()));
                      cont++;
              }
              myfile >> auxtype;
          }
          type = auxtype;
          {
              for(unsigned int i = 0; i < 3 ; i++)
              {
                  indexPointsTriangles.push_back(v[3*i] - 1);
                  if(v[3*i + 1] == 0)
                  {
                      indexTexTriangles.push_back(v[3*i] - 1);
                  }
                  else
                  {
                      indexTexTriangles.push_back(v[3*i + 1] - 1);
                  }
                  indexNormalsTriangles.push_back(v[3*i + 2] - 1);
              }
          }
      }
    }
    myfile.close();
}


