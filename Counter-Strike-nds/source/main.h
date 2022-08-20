// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef MAIN_H_ /* Include guard */
#define MAIN_H_
#include <NEMain.h>

#include <fat.h>
#include <filesystem.h>

#include "saveManager.h"
#include "ui.h"
#include "grenade.h"

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
#include "smokeSphere_bin.h"

#include "tutorialMapUnShadowed_3ds_bin.h"
#include "tutorialMapShadowed_3ds_bin.h"

#include "grenade_3ds_bin.h"

// Textures
#include "MapUI_bin.h"
#include "bomb_logo_bin.h"
#include "MapPointUI_bin.h"
#include "CheckMark_bin.h"
#include "WhiteScareRounded_bin.h"
#include "QuitButton_bin.h"
#include "scopeImage_bin.h"
#include "player_shadow_bin.h"

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
#include "kevlar_bin.h"
#include "kevlar_helmet_bin.h"
#include "defuser_bin.h"

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

//#define OcclusionZonesCount 21

#define bombDefuseTime 600	 // 10 secs
#define bombPlantingTime 210 // 3,5 secs

#define maxPoint 58
#define maxPath 15
#define ScreenShakeAmount 5
#define ScreenShakeMinAmount 3

#define inventoryCapacity 9
#define GunCount 25

#define GrenadeCount 20

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

//#define StairsCount 31
#define grenadeBoughtLength 5

#define CameraOffsetY 0.7
#define CameraOffsetYMultiplied 2867.20 // 0.7 * 4096

#define FLASH_MODELS_COUNT 6

#define INPUT_COUNT 14
#define INPUT_NAMES_COUNT 15
#define SHOP_DISABLE_TIMER 900

#define GAME_VERSION "1.0.0"

enum connectionType
{
	UNSELECTED,
	OFFLINE,
	DEBUG_IP_2,
	DEBUG_IP_1,
	ONLINE_SERVER_IP,
	LOCAL,
};

enum inputButtons
{
	FIRE_BUTTON = 0,
	JUMP_BUTTON = 1,

	LEFT_BUTTON = 2,
	RIGHT_BUTTON = 3,
	UP_BUTTON = 4,
	DOWN_BUTTON = 5,
	LOOK_LEFT_BUTTON = 6,
	LOOK_RIGHT_BUTTON = 7,
	LOOK_UP_BUTTON = 8,
	LOOK_DOWN_BUTTON = 9,

	DEFUSE_BUTTON = 10,
	SCOPE_BUTTON = 11,

	LEFT_GUN = 12,
	RIGHT_GUN = 13,
};

enum teamEnum
{
	SPECTATOR = -1,
	BOTH = -1,
	TERRORISTS = 0,
	COUNTERTERRORISTS = 1
};

enum actionAfterUiTimer
{
	SAVE
};

//////All structs

typedef struct
{
	int value;
	// enum KEYPAD_BITS value;
	//  const char *name;
	int nameIndex;
} Input;

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
	int BoxXRangeA;
	int BoxXRangeB;
	int BoxYRangeA;
	int BoxYRangeB;
	int BoxZRangeA;
	int BoxZRangeB;
} CollisionBox;

typedef struct // Physics values for raycasting
{
	float BoxXRangeA;
	float BoxXRangeB;
	float BoxYRangeA;
	float BoxYRangeB;
	float BoxZRangeA;
	float BoxZRangeB;
} CollisionBoxF;

typedef struct // 2D area (box) values for trigger
{
	float BoxXRangeA;
	float BoxXRangeB;
	float BoxZRangeA;
	float BoxZRangeB;
} CollisionBox2D;

typedef struct // Physics values for raycasting
{
	int waypoint;
	int *nearWaypoints;
	int nearWaypointCount;
	CollisionBox2D collisionBox;
} Site;

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
	float cameraAngle;
	float Angle;
	float AngleDestination;

	// int IsCounter;
	enum teamEnum Team;
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

	float flashAnimation;
	bool flashed;

	CollisionBox PlayerCollisionBox; //
	NE_Model *PlayerModel;			 //
	NE_Model *PlayerShadow;			 //
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
	bool tooFar;
	bool searchForDroppedBomb;

	// Raycasting
	bool IsHeadShot[FLASH_MODELS_COUNT];
	bool IsLegShot[FLASH_MODELS_COUNT];

	int ScanForGrenade;

	Vector3 startRaycastPosition;
	Vector3 startRaycastRotation;
	int startGunIdRaycast;

	int CurrentOcclusionZone; //

	int invincibilityTimer;

	int RespawnTimer;			  //
	bool NeedRespawn;			  //
	float BobbingOffset;		  //
	bool HasBobbed;				  //
	int Step;					  //
	char name[PLAYER_MAX_LENGTH]; //
	bool allPlayerScanned[MaxPlayer];
	float xSize;
	float ySize;
	float zSize;
	bool inShadow;
	int currentShadowCollBox;
	float lightCoef;
	int mapVisivilityTimer;
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
	int ZoneCollision;
	CollisionBox WallCollisionBox;
	NE_Model *WallModel;
	NE_Physics *WallPhysics;
} Wall;

typedef struct //
{
	int id;
	int ZoneCount;
	int visibleMapPart[5];
	CollisionBox2D collisionBox;
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
	char letter[2];
	char letterUpperCase[2];
	int xPos;
	int yPos;
	bool visible;
} Key;

typedef struct //
{
	void *texture;
	char name[8];
	int xPos;
	int yPos;
	int xSize;
	int xCenter;
	int ySize;
	bool visible;
	void (*OnClick)();
} OtherKey;

extern float flashaAnimation;
extern bool flashed;
extern int ShowMuzzle;
extern int rightGunX;
extern int rightGunY;
extern int leftGunX;
extern int leftGunY;
extern int rightGunXRecoil;
extern int rightGunYRecoil;
extern int GunMaxRecoil;
extern int GunMinRecoil;
extern bool isInFullSmoke;
extern int redHealthTextCounter;

extern bool BombPlanted;
extern int bombTimer;
extern int CurrentScopeLevel;

extern int KillTextShowTimer;

extern NE_Sprite *TopScreenSprites[2];
extern NE_Camera *Camera;
extern NE_Material *PlayerMaterial;
extern NE_Material *PlayerMaterialTerrorist;
extern NE_Material *PlayerShadowMaterial;

extern int textToShowTimer;
extern int CurrentCameraPlayer;

extern bool NeedChangeScreen;
extern bool AlwaysUpdateBottomScreen;
extern int UpdateBottomScreenFrameCount;
extern Player AllPlayers[MaxPlayer];
extern int SelectedGunShop;
extern bool WaitForTeamResponse;
extern Button AllButtons[ButtonCount];
extern CheckBox AllCheckBoxs[CheckBoxCount];
extern Slider AllSliders[SliderCount];
extern int ButtonToShow;
extern int currentMenu;
extern NE_Sprite *BottomScreenSprites[1];
extern NE_Material *GroundMaterial;
extern NE_Material *GroundMaterialShadowed;
extern float xWithoutYForMap;
extern float zWithoutYForMap;
extern float xWithoutYForOcclusionSide1;
extern float zWithoutYForOcclusionSide1;
extern float xWithoutYForOcclusionSide2;
extern float zWithoutYForOcclusionSide2;
extern int WallHitXPos[FLASH_MODELS_COUNT];
extern int WallHitYPos[FLASH_MODELS_COUNT];
extern int WallHitZPos[FLASH_MODELS_COUNT];
extern float hitDistance[FLASH_MODELS_COUNT];

extern int tempTeam;

extern float OldxPos, OldyPos, OldzPos;
extern int hittedClient[FLASH_MODELS_COUNT];
extern float xOffset, yOffset;
extern int PlayerCount;
extern float deathCameraYOffset;
extern float deathCameraAnimation;
extern int BombExplosionScale;
extern bool IsExplode;
extern int bombBipTimer;
extern bool BombDefused;
extern NE_Model *Model[13];
extern NE_Model *flashModels[FLASH_MODELS_COUNT];
extern bool BombWillExplode;
extern CollisionBox2D bombDefuseZone;
extern float x, y, z;
extern float xWithoutY, zWithoutY, xWithoutYForAudio, zWithoutYForAudio;
extern Vector4 BombPosition;
extern int ShowWallHitFlash[FLASH_MODELS_COUNT];
extern bool applyRules;
extern float BobbingSpeed;

extern int doubleTapTimer;
extern Scope AllScopeLevels[2];
extern bool PartyStarted;
extern enum connectionType Connection;
extern Player *localPlayer;

extern int uiTimer;
extern enum actionAfterUiTimer actionOfUiTimer;
extern int RumbleTimer;
extern Site AllBombsTriggersCollisions[2];
extern bool useRumble;
extern int shopDisableTimer;
extern CollisionBoxF shopZone;
extern bool isInShopZone;
extern bool NeedUpdateViewRotation;
extern int changeSecondTimer;
extern int changeMinuteTimer;
extern int LoseCountTerrorists;
extern int LoseCountCounterTerrorists;
extern bool bombSet;
extern int frameCountDuringAir;
extern bool NeedJump;
extern bool canChangeGun;
extern bool canShoot;
extern int playerWantToStart;
extern int playerWantToStartLimite;

//////All functions
void GameLoop();
double map(double x, double in_min, double in_max, double out_min, double out_max);
int mapInt(int x, int in_min, int in_max, int out_min, int out_max);
void JoinParty(int option);
void setNeedRespawn(Player *player);
void increaseFrameCount();
void resetFrameCount();

Stairs *GetStairs();
Wall *GetWalls();
Player *GetPlayers();
Button *GetButtons();
CheckBox *GetCheckBoxs();
NE_Sprite **GetSpritesTop();
NE_Sprite **GetSpritesBottom();
NE_Material **GetBottomScreenSpritesMaterials();
NE_Palette **GetPalettes();

void checkStartGameLoop();
int GetCurrentMenu();
void SetCurrentMenu(int value);
int GetUpdateBottomScreenOneFrame();
void SetUpdateBottomScreenOneFrame(int value);
int GetButtonToShow();
void SetButtonToShow(int value);

void SetWaitForTeamResponse(bool value);
void dropBomb(Player *HittedClient, int hittedPlayerIndex);

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
void SetSendBuyWeapon(bool Value);
void SetNeedUpdateViewRotation(bool Value);
void setPlayersPositionAtSpawns();

void rumble(int timer);
bool isLocalPlayerMoving();

#endif // MAIN_H_