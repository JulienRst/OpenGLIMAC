#include "engine/carre2D.hpp"

using namespace glm;

mat3 scale(float sx, float sy){
	return mat3(vec3(sx,0.f,0.f),vec3(0.f,sy,0.f),vec3(0.f,0.f,1.f));
}

Carre2D::Carre2D(){
    //Create vbo
    glGenBuffers(1,&vbo);
        //Binding VBO on GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        //Putting all vertices
        Vertex2DUV vertices[] = {
            Vertex2DUV(glm::vec2(-1., 1.),glm::vec2(0.f, 0.f)),
            Vertex2DUV(glm::vec2(1., 1.),glm::vec2(1.f, 0.f)),
            Vertex2DUV(glm::vec2(1., -1.),glm::vec2(1., 1.f)),
            Vertex2DUV(glm::vec2(-1., 1.),glm::vec2(0.f, 0.f)),
            Vertex2DUV(glm::vec2(1., -1.),glm::vec2(1.f, 1.f)),
            Vertex2DUV(glm::vec2(-1., -1.),glm::vec2(0., 1.f))
        };
        m_Vertices = vertices;
        //On envoie les données au buffer GL_ARRAY_BUFFER (que l'on a bindé précédement)
        glBufferData(GL_ARRAY_BUFFER,6 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
        //Après avoir modifier le buffer on peut le débinder pour éviter de le modifier inutilement
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //Create vao
    glGenVertexArrays(1,&vao); //Affectation d'un identifiant
    //On va binder le VAO a sa cible (unique donc pas de précision)
    glBindVertexArray(vao);
        //On active l'attribut position & texture position
        const GLuint VERTEX_ATTR_POSITION = 0;
        const GLuint VERTEX_ATTR_TEXTPOSITION = 1;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXTPOSITION);
        //Bind VBO on GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
            //Specify to VBO how to treat every VAO he found
            glVertexAttribPointer(VERTEX_ATTR_POSITION,2,GL_FLOAT,GL_FALSE, sizeof(Vertex2DUV),(const GLvoid*)offsetof(Vertex2DUV, position));
            glVertexAttribPointer(VERTEX_ATTR_TEXTPOSITION,2,GL_FLOAT,GL_FALSE, sizeof(Vertex2DUV),(const GLvoid*)offsetof(Vertex2DUV, textpos));
        //On debind le VBO
        glBindBuffer(GL_ARRAY_BUFFER,0);
        //On debind le VAO
    glBindVertexArray(0);
}

Carre2D::~Carre2D(){
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}

void Carre2D::draw(GLuint id,GLuint id_texture,GLuint displayedTexture){
    glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D,displayedTexture);
            glUniform1i(id_texture,0);
            glUniformMatrix3fv(id,1,GL_FALSE,glm::value_ptr(glm::mat3()));
            glDrawArrays(GL_TRIANGLES,0,6);
        glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
}
