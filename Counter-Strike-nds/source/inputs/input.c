// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "input.h"
#include "ui.h"
#include "movements.h"
#include "keyboard.h"
#include "gun.h"
#include "tutorial.h"

// Last position of the style / finger
Vector2 LastTouch;
// Held keys
uint32 keys = 0;
// Pressed keys
uint32 keysdown = 0;
// Released keys
uint32 keysup = 0;
// Position of the style / finger
touchPosition touch;

// Game pad areas
int gamepadTouchAreaX1 = GAMEPAD_TOUCH_AREA_X1_RIGHT_HANDLED;
int gamepadTouchAreaX2 = GAMEPAD_TOUCH_AREA_X2_RIGHT_HANDLED;
int gamepadTouchAreaY1 = 30;
int gamepadTouchAreaY2 = 194;

// Game pad sensitivity
float sensitivity = DEFAULT_SENSITIVITY;
float buttonsSensitivity = DEFAULT_SENSITIVITY;

// Current used slider index
int currentSlider = -1;

// Angles added to the player/camera when the stylus is moved
int xAngleAdded = 0;
int yAngleAdded = 0;

// All inputs data
Input inputs[INPUT_COUNT];
// Is scnanning for inputs?
bool scanForInput = false;
// Current input index scanned
int currentInputScanned = 0;
// All inputs names
const char *inputsNames[INPUT_NAMES_COUNT];

void initInputs()
{
    // Set default inputs
    inputs[FIRE_BUTTON].value = KEY_L;
    inputs[FIRE_BUTTON].nameIndex = 9;
    inputs[JUMP_BUTTON].value = KEY_R;
    inputs[JUMP_BUTTON].nameIndex = 8;

    inputs[LEFT_BUTTON].value = KEY_LEFT;
    inputs[LEFT_BUTTON].nameIndex = 5;
    inputs[RIGHT_BUTTON].value = KEY_RIGHT;
    inputs[RIGHT_BUTTON].nameIndex = 4;
    inputs[UP_BUTTON].value = KEY_UP;
    inputs[UP_BUTTON].nameIndex = 6;
    inputs[DOWN_BUTTON].value = KEY_DOWN;
    inputs[DOWN_BUTTON].nameIndex = 7;
    inputs[LOOK_LEFT_BUTTON].value = KEY_Y;
    inputs[LOOK_LEFT_BUTTON].nameIndex = 11;
    inputs[LOOK_RIGHT_BUTTON].value = KEY_A;
    inputs[LOOK_RIGHT_BUTTON].nameIndex = 0;
    inputs[LOOK_UP_BUTTON].value = KEY_X;
    inputs[LOOK_UP_BUTTON].nameIndex = 10;
    inputs[LOOK_DOWN_BUTTON].value = KEY_B;
    inputs[LOOK_DOWN_BUTTON].nameIndex = 1;

    inputs[DEFUSE_BUTTON].value = KEY_SELECT;
    inputs[DEFUSE_BUTTON].nameIndex = 2;
    inputs[SCOPE_BUTTON].value = KEY_START;
    inputs[SCOPE_BUTTON].nameIndex = 3;

    inputs[LEFT_GUN].value = -1; // Unasigned
    inputs[LEFT_GUN].nameIndex = 12;
    inputs[RIGHT_GUN].value = -1; // Unasigned
    inputs[RIGHT_GUN].nameIndex = 12;

    // Set inputs names list
    inputsNames[0] = "A";
    inputsNames[1] = "B";
    inputsNames[2] = "Select";
    inputsNames[3] = "Start";
    inputsNames[4] = "Right";
    inputsNames[5] = "Left";
    inputsNames[6] = "Up";
    inputsNames[7] = "Down";
    inputsNames[8] = "R";
    inputsNames[9] = "L";
    inputsNames[10] = "X";
    inputsNames[11] = "Y";
    inputsNames[12] = "Unassigned";
    inputsNames[13] = "Scanning...";
    inputsNames[14] = "Already used";
}

/**
 * @brief Read keys, touch screen and keyboard
 *
 */
void readKeys()
{
    scanKeys();
    keys = keysHeld();
    keysdown = keysDown();
    keysup = keysUp();
    touchRead(&touch);

    readKeyboard();
}

/**
 * @brief Read the touch screen
 *
 * @param touch
 * @param NeedChangeScreen
 * @param AlwaysUpdateBottomScreen
 * @param ButtonToShow
 * @param UpdateBottomScreenFrameCount
 * @param SendTeam
 * @param forceCheck
 */
void ReadTouchScreen(touchPosition touch, bool *NeedChangeScreen, bool *AlwaysUpdateBottomScreen, int *ButtonToShow, int *UpdateBottomScreenFrameCount, bool *SendTeam, bool forceCheck)
{
    if ((*UpdateBottomScreenFrameCount == 0 && uiTimer == 0) || forceCheck)
    {
        for (int i = 0; i < checkBoxToShow; i++)
        {
            // Check checkboxs
            if ((keysdown & KEY_TOUCH && touch.px >= AllCheckBoxs[i].xPos && touch.px <= AllCheckBoxs[i].xPos + AllCheckBoxs[i].xSize && touch.py >= AllCheckBoxs[i].yPos && touch.py <= AllCheckBoxs[i].yPos + AllCheckBoxs[i].ySize))
            {
                *AllCheckBoxs[i].value = !*AllCheckBoxs[i].value;
                *UpdateBottomScreenFrameCount += 8;
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
                // Check if the player is just touching a slider
                for (int i = 0; i < sliderToShow; i++)
                {
                    if ((touch.px >= AllSliders[i].xPos && touch.px <= AllSliders[i].xPos + AllSliders[i].xSize && touch.py >= AllSliders[i].yPos - 10 && touch.py <= AllSliders[i].yPos + 10))
                    {
                        currentSlider = i;
                        break;
                    }
                }
            }
            if (currentSlider != -1)
            {
                // Check used slider value
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

        if (!localPlayer->IsDead && currentMenu == CONTROLLER && keys & KEY_TOUCH && touch.px >= gamepadTouchAreaX1 && touch.px <= gamepadTouchAreaX2 && touch.py >= gamepadTouchAreaY1 && touch.py <= gamepadTouchAreaY2)
        {
            // Check if the player want to enable scope
            if (keysdown & KEY_TOUCH)
            {
                if (GetDoubleTapTimer() > 0)
                    SetAim();

                SetDoubleTapTimer(11);
            }
            else if (LastTouch.x != -1)
            {
                // Move the camera with the stylus

                int xAngleToAdd = 0;
                int yAngleToAdd = 0;

                if (LastTouch.x != touch.px)
                    xAngleToAdd = LastTouch.x - touch.px;

                if (LastTouch.y != touch.py)
                    yAngleToAdd = touch.py - LastTouch.y;

                xAngleToAdd = fmin(xAngleToAdd, 50);
                yAngleToAdd = fmin(yAngleToAdd, 50);

                // Total of angle added / removed to the camera
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

/**
 * @brief Update gamepad area
 *
 */
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

/**
 * @brief Scan of input (for input settings)
 *
 */
void ScanForInput()
{
    if (scanForInput)
    {
        for (int i = 0; i < 12; i++)
        {
            if (keysdown & BIT(i))
            {
                bool canChange = true;

                // Check if input is already used
                for (int j = 0; j < INPUT_COUNT; j++)
                {
                    if (inputs[j].value == BIT(i))
                    {
                        canChange = false;
                        break;
                    }
                }

                // If the input is available, change it
                if (canChange)
                {
                    inputs[currentInputScanned].value = BIT(i);
                    inputs[currentInputScanned].nameIndex = i;
                }
                else // If the input is already used, change the input text to already used
                {
                    inputs[currentInputScanned].value = -1;
                    inputs[currentInputScanned].nameIndex = 14;
                }

                // Is the player is in the tutorial, update the text
                if (isInTutorial && inputs[currentInputScanned].value != -1)
                {
                    updateTutorialDialogText();
                }
                scanForInput = false;
                break;
            }
        }
    }
}

/**
 * @brief Check if the input is pressed
 *
 * @param buttonName
 * @return true
 * @return false
 */
bool isKeyDown(enum inputButtons buttonName)
{
    return keysdown & inputs[buttonName].value && inputs[buttonName].value != -1;
}

/**
 * @brief Check if the input is released
 *
 * @param buttonName
 * @return true
 * @return false
 */
bool isKeyUp(enum inputButtons buttonName)
{
    return keysup & inputs[buttonName].value && inputs[buttonName].value != -1;
}

/**
 * @brief Check if the input is held
 *
 * @param buttonName
 * @return true
 * @return false
 */
bool isKey(enum inputButtons buttonName)
{
    return keys & inputs[buttonName].value && inputs[buttonName].value != -1;
}