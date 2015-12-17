#pragma once

#include <vector>
#include <string>

using namespace std;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glimac/Image.hpp>

#include "mesh.hpp"

class Model {
    public:
        /* Functions */
        Model(string path);
        void Draw(Shader shader);
    private:
        /* Model Data */
        vector<Mesh> meshes;
        string directory;
        vector<Texture> textures_loaded;
        /* Functions */
        void loadModel(string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
