#ifndef MOVEMENTS_H_ /* Include guard */
#define MOVEMENTS_H_

#include <NEMain.h>
#include "main.h"

extern int updateRate;

void MovePlayer(int CurrentSpeed, float xWithoutY, float zWithoutY, uint32 keys, bool *NeedBobbing); /**/
void RotatePlayer(uint32 keys, bool *NeedUpdateViewRotation, bool *SendPosition, float *CameraAngleY, int CurrentScopeLevel);
void SetPlayersForMovements();
void AddAnglesToPlayer(float xAngleToAdd, float yAngleToAdd);
void UpdateLookRotation(float CameraAngleY);
// void UpdateLookRotationAI(float CameraAngleY, int playerId, float *x, float *y, float *z);
void UpdateLookRotationAI(float CameraAngleY, float angle, float *x, float *y, float *z);
void SetOnlinelPlayersPositions();
void normalize(Vector3 *p);
void normalize2D(Vector2 *p);
void normalize1D(Vector2 *p);
void ForceUpdateLookRotation(float CameraAngleY);

#endif // MOVEMENTS_H_