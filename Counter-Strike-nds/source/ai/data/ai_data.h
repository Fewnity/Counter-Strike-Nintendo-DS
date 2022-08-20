// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef AI_DATA_H_ /* Include guard */
#define AI_DATA_H_

#include "ai.h"

void loadBotsNames();
void CreateWaypoints(int mapToLoad);
void createLengthMatrices(int mapToLoad);

#endif // AI_DATA_H_