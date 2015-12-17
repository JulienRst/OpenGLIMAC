#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "shader.hpp"

using namespace glm;
using namespace std;

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string type;
    aiString path;
};

class Mesh {
    public:
        /* Mesh Data */
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        /* Functions */
        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
        void Draw(Shader shader);
    private:
        /* Render data */
        GLuint VAO, VBO, EBO;
        /* Functions */
        void setupMesh();
};
