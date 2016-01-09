// ***************
// *** READ ME ***
// ***************

// - ATTENTION mes include ne sont pas adaptés pour vous !! pour capter SDL.h

// - L'organisation de ce fichier reprend les fonctions de la sdl_mixer mieux organisé
//   pour que music.cpp soit plus clair

// - Les includes ne fonctionne pas chez moi ! Toujours une erreur pour lire l'include
//   pour SDL_mixer à croire que je ne l'ai pas installé (au bon endroit peut-etre) !

// - L'enumeration sert à nommer un son pour une action
//   EXEMPLE : Le Cow-Boy marche => Bruit de pas "WALK" plus propre dans le tableau d'appel
//   des sons de music.cpp

// - LoadMusic c'est pour une musique d'ambience
//   LoadChunk c'est seulement pour des petits bruits


#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include "SDL_mixer.h"

//Tout les noms de sons à avoir
typedef enum{
	WALK,
	TOUCH,
}SoundEvent;

//Initialisation et fermeture
void InitAudio();
void QuitAudio();

//Gérer des musiques générale
Mix_Music* LoadMusic(char * filepath);
void FreeMusic(Mix_Music* music);
void PlayMusic(Mix_Music* music, int loops);
void StopMusic();

//Gérer des sons ponctuels
Mix_Chunk* LoadSound(char * filepath);
void FreeSound(Mix_Chunk* sound);
void PlaySound(Mix_Chunk* sound, int channel, int loop);
void AdjustChannelVolume(int channel, int volume);
