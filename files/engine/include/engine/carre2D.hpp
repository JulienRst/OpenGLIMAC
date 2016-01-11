#pragma once

#include <cstdlib>
#include <iostream>
#include <GL/glew.h>

#include <glimac/glm.hpp>

// -------- STRUCTURES --------------//

struct Vertex2DUV{
    glm::vec2 position;
    glm::vec2 textpos;

    Vertex2DUV(){
        position = glm::vec2(0,0);
        textpos = glm::vec2(0,0);
    }

    Vertex2DUV(glm::vec2 p, glm::vec2 t){
        position = p;
        textpos = t;
    }
};

glm::mat3 scale(float sx, float sy);

class Carre2D {
    private:
        GLuint vbo;
        GLuint vao;
        Vertex2DUV* m_Vertices;
    public:
        Carre2D();
        ~Carre2D();
        void draw(GLuint id,GLuint id_texture,GLuint displayedTexture);
};
