// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef DEBUG_H_ /* Include guard */
#define DEBUG_H_
#include "main.h"

// Text colors
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

void debugPrint(const char *text);
void error_handler(const char *text);
void initDebug();

extern bool isDebugTopScreen;
extern bool isDebugBottomScreen;
extern bool isDebugMode;

#endif // DEBUG_H_