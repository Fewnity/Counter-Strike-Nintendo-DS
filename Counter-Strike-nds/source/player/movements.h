// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef MOVEMENTS_H_ /* Include guard */
#define MOVEMENTS_H_

//#include <NEMain.h>
#include "main.h"

extern int updateRate;
extern int xSpeedAdded;
extern int zSpeedAdded;

void MovePlayer(int CurrentSpeed, float xWithoutY, float zWithoutY, bool *NeedBobbing); /**/
void RotatePlayer(bool *NeedUpdateViewRotation, bool *SendPosition, float *CameraAngleY);
void AddAnglesToPlayer(float xAngleToAdd, float yAngleToAdd);
void UpdateLookRotation(float CameraAngleY);
void UpdateLookRotationAI(float CameraAngleY, float angle, float *x, float *y, float *z);
void GetRotationForCullingAI(int playerIndex, float angle, float *xSide1, float *zSide1, float *xSide2, float *zSide2);
void SetOnlinelPlayersPositions();
void normalize(Vector3 *p);
void normalize2D(Vector2 *p);
void normalize1D(Vector2 *p);
void ForceUpdateLookRotation(float CameraAngleY);
void ApplyGunWalkAnimation(int playerIndex);

#endif // MOVEMENTS_H_