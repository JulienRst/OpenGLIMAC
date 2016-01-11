#include "engine/carre3D.hpp"

using namespace glm;
using namespace std;

mat4 scale(float sx, float sy, float sz){
	return mat4(vec4(sx,0.f,0.f,0.f),vec4(0.f,sy,0.f,0.f),vec4(0.f,0.f,sz,0.f),vec4(0.f,0.f,0.f,1.f));
}

Carre3D::Carre3D(string type){
    //Create vbo
    glGenBuffers(1,&vbo);
        //Binding VBO on GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        //Putting all vertices
        Vertex3DUV vertices[6];
        if(type == "BACK"){
            vertices[0] = Vertex3DUV(glm::vec3(-1., 1., -1.),glm::vec3(glm::normalize((vec3(1.,1.,-1.) - vec3(-1.,1.,-1.))*(vec3(1.,-1.,-1.) - vec3(-1.,1.,-1.)))), glm::vec2(0.f, 0.f));
            vertices[1] = Vertex3DUV(glm::vec3(1., 1., -1.),glm::vec3(glm::normalize((vec3(-1.,1.,-1.) - vec3(1.,1.,-1.))*(vec3(-1.,1.,-1.) - vec3(1.,1.,-1.)))),glm::vec2(1.f, 0.f));
            vertices[2] = Vertex3DUV(glm::vec3(1., -1., -1.),glm::vec3(glm::normalize((vec3(1.,1.,-1.) - vec3(1.,-1.,-1.))*(vec3(-1.,1.,-1.) - vec3(1.,-1.,-1.)))),glm::vec2(1., 1.f));
            vertices[3] = Vertex3DUV(glm::vec3(-1., 1., -1.),glm::vec3(glm::normalize((vec3(1.,1.,-1.) - vec3(-1.,1.,-1.))*(vec3(-1.,-1.,-1.) - vec3(-1.,1.,-1.)))),glm::vec2(0.f, 0.f));
            vertices[4] = Vertex3DUV(glm::vec3(1., -1., -1.),glm::vec3(glm::normalize((vec3(-1.,1.,-1.) - vec3(1.,-1.,-1.))*(vec3(-1.,-1.,-1.) - vec3(1.,-1.,-1.)))),glm::vec2(1.f, 1.f));
            vertices[5] = Vertex3DUV(glm::vec3(-1., -1., -1.),glm::vec3(glm::normalize((vec3(-1.,1.,-1.) - vec3(-1.,-1.,-1.))*(vec3(1.,-1.,-1.) - vec3(-1.,-1.,-1.)))),glm::vec2(0., 1.f));
        }

        if(type == "FRONT"){
            vertices[0] = Vertex3DUV(glm::vec3(-1., 1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,1.) - vec3(-1.,1.,1.))*(vec3(1.,-1.,1.) - vec3(-1.,1.,1.)))), glm::vec2(1.f, 0.f));
            vertices[1] = Vertex3DUV(glm::vec3(1., 1., 1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(1.,1.,1.))*(vec3(-1.,1.,1.) - vec3(1.,1.,1.)))),glm::vec2(0.f, 0.f));
            vertices[2] = Vertex3DUV(glm::vec3(1., -1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,1.) - vec3(1.,-1.,1.))*(vec3(-1.,1.,1.) - vec3(1.,-1.,1.)))),glm::vec2(0., 1.f));
            vertices[3] = Vertex3DUV(glm::vec3(-1., 1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,1.) - vec3(-1.,1.,1.))*(vec3(-1.,-1.,1.) - vec3(-1.,1.,1.)))),glm::vec2(1.f, 0.f));
            vertices[4] = Vertex3DUV(glm::vec3(1., -1., 1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(1.,-1.,1.))*(vec3(-1.,-1.,1.) - vec3(1.,-1.,1.)))),glm::vec2(0.f, 1.f));
            vertices[5] = Vertex3DUV(glm::vec3(-1., -1., 1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(-1.,-1.,1.))*(vec3(1.,-1.,1.) - vec3(-1.,-1.,1.)))),glm::vec2(1.f, 1.f));
        }

        if(type == "UP"){
            vertices[0] = Vertex3DUV(glm::vec3(-1., 1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,1.) - vec3(-1.,1.,1.))*(vec3(1., 1., -1.) - vec3(-1.,1.,1.)))), glm::vec2(1.f, 0.f));
            vertices[1] = Vertex3DUV(glm::vec3(1., 1., 1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(1.,1.,1.))*(vec3(1., 1., -1.) - vec3(1.,1.,1.)))),glm::vec2(1.f, 1.f));
            vertices[2] = Vertex3DUV(glm::vec3(1., 1., -1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(1.,1.,-1.))*(vec3(1., 1., 1.) - vec3(1.,1.,-1.)))),glm::vec2(0.f, 1.f));
            vertices[3] = Vertex3DUV(glm::vec3(-1., 1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,-1.) - vec3(-1.,1.,1.))*(vec3(-1., 1., -1.) - vec3(-1.,1.,1.)))),glm::vec2(1.f, 0.f));
            vertices[4] = Vertex3DUV(glm::vec3(1., 1., -1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(1.,1.,-1.))*(vec3(-1.,1.,-1.) - vec3(1.,1.,-1.)))),glm::vec2(0.f, 1.f));
            vertices[5] = Vertex3DUV(glm::vec3(-1., 1., -1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(-1.,1.,-1.))*(vec3(1.,1.,-1.) - vec3(-1.,1.,-1.)))),glm::vec2(0.f, 0.f));
        }

        if(type == "DOWN"){
            vertices[0] = Vertex3DUV(glm::vec3(-1., -1., 1.),glm::vec3(glm::normalize((vec3(1.,-1.,1.) - vec3(-1.,-1.,1.))*(vec3(1., -1., -1.) - vec3(-1.,-1.,1.)))), glm::vec2(1.f, 0.f));
            vertices[1] = Vertex3DUV(glm::vec3(1., -1., 1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(1.,-1.,1.))*(vec3(1., -1., -1.) - vec3(1.,-1.,1.)))),glm::vec2(1.f, 1.f));
            vertices[2] = Vertex3DUV(glm::vec3(1., -1., -1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(1.,-1.,-1.))*(vec3(1., -1., 1.) - vec3(1.,-1.,-1.)))),glm::vec2(0.f, 1.f));
            vertices[3] = Vertex3DUV(glm::vec3(-1., -1., 1.),glm::vec3(glm::normalize((vec3(1.,-1.,-1.) - vec3(-1.,-1.,1.))*(vec3(-1., -1., -1.) - vec3(-1.,-1.,1.)))),glm::vec2(1.f, 0.f));
            vertices[4] = Vertex3DUV(glm::vec3(1., -1., -1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(1.,-1.,-1.))*(vec3(-1.,-1.,-1.) - vec3(1.,-1.,-1.)))),glm::vec2(0.f, 1.f));
            vertices[5] = Vertex3DUV(glm::vec3(-1., -1., -1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(-1.,-1.,-1.))*(vec3(1.,-1.,-1.) - vec3(-1.,-1.,-1.)))),glm::vec2(0.f, 0.f));
        }

        if(type == "LEFT"){
            vertices[0] = Vertex3DUV(glm::vec3(1., -1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,1.) - vec3(1.,-1.,1.))*(vec3(1., 1., -1.) - vec3(1.,-1.,1.)))), glm::vec2(1.f, 1.f));
            vertices[1] = Vertex3DUV(glm::vec3(1., 1., 1.),glm::vec3(glm::normalize((vec3(1.,-1.,1.) - vec3(1.,1.,1.))*(vec3(1., 1., -1.) - vec3(1., 1., 1.)))),glm::vec2(1.f, 0.f));
            vertices[2] = Vertex3DUV(glm::vec3(1., 1., -1.),glm::vec3(glm::normalize((vec3(1.,-1.,1.) - vec3(1.,1.,-1.))*(vec3(1., 1., 1.) - vec3(1.,1.,-1.)))),glm::vec2(0.f, 0.f));
            vertices[3] = Vertex3DUV(glm::vec3(1., -1., 1.),glm::vec3(glm::normalize((vec3(1.,1.,-1.) - vec3(1.,-1.,1.))*(vec3(1., -1., -1.) - vec3(1.,-1.,1.)))),glm::vec2(1.f, 1.f));
            vertices[4] = Vertex3DUV(glm::vec3(1., 1., -1.),glm::vec3(glm::normalize((vec3(1.,-1.,1.) - vec3(1.,1.,-1.))*(vec3(1.,-1.,-1.) - vec3(1.,1.,-1.)))),glm::vec2(0.f, 0.f));
            vertices[5] = Vertex3DUV(glm::vec3(1., -1., -1.),glm::vec3(glm::normalize((vec3(1.,-1.,1.) - vec3(1.,-1.,-1.))*(vec3(1.,1.,1.) - vec3(1.,-1.,-1.)))),glm::vec2(0.f, 1.f));
        }

        if(type == "RIGHT"){
            vertices[0] = Vertex3DUV(glm::vec3(-1., -1., 1.),glm::vec3(glm::normalize((vec3(-1.,1.,1.) - vec3(-1.,-1.,1.))*(vec3(-1., 1., -1.) - vec3(-1.,-1.,1.)))), glm::vec2(0.f, 1.f));
            vertices[1] = Vertex3DUV(glm::vec3(-1., 1., 1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(-1.,1.,1.))*(vec3(-1., 1., -1.) - vec3(-1., 1., 1.)))),glm::vec2(0.f, 0.f));
            vertices[2] = Vertex3DUV(glm::vec3(-1., 1., -1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(-1.,1.,-1.))*(vec3(-1., 1., 1.) - vec3(-1.,1.,-1.)))),glm::vec2(1.f, 0.f));
            vertices[3] = Vertex3DUV(glm::vec3(-1., -1., 1.),glm::vec3(glm::normalize((vec3(-1.,1.,-1.) - vec3(-1.,-1.,1.))*(vec3(-1., -1., -1.) - vec3(-1.,-1.,1.)))),glm::vec2(0.f, 1.f));
            vertices[4] = Vertex3DUV(glm::vec3(-1., 1., -1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(-1.,1.,-1.))*(vec3(-1.,-1.,-1.) - vec3(-1.,1.,-1.)))),glm::vec2(1.f, 0.f));
            vertices[5] = Vertex3DUV(glm::vec3(-1., -1., -1.),glm::vec3(glm::normalize((vec3(-1.,-1.,1.) - vec3(-1.,-1.,-1.))*(vec3(-1.,1.,1.) - vec3(-1.,-1.,-1.)))),glm::vec2(1.f, 1.f));
        }

        m_Vertices = vertices;
        //On envoie les données au buffer GL_ARRAY_BUFFER (que l'on a bindé précédement)
        glBufferData(GL_ARRAY_BUFFER,6 * sizeof(Vertex3DUV), vertices, GL_STATIC_DRAW);
        //Après avoir modifier le buffer on peut le débinder pour éviter de le modifier inutilement
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //Create vao
    glGenVertexArrays(1,&vao); //Affectation d'un identifiant
    //On va binder le VAO a sa cible (unique donc pas de précision)
    glBindVertexArray(vao);
        //On active l'attribut position & texture position
        const GLuint VERTEX_ATTR_POSITION = 0;
        const GLuint VERTEX_ATTR_NORMAL = 1;
        const GLuint VERTEX_ATTR_TEXTPOSITION = 2;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXTPOSITION);
        //Bind VBO on GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
            //Specify to VBO how to treat every VAO he found
            glVertexAttribPointer(VERTEX_ATTR_POSITION,3,GL_FLOAT,GL_FALSE, sizeof(Vertex3DUV),(const GLvoid*)offsetof(Vertex3DUV, position));
            glVertexAttribPointer(VERTEX_ATTR_NORMAL,3,GL_FLOAT,GL_FALSE, sizeof(Vertex3DUV),(const GLvoid*)offsetof(Vertex3DUV, normal));
            glVertexAttribPointer(VERTEX_ATTR_TEXTPOSITION,2,GL_FLOAT,GL_FALSE, sizeof(Vertex3DUV),(const GLvoid*)offsetof(Vertex3DUV, textpos));
        //On debind le VBO
        glBindBuffer(GL_ARRAY_BUFFER,0);
        //On debind le VAO
    glBindVertexArray(0);
}

Carre3D::~Carre3D(){
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}

void Carre3D::draw(GLuint id_texture,GLuint displayedTexture){
    glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D,displayedTexture);
            glUniform1i(id_texture,0);
            glDrawArrays(GL_TRIANGLES,0,6);
        glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
}
