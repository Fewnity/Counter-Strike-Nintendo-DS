#ifndef NIFI_H_ /* Include guard */
#define NIFI_H_

#define EMPTY -1

// Nifi settings
#define MAX_CLIENT 6 // Max number of clients
//#define WIFI_RATE 2  // Read/Write rate per second
//#define WIFI_RATE 5 // Read/Write rate per second
#define WIFI_RATE 240 // Read/Write rate per second
//#define WIFI_TIMEOUT 2 // Number of wifi rate before the message is considered lost
#define WIFI_TIMEOUT 10      // Number of wifi rate before the message is considered lost
#define MAX_TIMEOUT_RETRY 10 // Number of try before client is considered as disconnected
#define CLIENT_TIMEOUT 600   // Number of frame before local client is considered as disconnected

// Network packet settings
// request example : {TYPE;NAME;PARAM1;PARAM2}
//#define REQUEST_TYPE_INDEX 0
//#define REQUEST_NAME_INDEX 1
#define MAX_REQUEST_LENGTH 256
#define MAX_REQUEST_PARAM_LENGTH 64
#define MAX_REQUEST_PARAM_COUNT 10

extern char WIFI_Buffer[1024]; // Wifi buffer is the incoming data from the wifi (DO NOT USE LIKE THIS, please use the "wirelessData" define)
extern int WIFI_ReceivedDataLength;
extern int joinRoomTimer; // Timer to limit the time to join a room
extern bool tryJoinRoom;  // Is the client trying to join a room
extern char tempSendBuffer[1024];
extern bool skipData;
extern int hostIndex;
extern int lastCommunication;
extern int speakTo;
extern bool isHost;
extern char tempMacAddress[13];
extern char Values[1024];     // Store all the values received from the wifi and wainting for treatment
extern char TempValues[1024]; // Store values that can't be treated yet
extern int timeoutCount;

#define wirelessData (WIFI_Buffer + 32) // Define to use the WIFI_Buffer array to avoid wrong values

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
    char macAddress[13];   // Mac address of the client
    char sendBuffer[2048]; // Wifi write buffer of the client
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

#endif // MAIN_H_