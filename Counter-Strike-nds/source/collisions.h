#ifndef COLLISION_H_ /* Include guard */
#define COLLISION_H_
#include "main.h"

extern PhysicalGrenade *grenades[GrenadeCount];

void AddAllCollisions(); /**/
void AddAllStairs();     /**/
void CalculatePlayerColBox(int PlayerId);
void CalculateWallColBox(int WallId);
void CalculateTriggerColBox(float xPos, float zPos, float xSize, float zSize, int TriggerId);
void SetBombZone(float xPos, float zPos, float xSize, float zSize, int TriggerId, CollisionBox2D AllBombsTriggersCollisions[]);
void SetBombDefuseZone(float xPos, float zPos, float xSize, float zSize, CollisionBox2D *DefuseZoneCollisions);
void CheckZones(CollisionBox2D AllTriggersCollisions[], CollisionBox2D AllBombsTriggersCollisions[], CollisionBox2D bombDefuseZone, bool *CanPutBomb, bool *canDefuseBomb);
void checkPlayerOcclusionZone(int playerIndex, int playerCameraTarget);
void CheckStairs(int StairsCount, int *CanJump, bool *isInDownStairs);
void CreateStairs(float xSideA, float xSideB, float zSideA, float zSideB, float startY, float endY, int direction, int index);
void CreateWall(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize, int Zone, int index);
void SetPlayerForCollisions();
bool CheckStairsForGrenades(PhysicalGrenade *grenade, int StairsCount);
void CalculateAllTriggerColBoxs();
int CheckWallHit(int playerId);
int Raycast(int playerId);
void prepareAiRaycast(int fromPlayerIndex, int toPlayerIndex, bool checkVisibility);

#endif // COLLISION_H_