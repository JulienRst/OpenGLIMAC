// --- Include STD & IOSTREAM --- //
#include <cstdlib>
#include <iostream>
// --- Include Needed From GLImac --- //
#include <glimac/SDLWindowManager.hpp>
#include <glimac/FilePath.hpp>
// --- Include Glew --- //
#include <GL/glew.h>
// --- Include Needed From Engine --- //
#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/shader.hpp"
#include "engine/freefly.hpp"
#include "engine/mouse.hpp"
#include "engine/music.hpp"

// -------- NAMESPACE -------------- //

using namespace glm;
using namespace std;
using namespace glimac;

// -------- MAIN PROGRAM -------------- //


int main(int argc, char** argv){

        // -------------------------------------------- //
        // -------------- GLOBAL VARIABLE ------------- //
        // -------------------------------------------- //

    //Windth and Height of the app
    GLuint screenWidth = 1920;
    GLuint screenHeight = 1080;
    //Define the path of the main app
    FilePath app = FilePath(argv[0]).dirPath() + "../";
    //Initialize Window
    SDLWindowManager windowManager(screenWidth, screenHeight, "Lost Town");
    //Check if Glew is ok
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    // Initialize Glew
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    // Initialize Models
    map<int, unique_ptr<Model> > models = modelsFromFile(app + FilePath("assets/models/models.txt"));

    // Initialize Camera FreeFly & it's viewMatrix
    Camera camera = Camera();
    mat4 viewMatrix;

    // Initialize Mouse & Offset
    Mouse mouse;
    float xOffset, yOffset;

    //Load & Compile Shader
    Shader shader_models(app + "shaders/model_loading.vs",app + "shaders/model_loading.frag");
    Shader shader_skybox(app + "shaders/texture.frag",app + "shaders/texture.vs");

        // -------------------------------------------- //
        // -------------- SOUND VARIABLE -------------- //
        // -------------------------------------------- //

    //Initialize SDL_Mixer and SDL_Audio
    SDL_Init(SDL_INIT_AUDIO);
    InitAudio();
    AdjustChannelVolume(-1, MIX_MAX_VOLUME/5);

    //Vector to put sounds (music and chunk)
    vector<Mix_Music*> musicList;
    vector<Mix_Chunk*> chunkList;

        // ------- GENERAL MUSIQUE ----- //

    //Music for the Menu
    musicList.push_back(LoadMusic((app + "assets/sounds/bruit_menu.mp3").c_str()));
    PlayMusic(musicList[0], -1); // -1 to load at infinity

    /* --- TODO : use :
        if(windowManager.isKeyPressed(SDLK_#)){
            StopMusic(); //Met la musique en pause
        }
        if(windowManager.isKeyPressed(SDLK_#)){
            ResumeMusic(); //Relance la musique de là où elle s'est arrêté
        }
        if(windowManager.isKeyPressed(SDLK_#)){
            Mix_RewindMusic(); //Revient au début de la musique
        }
        if(windowManager.isKeyPressed(SDLK_#)){
            Mix_HaltMusic(); //Arrête la musique
        }
    --- */

        // ------- CONTEXTUAL NOISE ----- //

    //Foot Steps
    chunkList.push_back(LoadSound((app + "assets/sounds/footstep_1pas.ogg").c_str()));
    Uint32 lastFootStep = 0;
    Uint32 limitBetweenFootStep = 600; // ms min between two foot step sound

        // -------------------------------------------- //
        // ----------- LOOP OF THE PROGRAM ------------ //
        // -------------------------------------------- //

    int loop = true;
    while(loop){

        // ---------------------------- CHECK IF SDL QUIT
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                loop = false; // Leave the loop after this iteration
            }
        }

        // ---------------------------- GL CLEAR
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f),
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---------------------------- LAUNCH OF SHADDER
        shader_models.Use();

        // ---------------------------- GET MOUSE
        xOffset = windowManager.getMousePosition().x - mouse.lastX;
        yOffset = windowManager.getMousePosition().y - mouse.lastY;
        mouse.lastX = windowManager.getMousePosition().x;
        mouse.lastY = windowManager.getMousePosition().y;

        // ---------------------------- UPDATE CAMERA
        camera.ProcessMouseMovement(xOffset,yOffset);

        // ---------------------------- PROCESS JUMP
        camera.ProcessJump();

            // ---------------------------------------------
            // ---------------------------- PROCESS KEYBOARD
            // ---------------------------------------------
        // ---------------------------- Initalize movement to FALSE
        bool isMovement = false;
        // ---------------------------- Z : Forward
        if(windowManager.isKeyPressed(SDLK_z)){
            camera.MoveFront(0.010);
            isMovement = true;
        }
        // ---------------------------- Q : Left
        if(windowManager.isKeyPressed(SDLK_q)){
            camera.MoveRight(-0.010);
            isMovement = true;
        }
        // ---------------------------- S : Backward
        if(windowManager.isKeyPressed(SDLK_s)){
            camera.MoveFront(-0.010);
            isMovement = true;
        }
        // ---------------------------- D : Right
        if(windowManager.isKeyPressed(SDLK_d)){
            camera.MoveRight(0.010);
            isMovement = true;
        }
        // ---------------------------- SPACE : Jump
        if(windowManager.isKeyPressed(SDLK_SPACE))
            camera.launchJump();

        if(isMovement && SDL_GetTicks() - lastFootStep > limitBetweenFootStep){
            PlaySound(chunkList[0]);
            lastFootStep = SDL_GetTicks();
        }
        // ---------------------------- SHIFT : Run
        if(windowManager.isKeyPressed(SDLK_LSHIFT)){
            limitBetweenFootStep = 400;
            camera.isShiftPressed = true;
        } else {
            limitBetweenFootStep = 600;
            camera.isShiftPressed = false;
        }
        // ----------------------------- ENTER : Go to next level : TODO : REMOVE IT !!
        if(windowManager.isKeyPressed(SDLK_RETURN)){
            models = modelsFromFile(app + FilePath("assets/models/models2.txt"));
        }


            // ---------------------------------------------
            // --------------------------------- SEND MATRIX
            // ---------------------------------------------

        // ---------------------------- GET THE VIEW MATRIX FROM CAMERA
        viewMatrix = camera.GetViewMatrix();
        // ---------------------------- TRANSFORM THE MATRIX AND SEND THEMP
        glm::mat4 projection = glm::perspective(70.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader_models.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader_models.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        // ---------------------------- CALLING THE DRAW METHOD OF ALL THE MODELS
        drawModels(models, shader_models);
        // ---------------------------- SWAP THE BUFFERS
        windowManager.swapBuffers();
    }

    // --------------------------------------------- //
    // ---------- FREE AND LEAVE PROPERLY ---------- //
    // --------------------------------------------- //

    FreeSound(chunkList[0]);
    FreeMusic(musicList[0]);
    QuitAudio();
    SDL_Quit();
    return EXIT_SUCCESS;
}
