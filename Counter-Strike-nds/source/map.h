#ifndef MAP_H_ /* Include guard */
#define MAP_H_

#include <NEMain.h>
#include "main.h"

typedef struct //
{
    char name[20];
    Vector3 allTerroristsSpawns[6];
    Vector3 allCounterTerroristsSpawns[6];
    float startPlayerAngleTerrorists;
    float startPlayerAngleCounterTerrorists;
} Map;

extern Map allMaps[1];

void AddAllSpawnPoints();
void AddSpawnPoint(int mapIndex, int team, int index, float x, float y, float z);
void SetAngles(int mapIndex, float terroristsAngle, float CounterAngle);

#endif // MAP_H_