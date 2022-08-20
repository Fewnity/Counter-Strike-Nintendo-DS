// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef CAMERA_H_ /* Include guard */
#define CAMERA_H_
//#include "main.h"

void checkCameraPlayerView(bool left, bool *Found, int *FirstFound, int i);
void addExplosionScreenShake();
void reduceScreenShake();
void changeCameraPlayerView(bool left);
void setCameraMapPosition();

extern int speedAimingReCenter;
extern int speedAimingReCenterTimer;

#endif // CAMERA_H_