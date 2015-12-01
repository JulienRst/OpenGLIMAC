#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cstdlib>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

using namespace glimac;
using namespace glm;
using namespace std;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(900, 900, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;


    //Creation du program et chargement des shaders

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                          applicationPath.dirPath() + "shaders/directionallight.fs.glsl");

    program.use();

    // Définition des variables uniformes à envoyer aux shaders
    GLint uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    GLint uKd = glGetUniformLocation(program.getGLId(), "uKd");
    GLint uKs = glGetUniformLocation(program.getGLId(), "uKs");
    GLint uShininess = glGetUniformLocation(program.getGLId(), "uShininess");
    GLint uLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
    GLint uLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
     glEnable(GL_DEPTH_TEST);

     // --------------------- INITIALISATION DES MATRICES

     mat4 ProjMatrix, MVMatrix, NormalMatrix, viewMatrix;

     // --------------------- CREATION D'UNE SPHERE

     Sphere sphere(1,32,32);

     // --------------------- VBO

     GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);

		//On rentre les valeurs dans le VBO

	glBufferData(GL_ARRAY_BUFFER,sizeof(ShapeVertex)*sphere.getVertexCount(), sphere.getDataPointer(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

    // --------------------- VAO

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXCOORDS = 2;

	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(VERTEX_ATTR_POSITION,3,GL_FLOAT,GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex,position));
	glVertexAttribPointer(VERTEX_ATTR_NORMAL,3,GL_FLOAT,GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex,normal));
	glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS,2,GL_FLOAT,GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex,texCoords));

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

    // --------------------- LIGHT

    vec4 lightPos = vec4(1.0f,0.0f,0.0f,0.f);

    // --------------------- ENVOIE DES VARIABLES UNIFORMES LIEES A LA LUMIERE

    //KD
    glUniform3f(uKd,1.0f,1.0f,1.0f);
    //KS
    glUniform3f(uKs,1.0f,1.0f,1.0f);
    //SHININESS
    glUniform1f(uShininess,0.5f);
    //LightIntensity
    glUniform3f(uLightIntensity,0.5f,0.5f,0.5f);

    // Application loop:
    bool done = false;

    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

            // --------------------- ENVOIE DES VARIABLES UNIFORMES
        ProjMatrix = perspective(radians(70.f),1.f,0.1f,100.f);
        MVMatrix = translate(mat4(1.f),vec3(0.0f,0.0f,-5.0f));
        NormalMatrix = transpose(inverse(MVMatrix));
        // ENVOIE DES VARIABLES UNIFORMES : LightDir et LightIntensity
        vec4 lightDir = lightPos;

        glUniform3f(uLightDir_vs,lightDir.x,lightDir.y,lightDir.z);

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, value_ptr(NormalMatrix));

            // --------------------- DESSINS
        glBindVertexArray(vao);

        //Dessin de la Terre
        glDrawArrays(GL_TRIANGLES,0,sphere.getVertexCount());

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
         glBindVertexArray(0);
         
        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
