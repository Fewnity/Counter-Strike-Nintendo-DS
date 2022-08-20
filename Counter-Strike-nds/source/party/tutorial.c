// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "tutorial.h"
#include "input.h"
#include "party.h"
#include "player.h"
#include "map.h"
#include "movements.h"

// Current tutorial step
enum TutorialTextEnum tutorialStep = -1;
// Is the player in the tutorial ?
bool isInTutorial = false;
// Is the player done the tutorial ?
bool tutorialDone = false;
// Timer to wait before the next tutorial step
int tutorialTimer = 0;
// Value to increment to pass to the next tutorial step
float tutorialAddedValue = 0;
// Tutorial text
char tutorialText[100] = "";

/**
 * @brief Get the tutorial text and put the text in the tutorialText variable
 *
 */
void getTutorialText()
{
    switch (tutorialStep)
    {
    case TextWelcome:
        sprintf(tutorialText, "Welcome to Counter Strike");
        return;

    case TextLookAround:
        sprintf(tutorialText, "Look around with the stylus");
        return;

    case TextMovePlayer:
        sprintf(tutorialText, "Move your player with %s %s %s %s", inputsNames[inputs[UP_BUTTON].nameIndex], inputsNames[inputs[DOWN_BUTTON].nameIndex], inputsNames[inputs[LEFT_BUTTON].nameIndex], inputsNames[inputs[RIGHT_BUTTON].nameIndex]);
        return;

    case TextJump:
        sprintf(tutorialText, "Jump with %s", inputsNames[inputs[JUMP_BUTTON].nameIndex]);
        return;

    case TextShoot:
        sprintf(tutorialText, "Shoot with %s", inputsNames[inputs[FIRE_BUTTON].nameIndex]);
        return;

    case TextReload:
        sprintf(tutorialText, "Reload with the reload button on the gamepad");
        return;

    case TextChangeGun:
        sprintf(tutorialText, "Change weapons with the left/right arrows on the gamepad");
        return;

    case TextPlantBomb:
        sprintf(tutorialText, "Plant the bomb with %s on the red cross", inputsNames[inputs[FIRE_BUTTON].nameIndex]);
        return;

    case TextDefuseBomb:
        sprintf(tutorialText, "Defuse the bomb with %s", inputsNames[inputs[DEFUSE_BUTTON].nameIndex]);
        return;

    case TextEnd:
        sprintf(tutorialText, "Tutorial finished!");
        return;

    case TextEmpty:
        return;
    }
    strcpy(tutorialText, "");
}

/**
 * @brief Check if a step is done and if so, go to the next step or end the tutorial
 *
 */
void checkTutorial()
{
    if (tutorialTimer > 0)
        tutorialTimer--;

    switch (tutorialStep)
    {
    case TextEmpty:
        nextStep();
        roundState = PLAYING;
        canChangeGun = false;
        canShoot = false;
        localPlayer->AllAmmoMagazine[0].AmmoCount = 1;
        tutorialTimer = 280;
        break;

    case TextWelcome:
        if (tutorialTimer == 0)
        {
            nextStep();
        }
        break;

    case TextLookAround:
        tutorialAddedValue += xAngleAdded + yAngleAdded;
        if (tutorialAddedValue >= 800)
        {
            nextStep();
        }
        break;

    case TextMovePlayer:
        tutorialAddedValue += xSpeedAdded + zSpeedAdded;
        if (tutorialAddedValue >= 110000)
        {
            nextStep();
        }
        break;

    case TextJump:
        if ((isKeyDown(JUMP_BUTTON) || NeedJump))
        {
            nextStep();
            canShoot = true;
        }
        break;

    case TextShoot:
        if ((isKeyDown(FIRE_BUTTON)))
        {
            nextStep();
        }
        break;

    case TextReload:
        if (localPlayer->AllAmmoMagazine[0].AmmoCount != 0)
        {
            nextStep();
            setSelectedGunInInventory(0, 0);
            canChangeGun = true;
        }
        else
        {
            canShoot = false;
        }
        break;

    case TextChangeGun:
        if (localPlayer->currentGunInInventory == 1)
        {
            nextStep();
            SetGunInInventory(28, 8);
            setSelectedGunInInventory(0, 8);
            canChangeGun = false;
        }
        break;

    case TextPlantBomb:
        if (BombPlanted)
        {
            nextStep();
            localPlayer->Team = COUNTERTERRORISTS;
        }
        break;

    case TextDefuseBomb:
        if (BombDefused)
        {
            nextStep();
            tutorialTimer = 280;
        }
        break;

    case TextEnd:
        if (tutorialTimer == 0)
        {
            nextStep();
            initMainMenu();
        }
        break;
    }
}

/**
 * @brief Go to next step of the tutorial
 *
 */
void nextStep()
{
    tutorialStep++;
    updateTutorialDialogText();
    tutorialAddedValue = 0;
}

/**
 * @brief Show/update the tutorial dialog text
 *
 */
void updateTutorialDialogText()
{
    getTutorialText();
    setDialogText(tutorialText);
}

/**
 * @brief Finish the tutorial
 *
 */
void endTutorial()
{
    tutorialStep = -1;
    setDialogText("");
    canChangeGun = true;
    canShoot = true;
    tutorialDone = true;
    isInTutorial = false;
    currentSelectionMap = DUST2;
}