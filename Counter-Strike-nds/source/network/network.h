// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef NETWORK_H_ /* Include guard */
#define NETWORK_H_

#define PARTY_CODE_LENGTH 5
enum ErrorType
{
    Ban = 0,
    WrongVersion = 1,
    MacAddressMissing = 2,
    WrongSecurityKey = 3,
    ServerFull = 4,
    ServerStopped = 5,
    SaveCorrupted = 6,
    IncorrectCode = 7,
    KickTeamKill = 8
};

enum JoinType
{
    JOIN_RANDOM_PARTY = 0,
    CREATE_PRIVATE_PARTY = 1,
    JOIN_PRIVATE_PARTY = 2
};

enum RequestType
{
    PING = 0,
    STPEP = 1,
    WALLHIT = 2,
    BOMBPLACE = 3,
    BOMBPLACING = 4,
    BOMBDEFUSE = 5,
    CURGUN = 6,
    LEAVE = 7,
    VOTE = 8,
    GRENADE = 9,
    POS = 10,
    SETNAME = 11,
    SHOOT = 12,
    HIT = 13,
    PARTY = 14,
    BUY = 15,
    TEAM = 16,
    KEY = 17,
    SETID = 18,
    TIMER = 19,
    SETMAP = 20,
    SETMODE = 21,
    VOTERESULT = 22,
    CONFIRM = 23,
    SETMONEY = 24,
    SCORE = 25,
    SCRBOARD = 26,
    SETSHOPZONE = 27,
    PARTYROUND = 28,
    SETHEALTH = 29,
    SETBOMB = 30,
    SETCODE = 31,
    HITSOUND = 32,
    ERROR = 33,
    TEXT = 34,
    TEXTPLAYER = 35,
    ADDRANGE = 36,
    ENDGAME = 37,
    ENDUPDATE = 38,
    INVTORY = 39,
    GETBOMB = 40,
    FRAME = 41,
    RELOADED = 42,
    STATUS = 43,
};

#define REQUEST_NAME_INDEX 0

#define DEBUG_IP_1_STRING ""
#define DEBUG_IP_2_STRING ""
#define ONLINE_SERVER_IP_STRING "cs.fewnity.com"
#define SERVER_PORT 6003
#define LOCAL_SERVER_PORT 1080 // For the android server version (but this is not used)

extern int ping;
extern char partyCode[PARTY_CODE_LENGTH];
extern bool isPrivate;

// Socket
extern int my_socket;

// Write part
extern bool SendPosition;
extern int SendPositionData;
extern bool SendShoot;
extern bool SendGrenade;
extern bool SendBombPlacing;
extern bool SendBombPlace;
extern bool SendBombDefused;
extern bool SendPing;
extern bool SendLeave;
extern bool SendTeam;
extern bool WaitForTeamResponse;
extern bool SendBuyWeapon;
extern bool SendWallHit;
extern bool SendSelectedGun;
extern bool SendPlayerName;
extern bool SendVoteStartNow;
extern bool SendGetDroppedBomb;
extern bool SendReloaded;

extern int timeOut;
extern int frameCount;

void SetPlayer();
void initNetwork(int option);
void connectToServer(char *url, bool LocalMode, int my_socket, enum JoinType option);
void ReadServerListData();
void ReadServerData();
void connectFromServerList(int unused);
Player *GetPlayer(int id);
void resetNetworkVariables();
void treatData();
void sendDataToServer();
int intParse(char *charToParse);
float floatParse(char *charToParse);

#endif // NETWORK_H_