// --- Include STD & IOSTREAM --- //
#include <cstdlib>
#include <iostream>
// --- Include Needed From GLImac --- //
#include <glimac/SDLWindowManager.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
// --- Include Glew --- //
#include <GL/glew.h>
// --- Include Needed From Engine --- //
#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/shader.hpp"
#include "engine/freefly.hpp"
#include "engine/mouse.hpp"
#include "engine/music.hpp"
#include "engine/texture.hpp"
#include "engine/menu.hpp"
// --- Include of STD Extends --- //
#include <map>


// -------- NAMESPACE -------------- //

using namespace glm;
using namespace std;
using namespace glimac;


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

mat3 scale(float sx, float sy){
	return mat3(vec3(sx,0.f,0.f),vec3(0.f,sy,0.f),vec3(0.f,0.f,1.f));
}

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

    Camera camera = Camera();
    mat4 viewMatrix;

    // Initialize Mouse & Offset
    Mouse mouse;
    float xOffset, yOffset;

    //Load & Compile Shader
    Shader shader_models(app + "shaders/model_loading.vs", app + "shaders/model_loading.frag");

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
    musicList.push_back(LoadMusic((app + "assets/sounds/genesis.mp3").c_str()));
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
        // ------------------ MENU -------------------- //
        // -------------------------------------------- //

    // ----------------------------------------------------------
    // ---------------------------------------- CREATING A SQUARE
    // ----------------------------------------------------------

    //Creating new shader
    Shader shader_menu(app + "shaders/text2D.vs.glsl", app + "shaders/text2D.fs.glsl");
    shader_menu.Use();
    //Get Uniform Variable
    GLint id = glGetUniformLocation(shader_menu.Program,"uModelMatrix");
	GLint id_texture = glGetUniformLocation(shader_menu.Program,"uTexture");

        //--------- VBO ----------//
    //Create a VBO
    GLuint vbo;
    glGenBuffers(1,&vbo);
    //Binding VBO on GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //On crée un tableau de GLfloat contenant les coordonnées
    Vertex2DUV vertices[] =  {
        Vertex2DUV(glm::vec2(-1., 1.),glm::vec2(0.f, 0.f)),
        Vertex2DUV(glm::vec2(1., 1.),glm::vec2(1.f, 0.f)),
        Vertex2DUV(glm::vec2(1., -1.),glm::vec2(1., 1.f)),
        Vertex2DUV(glm::vec2(-1., 1.),glm::vec2(0.f, 0.f)),
        Vertex2DUV(glm::vec2(1., -1.),glm::vec2(1.f, 1.f)),
        Vertex2DUV(glm::vec2(-1., -1.),glm::vec2(0., 1.f))
    };
    //On envoie les données au buffer GL_ARRAY_BUFFER (que l'on a bindé précédement)
    glBufferData(GL_ARRAY_BUFFER,6 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
    //Après avoir modifier le buffer on peut le débinder pour éviter de le modifier inutilement
    glBindBuffer(GL_ARRAY_BUFFER,0);

        //--------- VAO ----------//
    //Création d'un VAO
    GLuint vao; //Déclaration
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

    // ----------------------------------------------------------
    // ------------------------------------ CREATING THE TEXTURES
    // ----------------------------------------------------------

    map<string,unique_ptr<HTexture>> map_textures;
    map_textures["main_menu"].reset(new HTexture(app + "assets/textures/menu/main_menu.png"));
    map_textures["play_hover"].reset(new HTexture(app + "assets/textures/menu/main_menu_play.png"));
    map_textures["sound_hover"].reset(new HTexture(app + "assets/textures/menu/main_menu_sound.png"));
    map_textures["credit_hover"].reset(new HTexture(app + "assets/textures/menu/main_menu_credit.png"));
    map_textures["exit_hover"].reset(new HTexture(app + "assets/textures/menu/main_menu_exit.png"));
    map_textures["main_sound_menu"].reset(new HTexture(app + "assets/textures/menu/sound_gabarit.png"));
    map_textures["back_sound_menu"].reset(new HTexture(app + "assets/textures/menu/sound_menu_back.png"));
    map_textures["on_sound_menu"].reset(new HTexture(app + "assets/textures/menu/sound_menu_on.png"));
    map_textures["off_sound_menu"].reset(new HTexture(app + "assets/textures/menu/sound_menu_off.png"));
    map_textures["main_credit"].reset(new HTexture(app + "assets/textures/menu/credits_menu.png"));
    map_textures["back_credit"].reset(new HTexture(app + "assets/textures/menu/credits_menu_back.png"));
    map_textures["play_menu_back"].reset(new HTexture(app + "assets/textures/menu/play_menu_back.png"));
    map_textures["play_menu_go"].reset(new HTexture(app + "assets/textures/menu/play_menu_go.png"));
    map_textures["play_menu_help"].reset(new HTexture(app + "assets/textures/menu/play_menu_help.png"));
    map_textures["main_menu_play"].reset(new HTexture(app + "assets/textures/menu/menuplay_gabarit.png"));
    map_textures["main_help"].reset(new HTexture(app + "assets/textures/menu/help_menu.png"));
    map_textures["back_help"].reset(new HTexture(app + "assets/textures/menu/help_menu_back.png"));

        // -------------------------------------------- //
        // ------------- LOOP OF THE MENU ------------- //
        // -------------------------------------------- //

    string page = "home";
    string action = "";
    bool loop_game = false;
    bool loop_menu = true;
    GLuint displayedTexture;

    while(loop_menu){
        // ---------------------------- CHECK IF SDL QUIT
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                loop_menu = false;
                loop_game = false;
            }
        }
        // ---------------------------- PROCESS MENU::PLAY
        if(windowManager.isKeyPressed(SDLK_RETURN)){
            loop_menu = false;
            loop_game = true;
        }

        // ---------------------------- PROCESS MOUSE
        mouse.lastX = windowManager.getMousePosition().x;
        mouse.lastY = windowManager.getMousePosition().y;
        // ---------------------------- PROCESS MENU ACTION

        if(windowManager.isMouseButtonPressed(SDL_BUTTON_LEFT)){
            action = processMenuAction(page,mouse);
        }

            //What to do with the flag
        if(action == "quit"){
            loop_menu = false;
            loop_game = false;
        } else if(action == "play"){
            loop_menu = false;
            loop_game = true;
        } else if(action == "pauseSound"){
            StopMusic();
            Mix_RewindMusic();
        } else if(action == "playSound"){
            ResumeMusic();
        }

        action = "";

        if(mouse.hasJustClick && !windowManager.isMouseButtonPressed(SDL_BUTTON_LEFT))
            mouse.hasJustClick = false;

        displayedTexture = textureToDisplay(page,map_textures,mouse);


        // ---------------------------- GL CLEAR
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f),
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------- LAUNCH OF SHADDER
        shader_menu.Use();

            // ---------------------------------------------
            // -------------------------- DRAW + SEND MATRIX
            // ---------------------------------------------

        glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D,displayedTexture);
                glUniform1i(id_texture,0);
                glUniformMatrix3fv(id,1,GL_FALSE,glm::value_ptr(glm::mat3()));
                glDrawArrays(GL_TRIANGLES,0,6);
            glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(0);

        // ---------------------------- SWAP THE BUFFERS
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);

    // Change the main music for the game

    PlayMusic(musicList[1], -1); // -1 to load at infinity
    // Initialize Models
    map<int, unique_ptr<Model> > models = modelsFromFile(app + FilePath("assets/models/models.txt"));

        // -------------------------------------------- //
        // ------------- LOOP OF THE GAME ------------- //
        // -------------------------------------------- //

    while(loop_game){
        // ---------------------------- CHECK IF SDL QUIT
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                loop_game = false; // Leave the loop after this iteration
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
        if(windowManager.isKeyPressed(SDLK_SPACE)) {
            camera.launchJump();
        }

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
        // ---------------------------- CALLING THE DRAW METHOD OF ALL THE MODELS(
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

    std::cout << "EXIT_SUCCESS" << std::endl;

    return EXIT_SUCCESS;
}
