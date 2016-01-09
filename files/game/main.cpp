#include <cstdlib>
#include <iostream>

#include <string>
#include <vector>

#include <glimac/glm.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Sphere.hpp>

#include <GL/glew.h>

#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/shader.hpp"
#include "engine/freefly.hpp"
#include "engine/mouse.hpp"
// #include "engine/music.hpp"
#include "engine/music.hpp" //Normalement c'est lui qui accède correctement au fichier

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

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


    // **** SOUND **** 
    SDL_Init(SDL_INIT_AUDIO);
    InitAudio();
   // SDL_Surface *ecran = NULL;
    //ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    // SDL_WM_SetCaption("SDL_Mixer", NULL);
   //SDL_Flip(ecran);

    //Sons de base
    vector<Mix_Music*> musicList;
    //Musique du menu !!
    musicList.push_back(LoadMusic("../../assets/sounds/batman_saut.wav"));
    PlayMusic(musicList[0], -1); // -1 pour infini



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
        xOffset = windowManager.getMousePosition().x - mouse.lastX;// - 960;
        yOffset = windowManager.getMousePosition().y - mouse.lastY;// - 540;

        mouse.lastX = windowManager.getMousePosition().x;// - 960;
        mouse.lastY = windowManager.getMousePosition().y;// - 540;

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
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(10.0f,-7.0f,-20.0f));
        model = glm::scale(model,glm::vec3(2.0f,2.0f,2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        stormtrooper.Draw(shader);



        // Update the display
        windowManager.swapBuffers();
    }

    FreeMusic(musicList[0]);
    QuitAudio();
    SDL_Quit();
    return EXIT_SUCCESS;
}




// ***************
// *** READ ME ***
// ***************

// Bon en gros y'a 3 test différents et je ne m'y retrouve pas #super ...


// ***1***
// Le premier reprend les fonction de music.cpp mais je n'ai pas reusi à afficher
// une fenetre alors j'ai abandonné au début et depuis j'ai pas re-testé

// ***2***
// C'est celui que j'ai fai au début début sauf que je n'avais pas à l'origine un music.cpp et hpp
// mais seulement un main où tout était dedans ! Du coup je suis perdue ... oui je sais c'est #super

// ***3***
// J'ai pas encore regardé parce que là c'est le bordel :'/ ...










// ***1***
// #include "music.hpp"

// static const Uint32 FRAMERATE_MILLISECONDS = 3600 / 60;

// int main(int argc, char** argv) {
  
//      // Création fenetre
//  Fenetre* f = creerFenetre(800,600);
//  initialisationSDL();
//  setVideoMode(f->winWidth, f->winHeight);
//  SDL_WM_SetCaption("LOST TOWN", NULL);
//  pointSize(6);

//  //Chargement du jeu 
//  Jeu monJeu = initialiserJeu();
    
// // ****************************** SOUNDS LOADING ****************************** //

//  int k;  ------POUR SOUDS LOADING------
//  InitAudio();

//  // On initialise la SDL
//     if(SDL_Init(SDL_INIT_VIDEO)==-1)
//     {
//         fprintf(stderr, "Impossible de charger la SDL, %s \n", SDL_GetError());
//         exit(EXIT_FAILURE);
//     }

//  //Sons de base
//  niveau_4->soundList[0] = LoadSound("sounds/wall.wav");
//  niveau_4->soundList[1] = LoadSound("sounds/racket.wav");
//  niveau_4->soundList[2] = LoadSound("sounds/brick.wav");
//  niveau_4->soundList[3] = LoadSound("sounds/bonus.wav");
//  niveau_4->soundList[4] = LoadSound("sounds/lost.wav");
//  niveau_4->soundList[5] = LoadSound("sounds/win.wav");

//  //Son des girls
//  niveau_4->soundList[6] = LoadSound("sounds/girls/girl01.wav");
//  niveau_4->soundList[7] = LoadSound("sounds/girls/girl02.wav");
//  niveau_4->soundList[8] = LoadSound("sounds/girls/girl03.wav");
//  niveau_4->soundList[9] = LoadSound("sounds/girls/girl04.wav");

//  //Musique du menu !!
//  niveau_4->musicList[0] = LoadMusic("sounds/opening.mp3");

//  //PlayMusic(opening, -1);
//  for(k = 0; k < 10; k++){
//      niveau_4->soundEventTab[i] = VOID;
//  }

// // ****************************** DISPLAY GAME LOOP ****************************** //

//     while(monJeu.loop) {
//      /* temps au début de la boucle */
//      Uint32 startTime = SDL_GetTicks();
        
//      /* dessin */
//      glClear(GL_COLOR_BUFFER_BIT);

//      //Chargement du jeu ou du menu
//      if(monJeu.est_lance){
//          Jouer(&monJeu);
//      }
//      else {
//          Menu(&monJeu);
//      }

//      /* 25 img/s reactualisation*/
//      Uint32 elapsedTime = SDL_GetTicks() - startTime;
//      if(elapsedTime < FRAMERATE_MILLISECONDS) {
//          SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
//      }
//  }
      
//  freeJeu(&(monJeu));     
//     SDL_Quit();
    
//     return EXIT_SUCCESS;







// ***2***

// //Fonction principale
// int main(int argc, char *argv[])

// {
//     int continuer = 1;
//     SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
//     SDL_Surface *ecran = NULL;
//     ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
//     SDL_Event event;
//     SDL_WM_SetCaption("SDL_Mixer", NULL);
//     SDL_Flip(ecran);
    

//     //ON COMMENTE CETTE PARTIE DU CODE ...
//         // if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){ //Initialisation de l'API Mixer
//         //     printf("%s", Mix_GetError());
//         // }

//     //POUR LA REMPLACER PAR CELLE-LA
//     InitAudio();

// // On initialise la SDL
// if(SDL_Init(SDL_INIT_VIDEO)==-1){
//     fprintf(stderr, "Impossible de charger la SDL, %s \n", SDL_GetError());
//     exit(EXIT_FAILURE);
// }


//     Mix_Music *musique;
//     musique = Mix_LoadMUS("bruit_menu.mp3");
//     Mix_PlayMusic(musique, -1); // -1 pour infini

//     while(continuer)   {
//         SDL_WaitEvent(&event);
//         switch(event.type)
//     {
//         case SDL_QUIT:
//             continuer = 0;
//             break;

//         // case SDL_KEYDOWN:
//   //           switch (event.key.keysym.sym){
//   //               case SDLK_p: 
//   //                   if(Mix_PausedMusic() == 1){//Si la musique est en pause
//   //                        Mix_ResumeMusic(); //Reprendre la musique
//   //                   }
//   //                   else{
//   //                       Mix_PauseMusic(); //Mettre en pause la musique
//   //                   }
//   //               break;
//   //               case SDLK_BACKSPACE:
//   //                   Mix_RewindMusic(); //Revient au début de la musique
//   //                   break;
//   //               case SDLK_ESCAPE:
//   //                   Mix_HaltMusic(); //Arrête la musique
//   //                   break;
//   //           }
//   //       break;
//       }

//     }
//     Mix_FreeMusic(musique);
//     Mix_CloseAudio();
//     SDL_Quit();
//     return EXIT_SUCCESS;
// }







// ***3***
// CODE https://sites.google.com/site/sdlgamer/beginner/lesson-12



// #include "SDL.h"
// #include "SDL_image.h"
// #include "SDL_mixer.h"

// int FPS = 50;    // Assign a FPS
// int NextTick , interval ;

// // Initialize FPS_Fn( )
// void FPS_Initial(void) {
// NextTick = 0 ;
// interval = 1 * 1000 / FPS ;
// return;
// }

// // Frame Per Second Function  , put this in a loop
// void FPS_Fn(void) {
// if ( NextTick > SDL_GetTicks( ) ) SDL_Delay( NextTick - SDL_GetTicks( ) );
// NextTick = SDL_GetTicks( ) + interval ;
// return;
// }

// // This function load a image file to a surface
// // Set bCKey with colorkey (R,G,B) to clear a color on the image
// // Set alpha value for transparency 0(No transparent) ~ 255(Ivisible)
// inline SDL_Surface *ImgLoader(char *file,bool bCKey, int r , int g , int b , int alpha)
// {
//     SDL_Surface *pic;
//     pic = IMG_Load(file); // From SDL_image.h , load the image to pic
//     // Log error message into stderr.txt if happened
//     if(pic==NULL) fprintf(stderr,"Missing image %s : %s\n",file,IMG_GetError());
//     if( bCKey ) {
//         // Sets the color key (transparent pixel) in a blittable surface and RLE acceleration.
//         SDL_SetColorKey(pic,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(pic->format,r,g,b));
//          }
//     if(alpha) SDL_SetAlpha(pic, SDL_SRCALPHA|SDL_RLEACCEL , 255 - alpha);
//     pic = SDL_DisplayFormat(pic);
//     return (pic);
// }

// // Load a normal picture into a surface
// inline SDL_Surface *ImgLoader(char *file)       
//                 { return ImgLoader(file,1,0,0,0,0) ; }
// // Load a pic & set the transparent color to (255,255,255) , no alpha
// inline SDL_Surface *ImgLoader(char *file,bool bCKey)
//                 { return ImgLoader(file,1,255,255,255,0) ; }


// int main(int argc, char *argv[])
// {

// SDL_Event event;
// bool bRun = 1;
// int pidx = 0 , aTick = 0;
// SDL_Surface *screen ;
// enum walk { justice1 , justice2 , justice3 , justice4
//     , justice5 , justice6 , justice_No  };
// SDL_Surface *anim[justice_No];
// SDL_Rect animRect ;
// animRect.x = 160 ;
// animRect.y = 160 ;

// atexit(SDL_Quit);

// if( SDL_Init(SDL_INIT_VIDEO) < 0 ) exit(1);

// SDL_WM_SetCaption("SDL Window", NULL);

// screen = SDL_SetVideoMode( 400 , 300 , 32 , SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

// anim[0] = ImgLoader("./anim/justice1.gif",1,255,255,255,0);
// anim[1] = ImgLoader("./anim/justice2.gif",1,255,255,255,0);
// anim[2] = ImgLoader("./anim/justice3.gif",1,255,255,255,0);
// anim[3] = ImgLoader("./anim/justice4.gif",1);
// anim[4] = ImgLoader("./anim/justice5.gif",1);
// anim[5] = ImgLoader("./anim/justice6.gif",1);

// FPS_Initial( );

// // Inilialize SDL_mixer , exit if fail
// if( SDL_Init(SDL_INIT_AUDIO) < 0 ) exit(1);
// // Setup audio mode
// Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
// Mix_Music *mus , *mus2 ;  // Background Music
// Mix_Chunk *wav , *wav2 ;  // For Sounds
// //mus2 = Mix_LoadMUS("./mixer/ff2prlde.mid");
// mus = Mix_LoadMUS("bruit_menu.mp3");
// wav = Mix_LoadWAV("batman_saut.wav");
// //wav2 = Mix_LoadWAV("./mixer/start.wav");
// Mix_PlayMusic(mus,1); //Music loop=1

// while(bRun) {
//     aTick++;
//     SDL_FillRect(screen , NULL , 0x221122);
//     SDL_BlitSurface( anim[pidx] , NULL , screen , &animRect );
//     SDL_Flip(screen);
//     FPS_Fn();
//     if( aTick%10 == 0 ) pidx++;
//     if(pidx >= justice_No) pidx = 0;
//     while( SDL_PollEvent( &event ) ){
//         switch( event.type ){
//             case SDL_KEYDOWN:
//                 //use -1 for the first free unreserved channel , 0-> no loop
//                 Mix_PlayChannel(-1,wav,0);
//                 switch( event.key.keysym.sym ){
//                     case SDLK_UP:
//                         animRect.y = animRect.y - 10;
//                         break;
//                     case SDLK_DOWN:
//                         animRect.y = animRect.y + 10;
//                         break;
//                     case SDLK_LEFT:
//                         animRect.x = animRect.x - 10;
//                         break;
//                     case SDLK_RIGHT:
//                         animRect.x = animRect.x + 10;
//                         break;
//                     case SDLK_1:
//                         Mix_PlayMusic(mus,1); //Music loop=1
//                         break;
//                     case SDLK_2:
//                         Mix_PlayMusic(mus2,2); //Music loop=twice
//                         break;
//                     case SDLK_ESCAPE:
//                         bRun = 0 ;
//                         break;
//                     default:
//                         //use -1 for the first free unreserved channel , 0-> no loop
//                         Mix_PlayChannel(-1,wav2,0);
//                         break;
//                     } // switch( event.key.keysym.sym ){ END
//                 break; // case SDL_KEYDOWN: END
//             case SDL_QUIT:
//                 bRun = 0;
//                 break;
//             default:
//                 break;
//             } // switch( event.type ){ END
//         } // while( SDLK_PollEvent( &event ) ){ END
//     }; // while(bRun) { END

// Mix_FreeChunk(wav);
// Mix_FreeChunk(wav2);
// Mix_FreeMusic(mus);
// Mix_FreeMusic(mus2);
// return 0;
// }
