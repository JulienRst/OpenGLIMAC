#pragma once

#include <cstdlib>
#include <iostream>
#include "SDL.h"
#include "SDL_mixer.h"


//Initialisation et fermeture
void InitAudio();
void QuitAudio();

//Gérer des musiques générale
Mix_Music* LoadMusic(char const* filepath);
void FreeMusic(Mix_Music* music);
void PlayMusic(Mix_Music* music, int loops);
void StopMusic();
void ResumeMusic();

//Gérer des sons ponctuels
Mix_Chunk* LoadSound(char const* filepath);
void FreeSound(Mix_Chunk* sound);
void PlaySound(Mix_Chunk* sound);
void AdjustChannelVolume(int channel, int volume);
