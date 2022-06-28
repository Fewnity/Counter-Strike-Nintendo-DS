// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef KEYBOARD_H_ /* Include guard */
#define KEYBOARD_H_

#include <NEMain.h>
#include "main.h"

enum keyboardActionEnum
{
    KEYBOARD_ACTION_CHANGE_NAME = 0,
    KEYBOARD_ACTION_ENTER_CODE = 1,
};

extern char tempText[21];
extern int returnToMenuOnCancel;
extern int returnToMenuOnSucces;
extern bool isAzerty;
extern void (*onKeyboardCloseSucces)();
extern void (*onKeyboardCloseCancel)();
extern int keyboardAction;
extern int maxInputLength;
extern int minInputLength;
extern bool isUpperCase;

#define SHITF_KEY_INDEX 2

void initKeyboard();
void drawKeyBoard();
void readKeyboard();
void makeSpace();
void makeDelete();
void confirmKeyboard();
void cancelKeyboard();
void shiftKeyboard();
void changeKeyboardMode();
void SetCanChangeCase(bool value);

#endif // KEYBOARD_H_