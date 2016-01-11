#define GLM_FORCE_RADIANS
#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <memory>
#include <string>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glimac/glm.hpp>
//#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/SDLWindowManager.hpp>

#include <GL/glew.h>

#include "engine/mesh.hpp"
#include "engine/shader.hpp"

GLint TextureFromFile(const string path, string directory);

class Model
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    Model(string const& path, float tx, float ty, float tz, float sx, float sy, float sz);
    // Draws the model, and thus all its meshes
    void Draw(Shader const& shader);

    glm::mat4 getModelMatrix();

private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    glm::mat4 m_modelmat;
    vector<Texture> textures_loaded;    // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string path);
    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    GLint TextureFromFile(const string path, string directory);

};


void drawModels(map<int, unique_ptr<Model> > const& models, Shader const& shader);
map<int, unique_ptr<Model> > modelsFromFile(string const& filepath);
