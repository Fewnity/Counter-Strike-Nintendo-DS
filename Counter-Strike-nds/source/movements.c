// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "ai.h"
#include "ui.h"
#include "party.h"
#include "movements.h"
#include "input.h"
#include "sounds.h"
#include "network.h"
#include <math.h>

int xSpeedAdded = 0;
int zSpeedAdded = 0;
int fovCheckAngle = 80;

/**
 * @brief Move player according to his speed and direction
 *
 * @param CurrentSpeed Speed
 * @param xWithoutY X direction
 * @param zWithoutY Z direction
 * @param NeedBobbing Need to bobbing
 */
void MovePlayer(int CurrentSpeed, float xWithoutY, float zWithoutY, bool *NeedBobbing)
{
    // Move forward/backward
    if (isKey(UP_BUTTON))
    {
        localPlayer->PlayerPhysic->xspeed += CurrentSpeed * xWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += CurrentSpeed * zWithoutY * 2;
        *NeedBobbing = true;
    }
    else if (isKey(DOWN_BUTTON))
    {
        localPlayer->PlayerPhysic->xspeed += -CurrentSpeed * xWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += -CurrentSpeed * zWithoutY * 2;
        *NeedBobbing = true;
    }

    // Move left/right
    if (isKey(RIGHT_BUTTON))
    {
        localPlayer->PlayerPhysic->xspeed += CurrentSpeed * -zWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += CurrentSpeed * xWithoutY * 2;
        *NeedBobbing = true;
    }
    else if (isKey(LEFT_BUTTON))
    {
        localPlayer->PlayerPhysic->xspeed += CurrentSpeed * zWithoutY * 2;
        localPlayer->PlayerPhysic->zspeed += CurrentSpeed * -xWithoutY * 2;
        *NeedBobbing = true;
    }

    // Total of speed
    xSpeedAdded = abs(localPlayer->PlayerPhysic->xspeed);
    zSpeedAdded = abs(localPlayer->PlayerPhysic->zspeed);
}

/**
 * @brief Add angle to local player
 *
 * @param xAngleToAdd X angle to add
 * @param yAngleToAdd Y angle to add
 */
void AddAnglesToPlayer(float xAngleToAdd, float yAngleToAdd)
{
    float AngleSpeed = 1;

    // If the player is scoping, the angle speed is lower
    if (GetCurrentScopeLevel() == 1)
        AngleSpeed = 0.5;
    else if (GetCurrentScopeLevel() == 2)
        AngleSpeed = 0.25;

    // Muliply with the sensitivity for the angle speed
    AngleSpeed *= sensitivity;

    // Add angle to the player
    if (xAngleToAdd != 0)
    {
        localPlayer->Angle += AngleSpeed * xAngleToAdd;
        SetNeedUpdateViewRotation(true);
        SetSendPosition(true);
    }

    // Add angle to the camera
    if (yAngleToAdd != 0)
    {
        float NewyAngle = GetCameraAngleY() + AngleSpeed * yAngleToAdd;
        if (NewyAngle > 10 && NewyAngle < 246)
        {
            SetCameraAngleY(NewyAngle);
            SetNeedUpdateViewRotation(true);
            SetSendPosition(true);
        }
    }
}

/**
 * @brief Rotate player with physicals key
 *
 * @param NeedUpdateViewRotation
 * @param SendPosition
 * @param CameraAngleY
 * @param CurrentScopeLevel
 */
void RotatePlayer(bool *NeedUpdateViewRotation, bool *SendPosition, float *CameraAngleY)
{
    float AngleSpeed = 2;

    // If the player is scoping, the angle speed is lower
    if (GetCurrentScopeLevel() == 1)
        AngleSpeed *= 0.5;
    else if (GetCurrentScopeLevel() == 2)
        AngleSpeed *= 0.25;

    // Change player rotation
    if (isKey(LOOK_RIGHT_BUTTON))
    {
        localPlayer->Angle -= AngleSpeed;
        *NeedUpdateViewRotation = true;
        *SendPosition = true;
    }
    else if (isKey(LOOK_LEFT_BUTTON))
    {
        localPlayer->Angle += AngleSpeed;
        *NeedUpdateViewRotation = true;
        *SendPosition = true;
    }

    // Change camera rotation
    if (isKey(LOOK_UP_BUTTON) && *CameraAngleY > 9)
    {
        *CameraAngleY -= AngleSpeed;
        *NeedUpdateViewRotation = true;
    }
    else if (isKey(LOOK_DOWN_BUTTON) && *CameraAngleY < 245)
    {
        *CameraAngleY += AngleSpeed;
        *NeedUpdateViewRotation = true;
    }
}

int updateRate = 0;

/**
 * @brief Force to update the view rotation
 *
 * @param CameraAngleY Camera angle
 */
void ForceUpdateLookRotation(float CameraAngleY)
{
    updateRate = 0;
    UpdateLookRotation(CameraAngleY);
}

void stepSound(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];

    if (playerIndex == 0) // do a 2d sound
    {
        DoStepSound(110, 128, 0);
    }
    else // Do a 3d sound
    {
        int Panning, Volume;
        GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.11);
        DoStepSound(Volume, Panning, playerIndex);
    }
}

/**
 * @brief Apply animation when the player is moving
 *
 * @param playerIndex Player index
 */
void ApplyGunWalkAnimation(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];

    player->BobbingOffset += BobbingSpeed;

    // Do sound every half bobbing cycle
    if (!player->HasBobbed && player->BobbingOffset >= M_PI)
    {
        stepSound(playerIndex);

        player->HasBobbed = true;
    }
    else if (player->BobbingOffset >= M_TWOPI)
    {
        player->BobbingOffset = 0;
        stepSound(playerIndex);

        player->HasBobbed = false;
    }
}

/**
 * @brief Update look values for local player
 *
 * @param CameraAngleY Camera angle
 */
void UpdateLookRotation(float CameraAngleY)
{
    // Math formula to get a point position on sphere from the middle of the sphere with 2 angle
    float TempS = AllPlayers[GetCurrentCameraPlayer()].Angle / 512.0 * M_TWOPI;
    float TempT = (384 - CameraAngleY) / 512.0 * M_TWOPI;

    double cosTempT = cos(TempT);
    double cosTempS = cos(TempS);
    double SinTempS = sin(TempS);

    x = SinTempS * cosTempT;
    y = -sin(TempT);
    z = cosTempS * cosTempT;

    xWithoutY = -SinTempS;
    zWithoutY = -cosTempS;

    // Values for map
    if (isShowingMap)
    {
        float TempSForMap = (AllPlayers[GetCurrentCameraPlayer()].Angle) / 512.0 * M_TWOPI;
        xWithoutYForMap = -sin(TempSForMap);
        zWithoutYForMap = -cos(TempSForMap);
    }

    // Values for audio and occlusions
    if (updateRate == 0)
    {
        float TempSside1 = (AllPlayers[GetCurrentCameraPlayer()].Angle - 80) / 512.0 * M_TWOPI;
        float TempSside2 = (AllPlayers[GetCurrentCameraPlayer()].Angle + 80) / 512.0 * M_TWOPI;

        float TempSForAudio = (AllPlayers[GetCurrentCameraPlayer()].Angle - 128) / 512.0 * M_TWOPI;

        xWithoutYForAudio = -sin(TempSForAudio);
        zWithoutYForAudio = -cos(TempSForAudio);

        xWithoutYForOcclusionSide1 = -sin(TempSside1);
        zWithoutYForOcclusionSide1 = -cos(TempSside1);
        xWithoutYForOcclusionSide2 = -sin(TempSside2);
        zWithoutYForOcclusionSide2 = -cos(TempSside2);

        updateRate = 2;
    }
    updateRate--;
}

/**
 * @brief  Update look values for non local player
 *
 * @param CameraAngleY Camera angle
 * @param angle Angle of the player
 * @param x out x value
 * @param y out y value
 * @param z out z value
 */
void UpdateLookRotationAI(float CameraAngleY, float angle, float *x, float *y, float *z)
{
    // Math formula to get a point position on sphere from the middle of the sphere with 2 angle
    float TempS = angle / 512.0 * M_TWOPI;
    float TempT = (384 - CameraAngleY) / 512.0 * M_TWOPI;

    double cosTempT = cos(TempT);
    double cosTempS = cos(TempS);
    double SinTempS = sin(TempS);

    *x = SinTempS * cosTempT;
    *y = -sin(TempT);
    *z = cosTempS * cosTempT;
}

/**
 * @brief Update look values FOR culling for non local player
 *
 * @param playerIndex Player index
 * @param angle Angle of the player
 * @param xSide1 out x value for side 1
 * @param zSide1 out z value for side 1
 * @param xSide2 out x value for side 2
 * @param zSide2 out z value for side 2
 */
void GetRotationForCullingAI(int playerIndex, float angle, float *xSide1, float *zSide1, float *xSide2, float *zSide2)
{
    float TempSside1 = (AllPlayers[playerIndex].Angle - 100) / 512.0 * M_TWOPI;
    float TempSside2 = (AllPlayers[playerIndex].Angle + 100) / 512.0 * M_TWOPI;

    *xSide1 = -sin(TempSside1);
    *zSide1 = -cos(TempSside1);

    *xSide2 = -sin(TempSside2);
    *zSide2 = -cos(TempSside2);
}

float Speed = 0.05;
/**
 * @brief Normalize vector3
 *
 * @param p Vector3
 */
void normalize(Vector3 *p)
{
    float w = sqrtf(p->x * p->x + p->y * p->y + p->z * p->z);
    p->x /= w;
    p->y /= w;
    p->z /= w;
}

/**
 * @brief Normalize vector2 2D
 *
 * @param p Vector2
 */
void normalize2D(Vector2 *p)
{
    float w = sqrtf(p->x * p->x + p->y * p->y);
    p->x /= w;
    p->y /= w;
}

/**
 * @brief Normalize vector2 1D
 *
 * @param p Vector2
 */
void normalize1D(Vector2 *p)
{
    float w = sqrtf(p->x * p->x);
    p->x /= w;
}

/**
 * @brief Set non local player position
 *
 */
void SetOnlinelPlayersPositions()
{
    // Loop using "AllPlayers" array for updating non local player position smoothly
    for (int i = 1; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];

        if (player->Id == UNUSED || player->IsDead)
            continue;

        // If a player is spawned
        if (!player->isAi)
        {
            float Speed = 0.1;
            float Dis = fabs(player->position.x - player->lerpDestination.x) + fabs(player->position.y - player->lerpDestination.y) + fabs(player->position.z - player->lerpDestination.z);

            if (Dis < 4.0)
            {
                // Smoothing player position
                if (player->position.x != player->lerpDestination.x)                                                    // If player x position is not the same as x destination
                    player->position.x = player->position.x + Speed * (player->lerpDestination.x - player->position.x); // Move player with lerp system

                if (player->position.y != player->lerpDestination.y)                                                    // If player y position is not the same as y destination
                    player->position.y = player->position.y + Speed * (player->lerpDestination.y - player->position.y); // Move player with lerp system

                if (player->position.z != player->lerpDestination.z)                                                    // If player y position is not the same as z destination
                    player->position.z = player->position.z + Speed * (player->lerpDestination.z - player->position.z); // Move player with lerp system

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

                if (Dis > 0.05)
                    ApplyGunWalkAnimation(i);
            }
            else
            {
                player->position.x = player->lerpDestination.x;
                player->position.y = player->lerpDestination.y;
                player->position.z = player->lerpDestination.z;

                player->Angle = player->AngleDestination;
            }

            // Set new player position and angle
            NE_ModelSetCoord(player->PlayerModel, player->position.x, player->position.y, player->position.z);
            NE_ModelSetCoord(player->PlayerShadow, player->position.x, player->position.y - 0.845, player->position.z);
            player->PlayerModel->ry = player->Angle;
        }
        else if (player->PathCount != 0 && (player->target == NO_PLAYER || player->tooFar) && roundState != WAIT_START)
        {
            float Speed = 0.087;
            int CurPath = player->CurrentPath;
            int Ok = 0;

            Vector3 Direction;
            Direction.x = GetWaypoints()[player->Path[CurPath]].x - player->position.x;
            Direction.y = GetWaypoints()[player->Path[CurPath]].y - 0.1 - player->position.y;
            Direction.z = GetWaypoints()[player->Path[CurPath]].z - player->position.z;
            Vector2 Direction2D;
            Direction2D.x = GetWaypoints()[player->Path[CurPath]].x - GetWaypoints()[player->Path[CurPath - 1]].x;
            Direction2D.y = GetWaypoints()[player->Path[CurPath]].z - GetWaypoints()[player->Path[CurPath - 1]].z;
            normalize(&Direction);
            normalize2D(&Direction2D);

            // Smoothing player position
            if (player->position.x - 0.1 > GetWaypoints()[player->Path[CurPath]].x || player->position.x + 0.1 < GetWaypoints()[player->Path[CurPath]].x)
                player->position.x += Direction.x * Speed;
            else
                Ok++;

            // TODO remplace -0.1 by a player offset vector 3
            if (player->position.y - 0.1 > GetWaypoints()[player->Path[CurPath]].y - 0.1 || player->position.y + 0.1 < GetWaypoints()[player->Path[CurPath]].y - 0.1)
                player->position.y += Direction.y * Speed;

            if (player->position.z - 0.1 > GetWaypoints()[player->Path[CurPath]].z || player->position.z + 0.1 < GetWaypoints()[player->Path[CurPath]].z)
                player->position.z += Direction.z * Speed;
            else
                Ok++;

            if (Ok == 2) // if the player reach the waypoint
            {
                if (CurPath < player->PathCount - 1)
                {
                    // Go to next waypoint
                    player->CurrentPath++;
                }
                else
                {
                    // if the player is at the end of the path
                    if (applyRules)
                    {
                        player->LastWayPoint = player->Path[CurPath];
                        // Plant/defuse the bomb if needed
                        if (player->haveBomb || (player->Team == COUNTERTERRORISTS && BombPlanted && !BombDefused && bombPlantedAt == player->LastWayPoint))
                        {
                            if (player->LastWayPoint == 14 || player->LastWayPoint == 29) // TOTO Replace 14 and 29 by bomb point define
                            {
                                player->isPlantingBomb = true;
                                if (!BombDefused && BombPlanted) // Set timer
                                    player->bombTimer = bombDefuseTime;
                                else if (!BombPlanted) // Set timer
                                    player->bombTimer = bombPlantingTime;
                            }
                        }
                    }
                    player->PathCount = 0;
                }
            }
            else
            {
                // Move player
                ApplyGunWalkAnimation(i);
                NE_ModelSetCoord(player->PlayerModel, player->position.x, player->position.y, player->position.z);
                NE_ModelSetCoord(player->PlayerShadow, player->position.x, player->position.y - 0.845, player->position.z);

                float FinalAngle = atan2f(Direction2D.x, Direction2D.y) * 512.0 / (M_TWOPI) + 256.0;
                player->Angle = FinalAngle;
                player->PlayerModel->ry = player->Angle;
            }
        }
        else //?
        {
            Vector3 positionToGo;

            if (player->tooFar)
            {
                Player *targetPlayer = &AllPlayers[player->target];
                positionToGo.x = targetPlayer->position.x;
                positionToGo.y = targetPlayer->position.y;
                positionToGo.z = targetPlayer->position.z;
            }
            else if (player->searchForDroppedBomb)
            {
                positionToGo.x = droppedBombPositionAndRotation.x;
                positionToGo.y = droppedBombPositionAndRotation.y + 0.845;
                positionToGo.z = droppedBombPositionAndRotation.z;
            }
            else
            {
                player->Angle = player->AngleDestination;
                player->PlayerModel->ry = player->AngleDestination;
                continue;
            }

            float Speed = 0.087;
            int Ok = 0;

            Vector3 Direction;
            Direction.x = positionToGo.x - player->position.x;
            Direction.y = positionToGo.y - player->position.y;
            Direction.z = positionToGo.z - player->position.z;

            normalize(&Direction);

            // Smoothing player position
            if (player->position.x - 0.7 > positionToGo.x || player->position.x + 0.7 < positionToGo.x)
                player->position.x += Direction.x * Speed;
            else
                Ok++;

            if (player->position.y - 0.1 > positionToGo.y || player->position.y + 0.1 < positionToGo.y)
                player->position.y += Direction.y * Speed;

            if (player->position.z - 0.7 > positionToGo.z || player->position.z + 0.7 < positionToGo.z)
                player->position.z += Direction.z * Speed;
            else
                Ok++;

            NE_ModelSetCoord(player->PlayerModel, player->position.x, player->position.y, player->position.z);
            NE_ModelSetCoord(player->PlayerShadow, player->position.x, player->position.y - 0.845, player->position.z);

            player->Angle = player->AngleDestination;
            player->PlayerModel->ry = player->AngleDestination;
        }
    }
}