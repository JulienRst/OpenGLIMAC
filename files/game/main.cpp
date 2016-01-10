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


// -------- NAMESPACE -------------- //

using namespace glm;
using namespace std;
using namespace glimac;


// -------- VARIABLES --------------//

struct Vertex3DUV{
    glm::vec3 position;
    glm::vec2 textpos;

    Vertex3DUV(){
        position = glm::vec3(0,0,0);
        textpos = glm::vec2(0,0);
    }

    Vertex3DUV(glm::vec3 p, glm::vec2 t){
        position = p;
        textpos = t;
    }
};

mat3 translate(float tx, float ty, float tz){
    return mat3(vec3(1.f,0.f,0.f),vec3(0.f,1.f,0.f),vec3(tx,ty,tz));
}

mat3 scale(float sx, float sy, float sz){
    return mat3(vec3(sx,0.f,0.f),vec3(0.f,sy,0.f),vec3(0.f,0.f,sz));
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
    Shader shader_menu(app + "shaders/text2D.vs.glsl", app + "shaders/text2D.fs.glsl");

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

        
    //--------- VBO ----------//

    //Création d'un seul VBO
    GLuint vbo;
    glGenBuffers(1,&vbo);

    //Binding d'un VBO sur la cible GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //On peut à présent modfiier le VBO en passant par la cible GL_ARRAY_BUFFER

    //On crée un tableau de GLfloat contenant les coordonnées
    Vertex3DUV vertices[] =  {
        Vertex3DUV(glm::vec3(-1., 1., -2.),glm::vec2(0.f, 0.f)),
        Vertex3DUV(glm::vec3(1., 1., -2.),glm::vec2(1.f, 0.f)),
        Vertex3DUV(glm::vec3(1., -1., -2.),glm::vec2(1., 1.f)),
        Vertex3DUV(glm::vec3(-1., 1., -2.),glm::vec2(0.f, 0.f)),
        Vertex3DUV(glm::vec3(1., -1., -2.),glm::vec2(1.f, 1.f)),
        Vertex3DUV(glm::vec3(-1., -1., -2.),glm::vec2(0., 1.f))
    };

     //On envoie les données au buffer GL_ARRAY_BUFFER (que l'on a bindé précédement)
    glBufferData(GL_ARRAY_BUFFER,6 * sizeof(Vertex3DUV), vertices, GL_STATIC_DRAW);
    //Après avoir modifier le buffer on peut le débinder pour éviter de le modifier inutilement 
    glBindBuffer(GL_ARRAY_BUFFER,0);


    //--------- VAO ----------//
    
    //Création d'un VAO
    GLuint vao; //Déclaration
    glGenVertexArrays(1,&vao); //Affectation d'un identifiant
    
    //On va binder le VAO a sa cible (unique donc pas de précision)
    glBindVertexArray(vao);
    
    //On active l'attribut position
    //Code plus clair, on passe par une variable d'état
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_TEXTPOSITION = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTPOSITION);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //On spécifie le format de l'attribut de somemt position
    glVertexAttribPointer(VERTEX_ATTR_POSITION,2,GL_FLOAT,GL_FALSE, sizeof(Vertex3DUV),(const GLvoid*)offsetof(Vertex3DUV, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXTPOSITION,2,GL_FLOAT,GL_FALSE, sizeof(Vertex3DUV),(const GLvoid*)offsetof(Vertex3DUV, textpos));
    //On debind le VBO
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //On debind le VAO
    glBindVertexArray(0);

    std::unique_ptr<Image> main_menu = loadImage("/home/julien/OpenGLIMAC/files/assets/textures/menu/menu_gabarit.png");
        
    if(main_menu != NULL){
        std::cout << "Chargement de la main_menu OK" << std::endl;
    } else {
        std::cout << "Echec du chargement de la main_menu" << std::endl;
    }

    GLuint texturesMenu; 
    glGenTextures(1,&texturesMenu);
    glBindTexture(GL_TEXTURE_2D,texturesMenu);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,main_menu->getWidth(),main_menu->getHeight(),0,GL_RGBA,GL_FLOAT,main_menu->getPixels());
    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D,0);



        //Positionner bouton hover

        //Ecouteur souris sur zone bouton (--> hover)
        //Ecouteur click souris (--> navigation)

        //Gérer la page à afficher du menu

        //SOUNDS
        //EXIT
        //CREDIT
        //PLAY
        //HELP
        //GO ===> RUN GAME

    int loop_game = false;
    int loop_menu = true;
        // -------------------------------------------- //
        // ----------- LOOP OF THE PROGRAM ------------ //
        // -------------------------------------------- //

         while(loop_menu == true){
                std::cout << "Dans la boucle loop_menu" << std::endl;
                // ---------------------------- CHECK IF SDL QUIT
                SDL_Event e;
                while(windowManager.pollEvent(e)) {
                    if(e.type == SDL_QUIT) {
                        loop_menu = false; 
                        loop_game = false; // Leave the loop after this iteration
                        std::cout << "SDL_Quit : loop_menu = " << loop_menu << std::endl;
                        std::cout << "SDL_Quit : loop_game = " << loop_game << std::endl;
                    }
                }

                // ---------------------------- GL CLEAR
                glClearColor(0.05f, 0.05f, 0.05f, 1.0f),
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // ---------------------------- LAUNCH OF SHADDER
                shader_menu.Use();

                // ---------------------------- GET MOUSE
                xOffset = windowManager.getMousePosition().x - mouse.lastX;
                yOffset = windowManager.getMousePosition().y - mouse.lastY;
                mouse.lastX = windowManager.getMousePosition().x;
                mouse.lastY = windowManager.getMousePosition().y;

                    // ---------------------------------------------
                    // ---------------------------- PROCESS KEYBOARD
                    // ---------------------------------------------

                // ----------------------------- ENTER : Go to GAME : TODO : REMOVE IT !!
                if(windowManager.isKeyPressed(SDLK_RETURN)){
                    std::cout << "enter is key_pressed : loop_menu = " << loop_menu << std::endl;
                    loop_menu = false;
                    loop_game = true;
                    std::cout << "leave is key_pressed : loop_menu = " << loop_menu << std::endl;
                    std::cout << "leave is key_pressed : loop_game = " << loop_game << std::endl;

                }

                    // ---------------------------------------------
                    // --------------------------------- SEND MATRIX
                    // ---------------------------------------------

                // ---------------------------- GET THE VIEW MATRIX FROM CAMERA
                viewMatrix = glm::mat4();
                // ---------------------------- TRANSFORM THE MATRIX AND SEND THEMP
                glm::mat4 projection = glm::perspective(70.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader_models.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(shader_models.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
                GLint id = glGetUniformLocation(shader_menu.getGLId(),"uModelMatrix");
                GLint id_color = glGetUniformLocation(shader_menu.getGLId(),"uColor");
                GLint id_texture = glGetUniformLocation(shader_menu.getGLId(),"uTexture");

                // ---------------------------- On veut faire le menu 
                // On rebind notre VAO
                glBindVertexArray(vao);
                glBindTexture(GL_TEXTURE_2D,texturesMenu);
                glUniform1i(id_texture,0);
                    // On dessine en précisant ce que l'on dessine, puis où l'on commence, puis combien de valeurs on utilise
                    glDrawArrays(GL_TRIANGLES,0,3);
                    glDrawArrays(GL_TRIANGLES,0,3);
                // On debind notre VAO
                glBindTexture(GL_TEXTURE_2D,0);
                glBindVertexArray(0);

                // ---------------------------- SWAP THE BUFFERS
                windowManager.swapBuffers();
                std::cout << "swapping Buffers" << std::endl;
        }

        PlayMusic(musicList[1], -1); // -1 to load at infinity
        // Initialize Models
        map<int, unique_ptr<Model> > models = modelsFromFile(app + FilePath("assets/models/models.txt"));


        while(loop_game == true){
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

    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
    glDeleteTextures(1,&texturesMenu);

    FreeSound(chunkList[0]);
    FreeMusic(musicList[0]);
    QuitAudio();
    SDL_Quit();
    std::cout << "EXIT_SUCCESS" << std::endl;

    return EXIT_SUCCESS;
}
