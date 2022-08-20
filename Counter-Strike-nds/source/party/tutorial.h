// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef TUTORIAL_H_ /* Include guard */
#define TUTORIAL_H_

#include "main.h"

enum TutorialTextEnum
{
    TextEmpty = -1,
    TextWelcome = 0,
    TextLookAround = 1,
    TextMovePlayer = 2,
    TextJump = 3,
    TextShoot = 4,
    TextReload = 5,
    TextChangeGun = 6,
    TextPlantBomb = 7,
    TextDefuseBomb = 8,
    TextEnd = 9
};

extern enum TutorialTextEnum tutorialStep;
extern bool isInTutorial;
extern bool tutorialDone;

void getTutorialText();
void checkTutorial();
void nextStep();
void endTutorial();
void updateTutorialDialogText();

#endif // TUTORIAL_H_