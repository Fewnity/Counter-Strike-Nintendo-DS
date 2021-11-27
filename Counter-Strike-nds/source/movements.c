#include "main.h"
#include "ai.h"
#include "party.h"
#include <math.h>

// Player *AllPlayersRefForMovements;

void SetPlayersForMovements()
{
    // AllPlayersRefForMovements = GetPlayers();
}

void MovePlayer(int CurrentSpeed, float xWithoutY, float zWithoutY, uint32 keys, bool *NeedBobbing)
{
    if (keys & KEY_UP)
    {
        localPlayer->PlayerPhysic->xspeed += CurrentSpeed * xWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += CurrentSpeed * zWithoutY * 2;
        *NeedBobbing = true;
    }
    else if (keys & KEY_DOWN)
    {
        localPlayer->PlayerPhysic->xspeed += -CurrentSpeed * xWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += -CurrentSpeed * zWithoutY * 2;
        *NeedBobbing = true;
    }

    if (keys & KEY_RIGHT)
    {
        localPlayer->PlayerPhysic->xspeed += CurrentSpeed * -zWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += CurrentSpeed * xWithoutY * 2;
        *NeedBobbing = true;
    }
    else if (keys & KEY_LEFT)
    {
        localPlayer->PlayerPhysic->xspeed += CurrentSpeed * zWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += CurrentSpeed * -xWithoutY * 2;
        *NeedBobbing = true;
    }
}

void AddAnglesToPlayer(float xAngleToAdd, float yAngleToAdd)
{
    float AngleSpeed = 2;
    // if (GetCurrentScopeLevel())
    if (GetCurrentScopeLevel() == 1)
        AngleSpeed = 1;
    else if (GetCurrentScopeLevel() == 2)
        AngleSpeed = 0.5;

    // AngleSpeed /= 4;

    if (xAngleToAdd != 0)
    {
        localPlayer->Angle += AngleSpeed * xAngleToAdd;
        SetNeedUpdateViewRotation(true);
        SetSendPosition(true);
    }

    if (yAngleToAdd != 0)
    {
        float NewyAngle = GetCameraAngleY() + AngleSpeed * yAngleToAdd;
        if (NewyAngle > 10 && NewyAngle < 246)
        {
            SetCameraAngleY(NewyAngle);
            SetNeedUpdateViewRotation(true);
        }
    }
}

void RotatePlayer(uint32 keys, bool *NeedUpdateViewRotation, bool *SendPosition, float *CameraAngleY, int CurrentScopeLevel)
{
    int AngleSpeed = 2;
    if (CurrentScopeLevel)
        AngleSpeed = 1;

    // Change player rotation
    if (keys & KEY_A)
    {
        localPlayer->Angle -= AngleSpeed;
        *NeedUpdateViewRotation = true;
        *SendPosition = true;
    }
    else if (keys & KEY_Y)
    {
        localPlayer->Angle += AngleSpeed;
        *NeedUpdateViewRotation = true;
        *SendPosition = true;
    }

    // Change camera rotation
    if (keys & KEY_X && *CameraAngleY > 9)
    {
        *CameraAngleY -= AngleSpeed;
        *NeedUpdateViewRotation = true;
    }
    else if (keys & KEY_B && *CameraAngleY < 245)
    {
        *CameraAngleY += AngleSpeed;
        *NeedUpdateViewRotation = true;
    }
}

void UpdateLookRotation(float CameraAngleY)
{
    // Math formula to get a point position on sphere from the middle of the sphere with 2 angle
    float TempS = AllPlayers[GetCurrentCameraPlayer()].Angle / 512.0 * M_TWOPI;
    float TempSside1 = (AllPlayers[GetCurrentCameraPlayer()].Angle - 70) / 512.0 * M_TWOPI;
    float TempSside2 = (AllPlayers[GetCurrentCameraPlayer()].Angle + 70) / 512.0 * M_TWOPI;
    float TempSForAudio = (AllPlayers[GetCurrentCameraPlayer()].Angle - 128) / 512.0 * M_TWOPI;
    float TempSForMap = (AllPlayers[GetCurrentCameraPlayer()].Angle) / 512.0 * M_TWOPI;
    float TempT = (384 - CameraAngleY) / 512.0 * M_TWOPI;

    double cosTempT = cos(TempT);
    double cosTempS = cos(TempS);
    double SinTempS = sin(TempS);

    x = SinTempS * cosTempT;
    y = -sin(TempT);
    z = cosTempS * cosTempT;

    xWithoutY = -SinTempS;
    zWithoutY = -cosTempS;

    xWithoutYForAudio = -sin(TempSForAudio);
    zWithoutYForAudio = -cos(TempSForAudio);

    xWithoutYForMap = -sin(TempSForMap);
    zWithoutYForMap = -cos(TempSForMap);

    xWithoutYForOcclusionSide1 = -sin(TempSside1);
    zWithoutYForOcclusionSide1 = -cos(TempSside1);

    xWithoutYForOcclusionSide2 = -sin(TempSside2);
    zWithoutYForOcclusionSide2 = -cos(TempSside2);
}

void UpdateLookRotationAI(float CameraAngleY, int playerId, float *x, float *y, float *z)
{
    // Math formula to get a point position on sphere from the middle of the sphere with 2 angle
    float TempS = AllPlayers[playerId].Angle / 512.0 * M_TWOPI;
    float TempT = (384 - CameraAngleY) / 512.0 * M_TWOPI;

    double cosTempT = cos(TempT);
    double cosTempS = cos(TempS);
    double SinTempS = sin(TempS);

    *x = SinTempS * cosTempT;
    *y = -sin(TempT);
    *z = cosTempS * cosTempT;
}

float Speed = 0.05;
void normalize(Vector3 *p)
{
    float w = sqrtf(p->x * p->x + p->y * p->y + p->z * p->z);
    p->x /= w;
    p->y /= w;
    p->z /= w;
}

void normalize2D(Vector2 *p)
{
    float w = sqrtf(p->x * p->x + p->y * p->y);
    p->x /= w;
    p->y /= w;
}

void normalize1D(Vector2 *p)
{
    float w = sqrtf(p->x * p->x);
    p->x /= w;
}

void SetOnlinelPlayersPositions()
{
    // Loop using "AllPlayers" array for updating non local player position smoothly
    for (int i = 1; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];

        if (player->Id == -1 || player->IsDead)
            continue;

        // If a player is spawned
        if (!player->isAi)
        {
            float Speed = 0.1;
            float Dis = fabs(player->xPos - player->xDestination) + fabs(player->yPos - player->yDestination) + fabs(player->zPos - player->zDestination);

            if (Dis < 4.0)
            {
                // Smoothing player position
                if (player->xPos != player->xDestination)                                        // If player x position is not the same as x destination
                    player->xPos = player->xPos + Speed * (player->xDestination - player->xPos); // Move player with lerp system

                if (player->yPos != player->yDestination)                                        // If player y position is not the same as y destination
                    player->yPos = player->yPos + Speed * (player->yDestination - player->yPos); // Move player with lerp system

                if (player->zPos != player->zDestination)                                        // If player y position is not the same as z destination
                    player->zPos = player->zPos + Speed * (player->zDestination - player->zPos); // Move player with lerp system

                if (abs(player->AngleDestination - player->Angle) > 90)
                {
                    player->Angle = player->AngleDestination;
                }
                else
                {
                    // Smoothing player angle
                    float AngleAddAmout = Speed * (player->AngleDestination - player->Angle);
                    player->Angle = player->Angle + AngleAddAmout;
                }

                /*if (AllPlayersRefForMovements[i].Angle < AllPlayersRefForMovements[i].AngleDestination)
                    AllPlayersRefForMovements[i].Angle += 2;
                else if (AllPlayersRefForMovements[i].Angle > AllPlayersRefForMovements[i].AngleDestination)
                    AllPlayersRefForMovements[i].Angle -= 2;*/
            }
            else
            {
                player->xPos = player->xDestination;
                player->yPos = player->yDestination;
                player->zPos = player->zDestination;

                player->Angle = player->AngleDestination;
            }

            // Set new player position and angle
            NE_ModelSetCoord(player->PlayerModel, player->xPos, player->yPos, player->zPos);
            player->PlayerModel->ry = player->Angle;
        }
        else if (/*player->isAi && */ player->PathCount != 0 && player->target == -1 && RoundState != 0)
        {
            float Speed = 0.087;
            int CurPath = player->CurrentPath;
            int Ok = 0;

            Vector3 Direction;
            Direction.x = GetWaypoints()[player->Path[CurPath]].x - player->xPos;
            Direction.y = GetWaypoints()[player->Path[CurPath]].y - player->yPos;
            Direction.z = GetWaypoints()[player->Path[CurPath]].z - player->zPos;
            Vector2 Direction2D;
            Direction2D.x = GetWaypoints()[player->Path[CurPath]].x - GetWaypoints()[player->Path[CurPath - 1]].x;
            Direction2D.y = GetWaypoints()[player->Path[CurPath]].z - GetWaypoints()[player->Path[CurPath - 1]].z;
            normalize(&Direction);
            normalize2D(&Direction2D);

            // Smoothing player position
            if (player->xPos - 0.1 > GetWaypoints()[player->Path[CurPath]].x || player->xPos + 0.1 < GetWaypoints()[player->Path[CurPath]].x)
                player->xPos += Direction.x * Speed;
            else
                Ok++;

            if (player->yPos - 0.1 > GetWaypoints()[player->Path[CurPath]].y || player->yPos + 0.1 < GetWaypoints()[player->Path[CurPath]].y)
                player->yPos += Direction.y * Speed;

            if (player->zPos - 0.1 > GetWaypoints()[player->Path[CurPath]].z || player->zPos + 0.1 < GetWaypoints()[player->Path[CurPath]].z)
                player->zPos += Direction.z * Speed;
            else
                Ok++;

            if (Ok == 2)
            {
                if (CurPath < player->PathCount - 1)
                {
                    player->CurrentPath++;
                }
                else
                {
                    if (applyRules)
                    {
                        player->LastWayPoint = player->Path[CurPath];
                        if (player->haveBomb || (player->Team == COUNTERTERRORISTS && BombPlanted && !BombDefused))
                        {
                            iprintf("\nLastPath: %d", player->LastWayPoint);
                            if (player->LastWayPoint == 14 || player->LastWayPoint == 29)
                            {
                                if (player->Team == COUNTERTERRORISTS && bombPlantedAt != player->LastWayPoint)
                                {
                                    if (player->LastWayPoint == 14)
                                    {
                                        bombCheckedInA = true;
                                    }
                                    else if (player->LastWayPoint == 29)
                                    {
                                        bombCheckedInB = true;
                                    }
                                    SetDefuser(i);
                                }
                                else
                                {
                                    if (player->Team == COUNTERTERRORISTS)
                                        printf("(%d)DEFUSING\n", i);
                                    else
                                        printf("(%d)PLANTING\n", i);

                                    player->isPlantingBomb = true;
                                    if (!BombDefused && BombPlanted) // Set timer
                                        player->bombTimer = bombDefuseTime;
                                    else if (!BombPlanted) // Set timer
                                        player->bombTimer = bombPlantingTime;
                                }
                            }
                        }
                    }
                    player->PathCount = 0;
                }
            }

            NE_ModelSetCoord(player->PlayerModel, player->xPos, player->yPos, player->zPos);

            float FinalAngle = atan2f(Direction2D.x, Direction2D.y) * 512.0 / (M_TWOPI) + 256.0;
            /*if (AllPlayersRefForMovements[i].Angle + 2 < FinalAngle)
                AllPlayersRefForMovements[i].Angle += 2;
            else if (AllPlayersRefForMovements[i].Angle - 2 > FinalAngle)
                AllPlayersRefForMovements[i].Angle -= 2;*/
            player->Angle = FinalAngle;
            player->PlayerModel->ry = player->Angle;
        }
        else // if (player->isAi)
        {
            player->Angle = player->AngleDestination;
            player->PlayerModel->ry = player->AngleDestination;
        }
    }
}