#ifndef NIFI_H_ /* Include guard */
#define NIFI_H_

#define EMPTY -1

// Nifi settings
#define MAX_CLIENT 6 // Max number of clients
//#define WIFI_RATE 2  // Read/Write rate per second
//#define WIFI_RATE 5 // Read/Write rate per second
#define WIFI_RATE 240 // Read/Write rate per second
//#define WIFI_TIMEOUT 2 // Number of wifi rate before the message is considered lost
// Data is now treated in the main loop (once per frame) instead of in the wifi interrupt,
// so the timeout must be longer than a few frames to avoid useless retries (20 / 240 = ~83ms)
#define WIFI_TIMEOUT 20      // Number of wifi rate before the message is considered lost
#define MAX_TIMEOUT_RETRY 10 // Number of try before client is considered as disconnected
#define CLIENT_TIMEOUT 600   // Number of frame before local client is considered as disconnected
#define JOIN_ROOM_FIRST_DELAY 10 // Number of frame before the first room scan request
#define JOIN_ROOM_RETRY_DELAY 50 // Number of frame between two room scan requests

// Hardware timer used to manage the host server.
// Timer 3 is used by dswifi (Wifi_InitDefault) and timer 1 by maxmod (audio stream), do not use them
#define NIFI_SERVER_TIMER 2

// Nifi buffers settings
#define NIFI_MAX_PAYLOAD_SIZE 1024                                // Max size of a nifi packet payload (with the '\0')
#define NIFI_RAW_HEADER_SIZE 32                                   // Size of the header before the payload in a received raw packet
#define NIFI_RX_BUFFER_SIZE (NIFI_MAX_PAYLOAD_SIZE + NIFI_RAW_HEADER_SIZE + 2) // Size of the buffer used to read a raw packet
#define NIFI_RX_QUEUE_LENGTH 8                                    // Number of received packets that can wait for treatment
#define CLIENT_SEND_BUFFER_SIZE 2048                              // Size of the send buffer of each client
#define MAC_ADDRESS_LENGTH 13                                     // Mac address in hexadecimal (12 chars + '\0')

// Network packet settings
// request example : {TYPE;NAME;PARAM1;PARAM2}
//#define REQUEST_TYPE_INDEX 0
//#define REQUEST_NAME_INDEX 1
#define MAX_REQUEST_LENGTH 256
#define MAX_REQUEST_PARAM_LENGTH 64
#define MAX_REQUEST_PARAM_COUNT 10

extern int joinRoomTimer; // Timer to limit the time to join a room
extern bool tryJoinRoom;  // Is the client trying to join a room
extern char tempSendBuffer[NIFI_MAX_PAYLOAD_SIZE];
extern bool skipData;
extern int hostIndex;
extern int lastCommunication;
extern int speakTo;
extern bool isHost;
extern char tempMacAddress[MAC_ADDRESS_LENGTH];
extern char Values[1024]; // Store all the values received from the wifi and wainting for treatment
extern int timeoutCount;

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

typedef struct
{
    int id;                // Id of the client
    char macAddress[MAC_ADDRESS_LENGTH];      // Mac address of the client
    char sendBuffer[CLIENT_SEND_BUFFER_SIZE]; // Wifi write buffer of the client
    int lastMessageId;     // Last message id received by the client
} Client;

//////All functions
// void treatDataOld();
void sendPacketByte(u8 command, u8 data);
void createRoom();
void scanForRoom();
void addClient(int id, bool addHost);
void SendDataTo(Client *client);
void AddDataTo(Client *client, const char *data);
void removeClient(Client *client);
void resetClientValues(Client *client);
void managerServer();
void communicateWithNextClient();
void nifiInit();
Client *getClientById(int consoleId);
void shareRequest(Client *clientSender, enum RequestType requestType);
void createRequest(Client *clientSender, Client *clientToUpdate, enum RequestType requestType);
void resetNifiValues();
void nifiUpdate();
bool appendToValues(const char *data, int length);

#endif // MAIN_H_