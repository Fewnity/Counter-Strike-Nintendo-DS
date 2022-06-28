// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef SOUNDS_H_ /* Include guard */
#define SOUNDS_H_
#include <NEMain.h>
#include "main.h"

void DoStepSound(int Volume, int Panning, int playerIndex);
void GetPanning(int PlayerId, int *OutPanning, int *OutVolume, float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance);
void GetPanningByPosition(int *OutPanning, int *OutVolume, Vector4 PositionB, float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance);
void Play3DSound(mm_word sound, int Volume, int Panning, Player *player);
void Play2DSound(mm_word sound, int Volume);
void PlayBasicSound(mm_word sound);
void initSoundSystem();
void loadSounds();
void unloadSounds();
void launchMusic();
void loadMusic();
void stopMusic();

extern int musicLength;
extern bool isMusicPlaying;
extern bool soundsLoaded;

extern bool soundBankLoaded;
extern bool musicFilePresent;

#endif // SOUNDS_H_