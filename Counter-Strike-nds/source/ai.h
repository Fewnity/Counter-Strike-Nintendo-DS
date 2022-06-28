// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef AI_H_ /* Include guard */
#define AI_H_
#include <NEMain.h>
#include "main.h"

typedef struct
{
    float x;
    float y;
    float z;
    int edgeCount;
    int *edge;
    // int edge[5];
} Waypoint;

typedef struct
{
    // int matrixOneLength[58][58];
    int **matrixOneLength;
} PathLength;

#define BOTS_NAMES_COUNT 46
extern const char *botsNames[BOTS_NAMES_COUNT];
extern bool botsNamesTaken[BOTS_NAMES_COUNT];
extern PathLength *AllMatricesLength;

void StartChecking(int playerIndex, int FinalWaypoint);
void CreateWaypoints(int mapToLoad);
void CreateWaypoint(int id, float x, float y, float z, int edgeCount, int *edge);
void CheckPathWaypoint(int playerIndex, int StartWaypoint, int FinalWaypoint);
int GetDistanceBewteenTwoPlayers(int player1Index, int player2Index);
int getNearestWaypoint(float x, float y, float z);
void setBotsNames();
void ResetTakenBotsNames();
void CheckShopForBot(int playerId);
void checkShopForBots();
void AiCheckForAction();
void checkAiShoot();
void SetDefuser(int defuserIndex);
void SetRandomDefuser();
void createLengthMatrices(int mapToLoad);
void freeAllMatricesLength(int size);
void freeWaypoint();

Waypoint *GetWaypoints();

#endif // AI_H_