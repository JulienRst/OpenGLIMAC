// ***************
// *** READ ME ***
// ***************
// LETS GO LES AMIS


// ***1***
// Ici, tout est propre et beau ... mais très mal utilisé :(

// ***2***
// J'ai beaucoup travaillé avec cette version dès le début mais rien n'était concluant
// Je n'arrivais pas à compiler à cause des fonctions appelant la SDL_mixer qui ne veut
// pas fonctionner chez moi :/


// ***3***
// Regardez pas ça c'est pas regardable de toute manière !




// ***1***
#include "engine/music.hpp"



//Initialisation de l'audio dans le jeu
void InitAudio(){
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){ //Initialisation de l'API Mixer
		printf("%s", Mix_GetError());
	}
	Mix_AllocateChannels(10);
}

//Destruction de l'audio
void QuitAudio(){
	Mix_CloseAudio();
}

//Chargement du son (longue musique de fond)
Mix_Music* LoadMusic(char * filepath){
	return Mix_LoadMUS(filepath);
}

//Libération musique
void FreeMusic(Mix_Music* music){
	Mix_FreeMusic(music);
}

//Joue de la musique 
void PlayMusic(Mix_Music* music, int loops){
	Mix_PlayMusic(music, loops);
}

//Stop la musique
void StopMusic(){
	Mix_PauseMusic();	

	Mix_RewindMusic();
}

//Chargement du son( bruit court suivant action)
Mix_Chunk* LoadSound(char * filepath){
	return Mix_LoadWAV(filepath);
}

//Libération audio
void FreeSound(Mix_Chunk* sound){
	Mix_FreeChunk(sound);
}

//Joue de la musique
void PlaySound(Mix_Chunk* sound, int channel, int loop){
	Mix_PlayChannel(channel, sound, loop);
}

//Régler le volume
void AdjustChannelVolume(int channel, int volume){
	volume = (volume > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume;
	volume = (volume < 0) ? 0 : volume;
	Mix_Volume(channel, volume);
}




// ***2***
// PREMIER TEST


// #include <stdlib.h>
// #include <stdio.h>
// #include <../../SDL/SDL_mixer.h>
// #include <../../SDL/SDL.h>
// #include <GL/gl.h>
// #include <GL/glu.h>


// int main ( int argc, char* argv [])
// {
//     // On initialise la SDL
//     if(SDL_Init(SDL_INIT_VIDEO)==-1)
//     {
//         fprintf(stderr, "Impossible de charger la SDL, %s \n", SDL_GetError());
//         exit(EXIT_FAILURE);
//     }
// }



// //Déclaration des prototypes
// int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
// int Mix_PlayMusic(Mix_Music *music, int loops);


// //Fonction principale
// int main(int argc, char *argv[])

// {
// 	int continuer = 1;
// 	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
// 	SDL_Surface *ecran = NULL;
// 	ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
// 	SDL_Event event;
// 	SDL_WM_SetCaption("SDL_Mixer", NULL);
// 	SDL_Flip(ecran);
 	
//  	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){ //Initialisation de l'API Mixer
//       printf("%s", Mix_GetError());
//   	}

//   	Mix_Music *musique;
//   	musique = Mix_LoadMUS("bruit_menu.mp3");
//   	Mix_PlayMusic(musique, -1); // -1 pour infini

// 	while(continuer)   {
// 		SDL_WaitEvent(&event);
// 		switch(event.type)
// 	{
// 		case SDL_QUIT:
// 			continuer = 0;
// 			break;

// 		// case SDL_KEYDOWN:
//   //           switch (event.key.keysym.sym){
//   //               case SDLK_p: 
//   //                   if(Mix_PausedMusic() == 1){//Si la musique est en pause
//   //                 		Mix_ResumeMusic(); //Reprendre la musique
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

// 	}
// 	Mix_FreeMusic(musique);
// 	Mix_CloseAudio();
// 	SDL_Quit();
// 	return EXIT_SUCCESS;
// }







// ***3***
// **** DEUXIEME TEST ****


// #include <stdlib.h>
// #include <stdio.h>
// #include <../../..//home/lea/SDL-1.2.15/include/SDL.h>
// #include <../../..//home/lea/SDL-1.2.15/include/SDL_mixer.h>


// int main ( int argc, char* argv [])
// {
// 	SDL_Init(SDL_INIT_EVERYTHING);
// 	SDL_Surface* screen;
// 	screen = SDL_SetVideoMode(640,480,32,SDL_SWSURRFACE);
// 	bool running = true;
// 	const int FPS = 30;
// 	Uint32 start;
// 	SDL_Rect rect;
// 	rect.x=10;
// 	rect.y=10;
// 	rect.w=20;
// 	rect.h=20;

// 	Uint32 color2 = SDL_MapRGB(screen->format,0xff,0xff,0xff);
// 	while(running){
// 		start = SDL_GetTicks();
// 		SDL_Event event;
// 		while(SDL_PollEvent($event)){
			
// 		}
// 	}
//     // On initialise la SDL
//     if(SDL_Init(SDL_INIT_VIDEO)==-1)
//     {
//         fprintf(stderr, "Impossible de charger la SDL, %s \n", SDL_GetError());
//         exit(EXIT_FAILURE);
//     }
// }
