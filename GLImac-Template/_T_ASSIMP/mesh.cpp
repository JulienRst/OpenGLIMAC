#include "mesh.hpp"


using namespace std;

Mesh::Mesh(vector<Vertex> vertices,vector<GLuint> indices, vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->setupMesh();
}

void Mesh::setupMesh(){
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1,&this->VBO);
    glGenBuffers(1,&this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                &this->indices[0], GL_STATIC_DRAW);

    /// VERTEX POSITION ///
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)0);
    /// VERTEX NORMALS ///
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)offsetof(Vertex, Normal));
    /// VERTEX TEXTURE COORDS ///
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    // On débind le VAO

    glBindVertexArray(0);
}

void Mesh::Draw(Shader shader){
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); ++i){
        glActiveTexture(GL_TEXTURE0 + i); // Active la bonne texture avant le bind
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++;
        if(name == "texture_specular")
            ss << specularNr++;
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader.Program,
            ("material." + name  + number).c_str()), i);
    }
    glActiveTexture(GL_TEXTURE0);

    //Draw Mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}