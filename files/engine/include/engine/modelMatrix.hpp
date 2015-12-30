#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>

#include <string>

using namespace std;

class ModelMatrix{
    private:
    string m_path;
    vec3 m_translate, m_scale;
    
    public:
    ModelMatrix();

    ModelMatrix(string path, float tx, float ty, float tz, 
                float sx, float sy, float sz);
    
    //creation d'un vector de pointeurs vers des modelmatrix. ça evite de devoir faire des free.
    vector<unique_ptr<ModelMatrix>> ModelMatrix::loadModelsFromFile(string filepath);
};