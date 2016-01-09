#include "engine/music.hpp"
#include "SDL.h"
#include "SDL_mixer.h"

using namespace std;

//Audio Initialisation
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


// *************************************
// Music is a noise for an environnement
// *************************************

Mix_Music* LoadMusic(char const* filepath){
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
	// if(Mix_PausedMusic() ){//Si la musique est en pause
 //        Mix_ResumeMusic(); //Reprendre la musique
 //    }else{
 //    	Mix_PauseMusic(); //Mettre en pause la musique
 //    }
	if(!Mix_PausedMusic()){
		Mix_PauseMusic();
	}
}

void ResumeMusic(){
	if(Mix_PausedMusic())
		Mix_ResumeMusic();
}


// ******************************
// Chunk is a noise for an action
// ******************************

Mix_Chunk* LoadSound(char const* filepath){
	Mix_Chunk* sample = Mix_LoadWAV(filepath);
	// if(!sample) {
	//     printf("Mix_LoadWAV: %s\n", Mix_GetError());
	//     exit(-1);
	//     // handle error
	// }
	return sample;
}

void FreeSound(Mix_Chunk* sound){
	Mix_FreeChunk(sound);
}

void PlaySound(Mix_Chunk* sound){
	//Mix_PlayChannel(channel, sound, loop);
	if(Mix_PlayChannel(-1, sound, 0)==-1) {
	    printf("Mix_PlayChannel: %s\n",Mix_GetError());
	    // may be critical error, or maybe just no channels were free.
	    // you could allocated another channel in that case...
	}
}

//Manage volume (no need to modify, the init is great)
void AdjustChannelVolume(int channel, int volume){
	volume = (volume > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume;
	volume = (volume < 0) ? 0 : volume;
	Mix_Volume(channel, volume);
}