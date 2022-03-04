#include "main.h"
#include "input.h"
#include "ui.h"
#include "movements.h"
#include "keyboard.h"
#include "gun.h"
#include "tutorial.h"

Vector2 LastTouch;
uint32 keys;
uint32 keysdown;
uint32 keysup;
touchPosition touch;

int gamepadTouchAreaX1 = GAMEPAD_TOUCH_AREA_X1_RIGHT_HANDLED;
int gamepadTouchAreaX2 = GAMEPAD_TOUCH_AREA_X2_RIGHT_HANDLED;
int gamepadTouchAreaY1 = 30;
int gamepadTouchAreaY2 = 194;
float sensitivity = DEFAULT_SENSITIVITY;
int currentSlider = -1;

int xAngleAdded = 0;
int yAngleAdded = 0;

void readKeys()
{
    scanKeys();
    keys = keysHeld();
    keysdown = keysDown();
    keysup = keysUp();
    touchRead(&touch);

    readKeyboard();
}

void ReadTouchScreen(touchPosition touch, bool *NeedChangeScreen, bool *AlwaysUpdateBottomScreen, int *ButtonToShow, int *UpdateBottomScreenOneFrame, bool *SendTeam, bool forceCheck)
{
    if ((*UpdateBottomScreenOneFrame == 0 && uiTimer == 0) || forceCheck)
    {
        for (int i = 0; i < checkBoxToShow; i++)
        {
            // Check checkbox
            if ((keysdown & KEY_TOUCH && touch.px >= AllCheckBoxs[i].xPos && touch.px <= AllCheckBoxs[i].xPos + AllCheckBoxs[i].xSize && touch.py >= AllCheckBoxs[i].yPos && touch.py <= AllCheckBoxs[i].yPos + AllCheckBoxs[i].ySize))
            {
                // AllCheckBoxs[i].value = !AllCheckBoxs[i].value;
                *AllCheckBoxs[i].value = !*AllCheckBoxs[i].value;
                *UpdateBottomScreenOneFrame += 8;
                if (currentMenu == CONTROLSSETTINGS) // Update screen button (map menu)
                {
                    if (i == 0)
                    {
                        UpdateGamepadArea();
                    }
                }
                return;
            }
        }

        if (keys & KEY_TOUCH)
        {
            if (keysdown & KEY_TOUCH && currentSlider == -1)
            {
                for (int i = 0; i < sliderToShow; i++)
                {
                    if ((touch.px >= AllSliders[i].xPos && touch.px <= AllSliders[i].xPos + AllSliders[i].xSize && touch.py >= AllSliders[i].yPos - 10 && touch.py <= AllSliders[i].yPos + 10))
                    {
                        currentSlider = i;
                        break;
                    }
                }
            }
            else if (currentSlider != -1)
            {
                if ((touch.px >= AllSliders[currentSlider].xPos && touch.px <= AllSliders[currentSlider].xPos + AllSliders[currentSlider].xSize && touch.py >= AllSliders[currentSlider].yPos - 10 && touch.py <= AllSliders[currentSlider].yPos + 10))
                {
                    *AllSliders[currentSlider].value = map(touch.px, AllSliders[currentSlider].xPos, AllSliders[currentSlider].xPos + AllSliders[currentSlider].xSize, AllSliders[currentSlider].min, AllSliders[currentSlider].max);
                }
                return;
            }
        }
        else
        {
            currentSlider = -1;
        }

        // Check for all buttons click
        for (int i = 0; i < *ButtonToShow; i++)
        {
            if (!AllButtons[i].isHidden && (keysdown & KEY_TOUCH && touch.px >= AllButtons[i].xPos && touch.px <= AllButtons[i].xPos + AllButtons[i].xSize && touch.py >= AllButtons[i].yPos && touch.py <= AllButtons[i].yPos + AllButtons[i].ySize))
            {
                AllButtons[i].OnClick(AllButtons[i].parameter);
                return;
            }
        }

        // Check back button click
        if (useQuitButton && keysdown & KEY_TOUCH && touch.px >= 236 && touch.px <= 236 + 20 && touch.py >= 1 && touch.py <= 1 + 20)
        {
            if (currentMenu == SETTINGS)
            {
                uiTimer = 8;
                actionOfUiTimer = SAVE;
            }
            lastOpenedMenu();
            return;
        }

        if (currentMenu == CONTROLLER && keys & KEY_TOUCH && touch.px >= gamepadTouchAreaX1 && touch.px <= gamepadTouchAreaX2 && touch.py >= gamepadTouchAreaY1 && touch.py <= gamepadTouchAreaY2)
        {
            if (keysdown & KEY_TOUCH)
            {
                if (GetDoubleTapTimer() > 0)
                    SetAim();

                SetDoubleTapTimer(11);
            }
            else if (LastTouch.x != -1)
            {
                int xAngleToAdd = 0;
                int yAngleToAdd = 0;

                if (LastTouch.x != touch.px)
                    xAngleToAdd = LastTouch.x - touch.px;

                if (LastTouch.y != touch.py)
                    yAngleToAdd = touch.py - LastTouch.y;

                xAngleToAdd = fmin(xAngleToAdd, 50);
                yAngleToAdd = fmin(yAngleToAdd, 50);

                xAngleAdded = abs(xAngleToAdd);
                yAngleAdded = abs(yAngleToAdd);

                // if (xAngleToAdd + yAngleToAdd < 50) // Security to avoid touch screen stylus teleportation
                AddAnglesToPlayer(xAngleToAdd / 2.0, yAngleToAdd / 2.0);
            }

            LastTouch.x = touch.px;
            LastTouch.y = touch.py;
        }
        else
        {
            LastTouch.x = -1;
            LastTouch.y = -1;
        }
    }
}

void UpdateGamepadArea()
{
    if (isLeftControls)
    {
        gamepadTouchAreaX1 = GAMEPAD_TOUCH_AREA_X1_LEFT_HANDLED;
        gamepadTouchAreaX2 = GAMEPAD_TOUCH_AREA_X2_LEFT_HANDLED;
    }
    else
    {
        gamepadTouchAreaX1 = GAMEPAD_TOUCH_AREA_X1_RIGHT_HANDLED;
        gamepadTouchAreaX2 = GAMEPAD_TOUCH_AREA_X2_RIGHT_HANDLED;
    }
}

void ScanForInput()
{
    if (scanForInput)
    {
        for (int i = 0; i < INPUT_COUNT; i++)
        {
            if (keysdown & BIT(i))
            {
                bool canChange = true;
                for (int j = 0; j < INPUT_COUNT; j++)
                {
                    if (inputs[j].value == BIT(i))
                    {
                        canChange = false;
                        break;
                    }
                }

                if (canChange)
                {
                    inputs[currentInputScanned].value = BIT(i);
                    inputs[currentInputScanned].nameIndex = i;
                }
                else
                {
                    inputs[currentInputScanned].value = 0;
                    inputs[currentInputScanned].nameIndex = 14;
                }
                if (isInTutorial && inputs[currentInputScanned].value != 0)
                {
                    updateTutorialDialogText();
                }
                scanForInput = false;
                break;
            }
        }
    }
}

bool isKeyDown(enum inputButtons buttonName)
{
    return keysdown & inputs[buttonName].value;
}

bool isKeyUp(enum inputButtons buttonName)
{
    return keysup & inputs[buttonName].value;
}

bool isKey(enum inputButtons buttonName)
{
    return keys & inputs[buttonName].value;
}