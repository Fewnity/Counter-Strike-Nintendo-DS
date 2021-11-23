#ifndef NETWORK_H_ /* Include guard */
#define NETWORK_H_
#include <NEMain.h>
#include "main.h"

extern char **allIps;
extern int TotalIpCount;
extern int ping;

//Socket
extern int my_socket;

//Write part
extern bool SendPosition;
extern int SendPositionData;
extern bool SendStepSound;
extern bool SendShoot;
extern bool SendGrenade;
extern bool SendBombPlacing;
extern bool SendBombPlace;
extern bool SendBombDefused;
extern bool SendPing;
extern bool SendTeam;
extern bool WaitForTeamResponse;
extern bool SendBuyWeapon;
extern bool SendWallHit;
extern bool SendSelectedGun;

void SetPlayer();
void connectToServer(char *url, bool LocalMode, int my_socket, bool toGameServer);
void ReadServerListData();
void ReadServerData();
void connectFromServerList(int unused);

#endif // NETWORK_H_