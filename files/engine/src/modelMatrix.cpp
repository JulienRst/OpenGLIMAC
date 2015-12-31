#include "engine/modelMatrix.hpp"

using namespace std;
using namespace glm;

ModelMatrix::ModelMatrix(){
    m_path = "";
    m_translate = vec3(0., 0., 0.);
    m_scale = vec3(0., 0., 0.);
}

ModelMatrix::ModelMatrix(string path, float tx, float ty, float tz, float sx, float sy, float sz){
    m_path = path;
    m_translate = vec3(tx, ty, tz);
    m_scale = vec3(sx, sy, sz);
}

string ModelMatrix::getPath(){
    return m_path;
}

vec3 ModelMatrix::getTranslate(){
  return m_translate ;
}

vec3 ModelMatrix::getScale(){
  return m_scale ;
}

