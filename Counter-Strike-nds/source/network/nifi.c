#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
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
char Values[1024] = ""; // Store all the values received from the wifi and wainting for treatment
// Write part
char tempSendBuffer[NIFI_MAX_PAYLOAD_SIZE] = ""; // Temp buffer to use when the host need to resend the same data if the wifi is not working correctly

//// Interrupt <-> main loop communication
// The wifi packet handler and the server timer are called in interrupts.
// They must not touch the game data (or call non reentrant functions like sprintf/printf/strtok),
// they only store what happened and the main loop treats it in nifiUpdate()
typedef struct
{
	int length;						  // Length of the payload (with the '\0')
	char data[NIFI_MAX_PAYLOAD_SIZE]; // Payload of the packet
} NifiPacket;

static NifiPacket rxQueue[NIFI_RX_QUEUE_LENGTH];		   // Received packets waiting for treatment
static volatile int rxQueueHead = 0;					   // Index of the next packet to write (only changed by the wifi interrupt)
static volatile int rxQueueTail = 0;					   // Index of the next packet to read (only changed by the main loop)
static u16 rxBuffer[NIFI_RX_BUFFER_SIZE / 2];			   // Buffer used by the wifi interrupt to read a raw packet
static volatile int pendingServerTicks = 0;				   // Number of server timer ticks not treated yet by the main loop
#define MAX_PENDING_SERVER_TICKS (WIFI_TIMEOUT * 4)		   // Avoid a burst of ticks after a long freeze (loading)

////// Room variables :
int hostIndex = EMPTY; // Index of the host in the clients array
bool isHost = false;   // Is the Nintendo DS hosting a room?
// For host
int speakTo = 1;		   // Index of the speak to speak with
int idCount = 0;		   // Id of the next client to add
int lastCommunication = 0; // Countdown to check the wifi timeout
char tempMacAddress[MAC_ADDRESS_LENGTH]; // Mac address of the client that is trying to join the room
int timeoutCount = 0;	   // Store how many times the wifi has failed to communicate with the client
// For non-host
int joinRoomTimer = JOIN_ROOM_FIRST_DELAY; // Timer to limit the time to join a room
bool tryJoinRoom = false;		  // Is the client trying to join a room
bool sendPosition;				  // If true, the client will send its position to the server
bool skipData;					  // If true, the client will skip data of the current wifi packet

////TO DO
// Automatic channel selection (Check how many random packet are received on each channel and select the best one)
// Reset lastMessageId to 0 if the number is too high to not hit the int limit (maybe useless but great to have)

/**
 * @brief Wifi handler call when data is received (called in an interrupt!)
 * Only copy the packet in the receive queue, the data is treated in the main loop by nifiUpdate()
 *
 * @param packetID
 * @param readlength
 */
void WirelessHandler(int packetID, int readlength)
{
	// Ignore packets that are too small or too big for the buffer (packet from another game for example)
	if (readlength <= NIFI_RAW_HEADER_SIZE || readlength > NIFI_RAW_HEADER_SIZE + NIFI_MAX_PAYLOAD_SIZE)
		return;

	Wifi_RxRawReadPacket(packetID, readlength, rxBuffer);

	// Get the correct data length
	char *payload = (char *)rxBuffer + NIFI_RAW_HEADER_SIZE;
	int payloadLength = readlength - NIFI_RAW_HEADER_SIZE;

	// The payload must be a string that ends exactly at the end of the packet
	char *payloadEnd = memchr(payload, '\0', payloadLength);
	if (payloadEnd == NULL || payloadEnd - payload + 1 != payloadLength)
		return;

	// If the queue is full, drop the packet (the host will send it again after the timeout)
	int nextHead = (rxQueueHead + 1) % NIFI_RX_QUEUE_LENGTH;
	if (nextHead == rxQueueTail)
		return;

	memcpy(rxQueue[rxQueueHead].data, payload, payloadLength);
	rxQueue[rxQueueHead].length = payloadLength;
	rxQueueHead = nextHead;
}

/**
 * @brief Server timer handler (called in an interrupt!)
 * Only count the ticks, the server is managed in the main loop by nifiUpdate()
 *
 */
void serverTimerHandler()
{
	if (pendingServerTicks < MAX_PENDING_SERVER_TICKS)
		pendingServerTicks++;
}

/**
 * @brief Treat received packets and manage the server, must be called in the main loop
 *
 */
void nifiUpdate()
{
	// Treat all received packets
	// The wifi interrupt never writes in the slot at rxQueueTail, so it can be read without disabling interrupts
	while (rxQueueTail != rxQueueHead)
	{
		NifiPacket *packet = &rxQueue[rxQueueTail];
		appendToValues(packet->data, packet->length - 1);
		rxQueueTail = (rxQueueTail + 1) % NIFI_RX_QUEUE_LENGTH;

		// Treatment of the data
		treatData();
	}

	// Get the server ticks counted by the timer interrupt
	int oldIME = enterCriticalSection();
	int ticks = pendingServerTicks;
	pendingServerTicks = 0;
	leaveCriticalSection(oldIME);

	for (int i = 0; i < ticks; i++)
	{
		managerServer();
	}
}

/**
 * @brief Add received data to "Values" without overflowing it
 *
 * @param data data to add
 * @param length max length of the data to add
 * @return true if the data was added
 */
bool appendToValues(const char *data, int length)
{
	// Stop at the first '\0'
	const char *dataEnd = memchr(data, '\0', length);
	if (dataEnd != NULL)
		length = dataEnd - data;

	if (length <= 0)
		return false;

	int currentLength = strlen(Values);
	if (currentLength + length >= (int)sizeof(Values))
	{
		// No complete request was found in the old data, it can't be treated, so drop it
		printf("%sVALUES BUFFER FULL%s\n", RED, WHITE);
		currentLength = 0;
		Values[0] = '\0';
		if (length >= (int)sizeof(Values))
			return false;
	}

	memcpy(Values + currentLength, data, length);
	Values[currentLength + length] = '\0';
	return true;
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
	// Stop the server timer
	timerStop(NIFI_SERVER_TIMER);

	// Reset buffers
	Values[0] = '\0';
	tempSendBuffer[0] = '\0';

	// Clear interrupt data
	int oldIME = enterCriticalSection();
	rxQueueHead = 0;
	rxQueueTail = 0;
	pendingServerTicks = 0;
	leaveCriticalSection(oldIME);

	// Reset variables
	lastCommunication = 0;
	hostIndex = EMPTY;
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
	snprintf(localPlayer->client.macAddress, sizeof(localPlayer->client.macAddress), "%02X%02X%02X%02X%02X%02X", macAddressUnsigned[0], macAddressUnsigned[1], macAddressUnsigned[2], macAddressUnsigned[3], macAddressUnsigned[4], macAddressUnsigned[5]);

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
	else if (clientSender == &localPlayer->client && hostIndex != EMPTY) // Share the request to the host only
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

	// Do not use floats (%f) in requests, send fixed point values (x4096) or rounded values instead
	switch (requestType)
	{
	case POS:
		if (senderPlayer == localPlayer)
			snprintf(buffer, sizeof(buffer), "{POS;%d;%d;%d;%d;%d;%d;%d}", senderPlayer->Id, senderPlayer->PlayerModel->x, senderPlayer->PlayerModel->y, senderPlayer->PlayerModel->z, (int)senderPlayer->Angle, (int)roundf(senderPlayer->cameraAngle), clientToUpdate->id);
		else
			snprintf(buffer, sizeof(buffer), "{POS;%d;%d;%d;%d;%d;%d;%d}", senderPlayer->Id, (int)(senderPlayer->lerpDestination.x * 4096), (int)(senderPlayer->lerpDestination.y * 4096), (int)(senderPlayer->lerpDestination.z * 4096), (int)senderPlayer->AngleDestination, (int)roundf(senderPlayer->cameraAngle), clientToUpdate->id);
		break;
	case TEAM:
		snprintf(buffer, sizeof(buffer), "{TEAM;%d;%d;%d}", clientSender->id, tempTeam, clientToUpdate->id);
		break;
	case SHOOT:
		snprintf(buffer, sizeof(buffer), "{SHOOT;%d;%d;%d}", clientSender->id, senderPlayer->AllGunsInInventory[senderPlayer->currentGunInInventory], clientToUpdate->id);
		break;
	case BOMBDEFUSE:
		snprintf(buffer, sizeof(buffer), "{BOMBDEFUSE;%d}", clientToUpdate->id);
		break;
	case GRENADE:
	{
		// TO DO SEND GRENADE TYPE
		float x, y, z;
		UpdateLookRotationAI(senderPlayer->cameraAngle, senderPlayer->AngleDestination, &x, &y, &z);
		// Direction is sent in fixed point (x4096)
		if (senderPlayer == localPlayer)
			snprintf(buffer, sizeof(buffer), "{GRENADE;%d;%d;%d;%d;%d;%d;%d;%d;%d}", (int)(x * 4096), (int)(y * 4096), (int)(z * 4096), senderPlayer->PlayerModel->x, senderPlayer->PlayerModel->y, senderPlayer->PlayerModel->z, 0, clientSender->id, clientToUpdate->id);
		else
			snprintf(buffer, sizeof(buffer), "{GRENADE;%d;%d;%d;%d;%d;%d;%d;%d;%d}", (int)(x * 4096), (int)(y * 4096), (int)(z * 4096), (int)(senderPlayer->lerpDestination.x * 4096), (int)(senderPlayer->lerpDestination.y * 4096), (int)(senderPlayer->lerpDestination.z * 4096), 0, clientSender->id, clientToUpdate->id);
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

		// The timer interrupt only counts ticks, managerServer() is called by nifiUpdate() in the main loop
		timerStart(NIFI_SERVER_TIMER, ClockDivider_1024, TIMER_FREQ_1024(WIFI_RATE), serverTimerHandler);
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
					char buffer[32];
					snprintf(buffer, sizeof(buffer), "{ROOM;QUIT;%d;%d}", client->id, AllPlayers[i].client.id);
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
			snprintf(AllPlayers[addedClientIndex].client.macAddress, sizeof(AllPlayers[addedClientIndex].client.macAddress), "%s", tempMacAddress);
			//  Send the client his id
			char newClientBuffer[128];
			snprintf(newClientBuffer, sizeof(newClientBuffer), "{ROOM;CONFIRM_JOIN;%s;%d;%d}", tempMacAddress, localPlayer->client.id, AllPlayers[addedClientIndex].client.id);
			// Send the client all the other clients ids
			snprintf(newClientBuffer + strlen(newClientBuffer), sizeof(newClientBuffer) - strlen(newClientBuffer), "{ROOM;ADDCLIENTS;%d", AllPlayers[addedClientIndex].client.id);
			// Send the client id to all the other clients
			for (int i = 1; i < MAX_CLIENT; i++)
			{
				if (AllPlayers[i].client.id != EMPTY && i != addedClientIndex)
				{
					// Send the client all the other clients ids
					snprintf(newClientBuffer + strlen(newClientBuffer), sizeof(newClientBuffer) - strlen(newClientBuffer), ";%d", AllPlayers[i].client.id);
					// Send the client id to all the other clients
					char bufferForOtherClients[48];
					snprintf(bufferForOtherClients, sizeof(bufferForOtherClients), "{ROOM;ADDCLIENTS;%d;%d}", AllPlayers[i].client.id, AllPlayers[addedClientIndex].client.id);
					AddDataTo(&AllPlayers[i].client, bufferForOtherClients);
				}
			}
			snprintf(newClientBuffer + strlen(newClientBuffer), sizeof(newClientBuffer) - strlen(newClientBuffer), "}");
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

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "{ROOM;SCAN;%s}", localPlayer->client.macAddress);
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
void AddDataTo(Client *client, const char *data)
{
	if (client != NULL && client->id != EMPTY && data[0] != '\0')
	{
		int currentLength = strlen(client->sendBuffer);
		int dataLength = strlen(data);
		// Never cut a request, drop it if there is not enough space
		if (currentLength + dataLength >= (int)sizeof(client->sendBuffer))
		{
			printf("%sSEND BUF FULL, REQUEST DROPPED%s\n", RED, WHITE);
			return;
		}
		memcpy(client->sendBuffer + currentLength, data, dataLength + 1);
		printf("%sSEND BUF : %s%s\n", YELLOW, client->sendBuffer, WHITE);
	}
}

/**
 * @brief Send data to a client with Nifi
 *
 * @param client client to send data to
 */
void SendDataTo(Client *client)
{
	// If it's not a retry, create a new packet with the client's send buffer
	if (timeoutCount == 0)
	{
		int headerLength;
		if (isHost)
		{
			client->lastMessageId++;

			// Ask to client to communicate
			headerLength = snprintf(tempSendBuffer, sizeof(tempSendBuffer), "{ROOM;WANTSPEAK;%d;%d}", client->id, client->lastMessageId);
		}
		else // Tell the host that the local client has received the message
			headerLength = snprintf(tempSendBuffer, sizeof(tempSendBuffer), "{ROOM;CONFIRM_LISTEN;%d}", localPlayer->client.id);

		// Add as many complete requests as possible, the other requests stay in the send buffer for the next packet
		int availableSpace = (int)sizeof(tempSendBuffer) - 1 - headerLength;
		int dataLength = strlen(client->sendBuffer);
		int lengthToSend = dataLength;
		if (lengthToSend > availableSpace)
		{
			lengthToSend = 0;
			for (int i = availableSpace - 1; i >= 0; i--)
			{
				if (client->sendBuffer[i] == '}')
				{
					lengthToSend = i + 1;
					break;
				}
			}
		}

		memcpy(tempSendBuffer + headerLength, client->sendBuffer, lengthToSend);
		tempSendBuffer[headerLength + lengthToSend] = '\0';

		// Remove sent data from the send buffer
		memmove(client->sendBuffer, client->sendBuffer + lengthToSend, dataLength - lengthToSend + 1);
	}

	// Send data
	SendWirelessData((unsigned short *)tempSendBuffer, strlen(tempSendBuffer) + 1);
}
