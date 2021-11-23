#include "main.h"
#include "sounds.h"
#include "movements.h"
#include "grenade.h"
#include "gun.h"
#include "ui.h"
#include "collisions.h"
#include "network.h"

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char **allIps;
int ipCount = 0;
int TotalIpCount = 0;

int ping;

// Socket
int my_socket;

// Write part
bool SendPosition;
int SendPositionData = 3;
bool SendStepSound;
bool SendShoot;
bool SendGrenade;
bool SendBombPlacing;
bool SendBombPlace;
bool SendBombDefused;
bool SendPing;
bool SendTeam;
bool WaitForTeamResponse;
bool SendBuyWeapon;
bool SendWallHit;
bool SendSelectedGun;

// Connect the game to a server, with an url.
// LocalMode is used to connect on a localserver on an android phone
// toGameServer is true when this is a gameserver. False for others, to get the list of the game server for example
void connectToServer(char *url, bool LocalMode, int my_socket, bool toGameServer)
{
    // Prepare a request to send to the server
    char request_text[65] = "";

    // Request for a game server
    if (toGameServer)
    {
        // Get mac adress
        u8 macAddress[6];
        Wifi_GetData(WIFIGETDATA_MACADDRESS, 6, macAddress);
        sprintf(request_text, "{-4;MAC;%X%X%X%X%X%X}", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
        sprintf(request_text + strlen(request_text), "{-4;LOADED;}");
    }
    else // Request to get the list of game server ips
    {
        sprintf(request_text + strlen(request_text), "GET /addServer.php HTTP/1.1\r\nHost: fewnity.000webhostapp.com\r\n\r\n");
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
        // printf("Gateway      : %s\n", inet_ntoa(gateway));
    }
    else // Find the IP address of the server, with gethostbyname
        myhost = gethostbyname(url);

    // printf("Found IP Address!\n");

    // Create a TCP socket
    // my_socket = socket(AF_INET, SOCK_STREAM, 0);

    // printf("Created Socket!\n");

    // Tell the socket to connect to the IP address we found, on port 80 (HTTP) or 1080 for android phone
    struct sockaddr_in sain;
    sain.sin_family = AF_INET;
    if (!LocalMode)
        sain.sin_port = htons(80);
    else
        sain.sin_port = htons(1080);
    sain.sin_addr.s_addr = *((unsigned long *)(myhost->h_addr_list[0]));

    // Connect to the server
    connect(my_socket, (struct sockaddr *)&sain, sizeof(sain));

    // Set socked in non block mode
    int blockmode = 1;
    ioctl(my_socket, FIONBIO, &blockmode);

    printf("Connected to server!\n");

    // send our request
    send(my_socket, request_text, strlen(request_text), 0);
    // Launch the read data service
    if (toGameServer)
        ReadServerData();
    else
        ReadServerListData();
}

// Read incoming data from the server
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
                printf("\nNEW PACKET : ");
                printf(currentPacket);
                printf(" END");

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
                        /*for (int i2 = 0; i2 < TotalIpCount; i2++)
                        {
                            printf("\n");
                            printf("IP : ");
                            printf(allIps[i2]);
                        }*/

                        // Close the socket
                        shutdown(my_socket, 0);
                        closesocket(my_socket);
                        return;
                    }

                    /*shutdown(my_socket, 0); // good practice to shutdown the socket.
                    closesocket(my_socket); // remove the socket
                    my_socket = socket(AF_INET, SOCK_STREAM, 0);
                    //connectToServer(arr[1], false, my_socket, true); //Mère
                    connectToServer("88.127.205.17", false, my_socket, true); //Mère*/

                    // return;
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

// Connect the game to a game server from the ip list
void connectFromServerList(int unused)
{
    closesocket(my_socket); // remove the socket
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    connectToServer(allIps[selectedServer], false, my_socket, true);
}

void ReadServerData()
{
    // All temp variable for incoming data
    int recvd_len = 0;
    char incoming_buffer[64];

    char Values[256] = "";
    char TempValues[256] = "";

    // Read a maximum of 64 char in one loop
    while ((recvd_len = recv(my_socket, incoming_buffer, 63, 0)) != 0)
    {
        // if recv returns 0, the socket has been closed.
        if (recvd_len > 0)
        {
            incoming_buffer[recvd_len] = 0; // null-terminate
            // printf("\n{%d %s}\n", recvd_len, incoming_buffer);

            // add incoming_buffer to Values
            strncat(Values, incoming_buffer, recvd_len);
            int StartPosition, EndPosition;

            // If a complete packet is detected
            while ((StartPosition = strstr(Values, "{") - Values + 1) > 0 && (EndPosition = strstr(Values + StartPosition, "}") - Values) > 0)
            {
                char currentPacket[128] = "";
                strncpy(currentPacket, Values + StartPosition, EndPosition - StartPosition);
                printf("NEW PACKET : \n");
                printf(currentPacket);
                printf("\n");

                // Start spliting incoming data
                char *ptr = strtok(currentPacket, ";");
                int countI = 0;
                char arr[10][64] =
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
                    // printf(arr[countI]);
                    // printf("\n");

                    countI++;
                    ptr = strtok(NULL, ";");
                }

                // Check packet info

                if (strcmp(arr[1], "SETID") == 0 || strcmp(arr[1], "ADDID") == 0) // Add player
                {
                    int ParsedId = -1;
                    sscanf(arr[2], "%d", &ParsedId);

                    // Spawn player
                    AddNewPlayer(ParsedId, strstr(arr[1], "SETID"), false);
                }
                else if (strcmp(arr[1], "TimerA") == 0) // Timer changes
                {
                    // Parse seconds and minutes texts to int
                    sscanf(arr[2], "%d", &PartyMinutes);
                    sscanf(arr[3], "%d", &PartySeconds);
                }
                else if (strcmp(arr[1], "TEAM") == 0) // Change team
                {
                    // Get player id
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    // Get IsCounter value
                    int ParsedIsCounter = -1;
                    sscanf(arr[2], "%d", &ParsedIsCounter);

                    // Find player and affect new value
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            AllPlayers[i].IsCounter = ParsedIsCounter;
                            UpdatePlayerTexture(i);
                            break;
                        }

                    // if (currentMenu == 2 && UpdateBottomScreenOneFrame == 0) //Score button (Main menu)
                    // UpdateBottomScreenOneFrame = 4;
                    // UpdateBottomScreenOneFrame = 8;

                    // Update screen if team screen is opened
                    if (currentMenu == 2)
                        UpdateBottomScreenOneFrame += 8;

                    // TODO check this
                    if (localPlayer->Id == ParsedPlayerId)
                    {
                        if (ParsedIsCounter != -1)
                        {
                            AllButtons[0].isHidden = false;
                            AllButtons[1].isHidden = false;
                            AllButtons[2].isHidden = false;

                            WaitForTeamResponse = false;
                        }
                        else
                        {
                            AllButtons[1].isHidden = false;
                            AllButtons[2].isHidden = false;

                            WaitForTeamResponse = false;
                        }
                    }
                }
                else if (strcmp(arr[1], "PING") == 0) // Get ping request from server
                {
                    // Parse ping text to int
                    sscanf(arr[2], "%d", &ping);
                    SendPing = true;
                }
                else if (strcmp(arr[1], "POS") == 0) // Player position update
                {
                    // Get target player id
                    int PlayerIdInt = -1;
                    sscanf(arr[0], "%d", &PlayerIdInt);

                    float XFloat;
                    float YFloat;
                    float ZFloat;
                    int AngleInt = -1;

                    // Convert chars into floats
                    sscanf(arr[2], "%f", &XFloat);
                    sscanf(arr[3], "%f", &YFloat);
                    sscanf(arr[4], "%f", &ZFloat);
                    sscanf(arr[5], "%d", &AngleInt);

                    // Change scale
                    XFloat /= 4096.0;
                    YFloat /= 4096.0;
                    ZFloat /= 4096.0;

                    // If new position is for the local player
                    if (localPlayer->Id == PlayerIdInt)
                    {
                        // Set new position and teleport player
                        localPlayer->xPos = XFloat;
                        localPlayer->yPos = YFloat;
                        localPlayer->zPos = ZFloat;
                        localPlayer->Angle = AngleInt;
                        localPlayer->PlayerPhysic->xspeed = 0;
                        localPlayer->PlayerPhysic->yspeed = 0;
                        localPlayer->PlayerPhysic->zspeed = 0;
                        NE_ModelSetCoord(localPlayer->PlayerModel, localPlayer->xPos, localPlayer->yPos, localPlayer->zPos);
                        UpdateLookRotation(CameraAngleY, &x, &y, &z, &xWithoutY, &zWithoutY, &xWithoutYForAudio, &zWithoutYForAudio);
                    }
                    else
                    {
                        // Find player with unique ID to update his informations
                        for (int i = 1; i < MaxPlayer; i++)
                            if (AllPlayers[i].Id == PlayerIdInt)
                            {
                                // If player has no position, teleport the player to new updated position (0 default position)
                                if (AllPlayers[i].xDestination == 0)
                                {
                                    AllPlayers[i].xPos = XFloat;
                                    AllPlayers[i].yPos = YFloat;
                                    AllPlayers[i].zPos = ZFloat;
                                    AllPlayers[i].Angle = AngleInt;
                                }

                                // Set player destination
                                AllPlayers[i].xDestination = XFloat;
                                AllPlayers[i].yDestination = YFloat;
                                AllPlayers[i].zDestination = ZFloat;

                                AllPlayers[i].AngleDestination = AngleInt;
                                break;
                            }
                    }
                    // iprintf("\nPlayer pos update");
                }
                else if (strcmp(arr[1], "WALLHIT") == 0) // Wall hit position update for hit flash at wall animation
                {
                    float XFloat;
                    float YFloat;
                    float ZFloat;

                    // Convert chars into floats
                    sscanf(arr[2], "%f", &XFloat);
                    sscanf(arr[3], "%f", &YFloat);
                    sscanf(arr[4], "%f", &ZFloat);

                    // Change scale
                    XFloat /= 8192.0;
                    YFloat /= 8192.0;
                    ZFloat /= 8192.0;

                    // Set effect position
                    NE_ModelSetCoord(Model[8], XFloat, YFloat, ZFloat);

                    // Get wall hit flash direction
                    Vector2 Direction1D;
                    Direction1D.y = YFloat - localPlayer->yPos - CameraOffsetY + y;
                    Direction1D.x = 1;
                    normalize2D(&Direction1D);

                    Vector3 Direction;
                    Direction.x = XFloat - localPlayer->xPos;
                    Direction.y = YFloat - localPlayer->yPos;
                    Direction.z = ZFloat - localPlayer->zPos;
                    normalize(&Direction);

                    // Set wall hit angle
                    int FinalAngleY = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 384;
                    int FinalAngleZ = atan2f(Direction1D.y, 1) * 512.0 / (M_TWOPI) + 128;
                    NE_ModelSetRot(Model[8], 0, FinalAngleY, FinalAngleZ);

                    // Make 3D sound
                    int Panning, Volume;
                    Vector4 SoundPos;
                    SoundPos.x = XFloat;
                    SoundPos.y = YFloat;
                    SoundPos.z = ZFloat;
                    GetPanningByPosition(&Panning, &Volume, AllPlayers, SoundPos, xWithoutYForAudio, zWithoutYForAudio, 0.15);
                    Play3DSound(SFX_RIC, Volume, Panning);

                    // Set animation timer
                    ShowWallHitFlash = 3;
                    // iprintf("\nPlayer pos update");
                }
                else if (strcmp(arr[1], "BOMBPLACE") == 0) // Bomb place update
                {
                    // Temp variables
                    int XInt = -1;
                    int YInt = -1;
                    int ZInt = -1;
                    int AngleInt = -1;

                    // Convert chars into ints
                    sscanf(arr[2], "%d", &XInt);
                    sscanf(arr[3], "%d", &YInt);
                    sscanf(arr[4], "%d", &ZInt);
                    sscanf(arr[5], "%d", &AngleInt);

                    // Change values scale and set bomb position and angle value
                    BombPosition.x = XInt / 4096.0;
                    BombPosition.y = YInt / 4096.0;
                    BombPosition.z = ZInt / 4096.0;
                    BombPosition.r = AngleInt;

                    // Set bomb 3D model position and angle value
                    NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
                    NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
                    Model[7]->rz = BombPosition.r;

                    bombBipTimer = 120;
                    BombPlanted = true;
                    SetBombDefuseZone(BombPosition.x, BombPosition.z, 2, 2, &bombDefuseZone);
                    iprintf("\nBOMB UPDATE");
                }
                else if (strcmp(arr[1], "CONFIRM") == 0) // Confirm shop gun buy, or set player gun from server command
                {
                    int ConfirmTypeInt = -1;
                    int ConfirmArgumentInt = -1;
                    int ConfirmInventoryIndexInt = -1;
                    int ConfirmResultInt = -1; // 0 Error, 1 Okay

                    // Convert chars into ints
                    sscanf(arr[2], "%d", &ConfirmTypeInt);
                    sscanf(arr[3], "%d", &ConfirmArgumentInt);
                    sscanf(arr[4], "%d", &ConfirmInventoryIndexInt);
                    sscanf(arr[5], "%d", &ConfirmResultInt);

                    if (ConfirmTypeInt == 0) // Set gun in inventory
                    {
                        if (ConfirmResultInt == 1)
                        {
                            CurrentScopeLevel = 0;
                            SetGunInInventory(ConfirmArgumentInt, ConfirmInventoryIndexInt);
                        }
                    }
                    iprintf("\nCONFIRM UPDATE");
                }
                else if (strcmp(arr[1], "SETMONEY") == 0) // Set player money
                {
                    // Parse money text to int
                    sscanf(arr[2], "%d", &localPlayer->Money);
                }
                else if (strcmp(arr[1], "SCORE") == 0) // Party score changes
                {
                    // Parse scores texts to int
                    sscanf(arr[2], "%d", &CounterScore);
                    sscanf(arr[3], "%d", &TerroristsScore);

                    // if (currentMenu == 2 && UpdateBottomScreenOneFrame == 0)
                    // UpdateBottomScreenOneFrame = 8;

                    // Update screen if team screen is opened
                    if (currentMenu == 2)
                        UpdateBottomScreenOneFrame += 8;
                }
                else if (strcmp(arr[1], "SCRBOARD") == 0) // Score board score changes
                {
                    // Get player id
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    // Find player with unique ID to update his informations
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            sscanf(arr[2], "%d", &AllPlayers[i].KillCount);
                            sscanf(arr[3], "%d", &AllPlayers[i].DeathCount);
                            break;
                        }

                    // if (currentMenu == 2 && UpdateBottomScreenOneFrame == 0) //Score button (Main menu)
                    // UpdateBottomScreenOneFrame = 4;
                    // UpdateBottomScreenOneFrame = 8;

                    // Update screen if team screen is opened
                    if (currentMenu == 2)
                        UpdateBottomScreenOneFrame += 8;
                }
                else if (strcmp(arr[1], "PartyRound") == 0) // Round state changes
                {
                    // Parse text to int
                    sscanf(arr[2], "%d", &RoundState);

                    // Reset some values on new round
                    if (RoundState == 0)
                    {
                        NE_SpriteVisible(TopScreenSprites[0], true);
                        BombDefused = false;
                        BombPlanted = false;
                        BombWillExplose = false;
                        IsExplode = false;
                        BombExplosionScale = 0;
                        ResetGunsAmmo(0);

                        iprintf("\nRespawn");
                        for (int i = 0; i < MaxPlayer; i++)
                            AllPlayers[i].IsDead = FALSE;

                        deathCameraAnimation = 0;
                        deathCameraYOffset = 0;
                        redHealthTextCounter = 0;
                    }
                }
                else if (strcmp(arr[1], "LEAVE") == 0) // When a player leave
                {
                    // Get player id
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    // Find player
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            AllPlayers[i].Id = -1;
                            NE_ModelDelete(AllPlayers[i].PlayerModel);
                            if (i == 0) ////////////////////////////////////////////////////////////TODO KICK SYSTEM
                            {
                                NE_PhysicsDelete(AllPlayers[i].PlayerPhysic);
                            }
                            AllPlayers[i].KillCount = 0;
                            AllPlayers[i].DeathCount = 0;
                            PlayerCount--;
                            break;
                        }
                }
                else if (strcmp(arr[1], "SETHEALTH") == 0) // Set player health
                {
                    int PlayerIdInt = -1;
                    sscanf(arr[2], "%d", &PlayerIdInt);

                    // Find player to set health
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == PlayerIdInt)
                        {
                            int newHealth = 0;
                            sscanf(arr[3], "%d", &newHealth);
                            setPlayerHealth(i, newHealth);
                            break;
                        }
                    }
                }
                else if (strcmp(arr[1], "CURGUN") == 0) // Set current gun in inventory
                {
                    int PlayerIdInt = -1;
                    sscanf(arr[0], "%d", &PlayerIdInt);

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
                            // setPlayerHealth(i, newHealth);
                            break;
                        }
                    }
                }
                else if (strcmp(arr[1], "SHOOT") == 0) // Shoot from a player
                {
                    // Get player id
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    // Get gun id
                    int ParsedGunId = -1;
                    sscanf(arr[2], "%d", &ParsedGunId);

                    // Make a sound
                    int Panning, Volume;
                    GetPanning(ParsedPlayerId, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, AllGuns[ParsedGunId].MaxSoundDistance);
                    if (ParsedGunId < GunCount)
                        Play3DSound(AllGuns[ParsedGunId].gunSound, Volume, Panning);
                    // else if(ParsedGunId < GunCount + grenadeCount)
                    // Play3DSound(AllGrenades[ParsedGunId]., Volume, Panning);
                    // else if(ParsedGunId < GunCount + grenadeCount + equipementCount)
                    // Play3DSound(AllEquipements[ParsedGunId]., Volume, Panning);
                }
                else if (strcmp(arr[1], "SETBOMB") == 0) // Shoot from a player
                {
                    // Get player id
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    // Get gun id
                    int haveBomb = -1;
                    sscanf(arr[2], "%d", &haveBomb);

                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id != -1)
                        {
                            if (AllPlayers[i].Id == ParsedPlayerId)
                            {
                                if (haveBomb == 1)
                                {
                                    AllPlayers[i].haveBomb = true;
                                    if (i == 0)
                                        SetGunInInventory(28, 8);
                                    // AllGunsInInventory[8] = 28;
                                }
                                else
                                {
                                    AllPlayers[i].haveBomb = false;
                                    if (i == 0)
                                        SetGunInInventory(-1, 8);
                                    // AllGunsInInventory[8] = -1;
                                }
                            }
                            /*else
                            {
                                AllPlayers[i].haveBomb = false;
                                if (i == 0)
                                    SetGunInInventory(-1, 8);
                                //AllGunsInInventory[8] = -1;
                            }*/
                        }
                    }
                }
                else if (strcmp(arr[1], "HITSOUND") == 0) // Party score changes
                {
                    int PlayerId, HitType;

                    sscanf(arr[2], "%d", &PlayerId);
                    sscanf(arr[3], "%d", &HitType);

                    int Panning, Volume;
                    if (PlayerId != localPlayer->Id)
                        GetPanning(PlayerId, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);
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

                    if (HitType == 0)
                        Play3DSound(SFX_FLESH_IMPACT, Volume, Panning); // Check with kevlar
                    else if (HitType == 1)
                        Play3DSound(SFX_HEADSHOT1, Volume, Panning);
                    else if (HitType == 2)
                        Play3DSound(SFX_FLESH_IMPACT, Volume, Panning);
                    else
                        Play3DSound(SFX_KNIFE_HIT_PLAYER, Volume, Panning);
                }
                else if (strcmp(arr[1], "TEXT") == 0) // Show text from server
                {
                    int ParsedTextId = -1;
                    sscanf(arr[2], "%d", &ParsedTextId);

                    /*if (ParsedTextId == 0)
                    {
                        strncpy(textToShow, "Terrorists win", 30);
                        PlayBasicSound(SFX_TERRORISTSWIN);
                    }
                    else if (ParsedTextId == 1)
                    {
                        strncpy(textToShow, "Counter Terrorist win", 30);
                        PlayBasicSound(SFX_COUNTERTERRORISTWIN);
                    }
                    else if (ParsedTextId == 2)
                    {
                        strncpy(textToShow, "Bomb has been planted", 30);
                        PlayBasicSound(SFX_BOMBHASBEENPLANTED);
                    }
                    else if (ParsedTextId == 3)
                    {
                        strncpy(textToShow, "Bomb has been defused", 30);
                        PlayBasicSound(SFX_BOMBHASBEENDEFUSED);
                    }

                    textToShowTimer = 400;*/
                    showPartyEventText(ParsedTextId);
                }
                else if (strcmp(arr[1], "TEXTPLAYER") == 0) // Show text about a player from server
                {
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    int KillerIdInt = -1;
                    int TextIdInt = -1;

                    sscanf(arr[2], "%d", &KillerIdInt);
                    sscanf(arr[3], "%d", &TextIdInt);

                    // Find player with unique ID to update his informations
                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == KillerIdInt)
                        {
                            sprintf(killText, "%d", AllPlayers[i].Id); // GET PLAYER NAME
                            break;
                        }

                    sprintf(killText + strlen(killText), " killed ");

                    for (int i = 0; i < MaxPlayer; i++)
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            sprintf(killText + strlen(killText), "%d", AllPlayers[i].Id); // GET PLAYER NAME
                            break;
                        }

                    KillTextShowTimer = 240;
                }
                else if (strcmp(arr[1], "STEP") == 0) // Step sound
                {
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    int Panning, Volume;
                    GetPanning(ParsedPlayerId, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);

                    DoStepSound(Volume, Panning, 0);
                }
                else if (strcmp(arr[1], "BOMBPLACING") == 0) // Bomb placing sound
                {
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);

                    int Panning, Volume;
                    GetPanning(ParsedPlayerId, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);

                    Play3DSound(SFX_BOMBPLANTING, Volume, Panning);
                }
                else if (strcmp(arr[1], "BOMBDEFUSE") == 0) // Bomb defuse sound
                {
                    BombDefused = true;
                }
                else if (strcmp(arr[1], "ADDRANGE") == 0) // Add multiples non local players
                {
                    for (int i = 2; i < countI; i++)
                    {
                        int FoundId = -1;
                        sscanf(arr[i], "%d", &FoundId);
                        AddNewPlayer(FoundId, false, false);
                    }
                    printf("countI %d", countI);
                }
                else if (strcmp(arr[1], "INVTORY") == 0) // Add multiples non local players
                {
                    int ParsedPlayerId = -1;
                    sscanf(arr[0], "%d", &ParsedPlayerId);
                    int FoundPlayer = -1;
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        if (AllPlayers[i].Id == ParsedPlayerId)
                        {
                            FoundPlayer = i;
                            for (int i2 = 2; i2 < countI; i2++)
                            {
                                int FoundGunId = -1;
                                sscanf(arr[i2], "%d", &FoundGunId);
                                // AllPlayers[i].AllGunsInInventory[i2 - 2] = FoundGunId;

                                SetGunInInventoryForNonLocalPlayer(i, FoundGunId, i2 - 2);
                            }
                            break;
                        }
                    }
                    if (FoundPlayer == CurrentCameraPlayer)
                    {
                        UpdateGunTexture();
                    }
                }
                else if (strcmp(arr[1], "GRENADE") == 0) // Bomb place update
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
                    // sscanf(arr[2], "%d", &GrenadeType);
                    sscanf(arr[2], "%f", &XDirection);
                    sscanf(arr[3], "%f", &YDirection);
                    sscanf(arr[4], "%f", &ZDirection);

                    sscanf(arr[5], "%d", &XInt);
                    sscanf(arr[6], "%d", &YInt);
                    sscanf(arr[7], "%d", &ZInt);
                    sscanf(arr[8], "%d", &GrenadeType);

                    PhysicalGrenade *newGrenade = CreateGrenade(GrenadeType);
                    lanchGrenade(newGrenade, XDirection, YDirection, ZDirection, XInt, YInt, ZInt);
                }

                // Clear "TempValues"
                for (int i = 0; i < sizeof(TempValues); i++)
                    TempValues[i] = '\0';

                // Add all characters after current data packet to "TempValues"
                strcat(TempValues, Values + EndPosition + 1);
                // Copy "TempValues" to "Values"
                strcpy(Values, TempValues);

                // iprintf("\nCall type : ");
                // iprintf(Type);
            }
        }

        if (localPlayer->Id != -1)
        {
            // Play game code
            GameLoop();
            UpdateEngine();
        }
        else
        {
            UpdateEngine();
            continue;
        }

        // Check if position need to be updated for other player
        if (localPlayer->xPos != OldxPos || localPlayer->yPos != OldyPos || localPlayer->zPos != OldzPos)
        {
            OldxPos = localPlayer->xPos;
            OldyPos = localPlayer->yPos;
            OldzPos = localPlayer->zPos;
            SendPosition = true;
        }

        char InfoToSend[1024] = "";

        // Send hit data
        if (Hit != -1 && localPlayer->ScanFinished)
        {
            printf("APPLY DAMAGE");
            int hitType = 0;

            if (localPlayer->IsHeadShot)
            {
                hitType = 1;
            }
            else if (localPlayer->IsLegShot)
            {
                hitType = 2;
            }

            if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] < GunCount && AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].isKnife)
                hitType = 3;

            sprintf(InfoToSend /*+ strlen(InfoToSend)*/, "{-4;HIT;%d;%d}", AllPlayers[Hit].Id, hitType);

            int Panning, Volume;
            GetPanning(AllPlayers[Hit].Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);

            if (hitType == 0)
                Play3DSound(SFX_FLESH_IMPACT, Volume, Panning); // Check with kevlar
            else if (hitType == 1)
                Play3DSound(SFX_HEADSHOT1, Volume, Panning);
            else if (hitType == 2)
                Play3DSound(SFX_FLESH_IMPACT, Volume, Panning);
            else
                Play3DSound(SFX_KNIFE_HIT_PLAYER, Volume, Panning);

            Hit = -1;
        }

        // Send shoot data for sound/animation for clients
        if (SendShoot)
        {
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;SHOOT}");
            SendShoot = false;
        }

        if (SendGrenade)
        {
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;GRENADE;%f;%f;%f;%d;%d;%d}", x, y, z, localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z); // With grenade id a finir, look rotation, position
            SendGrenade = false;
        }

        // Send position one in two
        // SendPositionData = !SendPositionData;
        if (SendPositionData > 0)
            SendPositionData--;

        // If local player need to be updated for other player (59 bytes)
        if (localPlayer->Id != -1 && SendPosition && SendPositionData == 0)
        {
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;POS;%d;%d;%d;%d}", localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z, (int)localPlayer->Angle);
            SendPosition = false;
            SendPositionData = 4;
        }

        // Send step data
        if (SendStepSound)
        {
            SendStepSound = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-2;STEP}");
        }

        // Send bomb placement data
        if (SendBombPlace)
        {
            SendBombPlace = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-1;BOMBPLACE;%d;%d;%d;%d}", (int)(BombPosition.x * 4096.0), (int)(BombPosition.y * 4096.0), (int)(BombPosition.z * 4096.0), BombPosition.r);
        }

        // Send bomb defuse data
        if (SendBombDefused)
        {
            SendBombDefused = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-1;BOMBDEFUSE}");
        }

        // Send bomb planting data
        if (SendBombPlacing)
        {
            SendBombPlacing = false;
            // sprintf(InfoToSend + strlen(InfoToSend), "{-2;BOMBPLACING;%d}", localPlayer->Id);
            sprintf(InfoToSend + strlen(InfoToSend), "{-2;BOMBPLACING}");
        }

        // Send team data
        if (SendTeam)
        {
            SendTeam = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;TEAM;%d}", tempTeam);
            tempTeam = -1;
        }

        // Send ping reply
        if (SendPing)
        {
            SendPing = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;PING}");
        }

        // Send buy weapon request data
        if (SendBuyWeapon)
        {
            SendBuyWeapon = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;BUY;%d}", GetSelectedGunShop());
        }

        if (SendSelectedGun)
        {
            SendSelectedGun = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-4;CURGUN;%d}", localPlayer->currentGunInInventory);
        }

        // Send wall hit data
        if (SendWallHit)
        {
            SendWallHit = false;
            sprintf(InfoToSend + strlen(InfoToSend), "{-2;WALLHIT;%d;%d;%d}", WallHitXPos, WallHitYPos, WallHitZPos);

            int Panning, Volume;
            Vector4 SoundPos;
            SoundPos.x = WallHitXPos /= 8192.0;
            SoundPos.y = WallHitYPos /= 8192.0;
            SoundPos.z = WallHitZPos /= 8192.0;
            GetPanningByPosition(&Panning, &Volume, AllPlayers, SoundPos, xWithoutYForAudio, zWithoutYForAudio, 0.15);
            Play3DSound(SFX_RIC, Volume, Panning);
        }

        // Get string data length
        int InfoLentgh = strlen(InfoToSend);
        if (InfoLentgh != 0)
            send(my_socket, InfoToSend, InfoLentgh, 0); // Send data to server
                                                        // sendto(my_socket, InfoToSend, InfoLentgh, 0, (struct sockaddr *)&sain, sizeof(sain));
    }
}