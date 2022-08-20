// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef RAYCAST_H_ /* Include guard */
#define RAYCAST_H_
#include "../main.h"

void prepareAiRaycast(int fromPlayerIndex, int toPlayerIndex, bool checkVisibility);
void getValuesForRaycast(Vector3Int StartPosition, Vector3Int EndPosition, float *x, float *y, float *z);
void setRaycastValues(Player *shooterPlayer, float xRot, float yRot, float zRot);
int Raycast(int playerIndex, int currentShootIndex, float *distance);

#endif // RAYCAST_H_