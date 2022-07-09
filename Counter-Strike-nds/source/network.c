// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "sounds.h"
#include "movements.h"
#include "grenade.h"
#include "gun.h"
#include "ui.h"
#include "collisions.h"
#include "network.h"
#include "party.h"
#include "player.h"
#include "security.h"

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Unused, the server list is not used anymore
char **allIps;
int ipCount = 0;
int TotalIpCount = 0;

// Current ping of the nintendo ds
int ping = 0;

// Socket
int my_socket = 0;

// Retain if the game needs to send a packet
bool SendPosition = false;
int SendPositionData = 3;
bool SendShoot = false;
bool SendLeave = false;
bool SendGrenade = false;
bool SendBombPlacing = false;
bool SendBombPlace = false;
bool SendBombDefused = false;
bool SendPing = false;
bool SendTeam = false;
bool WaitForTeamResponse = false;
bool SendBuyWeapon = false;
bool SendWallHit = false;
bool SendSelectedGun = false;
bool SendPlayerName = false;
bool SendVoteStartNow = false;
bool SendKeyResponse = false;
bool SendGetDroppedBomb = false;
bool SendReloaded = false;

// Retain if the player is just connected
bool firstConnection = true;

// For security, we need to retain the number of frame showed
int frameCount = 0;
// Timeout timer for the connection
int timeOut = 0;

// Join type to send to the server
enum JoinType partyOption = JOIN_RANDOM_PARTY;

// Party code to send to the server or to receive from the server
char partyCode[PARTY_CODE_LENGTH];

// Is the party private?
bool isPrivate = false;

/**
 * @brief Connect the game to a server, with an url.
 *
 *
 * @param url Url of the server
 * @param LocalMode Used to connect on a localserver on an android phone
 * @param my_socket socket
 * @param toGameServer (true when this is a gameserver. False to get the list of the game server, but not used here, always trye)
 * @param option (JOIN_RANDOM_PARTY, JOIN_PARTY_CODE, JOIN_PRIVATE_PARTY)
 */
void connectToServer(char *url, bool LocalMode, int my_socket, bool toGameServer, enum JoinType option)
{
    partyOption = option;
    // Prepare a request to send to the server
    char request_text[65] = "";

    // Request
    if (!toGameServer) // Request to get the list of game servers IPs
    {
        sprintf(request_text + strlen(request_text), "GET /server.php HTTP/1.1\r\nHost: fewnity.000webhostapp.com\r\n\r\n");
    }

    struct hostent *myhost;
    if (LocalMode)
    {
        // Get ip adress of the phone
        struct in_addr gateway;
        Wifi_GetIPInfo(&gateway, NULL, NULL, NULL);

        // Find the IP address of the server, with gethostbyname
        char GatewayChar[22] = "";
        sprintf(GatewayChar, "%s", inet_ntoa(gateway));
        myhost = gethostbyname(GatewayChar);
    }
    else // Find the IP address of the server, with gethostbyname
        myhost = gethostbyname(url);

    // Tell the socket to connect to the IP address we found, on port 80 (HTTP) or 1080 for android phone
    struct sockaddr_in sain;
    sain.sin_family = AF_INET;
    if (!LocalMode)
        sain.sin_port = htons(6003);
    else
        sain.sin_port = htons(1080);
    sain.sin_addr.s_addr = *((unsigned long *)(myhost->h_addr_list[0]));

    // Connect to the server
    connect(my_socket, (struct sockaddr *)&sain, sizeof(sain));

    // Set socked in non block mode
    int blockmode = 1;
    ioctl(my_socket, FIONBIO, &blockmode);

    resetNetworkVariables();

    // send our request
    // send(my_socket, request_text, strlen(request_text), 0);
    // Launch the read data service
    if (toGameServer)
        ReadServerData();
    else
        ReadServerListData();
}

/**
 * @brief Reset network send values variables (call before connecting to a new server)
 *
 */
void resetNetworkVariables()
{
    SendShoot = false;
    SendLeave = false;
    SendGrenade = false;
    SendBombPlacing = false;
    SendBombPlace = false;
    SendBombDefused = false;
    SendPing = false;
    SendTeam = false;
    WaitForTeamResponse = false;
    SendBuyWeapon = false;
    SendWallHit = false;
    SendSelectedGun = false;
    SendPlayerName = false;
    SendVoteStartNow = false;
    SendKeyResponse = false;
    SendGetDroppedBomb = false;
    SendReloaded = false;
}

/**
 * @brief Read incoming data from the server (UNUSED, MAY NOT WORK)
 *
 */
void ReadServerListData()
{
    // All temp variable for incoming data
    int recvd_len = 0;
    char incoming_buffer[64];

    char Values[1024] = "";
    char TempValues[1024] = "";

    printf("%d", recv(my_socket, incoming_buffer, 63, 0));

    // Read a maximum of 64 char in one loop
    while ((recvd_len = recv(my_socket, incoming_buffer, 63, 0)) != 0)
    {
        // if recv returns 0, the socket has been closed.
        if (recvd_len > 0)
        {
            incoming_buffer[recvd_len] = 0; // null-terminate
            printf("\n{%d %s}\n", recvd_len, incoming_buffer);

            // add incoming_buffer to Values
            strncat(Values, incoming_buffer, recvd_len);
            int StartPosition, EndPosition;

            // If a complete packet is detected
            while ((StartPosition = strstr(Values, "{") - Values + 1) > 0 && (EndPosition = strstr(Values + StartPosition, "}") - Values) > 0)
            {
                char currentPacket[128] = "";
                strncpy(currentPacket, Values + StartPosition, EndPosition - StartPosition);

                // Start spliting incoming data
                char *ptr = strtok(currentPacket, ";");
                int countI = 0;
                char arr[9][64] =
                    {"",
                     "",
                     "",
                     "",
                     "",
                     "",
                     "",
                     ""};

                // Split data
                while (ptr != NULL)
                {
                    strcpy(arr[countI], ptr);
                    countI++;
                    ptr = strtok(NULL, ";");
                }

                // Check packet info

                if (strcmp(arr[0], "IP") == 0) // Get ip
                {
                    printf("\nNEW IP : ");
                    printf(arr[1]);
                    // Put ip in ip list
                    strcpy(allIps[ipCount], arr[1]);
                    ipCount++;

                    // If where are getting the last ip, close the socket
                    if (ipCount == TotalIpCount)
                    {
                        // Close the socket
                        shutdown(my_socket, 0);
                        closesocket(my_socket);
                        return;
                    }
                }
                else if (strcmp(arr[0], "COUNT") == 0) // Get the server count
                {
                    // Read server count
                    sscanf(arr[1], "%d", &TotalIpCount);
                    // Free memory if the player has already download a list
                    if (allIps != NULL)
                    {
                        free(allIps);
                    }
                    // Allocate memory for the server list
                    allIps = malloc(TotalIpCount * sizeof(*allIps));
                    for (int i2 = 0; i2 < TotalIpCount; i2++)
                    {
                        allIps[i2] = malloc(sizeof(**allIps) * 64);
                    }
                }

                // Clear "TempValues"
                for (int i = 0; i < sizeof(TempValues); i++)
                    TempValues[i] = '\0';

                // Add all characters after current data packet to "TempValues"
                strcat(TempValues, Values + EndPosition + 1);
                // Copy "TempValues" to "Values"
                strcpy(Values, TempValues);
            }
        }
    }
}

/**
 * @brief Connect the game to a game server from the ip list (UNUSED, MAY NOT WORK)
 *
 * @param unused
 */
void connectFromServerList(int unused)
{
    closesocket(my_socket); // remove the socket
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    connectToServer(allIps[selectedServer], false, my_socket, true, 0);
}

/**
 * @brief Read data from the game server
 *
 */
void ReadServerData()
{
    // All temp variable for incoming data
    int recvd_len = 0;
    char incoming_buffer[64];

    char Values[256] = "";
    char TempValues[256] = "";

    // Read a maximum of 64 char in one loop
    while ((recvd_len = recv(my_socket, incoming_buffer, 63, 0)) != 0) // if recv returns 0, the socket has been closed. (Sometimes yes, sometimes not, lol)
    {
        if (recvd_len > 0)
        {
            timeOut = 0;
            incoming_buffer[recvd_len] = 0; // null-terminate

            // add incoming_buffer to Values
            strncat(Values, incoming_buffer, recvd_len);
            int StartPosition, EndPosition;

            // If a complete packet is detected
            while ((StartPosition = strstr(Values, "{") - Values + 1) > 0 && (EndPosition = strstr(Values + StartPosition, "}") - Values) > 0)
            {
                char currentPacket[128] = "";
                strncpy(currentPacket, Values + StartPosition, EndPosition - StartPosition);

                // Start spliting incoming data
                char *ptr = strtok(currentPacket, ";");
                int SplitCount = 0;
                char arr[10][64] =
                    {"",
                     "",
                     "",
                     "",
                     "",
                     "",
                     "",
                     "",
                     "",
                     ""};

                // Split data
                while (ptr != NULL)
                {
                    strcpy(arr[SplitCount], ptr);
                    SplitCount++;
                    ptr = strtok(NULL, ";");
                }

                // Check packet info

                if (strcmp(arr[REQUEST_NAME_INDEX], "POS") == 0) // Player position update
                {
                    // Get target player id
                    int PlayerIdInt = -1;
                    sscanf(arr[1], "%d", &PlayerIdInt);

                    float XFloat;
                    float YFloat;
                    float ZFloat;
                    int AngleInt = -1;
                    int CameraAngleInt = -1;

                    // Convert chars into floats
                    sscanf(arr[2], "%f", &XFloat);
                    sscanf(arr[3], "%f", &YFloat);
                    sscanf(arr[4], "%f", &ZFloat);
                    sscanf(arr[5], "%d", &AngleInt);
                    sscanf(arr[6], "%d", &CameraAngleInt);

                    // Change scale
                    XFloat /= 4096.0;
                    YFloat /= 4096.0;
                    ZFloat /= 4096.0;

                    // If new position is for the local player
                    if (localPlayer->Id == PlayerIdInt)
                    {
                        // Set new position and teleport player
                        localPlayer->position.x = XFloat;
                        localPlayer->position.y = YFloat;
                        localPlayer->position.z = ZFloat;
                        localPlayer->Angle = AngleInt;
                        localPlayer->PlayerPhysic->xspeed = 0;
                        localPlayer->PlayerPhysic->yspeed = 0;
                        localPlayer->PlayerPhysic->zspeed = 0;
                        NE_ModelSetCoord(localPlayer->PlayerModel, localPlayer->position.x, localPlayer->position.y, localPlayer->position.z);
                        ForceUpdateLookRotation(CameraAngleY);
                    }
                    else
                    {
                        // Find player with unique ID to update his informations
                        for (int i = 1; i < MaxPlayer; i++)
                            if (AllPlayers[i].Id == PlayerIdInt)
                            {
                                // If player has no position, teleport the player to new updated position (0 default position)
                                if (AllPlayers[i].lerpDestination.x == 0)
                                {
                                    AllPlayers[i].position.x = XFloat;
                                    AllPlayers[i].position.y = YFloat;
                                    AllPlayers[i].position.z = ZFloat;
                                    AllPlayers[i].Angle = AngleInt;
                                }

                                // Set player destination
                                AllPlayers[i].lerpDestination.x = XFloat;
                                AllPlayers[i].lerpDestination.y = YFloat;
                                AllPlayers[i].lerpDestination.z = ZFloat;

                                AllPlayers[i].AngleDestination = AngleInt;

                                if (CurrentCameraPlayer == i)
                                    CameraAngleY = CameraAngleInt;

                                break;
                            }
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "WALLHIT") == 0) // Wall hit position update for hit flash at wall animation
                {
                    int flashIndex = 0;
                    for (int i = 0; i < FLASH_MODELS_COUNT; i++)
                    {
                        if (ShowWallHitFlash[i] == 0)
                        {
                            flashIndex = i;
                            break;
                        }
                    }

                    // Set animation timer
                    ShowWallHitFlash[flashIndex] = 3;

                    float XFloat;
                    float YFloat;
                    float ZFloat;

                    // Convert chars into floats
                    sscanf(arr[1], "%f", &XFloat);
                    sscanf(arr[2], "%f", &YFloat);
                    sscanf(arr[3], "%f", &ZFloat);

                    // Change scale
                    XFloat /= 4096.0;
                    YFloat /= 4096.0;
                    ZFloat /= 4096.0;

                    // Set effect position
                    NE_ModelSetCoord(flashModels[flashIndex], XFloat, YFloat, ZFloat);

                    // Get wall hit flash direction
                    Vector2 Direction1D;
                    Direction1D.y = YFloat - AllPlayers[CurrentCameraPlayer].position.y - CameraOffsetY + y;
                    Direction1D.x = 1;
                    normalize2D(&Direction1D);

                    Vector3 Direction;
                    Direction.x = XFloat - AllPlayers[CurrentCameraPlayer].position.x;
                    Direction.y = YFloat - AllPlayers[CurrentCameraPlayer].position.y;
                    Direction.z = ZFloat - AllPlayers[CurrentCameraPlayer].position.z;
                    normalize(&Direction);

                    // Set wall hit angle
                    int FinalAngleY = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 384;
                    int FinalAngleZ = atan2f(Direction1D.y, 1) * 512.0 / (M_TWOPI) + 128;
                    NE_ModelSetRot(flashModels[flashIndex], 0, FinalAngleY, FinalAngleZ);

                    // Make 3D sound
                    int Panning, Volume;
                    Vector4 SoundPos;
                    SoundPos.x = XFloat;
                    SoundPos.y = YFloat;
                    SoundPos.z = ZFloat;
                    GetPanningByPosition(&Panning, &Volume, SoundPos, xWithoutYForAudio, zWithoutYForAudio, 0.15);
                    Play3DSound(SFX_RIC, Volume, Panning, NULL);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SHOOT") == 0) // Shoot from a player
                {
                    // Get player id
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    // Get gun id
                    int ParsedGunId = EMPTY;
                    sscanf(arr[2], "%d", &ParsedGunId);

                    // Make a sound
                    int Panning, Volume;
                    GetPanning(ParsedPlayerId, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, AllGuns[ParsedGunId].MaxSoundDistance);
                    Player *player = NULL;
                    int index = UNUSED;
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            player = &AllPlayers[i];
                            index = i;
                            break;
                        }
                    }
                    if (player != NULL)
                    {
                        if (player->currentGunInInventory == 1 || player->currentGunInInventory == 2)
                            player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount--;

                        if (CurrentCameraPlayer == index)
                        {
                            setGunRecoil(player);
                        }

                        if (ParsedGunId < GunCount)
                            Play3DSound(AllGuns[ParsedGunId].gunSound, Volume, Panning, player);
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "TimerA") == 0) // Timer changes
                {
                    // Parse seconds and minutes texts to int
                    sscanf(arr[1], "%d", &PartyMinutes);
                    sscanf(arr[2], "%d", &PartySeconds);
                    if (firstConnection)
                    {
                        BombSeconds = PartySeconds;
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "PING") == 0) // Get ping request from server
                {
                    // Parse ping text to int
                    sscanf(arr[1], "%d", &ping);
                    SendPing = true;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "CURGUN") == 0) // Set current gun in inventory
                {
                    int PlayerIdInt = UNUSED;
                    sscanf(arr[1], "%d", &PlayerIdInt);

                    // Find player to set health
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == PlayerIdInt)
                        {
                            int newGun = 0;
                            sscanf(arr[2], "%d", &newGun);
                            AllPlayers[i].currentGunInInventory = newGun;
                            if (i == CurrentCameraPlayer)
                                UpdateGunTexture();

                            break;
                        }
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "HITSOUND") == 0) // Make a hit sound
                {
                    int PlayerId, HitType;

                    sscanf(arr[2], "%d", &PlayerId);
                    sscanf(arr[3], "%d", &HitType);

                    int Panning, Volume;
                    if (PlayerId != localPlayer->Id)
                        GetPanning(PlayerId, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);
                    else
                    {
                        Panning = 128;
                        Volume = 255;

                        xOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
                        if (rand() % 2 == 0)
                            xOffset = -xOffset;

                        yOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
                        if (rand() % 2 == 0)
                            yOffset = -yOffset;

                        redHealthTextCounter = 62;
                    }

                    Player *player = NULL;
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == PlayerId)
                        {
                            player = &AllPlayers[i];
                            break;
                        }
                    }

                    if (HitType == 0)
                        Play3DSound(SFX_FLESH_IMPACT, Volume, Panning, player); // Check with kevlar
                    else if (HitType == 1)
                        Play3DSound(SFX_HEADSHOT1, Volume, Panning, player);
                    else if (HitType == 2)
                        Play3DSound(SFX_FLESH_IMPACT, Volume, Panning, player);
                    else
                        Play3DSound(SFX_KNIFE_HIT_PLAYER, Volume, Panning, player);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETID") == 0) // Add player
                {
                    int ParsedId = -1;
                    sscanf(arr[1], "%d", &ParsedId);

                    // Spawn player
                    AddNewPlayer(ParsedId, true, false);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETMAP") == 0) // Set the map
                {
                    int newMapId = -1;
                    // Parse seconds and minutes texts to int
                    sscanf(arr[1], "%d", &newMapId);

                    UnLoadMap(currentMap);
                    currentMap = newMapId;
                    LoadMap(currentMap);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETMODE") == 0) // Set the party mode
                {
                    // Parse seconds and minutes texts to int
                    sscanf(arr[1], "%d", &currentPartyMode);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "TEAM") == 0) // Change team
                {
                    // Get player id
                    int ParsedPlayerId = -1;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    // Get Team value
                    int ParsedIsCounter = -1;
                    sscanf(arr[2], "%d", &ParsedIsCounter);

                    // Find player and affect new value
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            AllPlayers[i].Team = ParsedIsCounter;

                            UpdatePlayerTexture(i);
                            break;
                        }

                    // Update screen if team screen is opened
                    if (currentMenu == SCORE_BOARD)
                    {
                        UpdateBottomScreenFrameCount += 8;
                    }
                    else if (currentMenu == GAME)
                    {
                        initGameMenu();
                    }

                    // TODO check this
                    if (localPlayer->Id == ParsedPlayerId)
                    {
                        if (roundState != TRAINING)
                            changeCameraPlayerView(false);

                        if (ParsedIsCounter != -1)
                        {
                            AllButtons[0].isHidden = false;
                        }
                        AllButtons[1].isHidden = false;
                        AllButtons[2].isHidden = false;
                        WaitForTeamResponse = false;
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "BOMBPLACE") == 0) // Bomb place update
                {
                    // Temp variables
                    int XInt = -1;
                    int YInt = -1;
                    int ZInt = -1;
                    int AngleInt = -1;
                    int dropped = -1;

                    // Convert chars into ints
                    sscanf(arr[1], "%d", &XInt);
                    sscanf(arr[2], "%d", &YInt);
                    sscanf(arr[3], "%d", &ZInt);
                    sscanf(arr[4], "%d", &AngleInt);
                    sscanf(arr[5], "%d", &dropped);

                    if (dropped == 1)
                    {
                        // Change values scale and set bomb position and angle value
                        droppedBombPositionAndRotation.x = XInt / 4096.0;
                        droppedBombPositionAndRotation.y = YInt / 4096.0;
                        droppedBombPositionAndRotation.z = ZInt / 4096.0;
                        droppedBombPositionAndRotation.r = AngleInt;

                        // Set bomb 3D model position and angle value
                        NE_ModelSetCoord(Model[7], droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.y, droppedBombPositionAndRotation.z);
                        // NE_ModelSetCoord(Model[10], droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.y, droppedBombPositionAndRotation.z);
                        Model[7]->rz = droppedBombPositionAndRotation.r;

                        SetBombTakingZone(droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.z, &bombDefuseZone); // Set zone for taking the bomb
                        bombDropped = true;
                    }
                    else
                    {
                        // Change values scale and set bomb position and angle value
                        BombPosition.x = XInt / 4096.0;
                        BombPosition.y = YInt / 4096.0;
                        BombPosition.z = ZInt / 4096.0;
                        BombPosition.r = AngleInt;

                        // Set bomb 3D model position and angle value
                        NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
                        NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
                        Model[7]->rz = BombPosition.r;

                        BombSeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;

                        bombBipTimer = 120;
                        BombPlanted = true;
                        SetBombDefuseZone(BombPosition.x, BombPosition.z, &bombDefuseZone);
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "VOTERESULT") == 0) // Get the current vote values
                {
                    int VoteTypeInt = -1;
                    sscanf(arr[1], "%d", &VoteTypeInt);
                    if (VoteTypeInt == 0)
                    {
                        // int ResultInt = -1;
                        sscanf(arr[2], "%d", &playerWantToStart);
                        sscanf(arr[3], "%d", &playerWantToStartLimite);
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "CONFIRM") == 0) // Confirm shop gun buy, or set player gun from server command
                {
                    int ConfirmTypeInt = -1;
                    int ConfirmArgumentInt = -1;
                    int ConfirmInventoryIndexInt = -1;
                    int ConfirmResultInt = -1; // 0 Error, 1 Okay

                    // Convert chars into ints
                    sscanf(arr[1], "%d", &ConfirmTypeInt);
                    sscanf(arr[2], "%d", &ConfirmArgumentInt);
                    sscanf(arr[3], "%d", &ConfirmInventoryIndexInt);
                    sscanf(arr[4], "%d", &ConfirmResultInt);

                    if (ConfirmTypeInt == 0) // Set gun in inventory
                    {
                        if (ConfirmResultInt == 0) // Error
                        {
                        }
                        else if (ConfirmResultInt == 1) // Okay
                        {
                            DisableAim();
                            SetGunInInventory(ConfirmArgumentInt, ConfirmInventoryIndexInt);
                            setSelectedGunInInventory(0, ConfirmInventoryIndexInt);
                        }
                        else if (ConfirmResultInt == 2) // Not an error
                        {
                        }
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETMONEY") == 0) // Set player money
                {
                    // Parse money text to int
                    int ParsedMoney = -1;
                    sscanf(arr[1], "%d", &ParsedMoney);
                    setPlayerMoney(0, ParsedMoney);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SCORE") == 0) // Party score changes
                {
                    // Parse scores texts to int
                    sscanf(arr[1], "%d", &CounterScore);
                    sscanf(arr[2], "%d", &TerroristsScore);

                    // Update screen if team screen is opened
                    if (currentMenu == SCORE_BOARD)
                        UpdateBottomScreenFrameCount += 8;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SCRBOARD") == 0) // Score board score changes
                {
                    // Get player id
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    // Find player with unique ID to update his informations
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            sscanf(arr[2], "%d", &AllPlayers[i].KillCount);
                            sscanf(arr[3], "%d", &AllPlayers[i].DeathCount);
                            break;
                        }

                    // Update screen if team screen is opened
                    if (currentMenu == SCORE_BOARD)
                        UpdateBottomScreenFrameCount += 8;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETSHOPZONE") == 0) // Update the shop zone
                {
                    shopDisableTimer = SHOP_DISABLE_TIMER;
                    setShopZone(localPlayer);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "PartyRound") == 0) // Round state changes
                {
                    // Parse text to int
                    int tempRoundState = -1;
                    sscanf(arr[1], "%d", &tempRoundState);
                    roundState = tempRoundState;

                    // Reset some values on new round
                    if (roundState == WAIT_START)
                    {
                        onNewRoundStart();
                        NE_SpriteVisible(TopScreenSprites[0], true);

                        StopReloading(0);
                        ResetGunsAmmo(0);
                        if (CurrentCameraPlayer != 0)
                        {
                            SetCurrentCameraPlayer(0);
                        }
                        for (int i = 0; i < MaxPlayer; i++)
                            AllPlayers[i].IsDead = FALSE;

                        deathCameraAnimation = 0;
                        deathCameraYOffset = 0;
                        redHealthTextCounter = 0;
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "LEAVE") == 0) // When a player leave
                {
                    // Get player id
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    // Find player
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            AllPlayers[i].Id = UNUSED;
                            PlayerCount--;
                            NE_ModelDelete(AllPlayers[i].PlayerModel);
                            if (i == 0)
                            {
                                NE_PhysicsDelete(AllPlayers[i].PlayerPhysic);
                            }
                            else
                            {
                                showDisconnectedText(i);
                            }
                            AllPlayers[i].KillCount = 0;
                            AllPlayers[i].Team = SPECTATOR;
                            AllPlayers[i].DeathCount = 0;
                            if (currentMenu == SCORE_BOARD)
                                initScoreMenu();

                            if (CurrentCameraPlayer == i)
                            {
                                SetCurrentCameraPlayer(0);
                            }

                            break;
                        }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETHEALTH") == 0) // Set player health
                {
                    int PlayerIdInt = UNUSED;
                    sscanf(arr[1], "%d", &PlayerIdInt);

                    // Find player to set health
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == PlayerIdInt)
                        {
                            int newHealth = 0;
                            sscanf(arr[2], "%d", &newHealth);
                            setPlayerHealth(i, newHealth);
                            sscanf(arr[3], "%d", &AllPlayers[i].armor);

                            int haveHeadset = 0;
                            sscanf(arr[4], "%d", &haveHeadset);
                            AllPlayers[i].haveHeadset = haveHeadset == 1 ? true : false;
                            break;
                        }
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETBOMB") == 0) // Set/remove the bomb of a player
                {
                    // Get player id
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    // Get gun id
                    int haveBomb = -1;
                    sscanf(arr[2], "%d", &haveBomb);

                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == ParsedPlayerId && haveBomb == 1)
                        {
                            AllPlayers[i].haveBomb = true;
                            bombDropped = false;
                            SetGunInInventoryForNonLocalPlayer(i, 28, 8);
                        }
                        else
                        {
                            AllPlayers[i].haveBomb = false;
                            SetGunInInventoryForNonLocalPlayer(i, EMPTY, 8);
                        }
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "RELOADED") == 0) // Reload the gun of a player
                {
                    // Get player id
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    int Reset = 0;
                    sscanf(arr[2], "%d", &Reset);

                    if (Reset == 0)
                    {
                        ReloadGun(ParsedPlayerId);
                    }
                    else if (Reset == 1)
                    {
                        ResetGunsAmmo(ParsedPlayerId);
                    }
                    else if (Reset == 2)
                    {
                        int GunId = 0;
                        sscanf(arr[4], "%d", &GunId);
                        ResetGunAmmo(ParsedPlayerId, GunId);
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETNAME") == 0) // Get a player name
                {
                    // Get player id
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            strcpy(AllPlayers[i].name, arr[2]);
                            if (i != 0 && !firstConnection)
                                showConnectedText(i);
                            break;
                        }
                    }

                    // Update screen if team screen is opened
                    if (currentMenu == 2)
                        UpdateBottomScreenFrameCount += 8;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "SETCODE") == 0) // Get the party code
                {
                    strcpy(partyCode, arr[1]);
                    isPrivate = true;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "ERROR") == 0) // Show text from server
                {
                    int errorId = EMPTY;
                    sscanf(arr[1], "%d", &errorId);
                    if (errorId == Ban)
                    {
                        strncpy(errorText, "You are banned!", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == WrongVersion)
                    {
                        strncpy(errorText, "You are using an old version of the game, please update your game.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == MacAddressMissing)
                    {
                        strncpy(errorText, "Your Nintendo DS doesn't have a MAC address.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == WrongSecurityKey)
                    {
                        strncpy(errorText, "You are using a modified game.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == ServerFull)
                    {
                        strncpy(errorText, "The server is currently full, please try again later.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == ServerStopped)
                    {
                        strncpy(errorText, "The server is currently under maintenance.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == SaveCorrupted)
                    {
                        strncpy(errorText, "Your save is corrupted.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == IncorrectCode)
                    {
                        strncpy(errorText, "The code is incorrect.", ERROR_TEXT_LENGTH);
                    }
                    else if (errorId == KickTeamKill)
                    {
                        strncpy(errorText, "You have been kicked (Team kill).", ERROR_TEXT_LENGTH);
                    }
                    else
                    {
                        strncpy(errorText, "An error has occurred!", ERROR_TEXT_LENGTH);
                    }

                    initOnlineErrorMenu();
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "TEXT") == 0) // Show text from server
                {
                    int ParsedTextId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedTextId);

                    showPartyEventText(ParsedTextId);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "TEXTPLAYER") == 0) // Show text about a player from server
                {
                    int ParsedPlayerId = NO_PLAYER;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    int KillerIdInt = NO_PLAYER;
                    int TextIdInt = NO_PLAYER;

                    sscanf(arr[2], "%d", &KillerIdInt);
                    sscanf(arr[3], "%d", &TextIdInt);

                    int killerPlayerIndex = NO_PLAYER;
                    int killedPlayerIndex = NO_PLAYER;

                    if (KillerIdInt != NO_PLAYER)
                    {
                        // Find player with unique ID to update his informations
                        for (int i = 0; i < MaxPlayer; i++)
                        {
                            if (AllPlayers[i].Id == KillerIdInt)
                            {
                                killerPlayerIndex = i;
                                break;
                            }
                        }
                    }

                    if (ParsedPlayerId != NO_PLAYER)
                    {
                        for (int i = 0; i < MaxPlayer; i++)
                        {
                            if (AllPlayers[i].Id == ParsedPlayerId)
                            {
                                killedPlayerIndex = i;
                                break;
                            }
                        }
                    }

                    if (killerPlayerIndex != NO_PLAYER && killerPlayerIndex != NO_PLAYER)
                        showKillText(killerPlayerIndex, killedPlayerIndex);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "BOMBPLACING") == 0) // Bomb placing sound
                {
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);

                    int Panning, Volume;
                    GetPanning(ParsedPlayerId, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);

                    Play3DSound(SFX_BOMBPLANTING, Volume, Panning, GetPlayer(ParsedPlayerId));
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "BOMBDEFUSE") == 0) // Bomb defuse sound
                {
                    BombDefused = true;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "ADDRANGE") == 0) // Add multiples non local players
                {
                    for (int i = 1; i < SplitCount; i++)
                    {
                        int FoundId = -1;
                        sscanf(arr[i], "%d", &FoundId);
                        AddNewPlayer(FoundId, false, false);
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "ENDGAME") == 0) // End the party
                {
                    partyFinished = true;
                    initFinalScoreMenu();
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "ENDUPDATE") == 0) // End the update party sequence
                {
                    firstConnection = false;
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "INVTORY") == 0) // Get the inventory of a player
                {
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);
                    int FoundPlayer = UNUSED;
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            FoundPlayer = i;
                            for (int i2 = 2; i2 < SplitCount; i2++)
                            {
                                int FoundGunId = EMPTY;
                                sscanf(arr[i2], "%d", &FoundGunId);
                                SetGunInInventoryForNonLocalPlayer(i, FoundGunId, i2 - 2);
                            }
                            break;
                        }
                    }
                    // If the current player camera is looked by the local player, update the gun texture
                    if (FoundPlayer == CurrentCameraPlayer)
                    {
                        UpdateGunTexture();
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "AMMO") == 0) // Get the ammo count of a player
                {
                    int ParsedPlayerId = UNUSED;
                    sscanf(arr[1], "%d", &ParsedPlayerId);
                    int FoundPlayer = UNUSED;
                    for (int playerIndex = 0; playerIndex < MaxPlayer; playerIndex++)
                    {
                        if (AllPlayers[playerIndex].Id == ParsedPlayerId)
                        {
                            FoundPlayer = playerIndex;
                            for (int i2 = 0; i2 < 2; i2++)
                            {
                                int AmmoCountInt = 0;
                                sscanf(arr[2 + i2 * 2], "%d", &AmmoCountInt);
                                int TotalAmmoCountInt = 0;
                                sscanf(arr[2 + i2 * 2 + 1], "%d", &TotalAmmoCountInt);

                                AllPlayers[FoundPlayer].AllAmmoMagazine[i2].AmmoCount = AmmoCountInt;
                                AllPlayers[FoundPlayer].AllAmmoMagazine[i2].TotalAmmoCount = TotalAmmoCountInt;
                            }
                            break;
                        }
                    }
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "KEY") == 0) // Get the key from the server
                {
                    SendKeyResponse = true;
                    sscanf(arr[1], "%d", &serverKey);
                }
                else if (strcmp(arr[REQUEST_NAME_INDEX], "GRENADE") == 0) // Spawn a player's grenade
                {
                    // Temp variables
                    float XDirection = -1;
                    float YDirection = -1;
                    float ZDirection = -1;
                    int XInt = -1;
                    int YInt = -1;
                    int ZInt = -1;
                    int GrenadeType = -1;

                    // Convert chars into ints
                    sscanf(arr[1], "%f", &XDirection);
                    sscanf(arr[2], "%f", &YDirection);
                    sscanf(arr[3], "%f", &ZDirection);

                    sscanf(arr[4], "%d", &XInt);
                    sscanf(arr[5], "%d", &YInt);
                    sscanf(arr[6], "%d", &ZInt);
                    sscanf(arr[7], "%d", &GrenadeType);

                    // TODO add player id replace to 0
                    PhysicalGrenade *newGrenade = CreateGrenade(GrenadeType, 0);
                    if (newGrenade != NULL)
                    {
                        lanchGrenade(newGrenade, XDirection, YDirection, ZDirection, XInt, YInt, ZInt);
                    }
                }

                // Clear "TempValues"
                for (int i = 0; i < sizeof(TempValues); i++)
                    TempValues[i] = '\0';

                // Add all characters after current data packet to "TempValues"
                strcat(TempValues, Values + EndPosition + 1);
                // Copy "TempValues" to "Values"
                strcpy(Values, TempValues);
            }
        }
        else if (recvd_len == -1)
        {
            // If the recv() returns -1 for too long, the socket has been closed.
            timeOut++;
            if (timeOut == 400)
            {
                break;
            }
        }

        if (localPlayer->Id != UNUSED)
        {
            // Play game code
            GameLoop();
        }

        UpdateEngine();

        // Check if position need to be updated for other player
        if (localPlayer->position.x != OldxPos || localPlayer->position.y != OldyPos || localPlayer->position.z != OldzPos)
        {
            OldxPos = localPlayer->position.x;
            OldyPos = localPlayer->position.y;
            OldzPos = localPlayer->position.z;
            SendPosition = true;
        }

        char InfoToSend[1024] = "";

        // Send shoot data for sound/animation for clients
        if (SendLeave)
        {
            // sprintf(InfoToSend + strlen(InfoToSend), "{LEAVE}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", LEAVE);
            SendLeave = false;
        }

        // Send position one in two
        // SendPositionData = !SendPositionData;
        if (SendPositionData > 0)
            SendPositionData--;

        // If local player need to be updated for other player (59 bytes)
        if (localPlayer->Id != -1 && SendPosition && SendPositionData == 0)
        {
            // sprintf(InfoToSend + strlen(InfoToSend), "{POS;%d;%d;%d;%d;%0.0f}", localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z, (int)localPlayer->Angle, CameraAngleY);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d;%d;%d;%d;%0.0f}", POS, localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z, (int)localPlayer->Angle, CameraAngleY);

            SendPosition = false;
            SendPositionData = 4;
        }

        // Send grenade launch
        if (SendGrenade)
        {
            // sprintf(InfoToSend + strlen(InfoToSend), "{GRENADE;%f;%f;%f}", x, y, z);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%f;%f;%f}", GRENADE, x, y, z);
            SendGrenade = false;
        }

        // Send bomb placement data
        if (SendBombPlace)
        {
            SendBombPlace = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{BOMBPLACE;%d;%d;%d;%d}", (int)(BombPosition.x * 4096.0), (int)(BombPosition.y * 4096.0), (int)(BombPosition.z * 4096.0), BombPosition.r);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d;%d;%d;%d}", BOMBPLACE, (int)(BombPosition.x * 4096.0), (int)(BombPosition.y * 4096.0), (int)(BombPosition.z * 4096.0), BombPosition.r);
        }

        // Send bomb defuse data
        if (SendBombDefused)
        {
            SendBombDefused = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{BOMBDEFUSE}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", BOMBDEFUSE);
        }

        // Send bomb planting data
        if (SendBombPlacing)
        {
            SendBombPlacing = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{BOMBPLACING}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", BOMBPLACING);
        }

        // Send team data
        if (SendTeam)
        {
            SendTeam = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{TEAM;%d}", tempTeam);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d}", TEAM, tempTeam);
            tempTeam = -1;
        }

        // Send ping reply
        if (SendPing)
        {
            SendPing = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{PING}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", PING);
        }

        // Send buy weapon request data
        if (SendBuyWeapon)
        {
            SendBuyWeapon = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{BUY;%d}", GetSelectedGunShop());
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d}", BUY, GetSelectedGunShop());
        }

        // Send gun reloaded data
        if (SendReloaded)
        {
            SendReloaded = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{RELOADED}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", RELOADED);
        }

        // Send selected gun
        if (SendSelectedGun)
        {
            SendSelectedGun = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{CURGUN;%d}", localPlayer->currentGunInInventory);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d}", CURGUN, localPlayer->currentGunInInventory);
        }

        // Send shoot data for sound/animation for clients
        if (SendShoot)
        {
            // sprintf(InfoToSend + strlen(InfoToSend), "{SHOOT}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", SHOOT);
            SendShoot = false;
        }

        // Check if the game needs to send hitted clients
        bool sendHitClient = false;
        for (int i = 0; i < FLASH_MODELS_COUNT; i++)
        {
            if (hittedClient[i] != NO_PLAYER)
            {
                sendHitClient = true;
                break;
            }
        }

        // Send hit data
        if (sendHitClient)
        {
            // sprintf(InfoToSend + strlen(InfoToSend), "{HIT");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d", HIT);
            for (int i = 0; i < FLASH_MODELS_COUNT; i++)
            {
                if (hittedClient[i] != -1)
                {
                    sprintf(InfoToSend + strlen(InfoToSend), ";%d;%d;%d;%f", AllPlayers[hittedClient[i]].Id, localPlayer->IsHeadShot[i], localPlayer->IsLegShot[i], hitDistance[i]);
                    hittedClient[i] = NO_PLAYER;
                }
            }
            sprintf(InfoToSend + strlen(InfoToSend), "}");
        }

        // Send vote
        if (SendVoteStartNow)
        {
            SendVoteStartNow = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{VOTE;0}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;0}", VOTE);
        }

        // Send wall hit data
        if (SendWallHit)
        {
            SendWallHit = false;
            for (int i = 0; i < getPlayerCurrentGun(localPlayer).bulletCountPerShoot; i++)
            {
                // sprintf(InfoToSend + strlen(InfoToSend), "{WALLHIT;%d;%d;%d}", WallHitXPos[i], WallHitYPos[i], WallHitZPos[i]);
                sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d;%d;%d}", WALLHIT, WallHitXPos[i], WallHitYPos[i], WallHitZPos[i]);
            }
        }

        // Send local player name
        if (SendPlayerName)
        {
            SendPlayerName = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{SETNAME;%s}", localPlayer->name);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%s}", SETNAME, localPlayer->name);
        }

        // Send security key response
        if (SendKeyResponse)
        {
            SendKeyResponse = false;

            u8 macAddress[6];
            Wifi_GetData(WIFIGETDATA_MACADDRESS, 6, macAddress);

            // sprintf(InfoToSend + strlen(InfoToSend), "{KEY;%d;%X%X%X%X%X%X;%s;%s}", getKeyResponse(serverKey), macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5], localPlayer->name, GAME_VERSION);
            sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d;%02X%02X%02X%02X%02X%02X;%s;%s}", KEY, getKeyResponse(serverKey), macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5], localPlayer->name, GAME_VERSION);

            if (partyOption == JOIN_PRIVATE_PARTY)
                // sprintf(InfoToSend + strlen(InfoToSend), "{PARTY;%d;%s}", partyOption, partyCode);
                sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d;%s}", PARTY, partyOption, partyCode);
            else
                // sprintf(InfoToSend + strlen(InfoToSend), "{PARTY;%d}", partyOption);
                sprintf(InfoToSend + strlen(InfoToSend), "{%d;%d}", PARTY, partyOption);
        }

        // Send get bomb dropped event
        if (SendGetDroppedBomb)
        {
            SendGetDroppedBomb = false;

            // sprintf(InfoToSend + strlen(InfoToSend), "{GETBOMB}");
            sprintf(InfoToSend + strlen(InfoToSend), "{%d}", GETBOMB);
        }

        // Get string data length
        int InfoLentgh = strlen(InfoToSend);
        if (InfoLentgh != 0)
        {
            // Send the current frame
            char tmp[20];
            // sprintf(tmp, "{FRAME;%d}", frameCount);
            sprintf(tmp, "{%d;%d}", FRAME, frameCount);
            send(my_socket, tmp, strlen(tmp), 0); // Send data to server

            send(my_socket, InfoToSend, InfoLentgh, 0); // Send data to server
        }
        // sendto(my_socket, InfoToSend, InfoLentgh, 0, (struct sockaddr *)&sain, sizeof(sain)); //UDP
    }
    shutdown(my_socket, 0); // good practice to shutdown the socket.
    closesocket(my_socket);
    Connection = UNSELECTED;
    if (!partyFinished && currentMenu != ONLINE_ERROR)
    {
        initMainMenu();
    }
    partyFinished = false;
    isPrivate = false;
    firstConnection = true;
    OnPartyQuit();
}

/**
 * @brief Parse a string to an int
 *
 * @param charToParse
 * @return int
 */
int IntParse(char *charToParse)
{
    int result = EMPTY;
    sscanf(charToParse, "%d", &result);
    return result;
}

/**
 * @brief Get player from id
 *
 * @param id
 * @return Player*
 */
Player *GetPlayer(int id)
{
    for (int i = 0; i < MaxPlayer; i++)
    {
        if (AllPlayers[i].Id == id)
            return &AllPlayers[i];
    }
    return NULL;
}