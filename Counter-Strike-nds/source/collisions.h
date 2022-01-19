#ifndef COLLISION_H_ /* Include guard */
#define COLLISION_H_
#include "main.h"

extern PhysicalGrenade *grenades[GrenadeCount];

void AddAllCollisions(); /**/
void AddAllStairs();     /**/
void CalculatePlayerColBox(int PlayerId);
void CalculateWallColBox(int WallId);
void CalculateTriggerColBox(float xPos, float zPos, float xSize, float zSize, int TriggerId);
void SetBombZone(float xPos, float zPos, float xSize, float zSize, int TriggerId, int waypoint);
void SetBombDefuseZone(float xPos, float zPos, CollisionBox2D *DefuseZoneCollisions);
void SetBombTakingZone(float xPos, float zPos, CollisionBox2D *DefuseZoneCollisions);
void checkTakingBombZone(CollisionBox2D bombDefuseZone);

void CheckZones(CollisionBox2D AllTriggersCollisions[], CollisionBox2D bombDefuseZone, bool *CanPutBomb, bool *canDefuseBomb);
int checkBombZoneWaypoint();
void setRaycastValues(Player *shooterPlayer, float xRot, float yRot, float zRot);
void checkPlayerOcclusionZone(int playerIndex, int playerCameraTarget);
void CheckStairs(int *CanJump, bool *isInDownStairs);
void CreateStairs(float xSideA, float xSideB, float zSideA, float zSideB, float startY, float endY, int direction, int index);
void CreateWall(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize, int Zone, int index);
void SetPlayerForCollisions();
bool CheckStairsForGrenades(PhysicalGrenade *grenade);
void CalculateAllTriggerColBoxs();
int CheckWallHit(int playerId);
int Raycast(int playerId);
void prepareAiRaycast(int fromPlayerIndex, int toPlayerIndex, bool checkVisibility);
bool PointInTriangle(float px, float py, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
float sign(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
bool checkZoneForOcclusion(OcclusionZone *zone, int posx, int posz);
int signInt(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y);
bool PointInTriangleInt(int px, int py, int p1x, int p1y, int p2x, int p2y, int p3x, int p3y);

#endif // COLLISION_H_