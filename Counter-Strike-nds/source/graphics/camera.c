// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "camera.h"
#include "party.h"
#include "map.h"

// Speed of the camera to recenter the camera after using camera offset
int speedAimingReCenter = 3;
int speedAimingReCenterTimer = 10;

/**
 * @brief Check if player can be used for camera view
 *
 * @param left Way (true - left, false - right)
 * @param Found Found player (true - found, false - not found)
 * @param FirstFound First found player index
 * @param i Player index
 */
void checkCameraPlayerView(bool left, bool *Found, int *FirstFound, int i)
{
    Player *player = &AllPlayers[i];
    if (!player->IsDead && player->Id != UNUSED && (allPartyModes[currentPartyMode].canSeeOtherTeamView || player->Team == localPlayer->Team))
    {
        if (*FirstFound == NO_PLAYER)
            *FirstFound = i;

        if ((i < CurrentCameraPlayer && left) || (!left && i > CurrentCameraPlayer))
        {
            SetCurrentCameraPlayer(i);
            ShowMuzzle = 0;
            *Found = true;
        }
    }
}

/**
 * @brief Add screen shake for explosion
 */
void addExplosionScreenShake()
{
    if (IsExplode)
    {
        BombExplosionScale++;
        int Scale = 4096 * BombExplosionScale;
        NE_ModelScaleI(Model[10], Scale, Scale, Scale);

        xOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
        if (rand() % 2 == 0)
            xOffset = -xOffset;

        yOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
        if (rand() % 2 == 0)
            yOffset = -yOffset;

        speedAimingReCenter = 2;
        speedAimingReCenterTimer = 8;
    }
}

/**
 * @brief Reduce screen shake
 *
 */
void reduceScreenShake()
{
    // Screen shake
    if (xOffset >= 0.005)
        xOffset -= 0.005;
    else if (xOffset < -0.005)
        xOffset += 0.005;

    // Reduce yOffset with more affinity
    for (int i = 0; i < speedAimingReCenter; i++)
    {
        if (yOffset >= 0.0025)
            yOffset -= 0.0025;
        else if (yOffset < -0.0025)
            yOffset += 0.0025;
    }

    // Increase the speed reduction of the screen shake
    if (speedAimingReCenterTimer > 0)
    {
        speedAimingReCenterTimer--;
        if (speedAimingReCenterTimer == 0)
        {
            speedAimingReCenterTimer = 8;
            if (speedAimingReCenter < 5)
                speedAimingReCenter++;
        }
    }
}

/**
 * @brief Set camera position to look at the map
 *
 */
void setCameraMapPosition()
{
    Vector3 *pos = &allMaps[currentMap].cameraPosition;
    Vector3 *lookPos = &allMaps[currentMap].cameraLookPosition;
    NE_CameraSet(Camera,
                 pos->x, pos->y, pos->z,
                 lookPos->x, lookPos->y, lookPos->z,
                 0, 1, 0);
}

/**
 * @brief Change player's camera view
 *
 * @param left Way (true - left, false - right)
 */
void changeCameraPlayerView(bool left)
{
    bool Found = false;
    int FirstFound = NO_PLAYER;

    // Find first player available for view
    if (left)
    {
        for (int i = MaxPlayer - 1; i > 0; i--)
        {
            checkCameraPlayerView(left, &Found, &FirstFound, i);
            if (Found)
                break;
        }
    }
    else
    {
        for (int i = 1; i < MaxPlayer; i++)
        {
            checkCameraPlayerView(left, &Found, &FirstFound, i);
            if (Found)
                break;
        }
    }

    // Set new camera view
    if (!Found && FirstFound != NO_PLAYER)
    {
        if (CurrentCameraPlayer != FirstFound)
        {
            ShowMuzzle = 0;
            SetCurrentCameraPlayer(FirstFound);
        }
    }
}