#pragma once

#include <cstdlib>
#include <iostream>
#include <GL/glew.h>

#include <glimac/glm.hpp>

// -------- STRUCTURES --------------//

struct Vertex3DUV{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textpos;

    Vertex3DUV(){
        position = glm::vec3(0,0,0);
        normal = glm::vec3(0,0,0);
        textpos = glm::vec2(0,0);
    }

    Vertex3DUV(glm::vec3 p, glm::vec3 n, glm::vec2 t){
        position = p;
        textpos = t;
        normal = n;
    }
};

glm::mat4 scale(float sx, float sy, float sz);

class Carre3D {
    private:
        GLuint vbo;
        GLuint vao;
        Vertex3DUV* m_Vertices;
        std::string m_type;
    public:
        Carre3D(std::string type);
        ~Carre3D();
        void draw(GLuint id_texture,GLuint displayedTexture);
        void getFrontMatrix();
        void getBackMatrix();
};
