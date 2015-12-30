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
    string m_path;
    vec3 m_translate, m_scale;

    public:
    ModelMatrix();
    string getPath();
    ModelMatrix(string path, float tx, float ty, float tz,
                float sx, float sy, float sz);

    //creation d'un vector de pointeurs vers des modelmatrix. ça evite de devoir faire des free.
    vector<unique_ptr<ModelMatrix>> loadModelsFromFile(string filepath);
};
