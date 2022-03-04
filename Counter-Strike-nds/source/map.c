#include "main.h"
#include "map.h"
#include "collisions.h"
#include "party.h"
#include "ai.h"

Map allMaps[MAP_COUNT];

void UnLoadMap(int mapToUnload)
{
    for (int i = 0; i < allMaps[mapToUnload].CollisionsCount; i++)
    {
        NE_PhysicsDelete(allMaps[mapToUnload].AllWallsCollisions[i].WallPhysics);
        NE_ModelDelete(allMaps[mapToUnload].AllWallsCollisions[i].WallModel);
    }

    for (int i = 0; i < allMaps[mapToUnload].occlusionZoneCount; i++)
    {
        NE_ModelDelete(allMaps[mapToUnload].models[i].Model);
    }

    if (allMaps[mapToUnload].AllWallsCollisions != NULL)
        free(allMaps[mapToUnload].AllWallsCollisions);

    if (allMaps[mapToUnload].models != NULL)
        free(allMaps[mapToUnload].models);

    if (allMaps[mapToUnload].AllStairs != NULL)
        free(allMaps[mapToUnload].AllStairs);

    if (allMaps[mapToUnload].AllBombsTriggersCollisions != NULL)
        free(allMaps[mapToUnload].AllBombsTriggersCollisions);

    if (allMaps[mapToUnload].AllZones != NULL)
        free(allMaps[mapToUnload].AllZones);

    if (allMaps[mapToUnload].AllShadowCollisionBox != NULL)
        free(allMaps[mapToUnload].AllShadowCollisionBox);

    LastStairs = 0;

    for (int i = 0; i < GrenadeCount; i++)
    {
        DeleteGrenade(i);
    }
}

void LoadMap(int mapToLoad)
{
    CreateWaypoints(mapToLoad);
    createLengthMatrices(mapToLoad);

    LoadAllCollisions(mapToLoad);
    CalculateAllTriggerColBoxs(mapToLoad);
    LoadAllStairs(mapToLoad);

    LoadAllBombZones(mapToLoad);
    loadMapModels(mapToLoad);
    setCameraMapPosition();
}

void LoadMapTextures()
{
    allMaps[DUST2].image = (void *)dust2_map_image_bin;
    allMaps[TUTORIAL].image = (void *)tutorial_map_image_bin;
}

void SetMapNames()
{
    allMaps[DUST2].name = "Dust II";
    allMaps[TUTORIAL].name = "Training (unfinished)";
}

void SetMapPartyMode()
{
    allMaps[DUST2].forcePartyMode = -1;
    allMaps[TUTORIAL].forcePartyMode = 2;
}

void SetMapCameraPosition()
{
    // DUST2
    allMaps[DUST2].cameraPosition.x = -3;
    allMaps[DUST2].cameraPosition.y = 8;
    allMaps[DUST2].cameraPosition.z = -14;

    allMaps[DUST2].cameraLookPosition.x = -12;
    allMaps[DUST2].cameraLookPosition.y = 3.2;
    allMaps[DUST2].cameraLookPosition.z = -16;

    // TUTORIAL
    allMaps[TUTORIAL].cameraPosition.x = -12;
    allMaps[TUTORIAL].cameraPosition.y = 8;
    allMaps[TUTORIAL].cameraPosition.z = -5;

    allMaps[TUTORIAL].cameraLookPosition.x = -3;
    allMaps[TUTORIAL].cameraLookPosition.y = 3.2;
    allMaps[TUTORIAL].cameraLookPosition.z = -0;
}

void AddAllSpawnPoints()
{
    // DUST2
    AddSpawnPoint(DUST2, TERRORISTS, 0, -3, 6.43, 65);
    AddSpawnPoint(DUST2, TERRORISTS, 1, -6, 6.43, 65);
    AddSpawnPoint(DUST2, TERRORISTS, 2, -9, 6.43, 65);
    AddSpawnPoint(DUST2, TERRORISTS, 3, -3, 6.43, 62.5);
    AddSpawnPoint(DUST2, TERRORISTS, 4, -6, 6.43, 62.5);
    AddSpawnPoint(DUST2, TERRORISTS, 5, -9, 6.43, 62.5);

    AddSpawnPoint(DUST2, COUNTERTERRORISTS, 0, 16, 0, -22);
    AddSpawnPoint(DUST2, COUNTERTERRORISTS, 1, 19.5, 0, -22);
    AddSpawnPoint(DUST2, COUNTERTERRORISTS, 2, 23, 0, -22);
    AddSpawnPoint(DUST2, COUNTERTERRORISTS, 3, 16, 0, -19.5);
    AddSpawnPoint(DUST2, COUNTERTERRORISTS, 4, 19.5, 0, -22);
    AddSpawnPoint(DUST2, COUNTERTERRORISTS, 5, 23, 0, -22);

    SetAngles(DUST2, 0, -256);

    // TUTORIAL
    AddSpawnPoint(TUTORIAL, 0, 0, 0, 0, 0);

    AddSpawnPoint(TUTORIAL, 1, 0, 0, 0, 0);

    SetAngles(TUTORIAL, 0, 0);
}

void loadMapModels(int mapToLoad)
{
    Map *map = &allMaps[mapToLoad];

    Vector3 mapPos;
    Vector3Int Scale;
    int mapPartCount = 0;

    if (mapToLoad == DUST2)
    {
        mapPartCount = 7;
        mapPos.x = 0;
        mapPos.y = 1.5 + 0.8;
        mapPos.z = 0;
        Scale.x = Scale.y = Scale.z = 4096;
    }
    else if (mapToLoad == TUTORIAL)
    {
        mapPartCount = 2;
        mapPos.x = 0;
        mapPos.y = -1.5 + 0.5;
        mapPos.z = 0;
        Scale.x = Scale.y = Scale.z = 4096 / 2;
    }

    for (int i = 0; i < mapPartCount; i++)
    {
        map->models[i].Model = NE_ModelCreate(NE_Static);
        NE_ModelSetMaterial(map->models[i].Model, GroundMaterial);
        NE_ModelScaleI(map->models[i].Model, Scale.x, Scale.y, Scale.z);
        NE_ModelSetCoord(map->models[i].Model, mapPos.x, mapPos.y, mapPos.z);
        map->models[i].Model->rx = 128;
        map->models[i].Model->ry = 256;
    }

    if (mapToLoad == DUST2)
    {
        NE_ModelLoadStaticMesh(map->models[0].Model, (u32 *)DustPart0_bin);
        map->models[0].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[1].Model, (u32 *)DustPart1_bin);
        map->models[1].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[2].Model, (u32 *)DustPart2_bin);
        map->models[2].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[3].Model, (u32 *)DustPart3_bin);
        map->models[3].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[4].Model, (u32 *)DustPart4_bin);
        map->models[4].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[5].Model, (u32 *)DustPart5_bin);
        map->models[5].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[6].Model, (u32 *)DustPart6_bin);
        map->models[6].shadowed = false;
    }
    else if (mapToLoad == TUTORIAL)
    {
        NE_ModelLoadStaticMesh(map->models[0].Model, (u32 *)tutorialMapUnShadowed_3ds_bin);
        map->models[0].shadowed = false;
        NE_ModelLoadStaticMesh(map->models[1].Model, (u32 *)tutorialMapShadowed_3ds_bin);
        map->models[1].shadowed = true;
    }
}

Wall *getMapWalls()
{
    return allMaps[currentMap].AllWallsCollisions;
}

int getMapWallsCount()
{
    return allMaps[currentMap].CollisionsCount;
}

void SetName(int mapIndex, const char *name)
{
    allMaps[mapIndex].name = name;
}

void SetAngles(int mapIndex, float terroristsAngle, float CounterAngle)
{
    allMaps[mapIndex].startPlayerAngleTerrorists = terroristsAngle;
    allMaps[mapIndex].startPlayerAngleCounterTerrorists = CounterAngle;
}

void AddSpawnPoint(int mapIndex, int team, int index, float x, float y, float z)
{
    Map *map = &allMaps[mapIndex];
    if (team == TERRORISTS)
    {
        map->allTerroristsSpawns[index].x = x;
        map->allTerroristsSpawns[index].y = y;
        map->allTerroristsSpawns[index].z = z;
    }
    else
    {
        map->allCounterTerroristsSpawns[index].x = x;
        map->allCounterTerroristsSpawns[index].y = y;
        map->allCounterTerroristsSpawns[index].z = z;
    }
}
