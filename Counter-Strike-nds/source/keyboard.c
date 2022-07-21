// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "sounds.h"
#include "ui.h"
#include "keyboard.h"
#include "saveManager.h"
#include "input.h"
#include "network.h"

#define KEY_COUNT 39
#define OTHER_KEY_COUNT 5

// Loaded keys data
Key allKeys[KEY_COUNT];
OtherKey allOtherKeys[OTHER_KEY_COUNT];

// Temporary char array
char tempText[21];

// Is the keyboard in upper case
bool isUpperCase = false;

// Param to pass to the function to call if the keyboard is cancelled
int returnToMenuOnCancel = -1;
// Param to pass to the function to call if the keyboard is confirmed
int returnToMenuOnSucces = -1;
// Is the keyboard in azerty mode
bool isAzerty = false;

// function to call if the keyboard is cancelled
void (*onKeyboardCloseSucces)();
// function to call if the keyboard is confirmed
void (*onKeyboardCloseCancel)();

// What the keyboard used for?
int keyboardAction = -1;

// Max length of the text to enable the confirm button
int maxInputLength = 20;
// Min length of the text to enable the confirm button
int minInputLength = 1;

/**
 * @brief Set can change keyboard case
 *
 * @param value enable(true) or disable(false)
 */
void SetCanChangeCase(bool value)
{
    allOtherKeys[SHITF_KEY_INDEX].visible = value;
}

/**
 * @brief Set keyboard mode to azerty or qwerty
 *
 */
void changeKeyboardMode()
{
    // Change the keyboard mode
    isAzerty = !isAzerty;
    // Update setting text
    if (isAzerty)
    {
        AllButtons[2].text = "Azerty";
    }
    else
    {
        AllButtons[2].text = "Qwerty";
    }
    // Refresh screen
    UpdateBottomScreenFrameCount += 8;
    initKeyboard();
}

/**
 * @brief Init keyboard
 *
 */
void initKeyboard()
{
    // Reset values
    for (int i = 0; i < KEY_COUNT; i++)
    {
        allKeys[i].visible = true;
    }

    for (int i = 0; i < OTHER_KEY_COUNT; i++)
    {
        allOtherKeys[i].visible = true;
    }

    int index = 0;
    int yOffset = 4;
    int yOffset2 = 6;

    //
    strncpy(allKeys[index].letter, "1", 2);
    strncpy(allKeys[index].letterUpperCase, "1", 2);
    allKeys[index].xPos = 2 + 3 * 0;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "2", 2);
    strncpy(allKeys[index].letterUpperCase, "2", 2);
    allKeys[index].xPos = 2 + 3 * 1;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "3", 2);
    strncpy(allKeys[index].letterUpperCase, "3", 2);
    allKeys[index].xPos = 2 + 3 * 2;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "4", 2);
    strncpy(allKeys[index].letterUpperCase, "4", 2);
    allKeys[index].xPos = 2 + 3 * 3;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "5", 2);
    strncpy(allKeys[index].letterUpperCase, "5", 2);
    allKeys[index].xPos = 2 + 3 * 4;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "6", 2);
    strncpy(allKeys[index].letterUpperCase, "6", 2);
    allKeys[index].xPos = 2 + 3 * 5;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "7", 2);
    strncpy(allKeys[index].letterUpperCase, "7", 2);
    allKeys[index].xPos = 2 + 3 * 6;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "8", 2);
    strncpy(allKeys[index].letterUpperCase, "8", 2);
    allKeys[index].xPos = 2 + 3 * 7;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "9", 2);
    strncpy(allKeys[index].letterUpperCase, "9", 2);
    allKeys[index].xPos = 2 + 3 * 8;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    strncpy(allKeys[index].letter, "0", 2);
    strncpy(allKeys[index].letterUpperCase, "0", 2);
    allKeys[index].xPos = 2 + 3 * 9;
    allKeys[index].yPos = 2 + 3 * (yOffset - 1);
    index++;

    //
    if (isAzerty)
    {
        strncpy(allKeys[index].letter, "a", 2);
        strncpy(allKeys[index].letterUpperCase, "A", 2);
    }
    else
    {
        strncpy(allKeys[index].letter, "q", 2);
        strncpy(allKeys[index].letterUpperCase, "Q", 2);
    }
    allKeys[index].xPos = 1 + 3 * 0;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    if (isAzerty)
    {
        strncpy(allKeys[index].letter, "z", 2);
        strncpy(allKeys[index].letterUpperCase, "Z", 2);
    }
    else
    {
        strncpy(allKeys[index].letter, "w", 2);
        strncpy(allKeys[index].letterUpperCase, "W", 2);
    }
    allKeys[index].xPos = 1 + 3 * 1;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "e", 2);
    strncpy(allKeys[index].letterUpperCase, "E", 2);
    allKeys[index].xPos = 1 + 3 * 2;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "r", 2);
    strncpy(allKeys[index].letterUpperCase, "R", 2);
    allKeys[index].xPos = 1 + 3 * 3;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "t", 2);
    strncpy(allKeys[index].letterUpperCase, "T", 2);
    allKeys[index].xPos = 1 + 3 * 4;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "y", 2);
    strncpy(allKeys[index].letterUpperCase, "Y", 2);
    allKeys[index].xPos = 1 + 3 * 5;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "u", 2);
    strncpy(allKeys[index].letterUpperCase, "U", 2);
    allKeys[index].xPos = 1 + 3 * 6;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "i", 2);
    strncpy(allKeys[index].letterUpperCase, "I", 2);
    allKeys[index].xPos = 1 + 3 * 7;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "o", 2);
    strncpy(allKeys[index].letterUpperCase, "O", 2);
    allKeys[index].xPos = 1 + 3 * 8;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    strncpy(allKeys[index].letter, "p", 2);
    strncpy(allKeys[index].letterUpperCase, "P", 2);
    allKeys[index].xPos = 1 + 3 * 9;
    allKeys[index].yPos = 1 + 3 * (yOffset + 0);
    index++;

    // ROW 2
    if (isAzerty)
    {
        strncpy(allKeys[index].letter, "q", 2);
        strncpy(allKeys[index].letterUpperCase, "Q", 2);
    }
    else
    {
        strncpy(allKeys[index].letter, "a", 2);
        strncpy(allKeys[index].letterUpperCase, "A", 2);
    }
    allKeys[index].xPos = 2 + 3 * 0;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "s", 2);
    strncpy(allKeys[index].letterUpperCase, "S", 2);
    allKeys[index].xPos = 2 + 3 * 1;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "d", 2);
    strncpy(allKeys[index].letterUpperCase, "D", 2);
    allKeys[index].xPos = 2 + 3 * 2;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "f", 2);
    strncpy(allKeys[index].letterUpperCase, "F", 2);
    allKeys[index].xPos = 2 + 3 * 3;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "g", 2);
    strncpy(allKeys[index].letterUpperCase, "G", 2);
    allKeys[index].xPos = 2 + 3 * 4;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "h", 2);
    strncpy(allKeys[index].letterUpperCase, "H", 2);
    allKeys[index].xPos = 2 + 3 * 5;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "j", 2);
    strncpy(allKeys[index].letterUpperCase, "J", 2);
    allKeys[index].xPos = 2 + 3 * 6;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "k", 2);
    strncpy(allKeys[index].letterUpperCase, "K", 2);
    allKeys[index].xPos = 2 + 3 * 7;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    strncpy(allKeys[index].letter, "l", 2);
    strncpy(allKeys[index].letterUpperCase, "L", 2);
    allKeys[index].xPos = 2 + 3 * 8;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    if (isAzerty)
    {
        strncpy(allKeys[index].letter, "m", 2);
        strncpy(allKeys[index].letterUpperCase, "M", 2);
    }
    else
    {
        strncpy(allKeys[index].letter, "?", 2);
        strncpy(allKeys[index].letterUpperCase, "?", 2);
    }
    allKeys[index].xPos = 2 + 3 * 9;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 1);
    index++;

    // ROW 3
    if (isAzerty)
    {
        strncpy(allKeys[index].letter, "w", 2);
        strncpy(allKeys[index].letterUpperCase, "W", 2);
    }
    else
    {
        strncpy(allKeys[index].letter, "z", 2);
        strncpy(allKeys[index].letterUpperCase, "Z", 2);
    }
    allKeys[index].xPos = 3 + 3 * 0;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, "x", 2);
    strncpy(allKeys[index].letterUpperCase, "X", 2);
    allKeys[index].xPos = 3 + 3 * 1;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, "c", 2);
    strncpy(allKeys[index].letterUpperCase, "C", 2);
    allKeys[index].xPos = 3 + 3 * 2;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, "v", 2);
    strncpy(allKeys[index].letterUpperCase, "V", 2);
    allKeys[index].xPos = 3 + 3 * 3;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, "b", 2);
    strncpy(allKeys[index].letterUpperCase, "B", 2);
    allKeys[index].xPos = 3 + 3 * 4;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, "n", 2);
    strncpy(allKeys[index].letterUpperCase, "N", 2);
    allKeys[index].xPos = 3 + 3 * 5;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    if (isAzerty)
    {
        strncpy(allKeys[index].letter, "?", 2);
        strncpy(allKeys[index].letterUpperCase, "?", 2);
    }
    else
    {
        strncpy(allKeys[index].letter, "m", 2);
        strncpy(allKeys[index].letterUpperCase, "M", 2);
    }
    allKeys[index].xPos = 3 + 3 * 6;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, ".", 2);
    strncpy(allKeys[index].letterUpperCase, ".", 2);
    allKeys[index].xPos = 3 + 3 * 7;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    strncpy(allKeys[index].letter, "!", 2);
    strncpy(allKeys[index].letterUpperCase, "!", 2);
    allKeys[index].xPos = 3 + 3 * 8;
    allKeys[index].yPos = 1 + 2 * (yOffset2 + 2);
    index++;

    index = 0;
    strncpy(allOtherKeys[index].name, "space", 8);
    allOtherKeys[index].xPos = 14;
    allOtherKeys[index].yPos = 1 + 2 * (yOffset2 + 3);
    allOtherKeys[index].xCenter = 19;
    allOtherKeys[index].xSize = 30;
    allOtherKeys[index].ySize = 2;
    allOtherKeys[index].visible = false;
    allOtherKeys[index].OnClick = &makeSpace;
    index++;

    strncpy(allOtherKeys[index].name, "del", 8);
    allOtherKeys[index].xPos = 23;
    allOtherKeys[index].yPos = 1 + 2 * (yOffset2 + 3);
    allOtherKeys[index].xCenter = 11;
    allOtherKeys[index].xSize = 30;
    allOtherKeys[index].ySize = 2;
    allOtherKeys[index].OnClick = &makeDelete;
    index++;

    strncpy(allOtherKeys[index].name, "caps", 8);
    allOtherKeys[index].xPos = 6;
    allOtherKeys[index].yPos = 1 + 2 * (yOffset2 + 3);
    allOtherKeys[index].xCenter = 19;
    allOtherKeys[index].xSize = 30;
    allOtherKeys[index].ySize = 2;
    allOtherKeys[index].OnClick = &shiftKeyboard;
    index++;

    strncpy(allOtherKeys[index].name, "confirm", 8);
    allOtherKeys[index].xPos = 23;
    allOtherKeys[index].yPos = 2 + 2 * (yOffset2 + 4);
    allOtherKeys[index].xCenter = 26;
    allOtherKeys[index].xSize = 34;
    allOtherKeys[index].ySize = 2;
    allOtherKeys[index].OnClick = &confirmKeyboard;
    index++;

    strncpy(allOtherKeys[index].name, "cancel", 8);
    allOtherKeys[index].xPos = 10;
    allOtherKeys[index].yPos = 2 + 2 * (yOffset2 + 4);
    allOtherKeys[index].xCenter = 22;
    allOtherKeys[index].xSize = 30;
    allOtherKeys[index].ySize = 2;
    allOtherKeys[index].OnClick = &cancelKeyboard;
    index++;
}

/**
 * @brief Draw keyboard
 *
 */
void drawKeyBoard()
{
    // Screen background
    NE_2DDrawQuad(0, 0, 256, 196, 20, RGB15(3, 3, 3));
    // Keyboard background
    NE_2DDrawQuad(0, 80, 256, 168, 19, RGB15(4, 4, 4));

    // Draw every keys
    for (int i = 0; i < KEY_COUNT; i++)
    {
        if (allKeys[i].visible)
        {
            // Print text
            if (isUpperCase)
            {
                NE_TextPrint(0,                                // Font slot
                             allKeys[i].xPos, allKeys[i].yPos, // Coordinates x(column), y(row)
                             NE_White,                         // Color
                             allKeys[i].letterUpperCase);
            }
            else
            {
                NE_TextPrint(0,                                // Font slot
                             allKeys[i].xPos, allKeys[i].yPos, // Coordinates x(column), y(row)
                             NE_White,                         // Color
                             allKeys[i].letter);
            }
            float xPos = allKeys[i].xPos * 8;
            float yPos = allKeys[i].yPos * 8;
            NE_2DDrawQuad((xPos)-6, (yPos)-3, (xPos) + 13, (yPos) + 12, 11, RGB15(7, 7, 7));
            NE_2DDrawQuad((xPos)-5, (yPos)-2, (xPos) + 12, (yPos) + 11, 10, RGB15(6, 6, 6));
        }
    }

    // Draw every specials keys
    for (int i = 0; i < OTHER_KEY_COUNT; i++)
    {
        if (allOtherKeys[i].visible)
        {
            // Print text
            NE_TextPrint(0,                                          // Font slot
                         allOtherKeys[i].xPos, allOtherKeys[i].yPos, // Coordinates x(column), y(row)
                         NE_White,                                   // Color
                         allOtherKeys[i].name);
            float xPos = allOtherKeys[i].xPos * 8;
            float yPos = allOtherKeys[i].yPos * 8;
            NE_2DDrawQuad(xPos - allOtherKeys[i].xSize - 1 + allOtherKeys[i].xCenter, yPos - allOtherKeys[i].ySize - 1, xPos + allOtherKeys[i].xSize + 1 + allOtherKeys[i].xCenter, yPos + 12, 11, RGB15(7, 7, 7));
            NE_2DDrawQuad(xPos - allOtherKeys[i].xSize + allOtherKeys[i].xCenter, yPos - allOtherKeys[i].ySize, xPos + allOtherKeys[i].xSize + allOtherKeys[i].xCenter, yPos + 11, 10, RGB15(6, 6, 6));
        }
    }
}

/**
 * @brief Read keyboard inputs
 *
 */
void readKeyboard()
{
    if (isShowingKeyBoard && uiTimer == 0)
    {
        // Read keys
        for (int i = 0; i < KEY_COUNT; i++)
        {
            float xPos = allKeys[i].xPos * 8;
            float yPos = allKeys[i].yPos * 8;
            if ((keysdown & KEY_TOUCH && allKeys[i].visible && touch.px >= xPos - 6 && touch.px <= xPos + 13 && touch.py >= yPos - 4 && touch.py <= yPos + 8))
            {
                int tempTextLength = strlen(tempText);
                if (tempTextLength < maxInputLength)
                {
                    if (isUpperCase)
                        sprintf(tempText + tempTextLength, allKeys[i].letterUpperCase);
                    else
                        sprintf(tempText + tempTextLength, allKeys[i].letter);
                }
                PlayBasicSound(SFX_KEYBOARD_SOUND);
                break;
            }
        }

        // Read specials keys
        for (int i = 0; i < OTHER_KEY_COUNT; i++)
        {
            float xPos = allOtherKeys[i].xPos * 8;
            float yPos = allOtherKeys[i].yPos * 8;
            if ((keysdown & KEY_TOUCH && allOtherKeys[i].visible && touch.px >= xPos - allOtherKeys[i].xSize + allOtherKeys[i].xCenter && touch.px <= xPos + allOtherKeys[i].xSize + allOtherKeys[i].xCenter && touch.py >= yPos - 4 && touch.py <= yPos + 8))
            {
                allOtherKeys[i].OnClick();
                PlayBasicSound(SFX_KEYBOARD_SOUND);
                break;
            }
        }
    }
}

/**
 * @brief Delete a char from the input string
 *
 */
void makeDelete()
{
    int tempTextLength = strlen(tempText);
    if (tempTextLength > 0)
        tempText[tempTextLength - 1] = '\0';
}

/**
 * @brief Add a space in the input string
 *
 */
void makeSpace()
{
    int tempTextLength = strlen(tempText);
    if (tempTextLength < maxInputLength)
        sprintf(tempText + tempTextLength, " ");
}

/**
 * @brief Confirm the input string
 *
 */
void confirmKeyboard()
{
    // if the input string length is respected
    if (strlen(tempText) >= minInputLength && strlen(tempText) <= maxInputLength)
    {

        isShowingKeyBoard = false;
        isUpperCase = false;
        if (keyboardAction == KEYBOARD_ACTION_CHANGE_NAME) // Set player name
        {
            strncpy(localPlayer->name, tempText, PLAYER_MAX_LENGTH);
            uiTimer = 8;
            actionOfUiTimer = SAVE;
        }
        else if (keyboardAction == KEYBOARD_ACTION_ENTER_CODE) // Set online party code
        {
            for (int i = 0; i < 5; i++)
            {
                partyCode[i] = tempText[i];
            }
        }

        // Call the succes function
        onKeyboardCloseSucces(returnToMenuOnSucces);
    }
}

/**
 * @brief Cancel the input string
 *
 */
void cancelKeyboard()
{
    strncpy(tempText, "", 21);
    isShowingKeyBoard = false;
    isUpperCase = false;

    if (keyboardAction == KEYBOARD_ACTION_ENTER_CODE) // Empty the online party code
    {
        strncpy(partyCode, "", PARTY_CODE_LENGTH);
    }

    // Call the cancel function
    onKeyboardCloseCancel(returnToMenuOnCancel);
}

/**
 * @brief Change the keyboard to case
 *
 */
void shiftKeyboard()
{
    isUpperCase = !isUpperCase;

    // Refresh screen
    UpdateBottomScreenFrameCount += 8;
}