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
    int edge[4];
} Waypoint;

typedef struct
{
    int matrixOneLength[58][58];
} PathLength;

#define botsNamesCount 10
#define AllMatricesLengthCount 15
extern char botsNames[botsNamesCount][20];
extern bool botsNamesTaken[botsNamesCount];
extern PathLength AllMatricesLength[AllMatricesLengthCount];

// int CheckPath();
void StartChecking(int playerIndex, int FinalWaypoint);
void CreateWaypoints();
void CreateWaypoint(int id, float x, float y, float z, int edgeCount, int edge[4]);
void CheckPathWaypoint(int playerIndex, int StartWaypoint, int FinalWaypoint);
int GetDistanceBewteenTwoPlayers(int player1Index, int player2Index);
int getNearestWaypoint(float x, float y, float z);
void setBotsNames();
void ResetTakenBotsNames();
void checkShopForBot(int playerId);
void checkShopForBots();
void checkForPlayer();
void checkTarget();
void SetDefuser(int defuserIndex);
void SetRandomDefuser();
void createLengthMatrices();

Waypoint *GetWaypoints();

#endif // AI_H_