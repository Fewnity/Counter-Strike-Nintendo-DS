#ifndef MOVEMENTS_H_ /* Include guard */
#define MOVEMENTS_H_

#include <NEMain.h>
#include "main.h"

void MovePlayer(int CurrentSpeed, float xWithoutY, float zWithoutY, uint32 keys, bool *NeedBobbing); /**/
void RotatePlayer(uint32 keys, bool *NeedUpdateViewRotation, bool *SendPosition, float *CameraAngleY, int CurrentScopeLevel);
void SetPlayersForMovements();
void AddAnglesToPlayer(float xAngleToAdd, float yAngleToAdd);
void UpdateLookRotation(float CameraAngleY, float *x, float *y, float *z, float *xWithoutY, float *zWithoutY, float *xWithoutYForAudio, float *zWithoutYForAudio);
void UpdateLookRotationAI(float CameraAngleY, int playerId, float *x, float *y, float *z);
void SetOnlinelPlayersPositions();
void normalize(Vector3 *p);
void normalize2D(Vector2 *p);
void normalize1D(Vector2 *p);

#endif // MOVEMENTS_H_