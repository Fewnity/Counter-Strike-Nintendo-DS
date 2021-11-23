#ifndef SOUNDS_H_ /* Include guard */
#define SOUNDS_H_
#include <NEMain.h>
#include "main.h"

void DoStepSound(int Volume, int Panning, int playerIndex);
void GetPanning(int PlayerId, int *OutPanning, int *OutVolume, Player AllPlayers[], float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance);
void GetPanningByPosition(int *OutPanning, int *OutVolume, Player AllPlayers[], Vector4 PositionB, float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance);
void Play3DSound(mm_word sound, int Volume, int Panning);
void Play2DSound(mm_word sound, int Volume);
void PlayBasicSound(mm_word sound);
void initSoundSystem();

#endif // SOUNDS_H_