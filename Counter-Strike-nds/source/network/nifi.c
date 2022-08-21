#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include "nifi.h"
#include "../main.h"
#include "player.h"
#include "network.h"
#include "debug.h"
#include "movements.h"

////// Wifi variables :
int wifiChannel = 10;
//// Wifi buffers:
// Read part
char WIFI_Buffer[1024] = ""; // Wifi buffer is the incoming data from the wifi (DO NOT USE LIKE THIS, please use the "wirelessData" define)
//#define wirelessData (WIFI_Buffer + 32) // Define to use the WIFI_Buffer array to avoid wrong values
char Values[1024] = "";			 // Store all the values received from the wifi and wainting for treatment
char TempValues[1024] = "";		 // Store values that can't be treated yet
int WIFI_ReceivedDataLength = 0; // Size of data received from the wifi
// Write part
char tempSendBuffer[1024] = ""; // Temp buffer to use when the host need to resend the same data if the wifi is not working correctly

////// Room variables :
int hostIndex = EMPTY; // Index of the host in the clients array
bool isHost = false;   // Is the Nintendo DS hosting a room?
// For host
int speakTo = 1;		   // Index of the speak to speak with
int idCount = 0;		   // Id of the next client to add
int lastCommunication = 0; // Countdown to check the wifi timeout
char tempMacAddress[13];   // Mac address of the client that is trying to join the room
int timeoutCount = 0;	   // Store how many times the wifi has failed to communicate with the client
// For non-host
int joinRoomTimer = WIFI_TIMEOUT; // Timer to limit the time to join a room
bool tryJoinRoom = false;		  // Is the client trying to join a room
bool sendPosition;				  // If true, the client will send its position to the server
bool skipData;					  // If true, the client will skip data of the current wifi packet

////TO DO
// Automatic channel selection (Check how many random packet are received on each channel and select the best one)
// Reset lastMessageId to 0 if the number is too high to not hit the int limit (maybe useless but great to have)

/**
 * @brief Wifi handler call when data is received
 *
 * @param packetID
 * @param readlength
 */
void WirelessHandler(int packetID, int readlength)
{
	Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)WIFI_Buffer);

	// Get the correct data length
	WIFI_ReceivedDataLength = readlength - 32;

	// printf("a:%s\n", wirelessData);
	int recvd_len = strlen(wirelessData);
	if (WIFI_ReceivedDataLength == recvd_len + 1)
	{
		wirelessData[recvd_len] = '\0';
		strcat(Values, wirelessData);
		// Treatment of the data
		treatData();
	}
}

/**
 * @brief Send data to clients
 *
 * @param buffer
 * @param length
 */
void SendWirelessData(unsigned short *buffer, int length)
{
	if (Wifi_RawTxFrame(length, 0x0014, buffer) != 0)
	{
		printf("Error calling RawTxFrame\n");
	}
}

/**
 * @brief Reset variable used by the the Wifi to avoid wrong values when reconnecting
 *
 */
void resetNifiValues()
{
	// Reset clients data
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		resetClientValues(&AllPlayers[i].client);
	}
	// Reset buffers
	WIFI_Buffer[0] = '\0';
	Values[0] = '\0';
	TempValues[0] = '\0';
	tempSendBuffer[0] = '\0';
	// Reset variables
	lastCommunication = 0;
	hostIndex = EMPTY;
	WIFI_ReceivedDataLength = 0;
}

/**
 * @brief Init the Nifi system
 *
 */
void nifiInit()
{
	resetNifiValues();

	printf("\nInit NiFi...\n");

	// Changes how packets are handled
	Wifi_SetRawPacketMode(PACKET_MODE_NIFI);

	// Init Wifi without automatic settings
	Wifi_InitDefault(false);

	// Enable Wifi
	Wifi_EnableWifi();

	// Configure custom packet handler for when
	Wifi_RawSetPacketHandler(WirelessHandler);

	// Force specific channel for communication
	Wifi_SetChannel(wifiChannel);

	// Get MAC address of the Nintendo DS
	u8 macAddressUnsigned[6];
	Wifi_GetData(WIFIGETDATA_MACADDRESS, 6, macAddressUnsigned);
	// Convert unsigned values to hexa values
	sprintf(localPlayer->client.macAddress, "%02X%02X%02X%02X%02X%02X", macAddressUnsigned[0], macAddressUnsigned[1], macAddressUnsigned[2], macAddressUnsigned[3], macAddressUnsigned[4], macAddressUnsigned[5]);

	printf("NiFi initiated.\n\n");
	// printf("%s\n\n", localPlayer->client.macAddress);
	printf("%sUp to create a room.\n%s", YELLOW, WHITE);
	printf("%sDown to join the room.\n%s", CYAN, WHITE);
}

/**
 * @brief Share a request to clients
 *
 * @param clientSender
 * @param requestType
 */
void shareRequest(Client *clientSender, enum RequestType requestType)
{
	// If the local client is the host
	if (isHost)
	{
		// Send the data to all clients
		for (int clientIndex = 1; clientIndex < MAX_CLIENT; clientIndex++)
		{
			Client *clientToUpdate = &AllPlayers[clientIndex].client;
			// printf("%s%d %d|%s", CYAN, clientIndex, clientToUpdate->id, WHITE);
			//  Avoid sending the request to the current treated client
			if (clientSender->id != clientToUpdate->id && clientToUpdate->id != EMPTY)
			{
				createRequest(clientSender, clientToUpdate, requestType);
			}
		}
	}
	else if (clientSender == &localPlayer->client) // Share the request to the host only
	{
		Client *clientToUpdate = &AllPlayers[hostIndex].client;
		// printf("%s%d%s", CYAN, hostIndex, WHITE);
		createRequest(clientSender, clientToUpdate, requestType);
	}
}

/**
 * @brief Create a request and add it to the client to update's send buffer
 *
 * @param clientSender Client pointer to get data from
 * @param clientToUpdate Client pointer to send data to
 * @param requestType Request type (See RequestType enum in main.h)
 */
void createRequest(Client *clientSender, Client *clientToUpdate, enum RequestType requestType)
{
	if (clientSender == NULL || clientToUpdate == NULL || clientToUpdate->id == EMPTY || !isNifiMode)
		return;

	Player *senderPlayer = GetPlayer(clientSender->id);
	if (senderPlayer == NULL)
		return;

	char buffer[200] = "";

	switch (requestType)
	{
	case POS:
		// HERE : clientToUpdate->id looks incorrect in the buffer, but looks correct with a printf of the value
		// The value looks correct with when remplacing the float by an int
		// There are some dirty tricks to get the correct value but I have new problems
		if (senderPlayer == localPlayer)
			sprintf(buffer, "{POS;%d;%d;%d;%d;%d;%0.0f;%d}", senderPlayer->Id, senderPlayer->PlayerModel->x, senderPlayer->PlayerModel->y, senderPlayer->PlayerModel->z, (int)senderPlayer->Angle, senderPlayer->cameraAngle, clientToUpdate->id);
		else
			sprintf(buffer, "{POS;%d;%d;%d;%d;%d;%0.0f;%d}", senderPlayer->Id, (int)(senderPlayer->lerpDestination.x * 4096), (int)(senderPlayer->lerpDestination.y * 4096), (int)(senderPlayer->lerpDestination.z * 4096), (int)senderPlayer->AngleDestination, senderPlayer->cameraAngle, clientToUpdate->id);

		// INT VERSION (WORKING) dirty fix
		// if (senderPlayer == localPlayer)
		//  	sprintf(buffer, "{POS;%d;%d;%d;%d;%d;%d;%d}", senderPlayer->Id, senderPlayer->PlayerModel->x, senderPlayer->PlayerModel->y, senderPlayer->PlayerModel->z, (int)senderPlayer->Angle, (int)senderPlayer->cameraAngle, clientToUpdate->id);
		//  else
		//  	sprintf(buffer, "{POS;%d;%d;%d;%d;%d;%d;%d}", senderPlayer->Id, (int)(senderPlayer->lerpDestination.x * 4096), (int)(senderPlayer->lerpDestination.y * 4096), (int)(senderPlayer->lerpDestination.z * 4096), (int)senderPlayer->AngleDestination, (int)senderPlayer->cameraAngle, clientToUpdate->id);

		// FLOAT VERSION (WORKING) dirty fix, if clients is moving, the host will scan for room (tryJoinRoom changes value)
		// if (senderPlayer == localPlayer)
		// 		snprintf(buffer, sizeof(buffer), "{POS;%d;%d;%d;%d;%d;%0.0f;%d}", senderPlayer->Id, senderPlayer->PlayerModel->x, senderPlayer->PlayerModel->y, senderPlayer->PlayerModel->z, (int)senderPlayer->Angle, senderPlayer->cameraAngle, clientToUpdate->id);
		// else
		// 		snprintf(buffer, sizeof(buffer), "{POS;%d;%d;%d;%d;%d;%0.0f;%d}", senderPlayer->Id, (int)(senderPlayer->lerpDestination.x * 4096), (int)(senderPlayer->lerpDestination.y * 4096), (int)(senderPlayer->lerpDestination.z * 4096), (int)senderPlayer->AngleDestination, senderPlayer->cameraAngle, clientToUpdate->id);
		break;
	case TEAM:
		sprintf(buffer, "{TEAM;%d;%d;%d}", clientSender->id, tempTeam, clientToUpdate->id);
		break;
	case SHOOT:
		sprintf(buffer, "{SHOOT;%d;%d;%d}", clientSender->id, senderPlayer->AllGunsInInventory[senderPlayer->currentGunInInventory], clientToUpdate->id);
		break;
	case BOMBDEFUSE:
		sprintf(buffer, "{BOMBDEFUSE;%d}", clientToUpdate->id);
		break;
	case GRENADE:
	{
		// TO DO SEND GRENADE TYPE
		float x, y, z;
		UpdateLookRotationAI(senderPlayer->cameraAngle, senderPlayer->AngleDestination, &x, &y, &z);
		if (senderPlayer == localPlayer)
			sprintf(buffer, "{GRENADE;%f;%f;%f;%d;%d;%d;%d;%d;%d}", x, y, z, senderPlayer->PlayerModel->x, senderPlayer->PlayerModel->y, senderPlayer->PlayerModel->z, 0, clientSender->id, clientToUpdate->id);
		else
			sprintf(buffer, "{GRENADE;%f;%f;%f;%d;%d;%d;%d;%d;%d}", x, y, z, (int)(senderPlayer->lerpDestination.x * 4096), (int)(senderPlayer->lerpDestination.y * 4096), (int)(senderPlayer->lerpDestination.z * 4096), 0, clientSender->id, clientToUpdate->id);
		break;
	}
	case WALLHIT:
		// strcpy(buffer, "");
		// for (int i = 0; i < getPlayerCurrentGun(senderPlayer).bulletCountPerShoot; i++)
		// {
		// 	sprintf(buffer + strlen(buffer), "{WALLHIT;%d;%d;%d}", clientSender->id, senderPlayer->AllGunsInInventory[senderPlayer->currentGunInInventory], clientToUpdate->id);
		// }
		break;
	default:
		return;
	}

	AddDataTo(clientToUpdate, buffer);
}

/**
 * @brief Get the Client By Id (doens't return the local player)
 *
 * @param clientId
 * @return Client*
 */
Client *getClientById(int clientId)
{
	for (int i = 1; i < MAX_CLIENT; i++)
	{
		if (AllPlayers[i].client.id == clientId)
		{
			return &AllPlayers[i].client;
		}
	}
	return NULL;
}

/**
 * @brief Create a room
 *
 */
void createRoom()
{
	// Is the client is not already a host
	if (!isHost)
	{
		resetNifiValues();
		// Create a room
		isHost = true;
		idCount = 0;

		timerStart(3, ClockDivider_1024, TIMER_FREQ_1024(WIFI_RATE), managerServer);
		printf("Room created\n");
	}
}

/**
 * @brief Remove a client
 *
 * @param client Client pointer to remove
 */
void removeClient(Client *client)
{
	if (client->id != EMPTY)
	{
		for (int i = 0; i < MAX_CLIENT; i++)
		{
			if (client->id == AllPlayers[i].Id)
				AllPlayers[i].Id = EMPTY;
		}
		if (isHost)
		{
			for (int i = 1; i < MAX_CLIENT; i++)
			{
				// Send a request to all clients to remove the client from all other clients (the removed client is not called)
				if (&AllPlayers[i].client != client && AllPlayers[i].client.id != EMPTY)
				{
					char buffer[18];
					sprintf(buffer, "{ROOM;QUIT;%d;%d}", client->id, AllPlayers[i].client.id);
					AddDataTo(&AllPlayers[i].client, buffer);
				}
			}
		}
		printf("%sCLIENT REMOVED : %d%s\n", RED, client->id, WHITE);
	}
	resetClientValues(client);
}

/**
 * @brief Reset client values
 *
 * @param client Client pointer to reset
 */
void resetClientValues(Client *client)
{
	client->id = EMPTY;
	if (client != &localPlayer->client)
		strcpy(client->macAddress, "");
	else
		skipData = false;
	strcpy(client->sendBuffer, "");
	client->lastMessageId = 0;
}

/**
 * @brief Add a client
 *
 * @param id Client's id (not used by the room owner, set to -1/EMPTY)
 * @param addHost Are we adding the host?
 */
void addClient(int id, bool addHost)
{
	bool macAlreadyExists = false;
	bool idAlreadyExists = false;
	if (isHost) // If the local client is the host
	{
		// Check if the client to add is already in the room (same mac address), because the client is maybe trying to join the room multiple times if the wifi is not working properly
		for (int i = 1; i < MAX_CLIENT; i++)
		{
			if (strcmp(tempMacAddress, AllPlayers[i].client.macAddress) == 0)
			{
				macAlreadyExists = true;
				break;
			}
		}
	}
	else
	{
		// Check if the client to add is already in the room (same id)
		for (int i = 1; i < MAX_CLIENT; i++)
		{
			if (AllPlayers[i].client.id == id)
			{
				idAlreadyExists = true;
				break;
			}
		}
	}

	// If the client to add is not already in the room
	if (!macAlreadyExists && !idAlreadyExists)
	{
		int addedClientIndex = EMPTY;
		if (isHost)
		{
			idCount++;
			addedClientIndex = AddNewPlayer(idCount, false, false);
		}
		else
		{
			addedClientIndex = AddNewPlayer(id, false, false);
		}

		if (addHost && addedClientIndex != EMPTY)
			hostIndex = addedClientIndex;

		// If the client is added by the host
		if (addedClientIndex != EMPTY && isHost)
		{
			sprintf(AllPlayers[addedClientIndex].client.macAddress, tempMacAddress);
			//  Send the client his id
			char newClientBuffer[100];
			sprintf(newClientBuffer, "{ROOM;CONFIRM_JOIN;%s;%d;%d}", tempMacAddress, localPlayer->client.id, AllPlayers[addedClientIndex].client.id);
			// Send the client all the other clients ids
			sprintf(newClientBuffer + strlen(newClientBuffer), "{ROOM;ADDCLIENTS;%d", AllPlayers[addedClientIndex].client.id);
			// Send the client id to all the other clients
			for (int i = 1; i < MAX_CLIENT; i++)
			{
				if (AllPlayers[i].client.id != EMPTY && i != addedClientIndex)
				{
					// Send the client all the other clients ids
					sprintf(newClientBuffer + strlen(newClientBuffer), ";%d", AllPlayers[i].client.id);
					// Send the client id to all the other clients
					char bufferForOtherClients[24];
					sprintf(bufferForOtherClients, "{ROOM;ADDCLIENTS;%d;%d}", AllPlayers[i].client.id, AllPlayers[addedClientIndex].client.id);
					AddDataTo(&AllPlayers[i].client, bufferForOtherClients);
				}
			}
			sprintf(newClientBuffer + strlen(newClientBuffer), "}");
			AddDataTo(&AllPlayers[addedClientIndex].client, newClientBuffer);
		}
	}
}

/**
 * @brief Scan for a room
 *
 */
void scanForRoom()
{
	isHost = false;

	char buffer[25];
	sprintf(buffer, "{ROOM;SCAN;%s}", localPlayer->client.macAddress);
	SendWirelessData((unsigned short *)buffer, strlen(buffer) + 1);
}

/**
 * @brief Select the next client to communicate with
 *
 */
void communicateWithNextClient()
{
	// Reset values
	timeoutCount = 0;
	lastCommunication = 0;

	// Change client to communicate with
	speakTo++;
	if (speakTo == MAX_CLIENT) // Go back to the beginning of the list
		speakTo = 1;

	if (AllPlayers[speakTo].client.id != EMPTY)
	{
		// AddDataTo(&AllPlayers[speakTo].client, "{}"); // TO REMOVE FIX : Data can't be sent if the buffer is empty
		SendDataTo(&AllPlayers[speakTo].client);
	}
	else
	{
		lastCommunication = WIFI_TIMEOUT - 1;
	}
}

/**
 * @brief Manage client's communication order and timeout
 *
 */
void managerServer()
{
	// Only the host can manage the server
	if (isHost)
	{
		// Increase the time to check timeout
		lastCommunication++;
		if (lastCommunication == WIFI_TIMEOUT)
		{
			// If the clients is in the party (id not empty)
			if (AllPlayers[speakTo].client.id != EMPTY)
			{
				// If the timeout max count isn't reached, retry communication
				if (timeoutCount <= MAX_TIMEOUT_RETRY)
				{
					timeoutCount++;
					lastCommunication = 0;

					SendDataTo(&AllPlayers[speakTo].client);
				}
				else
				{
					// Remove the client
					removeClient(&AllPlayers[speakTo].client);
					communicateWithNextClient();
				}
			}
			else
			{
				communicateWithNextClient();
			}
		}
	}
}

/**
 * @brief Add data in the client's send buffer to send it to the client
 *
 * @param client client to add data to
 * @param data data to add
 */
void AddDataTo(Client *client, char *data)
{
	if (client != NULL && client->id != EMPTY && strlen(data) != 0)
	{
		// printf("AddDataTo : %s\n", data);
		//  printf("%sSEND BUF OLD : %s%s\n", RED, client->sendBuffer, WHITE);
		sprintf(client->sendBuffer + strlen(client->sendBuffer), "%s", data);
		// sprintf(client->sendBuffer, "%s", data);
		printf("%sSEND BUF : %s%s\n", YELLOW, client->sendBuffer, WHITE);
		// printf("%sSEND BUF : %s\n", YELLOW, WHITE);
	}
}

/**
 * @brief Send data to a client with Nifi
 *
 * @param client client to send data to
 */
void SendDataTo(Client *client)
{
	// If the buffer is not empty, copy the buffer to a new one and clear the buffer
	if (timeoutCount == 0)
	{
		if (isHost)
		{
			client->lastMessageId++;

			// Ask to client to communicate
			sprintf(tempSendBuffer, "{ROOM;WANTSPEAK;%d;%d}", client->id, client->lastMessageId);
		}
		else // Tell the host that the local client has received the message
			sprintf(tempSendBuffer, "{ROOM;CONFIRM_LISTEN;%d}", localPlayer->client.id);

		sprintf(tempSendBuffer + strlen(tempSendBuffer), "%s", client->sendBuffer);

		strcpy(client->sendBuffer, "");
		client->sendBuffer[0] = '\0';
	}

	// Send data
	SendWirelessData((unsigned short *)tempSendBuffer, strlen(tempSendBuffer) + 1);
}