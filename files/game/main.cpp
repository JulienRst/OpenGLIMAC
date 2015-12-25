#include <cstdlib>
#include <iostream>

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

    // Initialisation de la Caméra freefly

    Camera camera = Camera();
    mat4 viewMatrix;

    // Initialisation de l'objet Mouse

    Mouse mouse;

    //Load & Compile Shader
    Shader shader("shaders/model_loading.vs","shaders/model_loading.frag");

    //Load Modele
    Model nanosuit("../../assets/models/nanosuit/nanosuit.obj");
    Model stormtrooper("../../assets/models/stormtrooper/Stormtrooper.obj");

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
        xOffset = windowManager.getMousePosition().x - mouse.lastX - 960;
        yOffset = windowManager.getMousePosition().y - mouse.lastY - 540;

        mouse.lastX = windowManager.getMousePosition().x - 960;
        mouse.lastY = windowManager.getMousePosition().y - 540;

        camera.ProcessMouseMovement(xOffset,yOffset);


        if(windowManager.isKeyPressed(SDLK_z))
            camera.MoveFront(0.010);
        if(windowManager.isKeyPressed(SDLK_q))
            camera.MoveRight(-0.010);
        if(windowManager.isKeyPressed(SDLK_s))
            camera.MoveFront(-0.010);
        if(windowManager.isKeyPressed(SDLK_d))
            camera.MoveRight(0.010);
        if(windowManager.isKeyPressed(SDLK_SPACE))
            camera.MoveUp(0.010);
        if(windowManager.isKeyPressed(SDLK_LSHIFT))
            camera.MoveUp(-0.010);

        // RECUPERATION DE LA SOURIS / UPDATE CAMERA

        // ---------------------------- FIN RECUPERATION EVENTS


        //ENVOIE DES MATRICES

        //Récupération de la viewMatrix de la Caméra

        viewMatrix = camera.GetViewMatrix();

        // Transformation matrices
        glm::mat4 projection = glm::perspective(70.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // Draw the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -7.0f, -20.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        nanosuit.Draw(shader);

        model = glm::translate(model, glm::vec3(10.0f,0.0f,0.0f));
        model = glm::scale(model,glm::vec3(4.0f,4.0f,4.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        stormtrooper.Draw(shader);



        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
