#define GLM_FORCE_RADIANS

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <set>
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <memory>

using namespace std;
using namespace glm;


class ModelMatrix{
    private:
    vec3 m_translate;
    vec3 m_scale;
    string m_path;

    public:
    ModelMatrix();
    ModelMatrix(string path, float tx, float ty, float tz,
                float sx, float sy, float sz);

    string getPath();
    vec3 getTranslate();
    vec3 getScale();
};
