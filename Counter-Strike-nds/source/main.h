#ifndef MAIN_H_ /* Include guard */
#define MAIN_H_
#include <NEMain.h>

#include <fat.h>
#include <filesystem.h>

#include "saveManager.h"
#include "ui.h"

// Math functions
#include <math.h>

// Wifi
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Sound system
#include <maxmod9.h>
#include "soundbank.h"
//#include "soundbank_bin.h"

// 3D models
#include "playerAnimNea_bin.h"
#include "GIGNAnimNea_bin.h"
#include "GIGNNew_bin.h"
#include "DustPart0_bin.h"
#include "DustPart1_bin.h"
#include "DustPart2_bin.h"
#include "DustPart3_bin.h"

#include "DustPart4_bin.h"
#include "DustPart5_bin.h"
#include "DustPart6_bin.h"
#include "bomb_bin.h"
#include "plane_bin.h"
#include "repeat_bin.h"
#include "explosion_bin.h"

#include "road_bin.h"
#include "wallWindow_bin.h"
#include "wall_bin.h"
#include "DustPart3_1_3ds_bin.h"
#include "DustPart3_2_3ds_bin.h"
#include "DustPart3_3_3ds_bin.h"

#include "grenade_3ds_bin.h"

// Textures
#include "MapUI_bin.h"
#include "MapPointUI_bin.h"
#include "CheckMark_bin.h"
#include "WhiteScareRounded_bin.h"
#include "QuitButton_bin.h"
#include "scopeImage_bin.h"

#include "reload_bin.h"
#include "JumpArrow_bin.h"

// Player skins
#include "skin_soldier_bin.h"
#include "skin_adventurer_bin.h"
#include "skin_man_bin.h"
#include "gign_skin1_bin.h"
#include "terrorist_skin1_bin.h"

// crosshairs
#include "crosshair2_bin.h"

// Terrain texture
#include "Atlas_bin.h"

// Gun sprites and 3D models
#include "gun_bin.h"
#include "gun1_bin.h"
#include "gun2_bin.h"
#include "gunFixed_bin.h"

#include "knifect_bin.h"
#include "deagle_bin.h"
#include "elite_bin.h"
#include "fivesevent_bin.h"
#include "glock18_bin.h"
#include "p228_bin.h"
#include "uspsilencer_bin.h"
#include "m3super90_bin.h"
#include "xm1014_bin.h"
#include "mac10_bin.h"
#include "mp5_bin.h"
#include "m249_bin.h"
#include "p90_bin.h"
#include "tmp_bin.h"
#include "ump45_bin.h"
#include "ak47_bin.h"
#include "aug_bin.h"
#include "famas_bin.h"
#include "galil_bin.h"
#include "m4a1_bin.h"
#include "sg552_bin.h"
#include "sg550_bin.h"
#include "scout_bin.h"
#include "g3sg1_bin.h"
#include "awp_bin.h"
#include "fraggrenade_bin.h"
#include "smokegrenade_bin.h"
#include "flashthrowed_bin.h"
#include "c4_bin.h"

#include "muzzle_bin.h"

// Font
#include "text_bmp_bin.h"

//////All defines
#define Degres90 128

#define JumpForce 620
#define MaxPlayer 6
#define RaycastDistance 40
#define RayAccuracy 4

#define GunPositionX 160 // 256-96
#define GunPositionY 96	 // 192-96

#define MapPartCount 7

#define OcclusionZonesCount 21

#define bombDefuseTime 600	 // 10 secs
#define bombPlantingTime 210 // 3,5 secs

#define maxPoint 58
#define maxPath 15
#define ScreenShakeAmount 5
#define ScreenShakeMinAmount 3

#define inventoryCapacity 9
#define GunCount 25
#define shopGrenadeCount 3
#define GrenadeCount 20
#define equipementCount 1 // later 5 or 4 (bomb, headset, kevlar, defuser)
#define InventoryGrenadeStartPosition 3
#define InventoryEquipementStartPosition 7

#define defaultWalkSpeed 220
#define flashAnimationSpeed 0.0035

#define DEFAULTTERRORISTGUN 4
#define DEFAULTCOUNTERTERRORISTGUN 6
#define wallCount 210

#define WaypointA 14
#define WaypointB 29

#define NO_PLAYER -1
#define EMPTY -1
#define UNUSED -1

#define StairsCount 31
#define grenadeBoughtLength 5

#define CameraOffsetY 0.7
#define CameraOffsetYMultiplied 2867.20 // 0.7 * 4096

enum connectionType
{
	UNSELECTED,
	OFFLINE,
	LOCAL,
	IP1,
	IP2
};

enum actionAfterUiTimer
{
	SAVE
};

enum team
{
	SPECTATOR = -1,
	TERRORISTS = 0,
	COUNTERTERRORISTS = 1
};

//////All structs

// 3D point with angle
typedef struct
{
	float x;
	float y;
	float z;
	int r;
} Vector4;

// 3D point
typedef struct
{
	float x;
	float y;
	float z;
} Vector3;
// 3D point
typedef struct
{
	int x;
	int y;
	int z;
} Vector3Int;

// 2D point
typedef struct
{
	float x;
	float y;
} Vector2;

typedef struct
{
	int x;
	int y;
} Vector2Int;

typedef struct // Physics values for raycasting
{
	float xSize;
	float ySize;
	float zSize;
	int BoxXRangeA;
	int BoxXRangeB;
	int BoxYRangeA;
	int BoxYRangeB;
	int BoxZRangeA;
	int BoxZRangeB;

	Vector3 corner1;
	Vector3 corner2;
} CollisionBox;

typedef struct // Physics values for raycasting
{
	int waypoint;
	CollisionBox collisionBox;
} Site;

typedef struct // 2D area (box) values for trigger
{
	float BoxXRangeA;
	float BoxXRangeB;
	float BoxZRangeA;
	float BoxZRangeB;
} CollisionBox2D;

typedef struct // 2D area (box) values for trigger
{
	Vector2 angles[4];
	Vector2Int anglesInt[4];
} OcclusionZone;

typedef struct //
{
	int AmmoCount;
	int TotalAmmoCount;
} AmmoMagazine;

typedef struct // Player values
{
	int Id;

	Vector3 position;
	Vector3 lerpDestination;
	float Angle;
	float AngleDestination;

	// int IsCounter;
	enum team Team;
	int Money;
	int Health;
	int KillCount;
	int DeathCount;
	int GunWaitCount;
	int GunReloadWaitCount;
	bool isReloading;
	int currentGunInInventory;
	AmmoMagazine AllAmmoMagazine[2];
	int AllGunsInInventory[inventoryCapacity];
	int grenadeBought[grenadeBoughtLength];
	int armor;		  //
	bool haveHeadset; //
	int spawnAt;

	CollisionBox PlayerCollisionBox; //
	NE_Model *PlayerModel;			 //
	NE_Physics *PlayerPhysic;		 //
	bool IsDead;					 //
	bool haveDefuseKit;				 //
	bool haveBomb;					 //
	bool isPlantingBomb;			 //
	int bombTimer;					 //
	int rightGunXRecoil;			 //
	int rightGunYRecoil;			 //
	int leftGunXRecoil;				 //
	int leftGunYRecoil;				 //
	bool isRightGun;				 // For dual guns, 0 right, 1 left

	// AI
	int Path[maxPath];
	bool isAi;			//
	int PathCount;		//
	int CurrentPath;	//
	int LastWayPoint;	//
	int target;			//
	int lastSeenTarget; //
	bool justCheking;
	bool canCancelNextCheck;
	bool canShootEnemy;
	bool tooFar;
	bool searchForDroppedBomb;

	// Raycasting
	bool IsHeadShot;
	bool IsLegShot;
	int OldStopAt;
	int StopAt;
	int PlayerFoundAtDistance;
	bool ScanFinished;

	Vector3 startRaycastPosition;
	Vector3 startRaycastRotation;
	int startGunIdRaycast;

	int CurrentOcclusionZone; //

	int invisibilityTimer;

	int RespawnTimer;			 //
	bool NeedRespawn;			 //
	float BobbingOffset;		 //
	bool HasBobbed;				 //
	int Step;					 //
	char name[playerNameLength]; //
	bool allPlayerScanned[MaxPlayer];
} Player;

typedef struct // 2D area (box) values for trigger with stairs start height, final height and direction
{
	float xSideA;
	float xSideB;
	float zSideA;
	float zSideB;
	float startY;
	float endY;
	int direction;
} Stairs;

typedef struct // Physics values for raycasting
{
	Vector3Int position;
	/*int xPos;
	int yPos;
	int zPos;*/
	int ZoneCollision;
	CollisionBox WallCollisionBox;
	NE_Model *WallModel;
	NE_Physics *WallPhysics;
} Wall;

typedef struct //
{
	int id;
	int ZoneCount;
	int AllVisibleZones[5];
} Zone;

typedef struct //
{
	int scopeCount;
	int fov[2];
	int Speed;
} Scope;

typedef struct //
{
	void *texture;
	// int Damage;
	// int WalkSpeed;
	int isForCounterTerrorists; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
	int Price;
	char name[20];
	char description[50];
	int type;
	int maxQuantity[3];
	// char name[];
	mm_word collisionSound;
	mm_word finalSound;

	// int Timer;
	// int EffectTimer;
} Grenade;

typedef struct //
{
	void *texture;
	int Price;
	char name[20];
	char description[50];
	int isForCounterTerrorists; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
	mm_word usageSound;
} Equipement;

typedef struct
{
	NE_Model *Model;
	NE_Physics *Physic;
	NE_Model *EffectModel;
	int GrenadeType;
	int Timer;
	int EffectTimer;
	int effectAlpha;
	bool isVisible;
	int lastCollisionTimer;
	int LastStairs;
} PhysicalGrenade;

typedef struct //
{
	void *texture;
	char letter[2];
	char letterUpperCase[2];
	int xPos;
	int yPos;
} Key;

typedef struct //
{
	void *texture;
	char name[8];
	// int action;
	int xPos;
	int yPos;
	int xSize;
	int xCenter;
	int ySize;
	void (*OnClick)();
} OtherKey;

extern float flashaAnimation;
extern bool flashed;
extern int ShowMuzzle;
// extern int currentGunInInventory;
extern int rightGunX;
extern int rightGunY;
extern int leftGunX;
extern int leftGunY;
extern int rightGunXRecoil;
extern int rightGunYRecoil;
extern int GunMaxRecoil;
extern bool isInFullSmoke;
extern float BobbingOffset;
extern int redHealthTextCounter;

extern bool BombPlanted;
extern int bombTimer;
extern int CurrentScopeLevel;

extern int KillTextShowTimer;
// extern int AllGunsInInventory[inventoryCapacity];
extern NE_Material *TopScreenSpritesMaterials[5];
extern bool isDebugTopScreen;
// extern AmmoMagazine AllAmmoMagazine[2];
extern NE_Sprite *TopScreenSprites[5];
extern char killText[33];
extern NE_Camera *Camera;

extern Zone AllZones[OcclusionZonesCount];
extern int textToShowTimer;
extern int CurrentCameraPlayer;
// extern int Money;
extern char textToShow[30];
extern bool NeedChangeScreen;
extern bool AlwaysUpdateBottomScreen;
extern int UpdateBottomScreenOneFrame;
extern Player AllPlayers[MaxPlayer];
extern int SelectedGunShop;
extern bool WaitForTeamResponse;
extern Button AllButtons[7];
extern CheckBox AllCheckBoxs[CheckBoxCount];
extern int ButtonToShow;
extern int currentMenu;
extern NE_Material *BottomScreenSpritesMaterials[9];
extern NE_Sprite *BottomScreenSprites[10];
extern int BottomScreenSpriteCount;
extern CollisionBox2D AllTriggersCollisions[OcclusionZonesCount];
extern NE_Material *GroundMaterial;
extern Stairs AllStairs[StairsCount];
extern float CameraAngleY;
extern float xWithoutYForMap;
extern float zWithoutYForMap;
extern float xWithoutYForOcclusionSide1;
extern float zWithoutYForOcclusionSide1;
extern float xWithoutYForOcclusionSide2;
extern float zWithoutYForOcclusionSide2;

extern int WallHitXPos;
extern int WallHitYPos;
extern int WallHitZPos;
// extern bool IsHeadShot;
// extern bool IsLegShot;
extern int tempTeam;
// extern bool ScanFinished;
extern float OldxPos, OldyPos, OldzPos;
extern int Hit;
extern float xOffset, yOffset;
extern int PlayerCount;
extern float deathCameraYOffset;
extern float deathCameraAnimation;
extern int BombExplosionScale;
extern bool IsExplode;
extern int bombBipTimer;
extern bool BombDefused;
extern NE_Model *Model[11];
extern bool BombWillExplose;
extern CollisionBox2D bombDefuseZone;
extern float x, y, z;
extern float xWithoutY, zWithoutY, xWithoutYForAudio, zWithoutYForAudio;
extern Vector4 BombPosition;
extern int ShowWallHitFlash;
extern bool applyRules;
// extern int PlayerFoundAtDistance;
// extern int OldStopAt;
// extern int StopAt;
extern Wall AllWallsCollisions[wallCount];
// extern int CurrentOcclusionZone;
extern int doubleTapTimer;
extern Scope AllScopeLevels[2];
extern bool PartyStarted;
extern enum connectionType Connection;
extern int checkPlayerDistanceFromAiTimer;
extern Player *localPlayer;
extern touchPosition touch;
extern uint32 keys;
extern uint32 keysdown;
extern uint32 keysup;
extern OcclusionZone AllOcclusionZone[7];
extern int uiTimer;
extern enum actionAfterUiTimer actionOfUiTimer;
extern int RumbleTimer;
extern Site AllBombsTriggersCollisions[2];
extern bool useRumble;
extern bool is3dsMode;

/*typedef enum
{
	UNSELECTED,
	OFFLINE,
	LOCAL,
	IP1,
	IP2
} connectionType;*/

//////All functions
void CalculatePlayerPosition(int PlayerId);
void GameLoop();
void AddNewPlayer(int NewId, bool IsLocalPlayer, bool isAI);
void DrawGun();
double map(double x, double in_min, double in_max, double out_min, double out_max);
int mapInt(int x, int in_min, int in_max, int out_min, int out_max);
void ReadOneIntData(char *data, int *result, int EndPosition, int CallTypeEndPosition);
void ReadPlayerIdInt(char *data, int *result, int StartPosition, int FromPlayerIdEndPosition);
void UpdatePlayerTexture(int playerIndex);
void CheckTeamDeathCount(int *TerroristsCount, int *CounterTerroristsCount, int *TerroristDeadCount, int *CounterDeadCount);
void setPlayerPositionAtSpawns(int playerIndex);
void SetTwoScreenMode(bool value);

Stairs *GetStairs();
Wall *GetWalls();
Player *GetPlayers();
Button *GetButtons();
CheckBox *GetCheckBoxs();
NE_Sprite **GetSpritesTop();
NE_Sprite **GetSpritesBottom();
NE_Material **GetBottomScreenSpritesMaterials();
NE_Palette **GetPalettes();

void checkAfterDamage(int shooterPlayerIndex, int hittedPlayerIndex, bool CheckScore);
void StartGame();
void CheckJump();
int GetCurrentMenu();
void SetCurrentMenu(int value);
int GetUpdateBottomScreenOneFrame();
void SetUpdateBottomScreenOneFrame(int value);
int GetButtonToShow();
void SetButtonToShow(int value);
void StartSinglePlayer();
void SetWaitForTeamResponse(bool value);
void addMoneyToTeam(int Money, enum team Team);
bool GetAlwaysUpdateBottomScreen();
bool GetNeedUpdateViewRotation();
bool GetSendPosition();
float GetCameraAngleY();
int GetCurrentScopeLevel();
void SetCameraAngleY(float Value);
void SetAlwaysUpdateBottomScreen(bool value);
void SetSendPosition(bool Value);
bool GetNeedChangeScreen();
void SetDoubleTapTimer(int Value);
int GetDoubleTapTimer();
void SetNeedChangeScreen(bool value);
void SetSendTeam(bool Value);
void SetTempTeam(int Value);
int GetTempTeam();
void UpdateEngineNotInGame();
void UpdateEngine();
Grenade *GetAllGrenades();
void SetNeedJump();
void SetSelectedGunShop(int Value);
int GetSelectedGunShop();
void SetCurrentCameraPlayer(int Value);
int GetCurrentCameraPlayer();
void SetGunInInventory(int Value, int inventoryIndex);
void SetGunInInventoryForNonLocalPlayer(int playerIndex, int Value, int inventoryIndex);
void SetSendBuyWeapon(bool Value);
void SetNeedUpdateViewRotation(bool Value);
void ChangeGunInInventoryForLocalPlayer(int Left);
void ChangeGunInInventory(int playerIndex, int Left);
void StopReloading(int playerIndx);
void startReloadGun(int playerIndx);
void UpdateGunTexture();
void setPlayerHealth(int playerIndex, int health);
void setPlayersPositionAtSpawns();
void resetPlayer(int index);
void setBombForARandomPlayer();
void setNewRoundHandWeapon();
void makeHit(int hitBy, int playerHit);
void buyGun();
void CheckAfterRound();
void setPlayerMoney(int playerIndex, int Money);
void addPlayerMoney(int playerIndex, int Money);
void reducePlayerMoney(int playerIndex, int Money);
void removeAllPlayers();
void rumble(int timer);

#endif // MAIN_H_