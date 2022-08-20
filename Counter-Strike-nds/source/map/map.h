// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef MAP_H_ /* Include guard */
#define MAP_H_

#include "../main.h"

#include "dust2_map_image_bin.h"
#include "tutorial_map_image_bin.h"

enum mapEnum
{
    DUST2 = 0,
    TUTORIAL = 1
};

typedef struct //
{
    NE_Model *Model;
    bool shadowed;
} MapModel;

typedef struct //
{
    const char *name;
    Vector3 allTerroristsSpawns[6];
    Vector3 allCounterTerroristsSpawns[6];
    float startPlayerAngleTerrorists;
    float startPlayerAngleCounterTerrorists;
    int CollisionsCount;
    Wall *AllWallsCollisions;

    int StairsCount;
    Stairs *AllStairs;

    int zonesCount;
    Zone *AllZones;
    int BombsTriggersCollisionsCount;
    Site *AllBombsTriggersCollisions;
    int occlusionZoneCount;
    OcclusionZone *AllOcclusionZone;
    int shadowCollisionBoxCount;
    CollisionBox *AllShadowCollisionBox;

    MapModel *models;
    void *image;
    int forcePartyMode;

    Vector3 cameraPosition;
    Vector3 cameraLookPosition;
} Map;

#define MAP_COUNT 2
extern Map allMaps[MAP_COUNT];

void AddAllSpawnPoints();
void AddSpawnPoint(int mapIndex, int team, int index, float x, float y, float z);
void SetAngles(int mapIndex, float terroristsAngle, float CounterAngle);
Wall *getMapWalls();
int getMapWallsCount();
void LoadMapTextures();
void LoadMap(int mapToLoad);
void UnLoadMap(int mapToUnload);
void SetMapNames();
void SetMapPartyMode();
void SetMapCameraPosition();

#endif // MAP_H_