#include "main.h"
#include <float.h>
#include "ai.h"
#include "gun.h"
#include "party.h"
#include "sounds.h"
#include "collisions.h"

/*#define map_size_rows 10
#define map_size_cols 10
bool mapData[map_size_rows][map_size_cols];*/

/*bool mapData[map_size_rows][map_size_cols] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};*/

// bool mapData[map_size_rows][map_size_cols] = {{0}};

/* description of graph node */
/*struct stop
{
    int col, row;
    //array of indexes of routes from this stop to neighbours in array of all routes
    int *n;
    int n_len;
    float32 f, g, h;
    int from;
};*/

// int ind[map_size_rows][map_size_cols];

/*int ind[map_size_rows][map_size_cols] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};*/

/* description of route between two nodes */
/*struct route
{
    // route has only one direction!
    int x; // index of stop in array of all stops of src of this route
    int y; // intex of stop in array of all stops od dst of this route
    float32 d;
};*/

char botsNames[botsNamesCount][20];
bool botsNamesTaken[botsNamesCount];

void setBotsNames()
{
    strcpy(botsNames[0], "Bob");
    strcpy(botsNames[1], "Frank");
    strcpy(botsNames[2], "Brandon");
    strcpy(botsNames[3], "Josh");
    strcpy(botsNames[4], "Greg");
    strcpy(botsNames[5], "Steve ");
    strcpy(botsNames[6], "Xavier");
    strcpy(botsNames[7], "Joe");
    strcpy(botsNames[8], "Vladimir");
    strcpy(botsNames[9], "Kurt");
}

void ResetTakenBotsNames()
{
    for (int i = 0; i < botsNamesCount; i++)
    {
        botsNamesTaken[i] = false;
    }
}

void CreateWaypoints()
{
    int edge0[4] = {55, 4, 20};
    CreateWaypoint(0, -0.8, 0.0727, 5.94, 3, edge0);
    int edge1[4] = {56, 2, 32};
    CreateWaypoint(1, 1.588759, 0.0727, -13.34, 3, edge1);
    int edge2[4] = {1, 3, 57};
    CreateWaypoint(2, 24.565, 0.0727, -13.38, 3, edge2);
    int edge3[4] = {2, 9};
    CreateWaypoint(3, 37.474, 3.3017, -13.38, 2, edge3);
    int edge4[4] = {0, 5};
    CreateWaypoint(4, -10.82, 0.4764, 4.41, 2, edge4);
    int edge5[4] = {4, 6};
    CreateWaypoint(5, -15.714, 0.4764, 8.416, 2, edge5);
    int edge6[4] = {5, 7};
    CreateWaypoint(6, -15.714, 2.099, 11.648, 2, edge6);
    int edge7[4] = {6, 8};
    CreateWaypoint(7, -17.208, 2.103, 13.19, 2, edge7);
    int edge8[4] = {7, 21};
    CreateWaypoint(8, -20.2982, 4.1082, 13.15, 2, edge8);
    int edge9[4] = {3, 10, 54};
    CreateWaypoint(9, 47.92, 3.3017, -17.4, 3, edge9);
    int edge10[4] = {9, 11};
    CreateWaypoint(10, 47.92, 6.502, -30.298, 2, edge10);
    int edge11[4] = {10, 12};
    CreateWaypoint(11, 43.91, 6.502, -34.19, 2, edge11);
    int edge12[4] = {11, 13};
    CreateWaypoint(12, 40.44, 6.502, -30.322, 2, edge12);
    int edge13[4] = {12, 14, 15};
    CreateWaypoint(13, 40.44, 5.71, -28.29, 3, edge13);
    int edge14[4] = {13};
    CreateWaypoint(14, 41.02, 5.7171, -20.18, 1, edge14);
    int edge15[4] = {13, 16};
    CreateWaypoint(15, 21.45, 5.71, -24.71, 2, edge15);
    int edge16[4] = {15, 17};
    CreateWaypoint(16, 19.26, 5.71, -4.494, 2, edge16);
    int edge17[4] = {16, 18};
    CreateWaypoint(17, 21.03, 3.3012, 1.58, 2, edge17);
    int edge18[4] = {17, 19};
    CreateWaypoint(18, 6.953, 3.3012, 3.8, 2, edge18);
    int edge19[4] = {42, 20, 18, 46};
    CreateWaypoint(19, 6.953, 3.3012, 30.92, 4, edge19);
    int edge20[4] = {0, 19, 46};
    CreateWaypoint(20, 1.65, 3.3012, 21.326, 3, edge20);
    int edge21[4] = {8, 22, 53};
    CreateWaypoint(21, -30.27, 4.1082, 13.15, 3, edge21);
    int edge22[4] = {21, 23};
    CreateWaypoint(22, -38.8, 4.1082, 5.94, 2, edge22);
    int edge23[4] = {22, 24};
    CreateWaypoint(23, -38.275, 4.1082, -4.496, 2, edge23);
    int edge24[4] = {23, 25, 28};
    CreateWaypoint(24, -38.275, 3.3068, -6.1201, 3, edge24);
    int edge25[4] = {24, 26, 28};
    CreateWaypoint(25, -36.513, 3.3068, -18.07, 3, edge25);
    int edge26[4] = {25, 27};
    CreateWaypoint(26, -36.513, 4.1038, -20.6286, 2, edge26);
    int edge27[4] = {26};
    CreateWaypoint(27, -36.513, 4.1038, -26.5, 1, edge27);
    int edge28[4] = {24, 25, 29, 30};
    CreateWaypoint(28, -27.67, 3.3068, -13.19, 4, edge28);
    int edge29[4] = {28};
    CreateWaypoint(29, -27.17, 3.3068, -26.79, 1, edge29);
    int edge30[4] = {28, 31};
    CreateWaypoint(30, -22.02, 3.3068, -13.19, 2, edge30);
    int edge31[4] = {30, 32};
    CreateWaypoint(31, -17.385, 3.3068, -16.984, 2, edge31);
    int edge32[4] = {1, 31};
    CreateWaypoint(32, -7.695, -0.234, -16.36, 2, edge32);
    int edge33[4] = {36, 34, 54, 38};
    CreateWaypoint(33, 49.579, 3.3017, 18.17, 4, edge33);
    int edge34[4] = {33, 35};
    CreateWaypoint(34, 52.7995, 4.9118, 18.17, 2, edge34);
    int edge35[4] = {34};
    CreateWaypoint(35, 54.39, 4.9118, 26.67, 1, edge35);
    int edge36[4] = {33, 37};
    CreateWaypoint(36, 47.88, 3.3017, 21.315, 2, edge36);
    int edge37[4] = {36};
    CreateWaypoint(37, 47.88, -1.5331, 34.2316, 1, edge37);
    int edge38[4] = {33, 39};
    CreateWaypoint(38, 40.04, 3.3017, 17.9, 2, edge38);
    int edge39[4] = {38, 40};
    CreateWaypoint(39, 27.17, 3.3017, 18.47, 2, edge39);
    int edge40[4] = {39, 41};
    CreateWaypoint(40, 29.22, 3.3017, 32.01, 2, edge40);
    int edge41[4] = {40, 42, 43};
    CreateWaypoint(41, 22.81, 3.3017, 38, 3, edge41);
    int edge42[4] = {41, 19};
    CreateWaypoint(42, 15.47, 3.3017, 30.54, 2, edge42);
    int edge43[4] = {44, 47, 41};
    CreateWaypoint(43, 16.63, 3.3017, 60.19, 3, edge43);
    int edge44[4] = {45, 43};
    CreateWaypoint(44, 12.42, 3.3017, 55.01, 2, edge44);
    int edge45[4] = {46, 44};
    CreateWaypoint(45, 0.87, 3.3017, 55.01, 2, edge45);
    int edge46[4] = {19, 20, 45};
    CreateWaypoint(46, 0.83, 3.3017, 34.08, 3, edge46);
    int edge47[4] = {43, 48};
    CreateWaypoint(47, 11.65, 3.3017, 60.19, 2, edge47);
    int edge48[4] = {47, 49};
    CreateWaypoint(48, -1.254, 6.529, 60.19, 2, edge48);
    int edge49[4] = {50, 48};
    CreateWaypoint(49, -24.84, 6.529, 61.69, 2, edge49);
    int edge50[4] = {49, 51};
    CreateWaypoint(50, -37.03, 6.529, 56.829, 2, edge50);
    int edge51[4] = {50, 52};
    CreateWaypoint(51, -37.03, 3.278, 43.903, 2, edge51);
    int edge52[4] = {51, 53};
    CreateWaypoint(52, -30.061, 3.278, 28.464, 2, edge52);
    int edge53[4] = {52, 21};
    CreateWaypoint(53, -30.061, 4.1094, 26.966, 2, edge53);
    int edge54[4] = {33, 9};
    CreateWaypoint(54, 49.97, 3.3017, 2.61, 2, edge54);
    int edge55[4] = {0, 56};
    CreateWaypoint(55, 3.08, 0.0727, 0.82, 2, edge55);
    int edge56[4] = {1, 55};
    CreateWaypoint(56, 1.01, 0.0727, -2.04, 2, edge56);
    int edge57[4] = {2};
    CreateWaypoint(57, 19.58, 0.0727, -19.42, 1, edge57);
}
Waypoint Waypoints[maxPath];

Waypoint *GetWaypoints()
{
    return Waypoints;
}

int getNearestWaypoint(float x, float y, float z)
{
    int Id = -1;
    int Dis = 9999;
    for (int i = 0; i < maxPath; i++)
    {
        int CurDis = sqrtf(powf((Waypoints[i].x - x), 2.0) + powf((Waypoints[i].y - y), 2.0) + powf((Waypoints[i].z - z), 2.0));
        if (CurDis < Dis)
        {
            Id = i;
            Dis = CurDis;
        }
    }
    return Id;
}

int GetDistanceBewteenTwoPlayers(int player1Index, int player2Index)
{
    Player *player1 = &AllPlayers[player1Index];
    Player *player2 = &AllPlayers[player2Index];
    return sqrtf(powf((player1->xPos - player2->xPos), 2.0) + powf((player1->yPos - player2->yPos), 2.0) + powf((player1->zPos - player2->zPos), 2.0));
}

void CreateWaypoint(int id, float x, float y, float z, int edgeCount, int edge[4])
{
    Waypoints[id].x = x;
    Waypoints[id].y = y;
    Waypoints[id].z = z;
    Waypoints[id].edgeCount = edgeCount;

    for (int i = 0; i < edgeCount; i++)
        Waypoints[id].edge[i] = edge[i];
}

// int StartWaypoint = 0;
// int FinalWaypoint = 14;

void StartChecking(int playerIndex, int FinalWaypoint)
{
    Player *player = &AllPlayers[playerIndex];
    int StartWaypoint = getNearestWaypoint(player->xPos, player->yPos, player->zPos);
    CheckPathWaypoint(playerIndex, StartWaypoint, FinalWaypoint);
}

void CheckPathWaypoint(int playerIndex, int StartWaypoint, int FinalWaypoint)
{
    // printf("Start scan\n");
    int Path[maxPath];
    bool Found = false;
    int currentWaypoint = StartWaypoint;
    int PathCount = 0;

    while (!Found)
    {
        Path[PathCount] = currentWaypoint;
        PathCount++;

        // if (PathCount == maxPath)
        if (PathCount == 40) //////////////////////////////////////IF GAME CRASHS, INCREASE THIS NUMBER
        {
            CheckPathWaypoint(playerIndex, StartWaypoint, FinalWaypoint);
            return;
        }

        // printf("c %d\n", PathCount);
        if (currentWaypoint == FinalWaypoint)
        {
            // printf("path found\n");
            Found = true;
            Player *player = &AllPlayers[playerIndex];
            for (int i = 0; i < PathCount; i++)
            {
                player->Path[i] = Path[i];
            }
            player->PathCount = PathCount;
            player->CurrentPath = 0;
            // printf("y %d\n", Waypoints[Path[PathCount - 1]].y);
        }
        else
        {
            int NextWaypoint;
            int ScanedWaypoint[4] = {-1, -1, -1, -1};
            int ScanCount = 0;
            bool contains;
            do
            {
                contains = FALSE;
                bool CanBreak = false;
                while (!CanBreak)
                {
                    CanBreak = true;
                    NextWaypoint = rand() % Waypoints[currentWaypoint].edgeCount;
                    for (int i = 0; i < ScanCount; i++)
                        if (ScanedWaypoint[i] == NextWaypoint)
                        {
                            CanBreak = false;
                            break;
                        }
                }

                // NextWaypoint = rand() % Waypoints[currentWaypoint].edgeCount;
                // testNumber++;
                ScanedWaypoint[ScanCount] = NextWaypoint;

                for (int i = 0; i < PathCount; i++)
                    if (Path[i] == Waypoints[currentWaypoint].edge[NextWaypoint])
                    {
                        // printf("True\n");
                        contains = TRUE;
                        /*if (Waypoints[currentWaypoint].edgeCount == 1)
                        {
                            printf("Relance\n");
                            CheckPathWaypoint(playerIndex);
                            return;
                        }*/
                        break;
                    }

                ScanCount++;
                if (contains && Waypoints[currentWaypoint].edgeCount == ScanCount)
                {
                    // printf("Relance2\n");
                    CheckPathWaypoint(playerIndex, StartWaypoint, FinalWaypoint);
                    return;
                }
            } while (contains && NextWaypoint != FinalWaypoint /*&& testNumber < 100*/);

            /*if (testNumber == 100)
            {
                //printf("Return\n");
                CheckPathWaypoint(playerIndex);
                return;
            }*/

            currentWaypoint = Waypoints[currentWaypoint].edge[NextWaypoint];
        }
    }
}

void checkForPlayer()
{
    // Check sometime the distance between the player and a bot
    checkPlayerDistanceFromAiTimer--;
    if (checkPlayerDistanceFromAiTimer % 10 == 0)
    {
        int currentAiToCheck = checkPlayerDistanceFromAiTimer / 10;
        // printf("%d", currentAiToCheck);

        if (checkPlayerDistanceFromAiTimer == 0)
            checkPlayerDistanceFromAiTimer = 50 + 1;
        Player *playerToCheck = &AllPlayers[currentAiToCheck];
        if (playerToCheck->isAi && playerToCheck->Id != -1 && !playerToCheck->IsDead && !playerToCheck->isPlantingBomb)
        {
            // CalculatePlayerPosition(currentAiToCheck);
            int randomPlayerToCheck = playerToCheck->target;
            // int randomPlayerToCheck = 0;
            int distancePlayers = 999;
            // distancePlayers = GetDistanceBewteenTwoPlayers(randomPlayerToCheck, currentAiToCheck);
            if (randomPlayerToCheck != -1)
                distancePlayers = GetDistanceBewteenTwoPlayers(randomPlayerToCheck, currentAiToCheck);

            int scannedPlayerCount = 0;
            if (distancePlayers >= 20 && playerToCheck->ScanFinished && playerToCheck->lastSeenTarget == -1 && playerToCheck->target == -1)
            {
                while (scannedPlayerCount < MaxPlayer)
                {
                    randomPlayerToCheck = rand() % MaxPlayer;
                    if (playerToCheck->allPlayerScanned[randomPlayerToCheck])
                        continue;

                    scannedPlayerCount++;
                    playerToCheck->allPlayerScanned[randomPlayerToCheck] = true;

                    if (AllPlayers[randomPlayerToCheck].Team == SPECTATOR || AllPlayers[randomPlayerToCheck].Team == playerToCheck->Team || randomPlayerToCheck == currentAiToCheck)
                    {
                        continue;
                    }
                    else
                    {
                        distancePlayers = GetDistanceBewteenTwoPlayers(randomPlayerToCheck, currentAiToCheck);
                        if (distancePlayers < 20)
                            break;
                    }
                }
            }

            for (int i = 0; i < MaxPlayer; i++)
            {
                playerToCheck->allPlayerScanned[i] = false;
            }

            if (distancePlayers < 20 && playerToCheck->ScanFinished) // Is a player is near, set this player as target
            {
                prepareAiRaycast(currentAiToCheck, randomPlayerToCheck, true);

                if (Raycast(currentAiToCheck) != -1)
                {
                    playerToCheck->GunWaitCount = -30;
                    playerToCheck->target = randomPlayerToCheck;
                    if (playerToCheck->ScanFinished)
                        playerToCheck->lastSeenTarget = randomPlayerToCheck;
                }
                else
                {
                    // printf("lose Target 3\n");
                    playerToCheck->justCheking = false;
                    playerToCheck->target = -1;
                    // If bot finished his path, get a new one if no player is found
                    // TODO get a random one or to a nearest last seen player position waypoint
                    if (playerToCheck->lastSeenTarget != -1)
                    {
                        Player *lastSeenTargetPlayer = &AllPlayers[playerToCheck->lastSeenTarget];
                        int nearestWaypoint = getNearestWaypoint(lastSeenTargetPlayer->xPos, lastSeenTargetPlayer->yPos, lastSeenTargetPlayer->zPos);
                        StartChecking(currentAiToCheck, nearestWaypoint);
                        playerToCheck->lastSeenTarget = -1;
                    }
                    else if (playerToCheck->PathCount == 0)
                    {
                        StartChecking(currentAiToCheck, random() % 57);
                    }
                }
            }
            else if (playerToCheck->lastSeenTarget != -1 && playerToCheck->ScanFinished)
            {
                // printf("lose Target 4\n");
                Player *lastSeenTargetPlayer = &AllPlayers[playerToCheck->lastSeenTarget];
                int nearestWaypoint = getNearestWaypoint(lastSeenTargetPlayer->xPos, lastSeenTargetPlayer->yPos, lastSeenTargetPlayer->zPos);
                StartChecking(currentAiToCheck, nearestWaypoint);
                playerToCheck->target = -1;
                playerToCheck->lastSeenTarget = -1;
            }
            else if (playerToCheck->PathCount == 0 && playerToCheck->ScanFinished)
            {
                playerToCheck->target = -1;
                playerToCheck->lastSeenTarget = -1;

                if (playerToCheck->haveBomb)
                {
                    // TODO move to the nearest or random bomb site (random at the beginning and nearest after)
                    if (random() % 2 == 0)
                    {
                        StartChecking(currentAiToCheck, 14); // Bomb site A
                        printf("(%d)GO TO BOMB SITE A\n", currentAiToCheck);
                    }
                    else
                    {
                        StartChecking(currentAiToCheck, 29); // Bomb site B
                        printf("(%d)GO TO BOMB SITE B\n", currentAiToCheck);
                    }
                }
                else if (playerToCheck->Team == COUNTERTERRORISTS && BombPlanted && currentDefuserIndex == -1)
                {
                    SetDefuser(currentAiToCheck);
                }
                else
                {
                    // TODO move to a random waypoint or stay some seconds
                    StartChecking(currentAiToCheck, random() % 57); // TODO change 57 by a define of the count of waypoint of the map
                }
            }
        }
    }
}

void SetRandomDefuser()
{
    if (currentDefuserIndex == -1)
    {
        bool canAffectDefuser = false;
        for (int i = 1; i < MaxPlayer; i++)
        {
            Player *player = &AllPlayers[i];
            if (player->Team == COUNTERTERRORISTS && !player->IsDead && player->target == -1)
            {
                canAffectDefuser = true;
                break;
            }
        }
        if (canAffectDefuser)
        {
            int newDefuser = random() % 6;
            Player *defuser = &AllPlayers[newDefuser];
            while (defuser->Team != COUNTERTERRORISTS || defuser->IsDead || defuser->target != -1)
            {
                defuser = &AllPlayers[newDefuser];
                newDefuser = random() % 6;
            }
            currentDefuserIndex = newDefuser;
            defuser->PathCount = -1;
            defuser->lastSeenTarget = -1;
        }
    }
}

void SetDefuser(int defuserIndex)
{
    int randomSite = random() % 2;
    if (randomSite == 0)
    {
        if (!bombCheckedInB)
        {
            StartChecking(defuserIndex, 29); // Bomb site B
            printf("(%d)GO FOR DEFUSE BOMB B\n", defuserIndex);
        }
        else
        {
            StartChecking(defuserIndex, 14); // Bomb site A
            printf("(%d)GO FOR DEFUSE BOMB A\n", defuserIndex);
        }
    }
    else
    {
        if (!bombCheckedInA)
        {
            StartChecking(defuserIndex, 14); // Bomb site A
            printf("(%d)GO FOR DEFUSE BOMB A\n", defuserIndex);
        }
        else
        {
            StartChecking(defuserIndex, 29); // Bomb site B
            printf("(%d)GO FOR DEFUSE BOMB B\n", defuserIndex);
        }
    }

    currentDefuserIndex = defuserIndex;
}

void checkTarget()
{
    for (int i = 1; i < MaxPlayer; i++)
    // for (int i = 0; i < 2; i++)
    {
        Player *player = &AllPlayers[i];
        if (player->target != -1 && player->ScanFinished && player->isAi)
        {
            Vector3 Direction;
            Player *targetPlayer = &AllPlayers[player->target];
            Direction.x = targetPlayer->PlayerModel->x - player->PlayerModel->x;
            Direction.y = targetPlayer->PlayerModel->y - (player->PlayerModel->y + CameraOffsetY * 4096.0);
            Direction.z = targetPlayer->PlayerModel->z - player->PlayerModel->z;
            player->AngleDestination = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0;

            if (player->AllGunsInInventory[player->currentGunInInventory] < GunCount && player->GunWaitCount >= AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].fireRate && !player->IsDead)
            {
                if (((player->currentGunInInventory == 1 || player->currentGunInInventory == 2) && player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount > 0) || AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].isKnife)
                {
                    if (!AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].isKnife)
                    {
                        player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount--;
                        // Gun sound
                        int Panning, Volume;
                        GetPanning(player->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].MaxSoundDistance);
                        Play3DSound(AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].gunSound, Volume, Panning);
                    }

                    prepareAiRaycast(i, player->target, false);
                    player->GunWaitCount = 0;
                    Raycast(i);
                }
                else
                {
                    if (player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount == 0 && player->AllAmmoMagazine[player->currentGunInInventory - 1].TotalAmmoCount == 0)
                    {
                        ChangeGunInInventory(i, 1);
                    }
                    else
                    {
                        startReloadGun(i);
                    }
                }
            }
        }
    }
}

void checkShopForBot()
{
    for (int i = 1; i < MaxPlayer; i++) // TODO remove bottom line
    // for (int i = 0; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];
        if (player->Id != -1 && player->isAi)
        {
            if (player->Team == COUNTERTERRORISTS)
            {
                int random = 1 + rand() % 3;
                if (cheapestGunsCostCounter[random] <= player->Money && player->AllGunsInInventory[2] == -1)
                {
                    findGun(random, i);
                }
                else if (cheapestGunsCostCounter[0] <= player->Money && player->AllGunsInInventory[1] == -1)
                {
                    findGun(0, i);
                }
            }
            else
            {
                int random = 1 + rand() % 3;
                if (cheapestGunsCostTerrorists[random] <= player->Money && player->AllGunsInInventory[2] == -1)
                {
                    findGun(random, i);
                }
                else if (cheapestGunsCostTerrorists[0] <= player->Money && player->AllGunsInInventory[1] == -1)
                {
                    findGun(0, i);
                }
            }
        }
    }
}

/*int CheckPath()
{
    int startCase = 0;
    int lastCase = (map_size_rows - 2) * (map_size_cols - 2) - 1;
    //int lastCase = 10;
    int i, j, k, l, b;
    bool found;
    int p_len = 0;
    int *path = NULL;
    int c_len = 0;
    int *closed = NULL;
    int o_len = 1;
    int *open = (int *)calloc(o_len, sizeof(int));
    float32 min, tempg;
    int s = startCase; //Start case
    int e = lastCase;  //End case
    int current = 0;
    int s_len = 0;
    struct stop *stops = NULL;
    int r_len = 0;
    struct route *routes = NULL;

    for (i = 0; i < map_size_rows; i++)
    {
        for (j = 0; j < map_size_cols; j++)
        {
            ind[i][j] = -1;
            mapData[i][j] = 0;
        }
    }

    for (i = 1; i < map_size_rows - 1; i++)
    {
        for (j = 1; j < map_size_cols - 1; j++)
        {
            ++s_len;
            if (!mapData[i][j])
            {

                stops = (struct stop *)realloc(stops, s_len * sizeof(struct stop));
                int t = s_len - 1;
                stops[t].col = j;
                stops[t].row = i;
                stops[t].from = -1;
                stops[t].g = DBL_MAX;
                stops[t].n_len = 0;
                stops[t].n = NULL;
                ind[i][j] = t;
            }
        }
    }

    for (i = 0; i < s_len; i++)
    {
        stops[i].h = sqrtf(powf(stops[e].row - stops[i].row, 2) + powf(stops[e].col - stops[i].col, 2));
    }

    for (i = 1; i < map_size_rows - 1; i++)
    {
        for (j = 1; j < map_size_cols - 1; j++)
        {
            if (ind[i][j] >= 0)
            {
                for (k = i - 1; k <= i + 1; k++)
                {
                    for (l = j - 1; l <= j + 1; l++)
                    {
                        if (k == i && l == j)
                        {
                            continue;
                        }
                        if (ind[k][l] >= 0)
                        {
                            ++r_len;
                            routes = (struct route *)realloc(routes, r_len * sizeof(struct route));
                            int t = r_len - 1;
                            routes[t].x = ind[i][j];
                            routes[t].y = ind[k][l];
                            routes[t].d = sqrtf(powf(stops[routes[t].y].row - stops[routes[t].x].row, 2) + powf(stops[routes[t].y].col - stops[routes[t].x].col, 2));
                            ++stops[routes[t].x].n_len;
                            stops[routes[t].x].n = (int *)realloc(stops[routes[t].x].n, stops[routes[t].x].n_len * sizeof(int));
                            stops[routes[t].x].n[stops[routes[t].x].n_len - 1] = t;
                        }
                    }
                }
            }
        }
    }

    open[0] = s;
    stops[s].g = 0;
    stops[s].f = stops[s].g + stops[s].h;
    found = false;

    while (o_len && !found)
    {
        min = DBL_MAX;

        for (i = 0; i < o_len; i++)
        {
            if (stops[open[i]].f < min)
            {
                current = open[i];
                min = stops[open[i]].f;
            }
        }

        if (current == e)
        {
            found = true;

            ++p_len;
            path = (int *)realloc(path, p_len * sizeof(int));
            path[p_len - 1] = current;
            while (stops[current].from >= 0)
            {
                current = stops[current].from;
                ++p_len;
                path = (int *)realloc(path, p_len * sizeof(int));
                path[p_len - 1] = current;
            }
        }

        for (i = 0; i < o_len; i++)
        {
            if (open[i] == current)
            {
                if (i != (o_len - 1))
                {
                    for (j = i; j < (o_len - 1); j++)
                    {
                        open[j] = open[j + 1];
                    }
                }
                --o_len;
                open = (int *)realloc(open, o_len * sizeof(int));
                break;
            }
        }

        ++c_len;
        closed = (int *)realloc(closed, c_len * sizeof(int));
        closed[c_len - 1] = current;

        for (i = 0; i < stops[current].n_len; i++)
        {
            b = 0;

            for (j = 0; j < c_len; j++)
            {
                if (routes[stops[current].n[i]].y == closed[j])
                {
                    b = 1;
                }
            }

            if (b)
            {
                continue;
            }

            tempg = stops[current].g + routes[stops[current].n[i]].d;

            b = 1;

            if (o_len > 0)
            {
                for (j = 0; j < o_len; j++)
                {
                    if (routes[stops[current].n[i]].y == open[j])
                    {
                        b = 0;
                    }
                }
            }

            if (b || (tempg < stops[routes[stops[current].n[i]].y].g))
            {
                stops[routes[stops[current].n[i]].y].from = current;
                stops[routes[stops[current].n[i]].y].g = tempg;
                stops[routes[stops[current].n[i]].y].f = stops[routes[stops[current].n[i]].y].g + stops[routes[stops[current].n[i]].y].h;

                if (b)
                {
                    ++o_len;
                    open = (int *)realloc(open, o_len * sizeof(int));
                    open[o_len - 1] = routes[stops[current].n[i]].y;
                }
            }
        }
    }

    putchar('\n');

    for (i = 0; i < map_size_rows; i++)
    {
        for (j = 0; j < map_size_cols; j++)
        {
            if (mapData[i][j])
            {
                putchar('0');
            }
            else
            {
                b = 0;
                for (k = 0; k < p_len; k++)
                {
                    if (ind[i][j] == path[k])
                    {
                        ++b;
                    }
                }
                if (b)
                {
                    putchar('x');
                }
                else
                {
                    putchar('.');
                }
            }
        }
        putchar('\n');
    }

    if (!found)
    {
        puts("IMPOSSIBLE");
    }
    else
    {
        printf("\npath cost is %d:\n", p_len);
        for (i = p_len - 1; i >= 0; i--)
        {
            printf("(%d, %d)\n", stops[path[i]].col, stops[path[i]].row);
        }
    }

    for (i = 0; i < s_len; ++i)
    {
        free(stops[i].n);
    }
    free(stops);
    free(routes);
    free(path);
    free(open);
    free(closed);

    return 0;
}*/