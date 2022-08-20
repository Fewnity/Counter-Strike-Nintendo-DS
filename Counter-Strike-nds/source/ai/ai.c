// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "ai.h"
#include "raycast.h"
#include "data/ai_data.h"
#include "gun.h"
#include "party.h"
#include "map.h"
#include "sounds.h"
#include "movements.h"
#include "equipment.h"
#include "collisions.h"
#include "grenade.h"
#include "player.h"

// Bots names list
const char *botsNames[BOTS_NAMES_COUNT];

// Avoid multiple bot with the same name
bool botsNamesTaken[BOTS_NAMES_COUNT];

// Matrices for path finding
PathLength *AllMatricesLength;

// Current matrices size for the current map (MatricesSize * MatricesSize)
int MatricesSize = 0;
// Waypoint count for the current map
int waypointsSize = 0;
// Matrices count for the current map
int MatriceCount = 0;

// Timer to check the player's distance between the bot and the player
int checkPlayerDistanceFromAiTimer = 1;
Waypoint Waypoints[maxPoint]; // TODO replace this by a malloc

/**
 * @brief Reset botsNamesTaken list
 */
void ResetTakenBotsNames()
{
    for (int nameIndex = 0; nameIndex < BOTS_NAMES_COUNT; nameIndex++)
    {
        botsNamesTaken[nameIndex] = false;
    }
}

/**
 * @brief Copy one matrix to another matrix array
 *
 * @param size Size of the matrix
 * @param matrix Matrix to copy
 * @param index Index of the destination matrix
 */
void copyArrayToAllMatricesLength(int size, int matrix[size][size], int index)
{
    // Alloc memory for the matrix (create a list of int*)
    AllMatricesLength[index].matrixOneLength = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        // Alloc memory for the matrix (create a list of int)
        AllMatricesLength[index].matrixOneLength[i] = malloc(size * sizeof(int));
        memcpy(AllMatricesLength[index].matrixOneLength[i], matrix[i], sizeof(int) * size);
    }
}

/**
 * @brief Free all matrices from memory
 *
 * @param size Size of matrices
 */
void freeAllMatricesLength(int size)
{
    for (int i = 0; i < MatriceCount; i++)
    {
        if (AllMatricesLength[i].matrixOneLength)
        {
            for (int i2 = 0; i2 < size; i2++)
            {
                free(AllMatricesLength[i].matrixOneLength[i2]);
            }
            free(AllMatricesLength[i].matrixOneLength);
        }
    }
}

/**
 * @brief Get the Waypoints list
 *
 * @return Waypoint*
 */
Waypoint *GetWaypoints()
{
    return Waypoints;
}

/**
 * @brief Get the Nearest Waypoint object
 *
 * @param x x Position (float version)
 * @param y y position (float version)
 * @param z z position (float version)
 * @return * Get id of the nearest waypoint
 */
int getNearestWaypoint(float x, float y, float z)
{
    int nearestWaypointId = -1;
    int distance = 99999;
    // Check every waypoint
    for (int waypointIndex = 0; waypointIndex < waypointsSize; waypointIndex++)
    {
        // Calculate the distance between waypoint position and the position given
        int currentDistance = sqrtf(powf((Waypoints[waypointIndex].x - x), 2.0) + powf((Waypoints[waypointIndex].y - y), 2.0) + powf((Waypoints[waypointIndex].z - z), 2.0));
        if (currentDistance < distance)
        {
            nearestWaypointId = waypointIndex;
            distance = currentDistance;
        }
    }
    return nearestWaypointId;
}

/**
 * @brief Get the distance bewteen a player and the waypoint
 *
 * @param player1Index
 * @param waypointIndex
 * @return int distance between the player and the waypoint
 */
int GetDistanceBewteenPlayerAndWaypoint(int playerIndex, int waypointIndex)
{
    Player *player1 = &AllPlayers[playerIndex];
    return sqrtf(powf((player1->position.x - Waypoints[waypointIndex].x), 2.0) + powf((player1->position.y - Waypoints[waypointIndex].y), 2.0) + powf((player1->position.z - Waypoints[waypointIndex].z), 2.0));
}

/**
 * @brief Get the distance bewteen two players
 *
 * @param player1Index
 * @param player2Index
 * @return int distance between players
 */
int GetDistanceBewteenTwoPlayers(int player1Index, int player2Index)
{
    Player *player1 = &AllPlayers[player1Index];
    Player *player2 = &AllPlayers[player2Index];
    return sqrtf(powf((player1->position.x - player2->position.x), 2.0) + powf((player1->position.y - player2->position.y), 2.0) + powf((player1->position.z - player2->position.z), 2.0));
}

/**
 * @brief Create a Waypoint object
 *
 * @param id Waypoint id
 * @param x x Position (float version)
 * @param y y Position (float version)
 * @param z z Position (float version)
 * @param edgeCount Waypoint's edge count
 * @param edge Edge list
 */
void CreateWaypoint(int id, float x, float y, float z, int edgeCount, int *edge)
{
    Waypoints[id].x = x;
    Waypoints[id].y = y;
    Waypoints[id].z = z;
    Waypoints[id].edgeCount = edgeCount;

    Waypoints[id].edge = malloc(edgeCount * sizeof(int));
    memcpy(Waypoints[id].edge, edge, sizeof(int) * edgeCount);
}

/**
 * @brief Free the Waypoints' edges lists from memory
 *
 */
void freeWaypoint()
{
    for (int i = 0; i < waypointsSize; i++)
    {
        // If the edge list is not null
        if (Waypoints[i].edge)
            free(Waypoints[i].edge);
    }
}

/**
 * @brief Start checking for the shortest path between the nearest waypoint and the waypoint given
 *
 * @param playerIndex Player's index to apply the check
 * @param finalWaypointIndex final waypoint index (end of the path)
 */
void StartChecking(int playerIndex, int finalWaypointIndex)
{
    Player *player = &AllPlayers[playerIndex];
    int startWaypoint = getNearestWaypoint(player->position.x, player->position.y, player->position.z);

    // Check if we realy need to check a new path before checking
    if (player->PathCount == 0 || (player->Path[0] != startWaypoint || player->Path[player->PathCount - 1] != finalWaypointIndex))
    {
        CheckPathWaypoint(playerIndex, startWaypoint, finalWaypointIndex);
    }
}

/**
 * @brief Check the shortest path bewteen two waypoints
 *
 * @param playerIndex Player's index to apply the check
 * @param startWaypointIndex final waypoint index (start of the path)
 * @param finalWaypointIndex final waypoint index (end of the path)
 */
void CheckPathWaypoint(int playerIndex, int startWaypointIndex, int finalWaypointIndex)
{
    // Find the path length (ex : pathLength = 4 so the path is composed of 4 waypoints)
    int pathLength = 0;
    for (int matriceIndex = 0; matriceIndex < MatriceCount; matriceIndex++)
    {
        if (AllMatricesLength[matriceIndex].matrixOneLength[startWaypointIndex][finalWaypointIndex] != 0)
        {
            pathLength = matriceIndex;
            break;
        }
    }

    // All waypoints ids are in the path list
    int Path[pathLength + 2];
    int currentWaypoint = startWaypointIndex;
    int PathCount = 0;
    if (startWaypointIndex == finalWaypointIndex)
    {
        Path[0] = currentWaypoint;
        PathCount = 1;
    }
    else if (pathLength == 0) // If the length is 1 (pathLength = path length - 1), path contains the start waypoint and the final waypoint.
    {
        Path[0] = currentWaypoint;
        Path[1] = finalWaypointIndex;
        PathCount = 2;
    }
    else
    {
        // If the path is not finished yet
        while (PathCount != pathLength)
        {
            // Add verified waypoint to the path
            Path[PathCount] = currentWaypoint;
            PathCount++;
            do
            {
                // Take a random connected point from the last tested waypoint
                int NextWaypoint = rand() % Waypoints[currentWaypoint].edgeCount;

                // if the random connected point can reach the final point with (pathLength - Path.Count) movement
                if (AllMatricesLength[pathLength - PathCount].matrixOneLength[Waypoints[currentWaypoint].edge[NextWaypoint]][finalWaypointIndex] != 0)
                {
                    currentWaypoint = Waypoints[currentWaypoint].edge[NextWaypoint];
                    break;
                }
            } while (true);
        }

        // Finalise the path
        Path[PathCount] = currentWaypoint;
        PathCount++;
        Path[PathCount] = finalWaypointIndex;
        PathCount++;

        Player *player = &AllPlayers[playerIndex];
        for (int i = 0; i < PathCount; i++)
        {
            player->Path[i] = Path[i];
        }
        player->PathCount = PathCount;
        player->CurrentPath = 0;
    }
}

/**
 * @brief Call this every frame to check what AI can do
 */
void AiCheckForAction()
{
    // Check every 5 frames the distance between the player and a bot
    checkPlayerDistanceFromAiTimer--;

    if (checkPlayerDistanceFromAiTimer % 5 == 0)
    {
        int currentAiToCheck = checkPlayerDistanceFromAiTimer / 5;
        if (checkPlayerDistanceFromAiTimer == 0)
            checkPlayerDistanceFromAiTimer = 25 + 1;

        Player *playerToCheck = &AllPlayers[currentAiToCheck];
        // If current AI is in game, not dead, does not planting the bomb, and raycast cycle is finished
        if (playerToCheck->isAi && playerToCheck->Id != UNUSED && !playerToCheck->IsDead && !playerToCheck->isPlantingBomb)
        {
            // Check current target distance with current ia
            int randomPlayerToCheck = playerToCheck->target;
            int distancePlayers = 999;

            if (randomPlayerToCheck != NO_PLAYER)
                distancePlayers = GetDistanceBewteenTwoPlayers(randomPlayerToCheck, currentAiToCheck);

            int shootDistance = 20;
            if (playerToCheck->AllGunsInInventory[playerToCheck->currentGunInInventory] < GunCount)
            {
                shootDistance = AllGuns[playerToCheck->AllGunsInInventory[playerToCheck->currentGunInInventory]].maxBotShootDisance;
            }
            // If current AI has no target
            if (playerToCheck->lastSeenTarget == NO_PLAYER && randomPlayerToCheck == NO_PLAYER)
            {
                int scannedPlayerCount = 1;
                playerToCheck->allPlayerScanned[currentAiToCheck] = true;
                // Check every players distances to set a new target if the distance is small enough
                while (scannedPlayerCount < MaxPlayer)
                {
                    // Take a random unscanned player
                    randomPlayerToCheck = rand() % MaxPlayer;
                    if (playerToCheck->allPlayerScanned[randomPlayerToCheck])
                        continue;

                    // Mark current player has scanned to avoid multiple check
                    scannedPlayerCount++;
                    playerToCheck->allPlayerScanned[randomPlayerToCheck] = true;

                    // Check distance if current scanned player is not in spectator team and the same team of the current AI, and if the scanned player is in game
                    if (AllPlayers[randomPlayerToCheck].Team == SPECTATOR || AllPlayers[randomPlayerToCheck].Team == playerToCheck->Team || AllPlayers[randomPlayerToCheck].IsDead || AllPlayers[randomPlayerToCheck].invincibilityTimer > 0 || AllPlayers[randomPlayerToCheck].Id == UNUSED)
                    {
                        continue;
                    }
                    else
                    {
                        distancePlayers = GetDistanceBewteenTwoPlayers(randomPlayerToCheck, currentAiToCheck);
                        if (distancePlayers < shootDistance)
                            break;
                    }
                }
                // Reset scanned players list
                for (int playerIndex = 0; playerIndex < MaxPlayer; playerIndex++)
                {
                    playerToCheck->allPlayerScanned[playerIndex] = false;
                }
            }

            if (distancePlayers < shootDistance) // Is a player is near, set this player as target
            {
                float xSide1, zSide1, xSide2, zSide2;
                GetRotationForCullingAI(currentAiToCheck, playerToCheck->Angle, &xSide1, &zSide1, &xSide2, &zSide2);
                // Field of view end coordinates
                xSide1 = (xSide1 * 500 + playerToCheck->position.x) * 8192.0;
                zSide1 = (zSide1 * 500 + playerToCheck->position.z) * 8192.0;
                xSide2 = (xSide2 * 500 + playerToCheck->position.x) * 8192.0;
                zSide2 = (zSide2 * 500 + playerToCheck->position.z) * 8192.0;

                bool inFov = PointInTriangleInt(AllPlayers[randomPlayerToCheck].PlayerModel->x, AllPlayers[randomPlayerToCheck].PlayerModel->z, playerToCheck->PlayerModel->x, playerToCheck->PlayerModel->z, xSide1, zSide1, xSide2, zSide2);
                if (distancePlayers < 4)
                    inFov = true;
                // Check if the target is visible
                prepareAiRaycast(currentAiToCheck, randomPlayerToCheck, true);
                float hitDistance = 0;
                if (Raycast(currentAiToCheck, 0, &hitDistance) != NO_PLAYER && inFov)
                {
                    playerToCheck->target = randomPlayerToCheck;
                    playerToCheck->lastSeenTarget = randomPlayerToCheck;
                }
                else // If player is behind a wall
                {
                    playerToCheck->justCheking = false;
                    playerToCheck->target = NO_PLAYER;
                    int randomWait = 40 + rand() % 20;

                    playerToCheck->GunWaitCount = -randomWait;

                    // If bot finished his path, get a new one if no player is found
                    // TODO get a random one or to a nearest last seen player position waypoint
                    if (playerToCheck->lastSeenTarget != NO_PLAYER)
                    {
                        Player *lastSeenTargetPlayer = &AllPlayers[playerToCheck->lastSeenTarget];
                        int nearestWaypoint = getNearestWaypoint(lastSeenTargetPlayer->position.x, lastSeenTargetPlayer->position.y, lastSeenTargetPlayer->position.z);
                        StartChecking(currentAiToCheck, nearestWaypoint);
                        playerToCheck->lastSeenTarget = NO_PLAYER;
                    }
                    else if (playerToCheck->PathCount == 0) // If path is finished and no player detected
                    {
                        // If bomb is dropped and if the player is a terrorist
                        if (bombDropped && playerToCheck->Team == TERRORISTS)
                        {
                            // Get the nearest waypoint of the dropped bomb
                            int nearestWaypoint = getNearestWaypoint(droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.y, droppedBombPositionAndRotation.z);

                            // If the player is already at the nearest waypoint, go to the bomb position
                            if (playerToCheck->LastWayPoint == nearestWaypoint)
                            {
                                playerToCheck->searchForDroppedBomb = true;
                            }
                            else // Or go to the  nearest waypoint
                            {
                                StartChecking(currentAiToCheck, nearestWaypoint);
                            }
                        }
                        else
                        {
                            GetRandomPoint(currentAiToCheck);
                            if (playerToCheck->Team == COUNTERTERRORISTS && BombPlanted && currentDefuserIndex == NO_PLAYER)
                            {
                                SetDefuser(currentAiToCheck);
                            }
                        }
                    }
                }
            }
            else if (playerToCheck->lastSeenTarget != NO_PLAYER)
            {
                Player *lastSeenTargetPlayer = &AllPlayers[playerToCheck->lastSeenTarget];
                int nearestWaypoint = getNearestWaypoint(lastSeenTargetPlayer->position.x, lastSeenTargetPlayer->position.y, lastSeenTargetPlayer->position.z);
                StartChecking(currentAiToCheck, nearestWaypoint);
                playerToCheck->target = NO_PLAYER;
                playerToCheck->lastSeenTarget = NO_PLAYER;
                if (random() % 2 == 0)
                {
                    if (playerToCheck->Team == COUNTERTERRORISTS && BombPlanted && currentDefuserIndex == NO_PLAYER)
                    {
                        SetDefuser(currentAiToCheck);
                    }
                }
            }
            else if (playerToCheck->PathCount == 0)
            {
                playerToCheck->target = NO_PLAYER;
                playerToCheck->lastSeenTarget = NO_PLAYER;

                if (playerToCheck->haveBomb)
                {
                    float distanceA = GetDistanceBewteenPlayerAndWaypoint(currentAiToCheck, 14);
                    float distanceB = GetDistanceBewteenPlayerAndWaypoint(currentAiToCheck, 29);

                    // if (random() % 2 == 0)
                    if (distanceA < distanceB)
                    {
                        StartChecking(currentAiToCheck, 14); // Bomb site A
                    }
                    else
                    {
                        StartChecking(currentAiToCheck, 29); // Bomb site B
                    }
                }
                else if (bombDropped && playerToCheck->Team == TERRORISTS)
                {
                    // Get the nearest waypoint of the dropped bomb
                    int nearestWaypoint = getNearestWaypoint(droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.y, droppedBombPositionAndRotation.z);

                    // If the player is already at the nearest waypoint, go to the bomb position
                    if (playerToCheck->LastWayPoint == nearestWaypoint)
                    {
                        playerToCheck->searchForDroppedBomb = true;
                    }
                    else // Or go to the  nearest waypoint
                    {
                        StartChecking(currentAiToCheck, nearestWaypoint);
                    }
                }
                else if (playerToCheck->Team == COUNTERTERRORISTS && BombPlanted && currentDefuserIndex == NO_PLAYER)
                {
                    SetDefuser(currentAiToCheck);
                }
                else
                {
                    // TODO move to a random waypoint or stay some seconds
                    GetRandomPoint(currentAiToCheck);
                }
            }
        }
    }
}

void GetRandomPoint(int currentAiToCheck)
{
    if (IsExplode || (BombPlanted && ((BombSeconds <= 5 && AllPlayers[currentAiToCheck].Team == TERRORISTS) || (BombSeconds <= 3 && AllPlayers[currentAiToCheck].Team == COUNTERTERRORISTS)))) // Run away from the bomb
    {
        StartChecking(currentAiToCheck, random() % waypointsSize);
    }
    else
    {
        int site = random() % 2;

        if (bombPlantedAt == 14)
            site = 0;
        else if (bombPlantedAt == 29)
            site = 1;

        Site siteRef = allMaps[currentMap].AllBombsTriggersCollisions[site];

        StartChecking(currentAiToCheck, siteRef.nearWaypoints[random() % siteRef.nearWaypointCount]);
    }
}

/**
 * @brief Set the a Random Defuser
 */
void SetRandomDefuser()
{
    if (currentDefuserIndex == NO_PLAYER)
    {
        // Check if there is an AI is able to defuse the bomb
        bool canAffectDefuser = false;
        for (int playerIndex = 1; playerIndex < MaxPlayer; playerIndex++)
        {
            Player *player = &AllPlayers[playerIndex];
            if (player->Team == COUNTERTERRORISTS && !player->IsDead && player->target == NO_PLAYER && player->Id != UNUSED)
            {
                canAffectDefuser = true;
                break;
            }
        }

        if (canAffectDefuser)
        {
            // Take a random one and the bot will go to the bomb and defuse it
            int newDefuser = (random() % 5) + 1;
            Player *defuserPlayer = &AllPlayers[newDefuser];
            while (defuserPlayer->Team != COUNTERTERRORISTS || defuserPlayer->IsDead || defuserPlayer->target != NO_PLAYER || defuserPlayer->Id == UNUSED)
            {
                defuserPlayer = &AllPlayers[newDefuser];

                newDefuser = (random() % 5) + 1;
            }
            defuserPlayer->PathCount = -1;
            defuserPlayer->lastSeenTarget = NO_PLAYER;
            SetDefuser(newDefuser);
        }
    }
}

/**
 * @brief Force a bot to be a defuser and check a path to the bomb
 *
 * @param defuserIndex player index
 */
void SetDefuser(int defuserIndex)
{
    StartChecking(defuserIndex, bombPlantedAt);
    currentDefuserIndex = defuserIndex;
}

/**
 * @brief Check if AI can shoot his target
 *
 */
void checkAiShoot()
{
    for (int i = 1; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];
        if (player->target != NO_PLAYER && player->isAi && !player->IsDead)
        {
            Player *targetPlayer = &AllPlayers[player->target];

            // Get the direction to shoot the target
            Vector3 Direction;
            Direction.x = targetPlayer->PlayerModel->x - player->PlayerModel->x;
            Direction.y = targetPlayer->PlayerModel->y - (player->PlayerModel->y + CameraOffsetYMultiplied);
            Direction.z = targetPlayer->PlayerModel->z - player->PlayerModel->z;
            player->AngleDestination = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0;

            if (player->AllGunsInInventory[player->currentGunInInventory] < GunCount && !player->isReloading && player->GunWaitCount >= getPlayerCurrentGun(player).fireRate)
            {
                if (((player->currentGunInInventory == 1 || player->currentGunInInventory == 2) && player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount > 0) || getPlayerCurrentGun(player).isKnife)
                {
                    if (!getPlayerCurrentGun(player).isKnife)
                    {
                        player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount--;
                        // Gun sound
                        int Panning, Volume;
                        GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, getPlayerCurrentGun(player).MaxSoundDistance);
                        Play3DSound(getPlayerCurrentGun(player).gunSound, Volume, Panning, player);
                    }

                    // Animations
                    setGunRecoil(player);

                    for (int shootIndex = 0; shootIndex < getPlayerCurrentGun(player).bulletCountPerShoot; shootIndex++)
                    {
                        // Make a racast to shoot the target
                        prepareAiRaycast(i, player->target, false);
                        player->GunWaitCount = 0;
                        float hitDistance = 0;
                        if (Raycast(i, shootIndex, &hitDistance) != NO_PLAYER) // If shoot hit the target, apply damage
                        {
                            makeHit(i, player->target, hitDistance, shootIndex);
                        }
                    }
                }
                else
                {
                    // If AI do not have ammo, reload gun or change gun
                    if (player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount == 0 && player->AllAmmoMagazine[player->currentGunInInventory - 1].TotalAmmoCount == 0)
                    {
                        // Change gun
                        ChangeGunInInventory(i, 1);
                    }
                    else
                    {
                        // Reload
                        startReloadGun(i);
                    }
                }
            }
        }
    }
}

/**
 * @brief Check if bot can buy a weapon
 *
 * @param playerIndex
 */
void CheckShopForBot(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    if (player->Id != UNUSED && player->isAi)
    {
        if (player->armor == 0)
        {
            if (player->Money >= allEquipments[3].Price)
            {
                player->Money -= allEquipments[3].Price;
                player->armor = 100;
                player->haveHeadset = true;
            }
            else if (player->Money >= allEquipments[2].Price)
            {
                player->Money -= allEquipments[2].Price;
                player->armor = 100;
            }
        }

        if (player->Team == COUNTERTERRORISTS)
        {
            // Buy only counter terrorist gun

            // Take a random gun category
            int random = 1 + rand() % 3;

            if (cheapestGunsCostCounter[random] <= player->Money && player->AllGunsInInventory[2] == EMPTY) // If bot has enough money, buy a random gun from this category
            {
                findGun(random, playerIndex);
            }
            else if (cheapestGunsCostCounter[0] <= player->Money && player->AllGunsInInventory[1] == EMPTY) // If bot has enough money, buy a random gun from pistols category
            {
                findGun(0, playerIndex);
            }
        }
        else
        {
            // Buy only terrorist gun

            // Take a random gun category
            int random = 1 + rand() % 3;

            if (cheapestGunsCostTerrorists[random] <= player->Money && player->AllGunsInInventory[2] == EMPTY) // If bot has enough money, buy a random gun from this category
            {
                findGun(random, playerIndex);
            }
            else if (cheapestGunsCostTerrorists[0] <= player->Money && player->AllGunsInInventory[1] == EMPTY) // If bot has enough money, buy a random gun from pistols category
            {
                findGun(0, playerIndex);
            }
        }
    }
}

/**
 * @brief Check if bots can buy a weapon
 */
void checkShopForBots()
{
    for (int i = 1; i < MaxPlayer; i++)
    {
        CheckShopForBot(i);
    }
}