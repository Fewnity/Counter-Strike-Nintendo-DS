#ifndef MOVEMENTS_H_ /* Include guard */
#define MOVEMENTS_H_

#include <NEMain.h>
#include "main.h"

extern int updateRate;
extern int xSpeedAdded;
extern int zSpeedAdded;

void MovePlayer(int CurrentSpeed, float xWithoutY, float zWithoutY, bool *NeedBobbing); /**/
void RotatePlayer(bool *NeedUpdateViewRotation, bool *SendPosition, float *CameraAngleY, int CurrentScopeLevel);
void SetPlayersForMovements();
void AddAnglesToPlayer(float xAngleToAdd, float yAngleToAdd);
void UpdateLookRotation(float CameraAngleY);
void UpdateLookRotationAI(float CameraAngleY, float angle, float *x, float *y, float *z);
void GetRotationForCullingAI(int playerIndex, float angle, float *xSide1, float *zSide1, float *xSide2, float *zSide2);
void SetOnlinelPlayersPositions();
void normalize(Vector3 *p);
void normalize2D(Vector2 *p);
void normalize1D(Vector2 *p);
void ForceUpdateLookRotation(float CameraAngleY);

#endif // MOVEMENTS_H_