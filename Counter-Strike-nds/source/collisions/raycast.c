// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)
#include "raycast.h"
#include "collisions.h"
#include "network.h"
#include "ai.h"
#include "gun.h"
#include "sounds.h"
#include "movements.h"
#include "map.h"
#include "party.h"
#include "player.h"

/**
 * @brief Prepare a raycast for Ai, store values for the incomming raycast call
 *
 * @param fromPlayerIndex player index who is making the raycast
 * @param toPlayerIndex player index to shoot
 * @param checkVisibility true : just check if player to shoot is visible, false : apply damage to player if he is visible
 */
void prepareAiRaycast(int fromPlayerIndex, int toPlayerIndex, bool checkVisibility)
{
    Player *shooterPlayer = &AllPlayers[fromPlayerIndex];
    Player *targetPlayer = &AllPlayers[toPlayerIndex];

    // Get distance between shooter and target players
    float distance2D = sqrtf(powf(targetPlayer->PlayerModel->x - shooterPlayer->PlayerModel->x, 2.0) + powf(targetPlayer->PlayerModel->z - shooterPlayer->PlayerModel->z, 2.0));
    // Get shoot direction
    Vector3 Direction;
    Direction.x = targetPlayer->PlayerModel->x - shooterPlayer->PlayerModel->x;
    Direction.y = targetPlayer->PlayerModel->y - (shooterPlayer->PlayerModel->y + CameraOffsetYMultiplied);
    Direction.z = targetPlayer->PlayerModel->z - shooterPlayer->PlayerModel->z;

    float tempAngle = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0;
    float CameraAngleToGrenadeDirection = atan2f(distance2D, Direction.y) * 512.0 / (M_TWOPI); // Change var name

    float x2, y2, z2;
    // Is the gun is a shotgun, reduce accuracy
    if (getPlayerCurrentGun(shooterPlayer).bulletCountPerShoot != 1 && !checkVisibility)
    {
        float xOffset = ((rand() % 100) - 50) / 3.0;
        float yOffset2 = ((rand() % 100) - 50) / 3.0;
        UpdateLookRotationAI(CameraAngleToGrenadeDirection + yOffset2, tempAngle + xOffset, &x2, &y2, &z2);
    }
    else
    {
        UpdateLookRotationAI(CameraAngleToGrenadeDirection, tempAngle, &x2, &y2, &z2);
    }

    if (!checkVisibility)
    {
        // Reduce accuracy
        // Lower accuracy if target is moving
        // And lower accuracy if shooter is flahsed
        int coef = 1;
        if (shooterPlayer->flashed)
            coef = 3;

        if ((targetPlayer->isAi && targetPlayer->target != NO_PLAYER) || (!targetPlayer->isAi && targetPlayer->PlayerPhysic->xspeed + targetPlayer->PlayerPhysic->yspeed + targetPlayer->PlayerPhysic->zspeed == 0))
        {
            // Better accuracy if target is not moving
            x2 += ((rand() % 150) - 75) / 1300.0 * coef * (1 - getPlayerCurrentGun(shooterPlayer).inaccuracyReductionForBot);
            y2 += ((rand() % 220) - 110) / 1300.0 * coef * (1 - getPlayerCurrentGun(shooterPlayer).inaccuracyReductionForBot);
            z2 += ((rand() % 150) - 75) / 1300.0 * coef * (1 - getPlayerCurrentGun(shooterPlayer).inaccuracyReductionForBot);
        }
        else
        {
            x2 += ((rand() % 150) - 75) / 700.0 * coef * (1 - getPlayerCurrentGun(shooterPlayer).inaccuracyReductionForBot);
            y2 += ((rand() % 220) - 110) / 700.0 * coef * (1 - getPlayerCurrentGun(shooterPlayer).inaccuracyReductionForBot);
            z2 += ((rand() % 150) - 75) / 700.0 * coef * (1 - getPlayerCurrentGun(shooterPlayer).inaccuracyReductionForBot);
        }
    }

    CalculatePlayerPosition(fromPlayerIndex);
    setRaycastValues(shooterPlayer, x2, y2, z2);

    shooterPlayer->justCheking = checkVisibility;
}

/**
 * @brief Get camera angles value to do a raycast between the start position and the end position
 *
 * @param StartPosition Start position of the raycast
 * @param EndPosition End position of the raycast
 * @param x out : x angle
 * @param y out : y angle
 * @param z out : z angle
 */
void getValuesForRaycast(Vector3Int StartPosition, Vector3Int EndPosition, float *x, float *y, float *z)
{
    // Get distance between shooter and target players
    float distance2D = sqrtf(powf(EndPosition.x - StartPosition.x, 2.0) + powf(EndPosition.z - StartPosition.z, 2.0));
    // Get shoot direction
    Vector3 Direction;
    Direction.x = EndPosition.x - StartPosition.x;
    Direction.y = EndPosition.y - StartPosition.y;
    Direction.z = EndPosition.z - StartPosition.z;

    float tempAngle = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0; // TODO Change var name
    float CameraAngleDirection = atan2f(distance2D, Direction.y) * 512.0 / (M_TWOPI);

    UpdateLookRotationAI(CameraAngleDirection, tempAngle, x, y, z);
}

/**
 * @brief Set raycast values
 *
 * @param shooterPlayer
 * @param xRot x rotation
 * @param yRot y rotation
 * @param zRot z rotation
 */
void setRaycastValues(Player *shooterPlayer, float xRot, float yRot, float zRot)
{
    shooterPlayer->startRaycastPosition.x = shooterPlayer->position.x;
    shooterPlayer->startRaycastPosition.y = shooterPlayer->position.y;
    shooterPlayer->startRaycastPosition.z = shooterPlayer->position.z;

    shooterPlayer->startRaycastRotation.x = xRot;
    shooterPlayer->startRaycastRotation.y = yRot;
    shooterPlayer->startRaycastRotation.z = zRot;
    shooterPlayer->startGunIdRaycast = getPlayerCurrentGunIndex(shooterPlayer);
}

/**
 * @brief Make a raycast from a shooter player
 *
 * @param playerIndex index of the shooter
 * @param distance distance of the hit (output)
 * @return int Index of player hitted
 */
int Raycast(int playerIndex, int currentShootIndex, float *distance)
{
    Player *shooterPlayer = &AllPlayers[playerIndex];

    shooterPlayer->IsHeadShot[currentShootIndex] = false;
    shooterPlayer->IsLegShot[currentShootIndex] = false;
    int HitPlayerIndex = NO_PLAYER;

    // Get an array of all walls to test
    int WallCountToTest = 0;
    int AllWallsToCheck[wallCount];
    for (int i2 = 0; i2 < allMaps[currentMap].CollisionsCount; i2++)
    {
        // If the wall raycast is not affecting all zones:
        if (getMapWalls()[i2].ZoneCollision != -1)
        {
            for (int i3 = 0; i3 < allMaps[currentMap].AllZones[shooterPlayer->CurrentOcclusionZone].ZoneCount; i3++)
            {
                // If the wall is in the visible zone
                if (allMaps[currentMap].AllZones[shooterPlayer->CurrentOcclusionZone].visibleMapPart[i3] == getMapWalls()[i2].ZoneCollision)
                {
                    // Add the wall to the array
                    AllWallsToCheck[WallCountToTest] = i2;
                    WallCountToTest++;
                    break;
                }
            }
        }
        else
        {
            // Add the wall to the array
            AllWallsToCheck[WallCountToTest] = i2;
            WallCountToTest++;
        }
    }

    // Check who is the nearest object, a wall or a player?

    float t = 99999 * 8192;
    float tPlayer = 99999 * 10000;
    int nearestWallIndex = NO_PLAYER;
    int nearestPlayerIndex = NO_PLAYER;

    // Get shoot direction
    Vector3 dirfrac;
    dirfrac.x = 1.0f / shooterPlayer->startRaycastRotation.x;
    dirfrac.y = 1.0f / shooterPlayer->startRaycastRotation.y;
    dirfrac.z = 1.0f / shooterPlayer->startRaycastRotation.z;

    // Get int version * 2 of the start position for wall checking (I have no idea why I need to multiply this by 2 lol (8192 = 4096*2))
    Vector3Int startPosition;
    startPosition.x = shooterPlayer->startRaycastPosition.x * 8192;
    startPosition.y = (shooterPlayer->startRaycastPosition.y + CameraOffsetY) * 8192;
    startPosition.z = shooterPlayer->startRaycastPosition.z * 8192;

    for (int wallIndex = 0; wallIndex < WallCountToTest; wallIndex++)
    {
        // Get both opposite corners of the wall
        Vector3Int corner1;
        Vector3Int corner2;

        int WallIndex = AllWallsToCheck[wallIndex];
        corner1.x = getMapWalls()[WallIndex].WallCollisionBox.BoxXRangeA;
        corner1.y = getMapWalls()[WallIndex].WallCollisionBox.BoxYRangeA;
        corner1.z = getMapWalls()[WallIndex].WallCollisionBox.BoxZRangeA;

        corner2.x = getMapWalls()[WallIndex].WallCollisionBox.BoxXRangeB;
        corner2.y = getMapWalls()[WallIndex].WallCollisionBox.BoxYRangeB;
        corner2.z = getMapWalls()[WallIndex].WallCollisionBox.BoxZRangeB;
        // check if this hitted wall is nearest than the current found nearest wall
        float newWallDistance = t;
        if (getHitDistance(corner1, corner2, dirfrac, startPosition, &newWallDistance))
        {
            if (newWallDistance < t)
            {
                t = newWallDistance;
                nearestWallIndex = WallIndex;
            }
        }
    }

    // ScanForGrenade

    // Get int version of the start position for player checking
    startPosition.x = shooterPlayer->startRaycastPosition.x * 4096;
    startPosition.y = (shooterPlayer->startRaycastPosition.y + CameraOffsetY) * 4096;
    startPosition.z = shooterPlayer->startRaycastPosition.z * 4096;
    if (shooterPlayer->ScanForGrenade != EMPTY)
    {
        if (grenades[shooterPlayer->ScanForGrenade] != NULL)
        {
            // debugValue3 = 1;
            Vector3 grenadePos;
            grenadePos.x = grenades[shooterPlayer->ScanForGrenade]->Model->x;
            grenadePos.y = grenades[shooterPlayer->ScanForGrenade]->Model->y;
            grenadePos.z = grenades[shooterPlayer->ScanForGrenade]->Model->z;
            Vector3 grenadeSize;
            grenadeSize.x = grenades[shooterPlayer->ScanForGrenade]->Physic->xsize;
            grenadeSize.y = grenades[shooterPlayer->ScanForGrenade]->Physic->ysize;
            grenadeSize.z = grenades[shooterPlayer->ScanForGrenade]->Physic->zsize;

            CollisionBox box = CalculateColBox(grenadePos, grenadeSize);

            //  Get both opposite corners of the wall
            Vector3Int corner1;
            Vector3Int corner2;

            corner1.x = box.BoxXRangeA;
            corner1.y = box.BoxYRangeA;
            corner1.z = box.BoxZRangeA;

            corner2.x = box.BoxXRangeB;
            corner2.y = box.BoxYRangeB;
            corner2.z = box.BoxZRangeB;

            // check if this hitted wall is nearest than the current found nearest wall
            float newPlayerDistance = tPlayer;
            if (getHitDistance(corner1, corner2, dirfrac, startPosition, &newPlayerDistance))
            {
                if (newPlayerDistance < tPlayer)
                {
                    tPlayer = newPlayerDistance;
                    // nearestWallIndex = NO_PLAYER;
                    nearestPlayerIndex = shooterPlayer->ScanForGrenade;
                }
            }
        }
    }
    else
    {
        for (int PlayerIndex = 0; PlayerIndex < MaxPlayer; PlayerIndex++)
        {
            Player *testedPlayer = &AllPlayers[PlayerIndex];

            if (testedPlayer->Id != UNUSED && PlayerIndex != playerIndex && !testedPlayer->IsDead)
            {
                // Calculate for each player the collision box
                CalculatePlayerColBox(PlayerIndex);

                // Get both opposite corners of the player
                Vector3Int corner1;
                Vector3Int corner2;
                corner1.x = testedPlayer->PlayerCollisionBox.BoxXRangeA;
                corner1.y = testedPlayer->PlayerCollisionBox.BoxYRangeA;
                corner1.z = testedPlayer->PlayerCollisionBox.BoxZRangeA;

                corner2.x = testedPlayer->PlayerCollisionBox.BoxXRangeB;
                corner2.y = testedPlayer->PlayerCollisionBox.BoxYRangeB;
                corner2.z = testedPlayer->PlayerCollisionBox.BoxZRangeB;

                float newPlayerDistance = tPlayer;
                // check if this hitted player is nearest than the current found nearest player
                if (getHitDistance(corner1, corner2, dirfrac, startPosition, &newPlayerDistance))
                {
                    // If the found player is an AI in the same team and the player is too near
                    if (shooterPlayer->isAi && shooterPlayer->Team == testedPlayer->Team && newPlayerDistance < 1)
                    {
                        continue;
                    }
                    if (newPlayerDistance < tPlayer)
                    {
                        tPlayer = newPlayerDistance;
                        nearestPlayerIndex = PlayerIndex;
                    }
                }
            }
        }
    }

    if (tPlayer * 2.0 < t)
    {
        t = tPlayer * 2.0;
    }
    else
    {
        nearestPlayerIndex = NO_PLAYER;
    }

    if (shooterPlayer->ScanForGrenade == EMPTY && (nearestPlayerIndex != NO_PLAYER || nearestWallIndex != NO_PLAYER))
    {
        // Turn distance into float distance scale
        t /= 8192.0;
        t -= 0.2;
        *distance = t * 50;
        if (!shooterPlayer->justCheking)
        {
            //  Create wall shot flash position
            Vector3 hitPosition;
            hitPosition.x = shooterPlayer->startRaycastPosition.x + shooterPlayer->startRaycastRotation.x * t;
            hitPosition.y = shooterPlayer->startRaycastPosition.y + CameraOffsetY + shooterPlayer->startRaycastRotation.y * t;
            hitPosition.z = shooterPlayer->startRaycastPosition.z + shooterPlayer->startRaycastRotation.z * t;

            if (!AllGuns[shooterPlayer->startGunIdRaycast].isKnife)
            {
                // Check a free wall shot flash slot
                int flashIndex = 0;
                for (int i = 0; i < FLASH_MODELS_COUNT; i++)
                {
                    if (ShowWallHitFlash[i] == 0)
                    {
                        flashIndex = i;
                        break;
                    }
                }

                ShowWallHitFlash[flashIndex] = 3;

                // Create wall hit flash rotation
                Vector2 Direction1D;
                Direction1D.y = hitPosition.y - shooterPlayer->position.y - CameraOffsetY + y;
                Direction1D.x = 1;
                normalize2D(&Direction1D);

                Vector3 Direction;
                Direction.x = hitPosition.x - shooterPlayer->position.x;
                Direction.y = hitPosition.y - shooterPlayer->position.y;
                Direction.z = hitPosition.z - shooterPlayer->position.z;
                normalize(&Direction);

                // Set wall hit flash angles
                int FinalAngleY = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 384;
                int FinalAngleZ = atan2f(Direction1D.y, 1) * 512.0 / (M_TWOPI) + 128;
                NE_ModelSetRot(flashModels[flashIndex], 0, FinalAngleY, FinalAngleZ);

                //   Set wall hot flash position
                NE_ModelSetCoord(flashModels[flashIndex], hitPosition.x, hitPosition.y, hitPosition.z);

                if (!applyRules)
                {
                    SendWallHit = true;
                    WallHitXPos[currentShootIndex] = hitPosition.x * 4096;
                    WallHitYPos[currentShootIndex] = hitPosition.y * 4096;
                    WallHitZPos[currentShootIndex] = hitPosition.z * 4096;
                }
            }
            else
            {
                PlayBasicSound(AllGuns[shooterPlayer->startGunIdRaycast].gunSound);
            }

            if (nearestPlayerIndex != NO_PLAYER && (!AllGuns[shooterPlayer->startGunIdRaycast].isKnife || t <= 1))
            {

                if (AllPlayers[nearestPlayerIndex].Team != shooterPlayer->Team || allPartyModes[currentPartyMode].teamDamage)
                    HitPlayerIndex = nearestPlayerIndex;

                AllPlayers[nearestPlayerIndex].mapVisivilityTimer = 300;

                // Check if the hit is a headshot
                if (hitPosition.y - AllPlayers[nearestPlayerIndex].position.y >= 0.64)
                {
                    shooterPlayer->IsHeadShot[currentShootIndex] = true;
                }
                else if (hitPosition.y - AllPlayers[nearestPlayerIndex].position.y <= 0.2) // Check if the hit is a legshot
                {
                    shooterPlayer->IsLegShot[currentShootIndex] = true;
                }
            }
        }
        else
        {
            if (nearestPlayerIndex != NO_PLAYER)
            {
                if (AllPlayers[nearestPlayerIndex].Team != shooterPlayer->Team)
                    HitPlayerIndex = nearestPlayerIndex;

                if (AllGuns[shooterPlayer->startGunIdRaycast].isKnife)
                {
                    // If the target is too far
                    if (t <= 1)
                    {
                        // Walk to the target
                        shooterPlayer->tooFar = true;
                        Player *targetPlayer = &AllPlayers[nearestPlayerIndex];
                        int nearestWaypoint = getNearestWaypoint(targetPlayer->position.x, targetPlayer->position.y, targetPlayer->position.z);
                        StartChecking(playerIndex, nearestWaypoint);
                    }
                    else
                        shooterPlayer->tooFar = false;
                }
            }
        }
    }
    else if (shooterPlayer->ScanForGrenade != EMPTY)
    {
        HitPlayerIndex = nearestPlayerIndex;
    }

    return HitPlayerIndex;
}