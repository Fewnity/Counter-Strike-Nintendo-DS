// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef INPUT_H_ /* Include guard */
#define INPUT_H_

#include "main.h"

#define GAMEPAD_TOUCH_AREA_X1_RIGHT_HANDLED 40
#define GAMEPAD_TOUCH_AREA_X1_LEFT_HANDLED 0
#define GAMEPAD_TOUCH_AREA_X2_RIGHT_HANDLED 255
#define GAMEPAD_TOUCH_AREA_X2_LEFT_HANDLED 215
#define MIN_SENSITIVITY 0.1
#define DEFAULT_SENSITIVITY 2
#define MAX_SENSITIVITY 4

void initInputs();
void readKeys();
void ReadTouchScreen(touchPosition touch, bool *NeedChangeScreen, bool *AlwaysUpdateBottomScreen, int *ButtonToShow, int *UpdateBottomScreenOneFrame, bool *SendTeam, bool forceCheck);
void ScanForInput();
bool isKeyDown(enum inputButtons buttonName);
bool isKeyUp(enum inputButtons buttonName);
bool isKey(enum inputButtons buttonName);
void UpdateGamepadArea();

extern touchPosition touch;
extern uint32 keys;
extern uint32 keysdown;
extern uint32 keysup;
extern int gamepadTouchAreaX1;
extern int gamepadTouchAreaX2;
extern int gamepadTouchAreaY1;
extern int gamepadTouchAreaY2;
extern float sensitivity;
extern float buttonsSensitivity;
extern int xAngleAdded;
extern int yAngleAdded;
extern Input inputs[INPUT_COUNT];
extern bool scanForInput;
extern int currentInputScanned;
extern const char *inputsNames[INPUT_NAMES_COUNT];

#endif // INPUT_H_