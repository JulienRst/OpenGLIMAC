#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string>

#include <glimac/glm.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Sphere.hpp>

#include <GL/glew.h>

#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/shader.hpp"
#include "engine/freefly.hpp"
#include "engine/mouse.hpp"

// -------- NAMESPACE -------------- //

using namespace glm;
using namespace std;
using namespace glimac;

// TODO Ajouter ici l'initialisation de la caméra de Chamse

int main(int argc, char** argv){

    FilePath app = FilePath(argv[0]).dirPath();
    // -------- GLOBAL VARIABLE -------------- //

    GLuint screenWidth = 1920;
    GLuint screenHeight = 1080;


    //Initialisation de la fenêtre
    SDLWindowManager windowManager(screenWidth, screenHeight, "TEST ASSIMP");

    //TEST SUR GLEW
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    //Affichage des versions Software
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    // Initialisation de GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    // Initialisation des models

    map<int, unique_ptr<Model> > models = modelsFromFile(app + FilePath("../../../files/assets/models/models.txt"));

    // Initialisation de la Caméra freefly

    Camera camera = Camera();
    mat4 viewMatrix;

    // Initialisation de l'objet Mouse

    Mouse mouse;

    //Load & Compile Shader

    Shader shader("shaders/model_loading.vs","shaders/model_loading.frag");


    int loop = true;
    float xOffset, yOffset;

    while(loop){
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                loop = false; // Leave the loop after this iteration
            }
        }
        //glClear
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f),
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Lancement des shaders
        shader.Use();
        // ---------------------------- RECUPERATION DES EVENTS CLAVIER / UPDATE CAMERA
        xOffset = windowManager.getMousePosition().x - mouse.lastX;
        yOffset = windowManager.getMousePosition().y - mouse.lastY;

        mouse.lastX = windowManager.getMousePosition().x;
        mouse.lastY = windowManager.getMousePosition().y;

        camera.ProcessMouseMovement(xOffset,yOffset);
        camera.ProcessJump();

        if(windowManager.isKeyPressed(SDLK_z))
        camera.MoveFront(0.010);
        if(windowManager.isKeyPressed(SDLK_q))
        camera.MoveRight(-0.010);
        if(windowManager.isKeyPressed(SDLK_s))
        camera.MoveFront(-0.010);
        if(windowManager.isKeyPressed(SDLK_d))
        camera.MoveRight(0.010);
        // if(windowManager.isKeyPressed(SDLK_SPACE))
        // camera.MoveUp(0.010);
        // if(windowManager.isKeyPressed(SDLK_LSHIFT))
        // camera.MoveUp(-0.010);
        if(windowManager.isKeyPressed(SDLK_SPACE))
        camera.launchJump();
        if(windowManager.isKeyPressed(SDLK_LSHIFT)){
        camera.isShiftPressed = true;
        } else {
        camera.isShiftPressed = false;
        }

        // RECUPERATION DE LA SOURIS / UPDATE CAMERA

        // ---------------------------- FIN RECUPERATION EVENTS


        //ENVOIE DES MATRICES

        //Récupération de la viewMatrix de la Caméra

        viewMatrix = camera.GetViewMatrix();

        // Transformation matrices
        glm::mat4 projection = glm::perspective(70.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        drawModels(models, shader);

        //Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
