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

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
//#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/SDLWindowManager.hpp>

#include "engine/mesh.hpp"

class Shader;

class Model
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    Model(std::string const& path, std::vector<float>& xyz);
    // Draws the model, and thus all its meshes
    void Draw(Shader const& shader);

private:
    /*  Model Data  */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<glm::mat4> m_modelmatVector;
    std::vector<Texture> textures_loaded;    // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string path);
    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    GLint TextureFromFile(const std::string path, std::string directory);

};

GLint TextureFromFile(const std::string path, std::string directory);

void drawModels(std::map<int, std::unique_ptr<Model> > const& models, Shader const& shader);
std::map<int, std::unique_ptr<Model> > modelsFromFile(std::string const& filepath);

glm::mat4 getModelMatrix(int numModelMat);
