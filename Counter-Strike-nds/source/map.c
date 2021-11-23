#include "main.h"
#include "map.h"

Map allMaps[1];

void AddAllSpawnPoints()
{
    AddSpawnPoint(0, 0, 0, -3, 6.43, 65);
    AddSpawnPoint(0, 0, 1, -6, 6.43, 65);
    AddSpawnPoint(0, 0, 2, -9, 6.43, 65);
    AddSpawnPoint(0, 0, 3, -3, 6.43, 62.5);
    AddSpawnPoint(0, 0, 4, -6, 6.43, 62.5);
    AddSpawnPoint(0, 0, 5, -9, 6.43, 62.5);

    AddSpawnPoint(0, 1, 0, 16, 0, -22);
    AddSpawnPoint(0, 1, 1, 19.5, 0, -22);
    AddSpawnPoint(0, 1, 2, 23, 0, -22);
    AddSpawnPoint(0, 1, 3, 16, 0, -19.5);
    AddSpawnPoint(0, 1, 4, 19.5, 0, -22);
    AddSpawnPoint(0, 1, 5, 23, 0, -22);

    SetAngles(0, 0, -256);
}

void SetAngles(int mapIndex, float terroristsAngle, float CounterAngle)
{
    allMaps[mapIndex].startPlayerAngleTerrorists = terroristsAngle;
    allMaps[mapIndex].startPlayerAngleCounterTerrorists = CounterAngle;
}

void AddSpawnPoint(int mapIndex, int team, int index, float x, float y, float z)
{
    Map *map = &allMaps[mapIndex];
    if (team == 0)
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
