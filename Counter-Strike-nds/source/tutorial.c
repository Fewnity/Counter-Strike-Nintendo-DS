#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "party.h"
#include "movements.h"

int tutorialStep = -1;
bool isInTutorial = false;
bool tutorialDone = false;
int tutorialTimer;
float tutorialAddedValue;

char tutorialText[100];

void getTutorialText()
{
    switch (tutorialStep)
    {
    case 0:
        sprintf(tutorialText, "Welcome in Counter Strike");
        return;

    case 1:
        sprintf(tutorialText, "Look around you with the stylus");
        return;

    case 2:
        sprintf(tutorialText, "Move your player with ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[UP_BUTTON].nameIndex]);
        sprintf(tutorialText + strlen(tutorialText), " ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[DOWN_BUTTON].nameIndex]);
        sprintf(tutorialText + strlen(tutorialText), " ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[LEFT_BUTTON].nameIndex]);
        sprintf(tutorialText + strlen(tutorialText), " ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[RIGHT_BUTTON].nameIndex]);
        return;

    case 3:
        sprintf(tutorialText, "Jump with ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[JUMP_BUTTON].nameIndex]);
        return;

    case 4:
        sprintf(tutorialText, "Shoot with ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[FIRE_BUTTON].nameIndex]);
        return;

    case 5:
        sprintf(tutorialText, "Reload with the reload button in the gamepad");
        return;

    case 6:
        sprintf(tutorialText, "Change gun with left/right arrow in the gamepad");
        return;

    case 7:
        sprintf(tutorialText, "Plant the bomb with ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[FIRE_BUTTON].nameIndex]);
        sprintf(tutorialText + strlen(tutorialText), " on the red cross");
        return;

    case 8:
        sprintf(tutorialText, "Defuse the bomb with ");
        sprintf(tutorialText + strlen(tutorialText), inputsNames[inputs[DEFUSE_BUTTON].nameIndex]);
        return;

    case 9:
        sprintf(tutorialText, "Tutorial finished!");
        return;
    }
    strcpy(tutorialText, "");
}

void checkTutorial()
{
    if (tutorialTimer > 0)
        tutorialTimer--;

    switch (tutorialStep)
    {
    case -1:
        nextStep();
        roundState = PLAYING;
        canChangeGun = false;
        canShoot = false;
        localPlayer->AllAmmoMagazine[0].AmmoCount = 1;
        tutorialTimer = 280;
        break;

    case 0:
        if (tutorialTimer == 0)
        {
            nextStep();
        }
        break;

    case 1:
        tutorialAddedValue += xAngleAdded + yAngleAdded;
        if (tutorialAddedValue >= 800)
        {
            nextStep();
        }
        break;

    case 2:
        tutorialAddedValue += xSpeedAdded + zSpeedAdded;
        if (tutorialAddedValue >= 110000)
        {
            nextStep();
        }
        break;

    case 3:
        if ((isKeyDown(JUMP_BUTTON) || NeedJump))
        {
            nextStep();
            canShoot = true;
        }
        break;

    case 4:
        if ((isKeyDown(FIRE_BUTTON)))
        {
            nextStep();
        }
        break;

    case 5:
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

    case 6:
        if (localPlayer->currentGunInInventory == 1)
        {
            nextStep();
            SetGunInInventory(28, 8);
            setSelectedGunInInventory(0, 8);
            canChangeGun = false;
        }
        break;

    case 7:
        if (BombPlanted)
        {
            nextStep();
            localPlayer->Team = COUNTERTERRORISTS;
        }
        break;

    case 8:
        if (BombDefused)
        {
            nextStep();
            tutorialTimer = 280;
        }
        break;

    case 9:
        if (tutorialTimer == 0)
        {
            nextStep();
            initMainMenu();
        }
        break;
    }
}

void nextStep()
{
    tutorialStep++;
    updateTutorialDialogText();
    tutorialAddedValue = 0;
}

void updateTutorialDialogText()
{
    getTutorialText();
    setDialogText(tutorialText);
}

void endTutorial()
{
    tutorialStep = -1;
    setDialogText("");
    canChangeGun = true;
    canShoot = true;
    tutorialDone = true;
    isInTutorial = false;
    currentSelectionMap = 0;
}