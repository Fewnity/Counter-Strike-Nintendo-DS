////
// Creator : Fewnity
//
// dd/mm/yyyy
// Creation date --/01/2021
//
// Last modification : 19/12/2021
//
// This file need Nitro Engine
////

#include "collisions.h"
#include "movements.h"
#include "sounds.h"
#include "main.h"
#include "network.h"
#include "debug.h"
#include "ui.h"
#include "gun.h"
#include "grenade.h"
#include "equipment.h"
#include "ai.h"
#include "map.h"
#include "party.h"
#include "keyboard.h"
#include "draw3d.h"
#include "saveManager.h"
#include "input.h"
#include "player.h"
#include "tutorial.h"

//
//////Level
// Whole level 3D models and physics
NE_Model *Model[13]; ////////////REMOVE OR CHANGE THIS

// Materials
NE_Material *GroundMaterial;
NE_Material *GroundMaterialShadowed;

//
//////Players
Player AllPlayers[MaxPlayer];
// Player values
int PlayerCount = 0;
float OldxPos, OldyPos, OldzPos = 0;
int Health = 100;
int CanJump = 0;
int CanJumpRealTimer = 2;
bool Aiming = false;

int frameCountDuringAir = 0;
bool isOnStairs = false;
bool isInDownStairs = false;
bool NeedJump = false; // Ask jump from UI button
// Head bobing
float BobbingOffset = 0;
float XBobbingOffset = 0;
float BobbingSpeed = 0.07;
bool HasBobbed = false;
// Materials
NE_Material *PlayerMaterial;
NE_Material *PlayerMaterialTerrorist;
NE_Material *PlayerShadowMaterial;
//////Guns
// Values

int ShowMuzzle = 0;
int ShowWallHitFlash = 0;
int rightGunX = 0;
int rightGunY = 0;
int leftGunX = 0;
int leftGunY = 0;

Scope AllScopeLevels[2];
int CurrentScopeLevel = 0;

// UI
// int rightGunXRecoil = 6, rightGunYRecoil = 6;
int GunMaxRecoil = 16;
int GunMinRecoil = 6;

// Animations

//
//////Camera
NE_Camera *Camera;
// Values
float x = 0, y = 0, z = 0;
float xOffset = 0, yOffset = 0;
float CameraAngleY = 128;

float xWithoutY = 0, zWithoutY = 0, xWithoutYForAudio = 0, zWithoutYForAudio = 0, xWithoutYForMap = 0, zWithoutYForMap = 0, xWithoutYForOcclusionSide1 = 0, zWithoutYForOcclusionSide1 = 0, xWithoutYForOcclusionSide2 = 0, zWithoutYForOcclusionSide2 = 0;
int CurrentCameraPlayer = 0;
bool NeedUpdateViewRotation = false;

// Animations
#define deathCameraAnimationSpeed 0.025
float deathCameraAnimation = 0;
float deathCameraYOffset = 0;

// Raycasting
int Hit = -1;

int WallHitXPos = 0;
int WallHitYPos = 0;
int WallHitZPos = 0;

// Other
bool isInFullSmoke = false;

// Other
int tempTeam = 0;

// AI
int checkPlayerDistanceFromAiTimer = 1;

//
//////UI
NE_Palette *Palettes[17]; // 0 MapUI, 1 Map atlas, 2 text, 3 player, 4 gun sprite, 5 map point

NE_Palette *roadPalette;
NE_Palette *wallWindowPalette;
NE_Palette *wallPalette;
NE_Material *roadMaterials;
NE_Material *wallWindowMaterials;
NE_Material *wallMaterials;
NE_Material *TextMaterial;

int TopScreenSpriteCount = 1;
NE_Sprite *TopScreenSprites[5];
NE_Material *TopScreenSpritesMaterials[6];
int BottomScreenSpriteCount = 6;
NE_Sprite *BottomScreenSprites[10];
NE_Material *BottomScreenSpritesMaterials[9]; // Remove this
CheckBox AllCheckBoxs[CheckBoxCount];
Slider AllSliders[SliderCount];
Button AllButtons[8];

char textToShow[30] = "";
char killText[33] = "";
int textToShowTimer = 0;
int KillTextShowTimer = 0;
int SelectedGunShop = 0;

bool AlwaysUpdateBottomScreen = true;
bool NeedChangeScreen = false;
int UpdateBottomScreenOneFrame = 8;
int ButtonToShow = 0;
int redHealthTextCounter = 0;
int doubleTapTimer = 0;

//
//////Party
int currentMenu = -1; // 0 Main menu //1 Map //2 Score //3 Shop categories //4 Settings //5 Quit //6 Shop
bool PartyStarted = false;

bool applyRules = false;
int LoseCountTerrorists = 0;
int LoseCountCounterTerrorists = 0;

int changeSecondTimer = 60;
int changeMinuteTimer = 60;
bool bombSet = false;

//
//////Bomb
int bombBipTimer = 120;
Vector4 BombPosition;
bool BombPlanted = false;
bool BombDefused = false;
bool BombWillExplose = false;
CollisionBox2D bombDefuseZone;
int BombExplosionScale = 0;
bool IsExplode = false;

int shopDisableTimer = SHOP_DISABLE_TIMER;
CollisionBoxF shopZone;
bool isInShopZone = false;

//
//////Debug
bool isDebugTopScreen = false;
bool isDebugBottomScreen = false;

//
//////Rumble pack
int RumbleTimer = 0;

enum connectionType Connection = UNSELECTED;

Player *localPlayer = &AllPlayers[0];

int uiTimer = 0;

enum actionAfterUiTimer actionOfUiTimer = SAVE;
int speedAimingReCenter = 3;
int speedAimingReCenterTimer = 10;

bool useRumble = false;
bool is3dsMode = false;

Input inputs[INPUT_COUNT];
bool scanForInput = false;
int currentInputScanned = 0;
const char *inputsNames[INPUT_NAMES_COUNT];

bool isUsingBomb = false;
bool canChangeGun = true;
bool canShoot = true;

// Min x -44.812
// Max x 56.8331

// Min z 66.496
// Max z 41.601

// Draw bottom screen
void Draw3DScene2(void)
{
	drawBottomScreenUI();
}

// For debug
void error_handler(const char *text)
{
	// Simple handler. You could write this to a file instead, for example.
	printf(text);
}

/*
int sine; // sine position
int lfo;  // LFO position

FILE *file;
mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format)
{
	//if (file)
	//{
	size_t samplesize;
	switch (format)
	{
	case MM_STREAM_8BIT_MONO:
		samplesize = 1;
		break;
	case MM_STREAM_8BIT_STEREO:
		samplesize = 2;
		break;
	case MM_STREAM_16BIT_MONO:
		samplesize = 2;
		break;
	case MM_STREAM_16BIT_STEREO:
		samplesize = 4;
		break;
	}

	int res = fread(dest, samplesize, length, file);

	if (res)
	{
		length = res;
	}
	else
	{
		mmStreamClose();
		fclose(file);
		length = 0;
	}
	//}
	return length;
}*/

// Init the game (launch the engine, load textures, sounds, 3D models, init keyboard...)
int main(void)
{
	irqEnable(IRQ_HBLANK);
	irqSet(IRQ_VBLANK, NE_VBLFunc);
	irqSet(IRQ_HBLANK, NE_HBLFunc);

	// Init random
	srand(time(NULL));

	// Init
	NE_InitDual3D();

	// Anti aliasing
	NE_AntialiasEnable(true);

	// Error debug
	NE_DebugSetHandler(error_handler);

	NE_SetFov(70);											  // Set FOV
	NE_ClippingPlanesSetI(floattof32(0.1), floattof32(90.0)); // Set render distance
	// NE_ClippingPlanesSetI(floattof32(0.5), floattof32(90.0)); // Set render distance
	//  Init debug console
	if (isDebugBottomScreen)
		consoleDemoInit();

	// Init fat file system
	if (!fatInitDefault())
	{
		// iprintf("Warning: fatInit failure!\n");
	}

	setDialogText("");

	inputs[FIRE_BUTTON].value = KEY_L;
	inputs[FIRE_BUTTON].nameIndex = 9;
	inputs[JUMP_BUTTON].value = KEY_R;
	inputs[JUMP_BUTTON].nameIndex = 8;

	inputs[LEFT_BUTTON].value = KEY_LEFT;
	inputs[LEFT_BUTTON].nameIndex = 5;
	inputs[RIGHT_BUTTON].value = KEY_RIGHT;
	inputs[RIGHT_BUTTON].nameIndex = 4;
	inputs[UP_BUTTON].value = KEY_UP;
	inputs[UP_BUTTON].nameIndex = 6;
	inputs[DOWN_BUTTON].value = KEY_DOWN;
	inputs[DOWN_BUTTON].nameIndex = 7;
	inputs[LOOK_LEFT_BUTTON].value = KEY_Y;
	inputs[LOOK_LEFT_BUTTON].nameIndex = 11;
	inputs[LOOK_RIGHT_BUTTON].value = KEY_A;
	inputs[LOOK_RIGHT_BUTTON].nameIndex = 0;
	inputs[LOOK_UP_BUTTON].value = KEY_X;
	inputs[LOOK_UP_BUTTON].nameIndex = 10;
	inputs[LOOK_DOWN_BUTTON].value = KEY_B;
	inputs[LOOK_DOWN_BUTTON].nameIndex = 1;

	inputs[DEFUSE_BUTTON].value = KEY_SELECT;
	inputs[DEFUSE_BUTTON].nameIndex = 2;
	inputs[SCOPE_BUTTON].value = KEY_START;
	inputs[SCOPE_BUTTON].nameIndex = 3;

	inputsNames[0] = "A";
	inputsNames[1] = "B";
	inputsNames[2] = "Select";
	inputsNames[3] = "Start";
	inputsNames[4] = "Right";
	inputsNames[5] = "Left";
	inputsNames[6] = "Up";
	inputsNames[7] = "Down";
	inputsNames[8] = "R";
	inputsNames[9] = "L";
	inputsNames[10] = "X";
	inputsNames[11] = "Y";
	inputsNames[12] = "Unassigned";
	inputsNames[13] = "Scanning...";
	inputsNames[14] = "Already used";

	Load();

	isRumbleInserted();

	// useRumble = isRumbleInserted();

	// iprintf("Rumble %d\n", isRumbleInserted());
	/*if (!nitroFSInit(NULL))
	{
		iprintf("nitroFSInit failure!\n");
	}*/

	initSoundSystem();
	initKeyboard();

	// Remove this later
	SetSpritesForUI();
	SetCheckBoxsRefForUI();

	setBotsNames();

	// Load guns data
	AddGuns();
	LoadGrenades(AllGrenades);
	LoadEquipments(AllEquipements);

	LoadMapTextures();
	SetMapCameraPosition();
	AddAllSpawnPoints();

	SetMapNames();
	SetMapPartyMode();

	// Camera
	Camera = NE_CameraCreate();
	ForceUpdateLookRotation(CameraAngleY);

	// Set start camera position to look the map

	setCameraMapPosition();

	// create Materials
	GroundMaterial = NE_MaterialCreate();
	GroundMaterialShadowed = NE_MaterialCreate();
	PlayerMaterial = NE_MaterialCreate();
	PlayerMaterialTerrorist = NE_MaterialCreate();
	PlayerShadowMaterial = NE_MaterialCreate();
	TopScreenSpritesMaterials[0] = NE_MaterialCreate();
	TopScreenSpritesMaterials[2] = NE_MaterialCreate();
	TopScreenSpritesMaterials[3] = NE_MaterialCreate();
	TopScreenSpritesMaterials[4] = NE_MaterialCreate();
	TopScreenSpritesMaterials[5] = NE_MaterialCreate();

	// TopScreenSpritesMaterials[1] = NE_MaterialCreate();
	BottomScreenSpritesMaterials[0] = NE_MaterialCreate();
	// BottomScreenSpritesMaterials[1] = NE_MaterialCreate();
	BottomScreenSpritesMaterials[2] = NE_MaterialCreate();
	BottomScreenSpritesMaterials[3] = NE_MaterialCreate();
	BottomScreenSpritesMaterials[4] = NE_MaterialCreate();
	BottomScreenSpritesMaterials[5] = NE_MaterialCreate();
	// BottomScreenSpritesMaterials[6] = NE_MaterialCreate();

	// Create palettes
	Palettes[0] = NE_PaletteCreate();
	Palettes[2] = NE_PaletteCreate();
	Palettes[1] = NE_PaletteCreate();
	Palettes[3] = NE_PaletteCreate();
	Palettes[5] = NE_PaletteCreate();
	Palettes[6] = NE_PaletteCreate();
	Palettes[7] = NE_PaletteCreate();
	Palettes[8] = NE_PaletteCreate();
	Palettes[9] = NE_PaletteCreate();
	// Palettes[10] = NE_PaletteCreate();
	Palettes[11] = NE_PaletteCreate();
	Palettes[12] = NE_PaletteCreate();
	Palettes[13] = NE_PaletteCreate();
	Palettes[14] = NE_PaletteCreate();
	Palettes[15] = NE_PaletteCreate();
	Palettes[16] = NE_PaletteCreate();
	// Load .bin textures

	/*roadMaterials = NE_MaterialCreate();
	wallWindowMaterials = NE_MaterialCreate();
	wallMaterials = NE_MaterialCreate();

	roadPalette = NE_PaletteCreate();
	wallWindowPalette = NE_PaletteCreate();
	wallPalette = NE_PaletteCreate();

	NE_MaterialTexLoadBMPtoRGB256(roadMaterials, roadPalette, (void *)road_bin, false);					  // Load bmp font format
	NE_MaterialTexLoadBMPtoRGB256(wallWindowMaterials, wallWindowPalette, (void *)wallWindow_bin, false); // Load bmp font format
	NE_MaterialTexLoadBMPtoRGB256(wallMaterials, wallPalette, (void *)wall_bin, false);					  // Load bmp font format*/

	// Load font
	TextMaterial = NE_MaterialCreate();
	NE_MaterialTexLoadBMPtoRGB256(TextMaterial, Palettes[2], (void *)text_bmp_bin, true); // Load bmp font format

	// Create font
	NE_TextInit(0,			  // Font slot
				TextMaterial, // Image
				8, 8);		  // Size of one character (x, y)

	// NE_MaterialTexLoadBMPtoRGB256(PlayerMaterial, Palettes[3], (void *)skin_man_bin, 0);
	// NE_MaterialTexLoadBMPtoRGB256(PlayerMaterialTerrorist, Palettes[13], (void *)skin_soldier_bin, 0);

	NE_MaterialTexLoadBMPtoRGB256(PlayerMaterial, Palettes[3], (void *)gign_skin1_bin, 0);
	NE_MaterialTexLoadBMPtoRGB256(PlayerMaterialTerrorist, Palettes[13], (void *)terrorist_skin1_bin, 0);

	NE_MaterialTexLoadBMPtoRGB256(GroundMaterial, Palettes[1], (void *)Atlas_bin, 1);
	NE_MaterialTexClone(GroundMaterial, GroundMaterialShadowed);

	//  NE_MaterialTexLoadBMPtoRGB256(GroundMaterial, Palettes[1], (void *)player_shadow_bin, 1);
	//   NE_MaterialTexLoadBMPtoRGB256(GroundMaterial, Palettes[1], (void *)road_bin, 1);

	// Palettes[4] = NE_PaletteCreate();
	// NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], AllGuns[CurrentLittleGun].texture, 1);
	// UpdateGunTexture();

	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[0], Palettes[9], (void *)QuitButton_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[2], Palettes[5], (void *)MapPointUI_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[5], Palettes[16], (void *)bomb_logo_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[4], Palettes[7], (void *)CheckMark_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[5], Palettes[6], (void *)WhiteScareRounded_bin, 1);
	// NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[6], Palettes[10], AllGuns[0].texture, 1);

	// Load .bmp textures top screen
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[0], Palettes[8], (void *)crosshair2_bin, 1);

	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[2], Palettes[11], (void *)muzzle_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[3], Palettes[12], (void *)scopeImage_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[4], Palettes[14], (void *)MapUI_bin, 0);
	NE_MaterialTexLoadBMPtoRGB256(PlayerShadowMaterial, Palettes[15], (void *)player_shadow_bin, 1);

	// Create crosshair sprite
	TopScreenSprites[0] = NE_SpriteCreate();
	NE_SpriteSetPos(TopScreenSprites[0], ScreenCenterWidth - 20 / 2, ScreenCenterHeight - 20 / 2);
	NE_SpriteSetSize(TopScreenSprites[0], 20, 20);
	NE_SpriteSetPriority(TopScreenSprites[0], 10);
	NE_SpriteSetMaterial(TopScreenSprites[0], TopScreenSpritesMaterials[0]);

	// Create map sprite
	TopScreenSprites[4] = NE_SpriteCreate();
	NE_SpriteSetSize(TopScreenSprites[4], 170, 177);
	NE_SpriteSetPriority(TopScreenSprites[4], 2);
	NE_SpriteSetMaterial(TopScreenSprites[4], TopScreenSpritesMaterials[4]);

	//  Create map point
	BottomScreenSprites[1] = NE_SpriteCreate();
	NE_SpriteSetSize(BottomScreenSprites[1], 6, 6);
	NE_SpriteSetPriority(BottomScreenSprites[1], 1);
	NE_SpriteSetMaterial(BottomScreenSprites[1], BottomScreenSpritesMaterials[2]);

	NE_SpriteVisible(BottomScreenSprites[1], false);

	// Create quit button
	BottomScreenSprites[2] = NE_SpriteCreate();
	NE_SpriteSetSize(BottomScreenSprites[2], 20, 20);
	NE_SpriteSetPriority(BottomScreenSprites[2], 0);
	NE_SpriteSetMaterial(BottomScreenSprites[2], BottomScreenSpritesMaterials[0]);
	NE_SpriteSetPos(BottomScreenSprites[2], 236, 2);
	NE_SpriteVisible(BottomScreenSprites[2], false);

	BottomScreenSpriteCount = 3;

	// TopScreenSprites[4] = NE_MaterialCreate();

	// gui
	// Material->diffuse = RGB15(31, 0, 0);
	// Material->ambient = RGB15(31, 0, 0);

	// Create models for the map
	// for (int i = 0; i < MapPartCount + 4; i++)
	for (int i = 7; i < 7 + 4; i++)
	{
		Model[i] = NE_ModelCreate(NE_Static);
		NE_ModelSetMaterial(Model[i], GroundMaterial);
		NE_ModelScaleI(Model[i], 4096, 4096, 4096);
		NE_ModelSetCoord(Model[i], 0, 1.5 + 0.8, 0);
		Model[i]->rx = 128;
		Model[i]->ry = 256;
	}

	// Load bomb model
	NE_ModelLoadStaticMesh(Model[7], (u32 *)bomb_bin);

	// Load Plane model
	NE_ModelSetMaterial(Model[8], TopScreenSpritesMaterials[2]);
	NE_ModelLoadStaticMesh(Model[8], (u32 *)plane_bin);
	NE_ModelSetCoord(Model[8], 0, 0, 1);

	createPlayerShadow();

	// NE_ModelSetCoord(Model[9], 10.86, 0.08, 7.783);

	NE_ModelLoadStaticMesh(Model[10], (u32 *)explosion_bin);
	Model[10]->rx = 0;
	Model[10]->ry = 256;
	NE_ModelScaleI(Model[10], 0, 0, 0);

	LoadMap(currentMap);

	AddAllPartyModes();

	// Set scope levels
	AllScopeLevels[0].scopeCount = 2;
	AllScopeLevels[0].fov[0] = 20;
	AllScopeLevels[0].fov[1] = 7;
	AllScopeLevels[0].Speed = 140;

	AllScopeLevels[1].scopeCount = 1;
	AllScopeLevels[1].fov[0] = 20;
	AllScopeLevels[1].Speed = 140;

	// sprintf(killText, "PlayerName01 killed PlayerName02");

	// Set all player ID to UNUSED
	for (int i = 0; i < MaxPlayer; i++)
	{
		AllPlayers[i].Id = UNUSED;
		AllPlayers[i].Team = SPECTATOR;
	}

	// Lights
	NE_LightSet(0, RGB15(31, 31, 31), -0.5, -0.5, -0.5);
	NE_LightSet(1, RGB15(15, 15, 15), -0.5, -0.5, -0.5);

	// Set background color
	NE_ClearColorSet(RGB15(17, 26, 29), 31, 63); // Blue sky

	// Open default menu
	initMainMenu();

	// DEBUG show connection mode
	printf("A : offline mode\n");  // 0
	printf("B : 88.127.205.17\n"); // 1
	printf("Y : 88.127.205.50\n"); // 2
	printf("X : Local mode\n");	   // 3

	/*if (!Wifi_InitDefault(WFC_CONNECT))
	{
		printf("Failed to connect! Restart the game.");
	}
	else //Then connect to server
	{
		PartyStarted = true;
		printf("Connected\n\n");
		my_socket = socket(AF_INET, SOCK_STREAM, 0);
		//char *ts = "fewnity.000webhostapp.com";
		connectToServer("fewnity.000webhostapp.com", false, my_socket, false); //Server list
	}*/

	if (!tutorialDone)
	{
		isInTutorial = true;
		currentSelectionMap = TUTORIAL;
		StartSinglePlayer();
	}

	while (true)
	{
		StartGame();
	}

	return 0;
}

void setCameraMapPosition()
{
	Vector3 *pos = &allMaps[currentMap].cameraPosition;
	Vector3 *lookPos = &allMaps[currentMap].cameraLookPosition;
	NE_CameraSet(Camera,
				 pos->x, pos->y, pos->z,
				 lookPos->x, lookPos->y, lookPos->z,
				 0, 1, 0);
}

void StartSinglePlayer()
{
	Connection = OFFLINE;
}

bool alreadyLanched = false;

void StartGame()
{
	// Play without internet
	if (Connection == OFFLINE)
	{
		UnLoadMap(currentMap);
		currentMap = currentSelectionMap;
		LoadMap(currentMap);

		currentPartyMode = allMaps[currentMap].forcePartyMode;

		if (currentPartyMode == -1)
			currentPartyMode = 1;
		// currentPartyMode = 2;
		// if(currentPartyMode == -1)
		// init
		// while (currentPartyMode == -1)
		// {
		// 	readKeys();

		// 	ReadTouchScreen(touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenOneFrame, &SendTeam, true);
		// 	UpdateEngineNotInGame();
		// }

		partyFinished = false;
		initScoreMenu();

		applyRules = true;

		ResetTakenBotsNames();
		roundState = TRAINING;

		AddNewPlayer(0, true, false);
		printf("START PARTY\n\n");

		// Add bots
		if (currentPartyMode != 2)
		{
			for (int i = 1; i < MaxPlayer; i++)
				AddNewPlayer(i, false, true);
		}

		iprintf("\n");

		localPlayer->CurrentOcclusionZone = 15;
		TerroristsScore = 0;
		CounterScore = 0;

		if (isInTutorial)
		{
			AllPlayers[0].Team = TERRORISTS;
		}

		while (AllPlayers[0].Team == SPECTATOR)
		{
			readKeys();

			ReadTouchScreen(touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenOneFrame, &SendTeam, true);
			UpdateEngineNotInGame();

			if (Connection == UNSELECTED)
				return;
		}

		SetNeedChangeScreen(true);
		initControllerMenu();

		// Set randomly players in a team
		// int rnd = random() % 2;

		int rnd = AllPlayers[0].Team;
		for (int i = 0; i < PlayerCount; i++) // TODO replace
		{
			if (rnd != i % 2)
			{
				AllPlayers[i].Team = COUNTERTERRORISTS;
			}
			else
			{
				AllPlayers[i].Team = TERRORISTS;
			}
			/*if (i == 0)
			{
				AllPlayers[i].Team = COUNTERTERRORISTS;
			}
			else
			{
				AllPlayers[i].Team = TERRORISTS;
			}*/
			UpdatePlayerTexture(i);
		}
		AllPlayers[2].Team = AllPlayers[1].Team;
		UpdatePlayerTexture(2);

		setPlayersPositionAtSpawns();

		checkShopForBots();

		bombDropped = false;
		bombSet = false;
		BombDefused = false;
		BombPlanted = false;
		PartySeconds = allPartyModes[currentPartyMode].trainingSecondsDuration;
		PartyMinutes = allPartyModes[currentPartyMode].trainingMinutesDuration;
		changeSecondTimer = 1;
		changeMinuteTimer = 60;
		bombBipTimer = 0;
		BombWillExplose = false;
		currentDefuserIndex = NO_PLAYER;
		PartyStarted = false;

		textToShowTimer = 0;

		// alreadyLanched = true;
		while (Connection == OFFLINE)
		{
			// Play game code
			GameLoop();
			UpdateEngine();
		}
	}
	else if (Connection != UNSELECTED)
	{
		SetNeedChangeScreen(true);
		partyFinished = false;
		initScoreMenu();
		AllButtons[0].isHidden = true;
		textToShowTimer = 0;

		printf("Connecting...\n");
		// Try to connect to wifi
		if (!Wifi_InitDefault(WFC_CONNECT))
		{
			printf("Failed to connect! Restart the game.");
		}
		else // Then connect to server
		{
			PartyStarted = true;
			applyRules = false;

			printf("Connected\n\n");
			my_socket = socket(AF_INET, SOCK_STREAM, 0);
			/*if (Connection == IP1)
				connectToServer("88.127.205.17", false, my_socket, true); // Mère
			else if (Connection == IP2)
				connectToServer("88.127.205.50", false, my_socket, true); // Père
			else if (Connection == LOCAL)
				connectToServer("", true, my_socket, true); // Local mode (Android phone)*/
		}
	}
	else
	{
		readKeys();

		/*if ((keysdown & KEY_A) == KEY_A)
			Connection = OFFLINE;
		else if ((keysdown & KEY_B) == KEY_B)
			Connection = IP1;
		else if ((keysdown & KEY_Y) == KEY_Y)
			Connection = IP2;
		else if ((keysdown & KEY_X) == KEY_X)
			Connection = LOCAL;*/

		ReadTouchScreen(touch, &NeedChangeScreen, false, &ButtonToShow, &UpdateBottomScreenOneFrame, &SendTeam, true);
		UpdateEngineNotInGame();
	}
}

// Getters / setters

int GetCurrentMenu()
{
	return currentMenu;
}

void SetCurrentMenu(int value)
{
	currentMenu = value;
}

int GetUpdateBottomScreenOneFrame()
{
	return UpdateBottomScreenOneFrame;
}

void SetUpdateBottomScreenOneFrame(int value)
{
	UpdateBottomScreenOneFrame = value;
}

int GetButtonToShow()
{
	return ButtonToShow;
}

void SetButtonToShow(int value)
{
	ButtonToShow = value;
}

void SetWaitForTeamResponse(bool value)
{
	WaitForTeamResponse = value;
}

Grenade *GetAllGrenades()
{
	return AllGrenades;
}

CheckBox *GetCheckBoxs()
{
	return AllCheckBoxs;
}

NE_Sprite **GetSpritesTop()
{
	return TopScreenSprites;
}

NE_Sprite **GetSpritesBottom()
{
	return BottomScreenSprites;
}

NE_Material **GetBottomScreenSpritesMaterials()
{
	return BottomScreenSpritesMaterials;
}

NE_Palette **GetPalettes()
{
	return Palettes;
}

bool GetAlwaysUpdateBottomScreen()
{
	return AlwaysUpdateBottomScreen;
}

void SetAlwaysUpdateBottomScreen(bool value)
{
	AlwaysUpdateBottomScreen = value;
}

bool GetNeedChangeScreen()
{
	return NeedChangeScreen;
}

bool GetIsDebugBottomScreen()
{
	return isDebugBottomScreen;
}

void SetIsDebugBottomScreen(bool value)
{
	isDebugBottomScreen = value;
}

bool GetIsDebugTopScreen()
{
	return isDebugTopScreen;
}

void SetIsDebugTopScreen(bool value)
{
	isDebugTopScreen = value;
}

void SetNeedChangeScreen(bool value)
{
	NeedChangeScreen = value;
}

void SetTwoScreenMode(bool value)
{
	if (AlwaysUpdateBottomScreen && !value && UpdateBottomScreenOneFrame == 0)
	{
		NeedChangeScreen = true;
	}
	else if (!AlwaysUpdateBottomScreen && value)
	{
		NeedChangeScreen = true;
	}
}

void SetSendTeam(bool Value)
{
	SendTeam = Value;
}

void SetTempTeam(int Value)
{
	tempTeam = Value;
}

int GetTempTeam()
{
	return tempTeam;
}

void SetPing(int Value)
{
	ping = Value;
}
int GetPing()
{
	return ping;
}

void SetCounterScore(int Value)
{
	CounterScore = Value;
}
int GetCounterScore()
{
	return CounterScore;
}

void SetTerroristsScore(int Value)
{
	TerroristsScore = Value;
}
int GetTerroristsScore()
{
	return TerroristsScore;
}

void SetTextToShowTimer(int Value)
{
	textToShowTimer = Value;
}
int GetTextToShowTimer()
{
	return textToShowTimer;
}

void SetNeedJump()
{
	if (!localPlayer->IsDead && roundState != WAIT_START)
		NeedJump = true;
}

void SetSelectedGunShop(int Value)
{
	SelectedGunShop = Value;
}

int GetSelectedGunShop()
{
	return SelectedGunShop;
}

void SetCurrentCameraPlayer(int Value)
{
	CurrentCameraPlayer = Value;
}

int GetCurrentCameraPlayer()
{
	return CurrentCameraPlayer;
}

void SetGunInInventory(int Value, int inventoryIndex)
{
	localPlayer->AllGunsInInventory[inventoryIndex] = Value;
	if (getPlayerCurrentGunIndex(localPlayer) == UNUSED)
		ChangeGunInInventoryForLocalPlayer(0);
	else
	{
		ResetGunAmmo(0, inventoryIndex);
		UpdateGunTexture();
	}
}

void SetGunInInventoryForNonLocalPlayer(int playerIndex, int Value, int inventoryIndex)
{
	AllPlayers[playerIndex].AllGunsInInventory[inventoryIndex] = Value;
	if (playerIndex == 0 && getPlayerCurrentGunIndex(localPlayer) == UNUSED)
		ChangeGunInInventoryForLocalPlayer(0);
}

void SetSendBuyWeapon(bool Value)
{
	SendBuyWeapon = Value;
}

void SetNeedUpdateViewRotation(bool Value)
{
	NeedUpdateViewRotation = Value;
}

bool GetNeedUpdateViewRotation()
{
	return NeedUpdateViewRotation;
}

void SetSendPosition(bool Value)
{
	SendPosition = Value;
}

bool GetSendPosition()
{
	return SendPosition;
}

void SetCameraAngleY(float Value)
{
	CameraAngleY = Value;
}

float GetCameraAngleY()
{
	return CameraAngleY;
}

int GetCurrentScopeLevel()
{
	return CurrentScopeLevel;
}

void SetDoubleTapTimer(int Value)
{
	doubleTapTimer = Value;
}

int GetDoubleTapTimer()
{
	return doubleTapTimer;
}

void ChangeGunInInventoryForLocalPlayer(int Left)
{
	DisableAim();
	ChangeGunInInventory(0, Left);
}

void ChangeGunInInventory(int playerIndex, int Left)
{
	Player *player = &AllPlayers[playerIndex];

	int oldCurrentGun = player->currentGunInInventory;
	do
	{
		if (Left == 1)
		{
			player->currentGunInInventory--;
			if (player->currentGunInInventory == -1)
				player->currentGunInInventory = inventoryCapacity - 1;
		}
		else
		{
			player->currentGunInInventory++;
			if (player->currentGunInInventory == inventoryCapacity)
				player->currentGunInInventory = 0;
		}
	} while (getPlayerCurrentGunIndex(player) == EMPTY);

	if (playerIndex == 0 && !canChangeGun && player->AllGunsInInventory[oldCurrentGun] != -1)
	{
		player->currentGunInInventory = oldCurrentGun;
		return;
	}

	if (oldCurrentGun != player->currentGunInInventory)
	{
		StopReloading(playerIndex);
		if (playerIndex == 0)
		{
			SendSelectedGun = true;
			UpdateGunTexture();
		}
	}
}

void setSelectedGunInInventory(int playerIndex, int gunIndex)
{
	Player *player = &AllPlayers[playerIndex];
	if (player->AllGunsInInventory[gunIndex] != EMPTY)
	{
		player->currentGunInInventory = gunIndex;
		StopReloading(playerIndex);
		if (playerIndex == 0)
		{
			SendSelectedGun = true;
			UpdateGunTexture();
		}
	}
}

void reduceRumbleTimer()
{
	// If rumble timer is on, reduce it
	if (useRumble && RumbleTimer != 0)
	{
		RumbleTimer--;
		// If timer = 0, disable rumble
		if (RumbleTimer == 0)
			setRumble(false);
	}
}

void reduceDoubleTapTimer()
{
	// Reduce double tab timer
	if (doubleTapTimer > 0)
		doubleTapTimer--;
}

void reduceScreenShake()
{
	// Screen shake
	if (xOffset >= 0.005)
		xOffset -= 0.005;
	else if (xOffset < -0.005)
		xOffset += 0.005;

	for (int i = 0; i < speedAimingReCenter; i++)
	{
		if (yOffset >= 0.0025)
			yOffset -= 0.0025;
		else if (yOffset < -0.0025)
			yOffset += 0.0025;
	}

	if (speedAimingReCenterTimer > 0)
	{
		speedAimingReCenterTimer--;
		if (speedAimingReCenterTimer == 0)
		{
			speedAimingReCenterTimer = 8;
			if (speedAimingReCenter < 5)
				speedAimingReCenter++;
		}
	}
}

void GameLoop()
{
	// Read keys
	readKeys();

	reduceRumbleTimer();

	// Get local player position
	CalculatePlayerPosition(0);

	// Check if player can defuse or plant the bomb
	bool CanPutBomb = false, canDefuseBomb = false;

	CheckZones(bombDefuseZone, &CanPutBomb, &canDefuseBomb);
	if (bombDropped)
	{
		canDefuseBomb = false;
		CanPutBomb = false;
	}

	checkTakingBombZone(bombDefuseZone);

	if (allPartyModes[currentPartyMode].limitedShopByZoneAndTimer)
		checkShopZone();
	else
		isInShopZone = true;

	UpdateGrenades();

	reduceDoubleTapTimer();

	// Check touchscreen input for menus
	ReadTouchScreen(touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenOneFrame, &SendTeam, false);

	if (isInTutorial)
		checkTutorial();

	// If local player is not a spectator
	if (localPlayer->Team != SPECTATOR)
	{
		// Check input and rotate player
		RotatePlayer(&NeedUpdateViewRotation, &SendPosition, &CameraAngleY, CurrentScopeLevel);

		// Update look rotation value
		if (NeedUpdateViewRotation || CurrentCameraPlayer != 0)
		{
			UpdateLookRotation(CameraAngleY);
			NeedUpdateViewRotation = false;
		}

		isInDownStairs = false;
		// Check if the player is on a stairs
		CheckStairs(&CanJump, &isInDownStairs);

		// Reduce jump timer
		if (CanJump > 0)
			CanJump--;

		// Check for jump
		if (roundState != WAIT_START && !localPlayer->IsDead)
		{
			// CanJumpRealTimer is used wait a little bit of time before the player can jump
			// Set CanJumpReal timer
			if (localPlayer->PlayerPhysic->yspeed == 0 && CanJumpRealTimer > 0)
				CanJumpRealTimer--;
			else if (localPlayer->PlayerPhysic->yspeed != 0)
				CanJumpRealTimer = 2;

			// If player is in the air, increase the frameCountDuringAir
			if (CanJumpRealTimer != 0 && !isInDownStairs)
				frameCountDuringAir++;
			else if ((CanJumpRealTimer == 0 || isInDownStairs) && frameCountDuringAir > 20) // Make jump land sound if the player was more than 0,33 secs in the air
			{
				frameCountDuringAir = 0;
				Play2DSound(SFX_LAND, 140);
				NeedJump = false;
			}
			else
				frameCountDuringAir = 0;

			// If the player can jump and if jump input is down
			if ((isKeyDown(JUMP_BUTTON) || NeedJump) && (CanJumpRealTimer == 0 || CanJump > 0))
			{
				// Apply force on the player
				NeedJump = false;
				localPlayer->PlayerPhysic->yspeed = JumpForce;
				CanJump = 0;
			}
		}

		// Set aiming view
		if (isKeyDown(SCOPE_BUTTON))
		{
			SetAim();
		}

		Player *playerWithView = &AllPlayers[CurrentCameraPlayer];
		float cameraFinalY = playerWithView->position.y + CameraOffsetY;

		// Set camera position
		NE_CameraSet(Camera,
					 playerWithView->position.x, cameraFinalY - deathCameraYOffset, playerWithView->position.z,
					 playerWithView->position.x + x + xOffset, cameraFinalY + y + yOffset + deathCameraYOffset, playerWithView->position.z + z + xOffset,
					 0, 1, 0);
	}

	reduceScreenShake();

	for (int i = 0; i < MaxPlayer; i++)
	{
		Player *player = &AllPlayers[i];
		if (player->Id == UNUSED)
			continue;

		if (player->inShadow)
		{
			if (player->lightCoef > 0.7)
				player->lightCoef -= 0.05;
		}
		else
		{
			if (player->lightCoef < 1)
				player->lightCoef += 0.05;
		}

		if (player->NeedRespawn)
		{
			player->RespawnTimer--;
			if (player->RespawnTimer == 0)
			{
				player->NeedRespawn = false;
				setPlayerPositionAtSpawns(i);
				resetPlayer(i);
				CheckShopForBot(i);
			}
		}

		if (player->invisibilityTimer > 0)
		{
			player->invisibilityTimer--;
		}

		// Change gun sprite position to normal position if needed
		if (player->rightGunXRecoil > GunMinRecoil)
			player->rightGunXRecoil--;
		if (player->rightGunYRecoil > GunMinRecoil)
			player->rightGunYRecoil--;

		if (player->leftGunXRecoil > GunMinRecoil)
			player->leftGunXRecoil--;
		if (player->leftGunYRecoil > GunMinRecoil)
			player->leftGunYRecoil--;

		if (player->Id == UNUSED)
			continue;

		// Gun timer
		if (getPlayerCurrentGunIndex(player) < GunCount && player->GunWaitCount < getPlayerCurrentGun(player).fireRate)
			player->GunWaitCount++;

		// Gun reload timer
		if (player->isReloading)
		{
			if (getPlayerCurrentGunIndex(player) < GunCount && player->GunReloadWaitCount < getPlayerCurrentGun(player).ReloadTime)
				player->GunReloadWaitCount++;

			// If reload timer is ended, reload gun and stop reloading
			if (player->GunReloadWaitCount == getPlayerCurrentGun(player).ReloadTime)
			{
				player->isReloading = false;
				ReloadGun(i);
			}
		}
	}

	if (localPlayer->Team != SPECTATOR && !localPlayer->IsDead && roundState != WAIT_START)
	{
		if (shopDisableTimer > 0 && roundState != TRAINING && allPartyModes[currentPartyMode].limitedShopByZoneAndTimer)
		{
			shopDisableTimer--;
			if (shopDisableTimer == 0 && (currentMenu == SHOP || currentMenu == SHOPCATEGORIES))
			{
				initGameMenu();
			}
		}

		// Avoid game soft lock if the player pass through the map
		if (localPlayer->position.y < -5)
		{
			setPlayerPositionAtSpawns(0);
		}

		// Gun shoot
		if (canShoot && !localPlayer->isReloading && ((getPlayerCurrentGunIndex(localPlayer) < GunCount && ((isKey(FIRE_BUTTON) && getPlayerCurrentGun(localPlayer).holdFireButton) || (isKeyDown(FIRE_BUTTON) && !getPlayerCurrentGun(localPlayer).holdFireButton)) && localPlayer->GunWaitCount >= getPlayerCurrentGun(localPlayer).fireRate)) && !isUsingBomb)
		{
			if (((localPlayer->currentGunInInventory == 1 || localPlayer->currentGunInInventory == 2) && localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount > 0) || getPlayerCurrentGun(localPlayer).isKnife)
			// if (true)
			{
				if (!getPlayerCurrentGun(localPlayer).isKnife)
				{
					localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount--;
					// Gun sound
					PlayBasicSound(getPlayerCurrentGun(localPlayer).gunSound);
				}

				// Reset gun timer
				localPlayer->GunWaitCount = 0;

				if (yOffset < 0.1)
					yOffset += (rand() % 2 + 2) / 100.0;
				else if (yOffset < 0.16)
					yOffset += (rand() % 2 + 2) / 150.0;
				else
					yOffset += (rand() % 2 + 2) / 200.0;
				// Set gun sprite position

				speedAimingReCenter = 1;
				speedAimingReCenterTimer = 8;

				if (getPlayerCurrentGun(localPlayer).isDualGun)
					AllPlayers[0].isRightGun = !AllPlayers[0].isRightGun;
				else
					AllPlayers[0].isRightGun = true;

				setGunRecoil(localPlayer);

				ShowMuzzle = 3;

				setRaycastValues(localPlayer, x, y + yOffset, z);

				// Raycast
				int hittedClient = Raycast(0);
				if (hittedClient != NO_PLAYER)
				{
					makeHit(0, hittedClient);
				}

				rumble(1);

				// Send shoot on network
				SendShoot = true;
			}
			else
			{
				startReloadGun(0);
			}
		}
		else if (getPlayerCurrentGunIndex(localPlayer) >= GunCount + shopGrenadeCount && isKey(FIRE_BUTTON) && !isLocalPlayerMoving())
		{
			if (getPlayerCurrentGunIndex(localPlayer) == GunCount + shopGrenadeCount && CanPutBomb && roundState == PLAYING && !BombPlanted && (CanJumpRealTimer == 0 || CanJump > 0))
			{
				isUsingBomb = true;
				// On bomb planting make a sound
				if (localPlayer->bombTimer == bombPlantingTime)
				{
					PlayBasicSound(SFX_BOMBPLANTING);
					SendBombPlacing = true;
				}
				localPlayer->bombTimer--;
				// Set bomb position when planted
				if (localPlayer->bombTimer == 0)
				{
					BombPosition.x = localPlayer->position.x;
					BombPosition.y = localPlayer->position.y - 0.845;
					BombPosition.z = localPlayer->position.z;
					BombPosition.r = localPlayer->Angle;

					bombPlantedAt = checkBombZoneWaypoint();
					BombPlanted = true;
					if (applyRules)
					{
						bombSet = true;
						if (roundState == PLAYING)
						{
							PartyMinutes = allPartyModes[currentPartyMode].bombWaitingMinutesDuration;
							PartySeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						}
						BombSeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						localPlayer->haveBomb = false;
						SetGunInInventory(-1, 8);
						NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
						NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
						Model[7]->rz = BombPosition.r;
						bombBipTimer = 120;
						SetBombDefuseZone(BombPosition.x, BombPosition.z, &bombDefuseZone);
						showPartyEventText(2);
					}
					else
					{
						SendBombPlace = true;
					}
					SetRandomDefuser();
				}
			}
		}
		else if (getPlayerCurrentGunIndex(localPlayer) < GunCount + shopGrenadeCount && getPlayerCurrentGunIndex(localPlayer) >= GunCount && isKeyDown(FIRE_BUTTON) && !isUsingBomb) // Launch grenade
		{
			PhysicalGrenade *newGrenade = CreateGrenade(getPlayerCurrentGunIndex(localPlayer), localPlayer->Id);
			if (newGrenade != NULL)
			{
				lanchGrenade(newGrenade, x, y, z, localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z);

				localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] = EMPTY;
				ChangeGunInInventoryForLocalPlayer(1);

				SendGrenade = true;
			}
		}
		else if (isKey(DEFUSE_BUTTON) && canDefuseBomb && (roundState == PLAYING || roundState == END_ROUND) && !BombDefused && BombPlanted && localPlayer->Team == COUNTERTERRORISTS && !isLocalPlayerMoving()) // Defuse bomb
		{
			isUsingBomb = true;
			// On bomb defuse make a sound
			if (localPlayer->bombTimer == bombDefuseTime)
			{
				PlayBasicSound(SFX_BOMBPLANTING);
				SendBombPlacing = true;
			}
			localPlayer->bombTimer--;
			// Set bomb when defused
			if (localPlayer->bombTimer == 0)
			{
				BombDefused = true;

				if (applyRules)
				{
					CounterScore++;

					if (LoseCountTerrorists > 0)
						LoseCountTerrorists--;

					if (LoseCountCounterTerrorists < 4)
						LoseCountCounterTerrorists++;

					addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney, TERRORISTS);
					addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, COUNTERTERRORISTS);

					showPartyEventText(3);
					PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
					PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
					roundState = END_ROUND;
					CheckAfterRound();
				}
				else
				{
					SendBombDefused = true;
				}
			}
		}
		else
		{
			isUsingBomb = false;
			if (localPlayer->Team == COUNTERTERRORISTS)
			{
				if (!BombDefused && BombPlanted) // Set timer
					localPlayer->bombTimer = bombDefuseTime;
				else
				{
					localPlayer->bombTimer = 0;
				}
			}
			else if (localPlayer->Team == TERRORISTS)
			{
				if (!BombPlanted) // Set timer
					localPlayer->bombTimer = bombPlantingTime;
				else
				{
					localPlayer->bombTimer = 0;
				}
			}
		}

		// Reset player speed
		localPlayer->PlayerPhysic->xspeed = 0;
		localPlayer->PlayerPhysic->zspeed = 0;

		// Cut 3.2 220
		// Pistol heavy like Revolver 3,6 200
		// Pistol like Dual Berattas, glock,usp-s, five-seveN, tec-9, p250, 3,3 210
		// AK 3.7 195
		// AWP 4.0 180 Scope : 2 time speed 8
		// Negev 5.2 130
		// MP9 3.3 215
		// Deagle 3.6 200
		// SSG 08 3.5 205 Scope : 2 3.5
		// XM104 3.7 185
		// Aug 3.6 200 Scope : 1 speed 5
		// Nova 3.6 200
		// galil AR 3.7 185
		// M249 4.1 175
		// MP5SD 3.4 210
		// UMP45 3.4 210
		// SG 553 3.8 190 Scope : 1 speed 5
		// FAMAS 3.6 200
		// G3SG1 3.7 Scope : 2 speed 6,5
		// SCAR-20 3.7 Scope : 2 speed 6,5
		// P90 3.5
		// M4A4 3.6
		// PP bizon 3.4
		// Mag 7 3.6
		// Sawed off 3.8
		// Mac 10 3.3

		// 3.2 220
		// 3.4 220
		// 3.6 200
		// 3.8 190
		// 4.0 180
		// 4.2 160
		// 4.4 140
		// 4.6 120

		// 5.2 140
		// 5.2 130

		// Player movements
		bool NeedBobbing = false;
		int CurrentSpeed = defaultWalkSpeed;
		if (getPlayerCurrentGunIndex(localPlayer) < GunCount)
			CurrentSpeed = getPlayerCurrentGun(localPlayer).WalkSpeed;

		if (roundState != WAIT_START && !localPlayer->IsDead)
			MovePlayer(CurrentSpeed, xWithoutY, zWithoutY, &NeedBobbing);

		// Gun headbobing
		if (NeedBobbing && (CanJumpRealTimer == 0 || CanJump > 0))
		{
			BobbingOffset += BobbingSpeed;

			// Do sound every half bobbing cycle
			if (!HasBobbed && BobbingOffset >= M_PI)
			{
				DoStepSound(110, 128, 0);
				SendStepSound = true;
				HasBobbed = true;
			}
			else if (BobbingOffset >= M_TWOPI)
			{
				BobbingOffset = 0;
				DoStepSound(110, 128, 0);
				SendStepSound = true;
				HasBobbed = false;
			}
		}
	}
	else
	{
		if (localPlayer->IsDead)
		{
			if (keysdown & KEY_LEFT)
			{
				bool Found = false;
				int FirstFound = NO_PLAYER;
				for (int i = 1; i < MaxPlayer; i++)
				{
					Player *player = &AllPlayers[i];
					if (!player->IsDead && player->Id != UNUSED && (allPartyModes[currentPartyMode].canSeeOtherTeamView || player->Team == localPlayer->Team))
					{
						if (FirstFound == NO_PLAYER)
							FirstFound = i;

						if (i > CurrentCameraPlayer)
						{
							CurrentCameraPlayer = i;
							ShowMuzzle = 0;
							UpdateGunTexture();
							Found = true;
							break;
						}
					}
				}

				if (!Found && FirstFound != NO_PLAYER)
				{
					if (CurrentCameraPlayer != FirstFound)
					{
						CurrentCameraPlayer = FirstFound;
						UpdateGunTexture();
					}
				}
			}
			else if (keysdown & KEY_RIGHT)
			{
				bool Found = false;
				int FirstFound = NO_PLAYER;
				for (int i = MaxPlayer - 1; i > 0; i--)
				{
					Player *player = &AllPlayers[i];
					if (!player->IsDead && player->Id != UNUSED && (allPartyModes[currentPartyMode].canSeeOtherTeamView || player->Team == localPlayer->Team))
					{
						if (FirstFound == NO_PLAYER)
							FirstFound = i;

						if (i < CurrentCameraPlayer)
						{
							CurrentCameraPlayer = i;
							ShowMuzzle = 0;
							UpdateGunTexture();
							Found = true;
							break;
						}
					}
				}

				if (!Found && FirstFound != NO_PLAYER)
				{
					if (CurrentCameraPlayer != FirstFound)
					{
						ShowMuzzle = 0;
						CurrentCameraPlayer = FirstFound;
						UpdateGunTexture();
					}
				}
			}

			if (deathCameraAnimation < 1)
			{
				deathCameraAnimation += deathCameraAnimationSpeed;
				deathCameraYOffset = (1 - cosf(deathCameraAnimation * M_PI / 2.0)) * 1.4;
			}

			if (CurrentCameraPlayer != 0)
				deathCameraYOffset = 0;
		}

		// Reset player speed
		localPlayer->PlayerPhysic->xspeed = localPlayer->PlayerPhysic->yspeed = localPlayer->PlayerPhysic->zspeed = 0;
	}

	if (applyRules)
	{
		for (int i = 1; i < MaxPlayer; i++)
		{
			Player *player = &AllPlayers[i];
			if (player->Id == UNUSED)
				continue;

			if (player->PathCount != 0)
			{
				player->BobbingOffset += BobbingSpeed;

				// Do sound every half bobbing cycle
				if (!player->HasBobbed && player->BobbingOffset >= M_PI)
				{
					int Panning, Volume;
					GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.11);
					DoStepSound(Volume, Panning, i);
					player->HasBobbed = true;
				}
				else if (player->BobbingOffset >= M_TWOPI)
				{
					int Panning, Volume;
					GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.11);
					player->BobbingOffset = 0;
					DoStepSound(Volume, Panning, i);
					player->HasBobbed = false;
				}
			}

			if (player->isPlantingBomb)
			{
				if (player->Team == TERRORISTS)
				{
					// On bomb planting make a sound
					if (player->bombTimer == bombPlantingTime)
					{
						int Panning, Volume;
						GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);
						Play3DSound(SFX_BOMBPLANTING, Volume, Panning);
					}
					player->bombTimer--;
					// Set bomb position when planted
					if (player->bombTimer == 0)
					{
						CalculatePlayerPosition(i);
						BombPosition.x = player->position.x;
						BombPosition.y = player->position.y - 0.845;
						BombPosition.z = player->position.z;
						BombPosition.r = player->Angle;

						BombPlanted = true;
						bombSet = true;
						player->isPlantingBomb = false;
						bombPlantedAt = player->LastWayPoint;
						if (roundState == PLAYING)
						{
							PartyMinutes = allPartyModes[currentPartyMode].bombWaitingMinutesDuration;
							PartySeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						}
						BombSeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						player->haveBomb = false;
						SetGunInInventoryForNonLocalPlayer(i, EMPTY, 8);
						NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
						NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
						Model[7]->rz = BombPosition.r;
						bombBipTimer = 120;
						SetBombDefuseZone(BombPosition.x, BombPosition.z, &bombDefuseZone);
						showPartyEventText(2);
						SetRandomDefuser();
					}
				}
				else
				{
					if (!BombDefused)
					{
						// On bomb defuse make a sound
						if (player->bombTimer == bombDefuseTime)
						{
							int Panning, Volume;
							GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);
							Play3DSound(SFX_BOMBPLANTING, Volume, Panning);
						}
						player->bombTimer--;
						// Set bomb when defused
						if (player->bombTimer == 0)
						{
							BombDefused = true;
							player->isPlantingBomb = false;
							currentDefuserIndex = NO_PLAYER;
							CounterScore++;

							if (LoseCountTerrorists > 0)
								LoseCountTerrorists--;

							if (LoseCountCounterTerrorists < 4)
								LoseCountCounterTerrorists++;

							addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney, TERRORISTS);
							addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, COUNTERTERRORISTS);

							showPartyEventText(3);
							PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
							PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
							roundState = END_ROUND;
							CheckAfterRound();
						}
					}
				}
			}
		}

		partyTimerTick();
	}

	// Make bomb sounds
	if (BombPlanted && !BombDefused)
	{
		if (bombBipTimer > 0)
			bombBipTimer--;

		if (bombBipTimer == 0 && BombSeconds > 0 && !BombWillExplose)
		{
			// make bib sound
			int Panning, Volume;
			GetPanningByPosition(&Panning, &Volume, BombPosition, xWithoutYForAudio, zWithoutYForAudio, 0.12);
			Play3DSound(SFX_BOMBBIP, Volume, Panning);
			bombBipTimer = (BombSeconds / 40.0) * 120.0 + 5;
			if (bombBipTimer > 50)
			{
				SetRandomDefuser();
			}
		}
		else if (bombBipTimer == 0 && BombSeconds == 0 && !BombWillExplose)
		{
			// Make detonate sound
			int Panning, Volume;
			GetPanningByPosition(&Panning, &Volume, BombPosition, xWithoutYForAudio, zWithoutYForAudio, 0.15);
			Play3DSound(SFX_DETONATE, Volume, Panning);
			bombBipTimer = 120;
			BombWillExplose = true;
		}
		else if (bombBipTimer == 0 && BombWillExplose) // TODO Check bomb explosion online without PartySeconds == 0
		{
			// Make explosion sound
			PlayBasicSound(SFX_BOMBEXPLODE);
			BombPlanted = false;
			IsExplode = true;

			rumble(2);

			if (applyRules)
			{
				BombWillExplose = false;
				for (int i = 0; i < MaxPlayer; i++)
				{
					Player *player = &AllPlayers[i];
					if (player->IsDead || player->Id == NO_PLAYER)
						continue;

					float Distance = (float)sqrt(pow(player->PlayerModel->x - BombPosition.x * 4096.0, 2.0) + pow(player->PlayerModel->y - BombPosition.y * 4096.0, 2.0) + pow(player->PlayerModel->z - BombPosition.z * 4096.0, 2.0)) / 8096.0;
					if (Distance > 19)
						Distance = 0;

					if (Distance > 0)
					{
						int newHealh = player->Health - (int)map(Distance, 0.3, 19, 200, 0);
						setPlayerHealth(i, newHealh);
						checkAfterDamage(NO_PLAYER, i, false);
					}
				}
			}
		}
	}

	addExplosionScreenShake();

	// Loop using "AllPlayers" array for updating non local player (online players or bots) position smoothly
	SetOnlinelPlayersPositions();

	ScanForInput();

	if (roundState != WAIT_START)
	{
		AiCheckForAction();

		checkAiShoot();
	}
}

void addExplosionScreenShake()
{
	if (IsExplode)
	{
		BombExplosionScale++;
		int Scale = 4096 * BombExplosionScale;
		NE_ModelScaleI(Model[10], Scale, Scale, Scale);

		xOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
		if (rand() % 2 == 0)
			xOffset = -xOffset;

		yOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
		if (rand() % 2 == 0)
			yOffset = -yOffset;

		speedAimingReCenter = 2;
		speedAimingReCenterTimer = 8;
	}
}

bool isLocalPlayerMoving()
{
	return localPlayer->PlayerPhysic->xspeed + localPlayer->PlayerPhysic->yspeed + localPlayer->PlayerPhysic->zspeed != 0;
}

void UpdateEngineNotInGame()
{
	if (UpdateBottomScreenOneFrame > 8)
		UpdateBottomScreenOneFrame--;

	NE_ProcessDual(Draw3DSceneNotInGame, Draw3DScene2);

	if (uiTimer > 0)
	{
		uiTimer--;
		if (uiTimer == 0)
		{
			if (actionOfUiTimer == SAVE)
			{
				Save();
			}
		}
	}

	NE_WaitForVBL(NE_CAN_SKIP_VBL);
}

void UpdateEngine()
{
	if (!isDebugBottomScreen)
	{
		// Draw 3D objects and sprites
		if (!AlwaysUpdateBottomScreen && UpdateBottomScreenOneFrame == 0)
			NE_Process(Draw3DScene);
		else
		{
			if (UpdateBottomScreenOneFrame > 0)
				UpdateBottomScreenOneFrame--;

			NE_ProcessDual(Draw3DScene, Draw3DScene2);
		}
	}
	else
	{
		NE_Process(Draw3DScene); // For debug
	}

	if (uiTimer > 0)
	{
		uiTimer--;
		if (uiTimer == 0)
		{
			if (actionOfUiTimer == SAVE)
			{
				Save();
			}
		}
	}

	// Update physics and animations
	if (localPlayer->Id != UNUSED)
		NE_PhysicsUpdate(localPlayer->PlayerPhysic);
	// NE_WaitForVBL(NE_UPDATE_ANIMATIONS | NE_CAN_SKIP_VBL);
	// NE_WaitForVBL(NE_UPDATE_ANIMATIONS);
	// NE_WaitForVBL(NE_UPDATE_ANIMATIONS | NE_CAN_SKIP_VBL);
	NE_WaitForVBL(NE_CAN_SKIP_VBL);
}

void buyGun()
{
	int grenadeIndex = GetSelectedGunShop() - GunCount;
	int equipmentIndex = GetSelectedGunShop() - GunCount - shopGrenadeCount;
	if (ShopCategory < EQUIPMENT && AllGuns[GetSelectedGunShop()].Price <= localPlayer->Money)
	{
		if (!allPartyModes[currentPartyMode].infiniteMoney)
		{
			reducePlayerMoney(0, AllGuns[GetSelectedGunShop()].Price);
		}

		if (ShopCategory == PISTOLS)
		{
			SetGunInInventory(GetSelectedGunShop(), 1);
		}
		else
		{
			SetGunInInventory(GetSelectedGunShop(), 2);
		}
	}
	else if (ShopCategory == GRENADES && AllGrenades[grenadeIndex].Price <= localPlayer->Money)
	{
		for (int grenadeCheckIndex = InventoryGrenadeStartPosition; grenadeCheckIndex < InventoryEquipementStartPosition; grenadeCheckIndex++)
		{
			if (localPlayer->AllGunsInInventory[grenadeCheckIndex] == -1 && localPlayer->grenadeBought[grenadeIndex] < AllGrenades[grenadeIndex].maxQuantity[currentPartyMode])
			{
				localPlayer->AllGunsInInventory[grenadeCheckIndex] = GetSelectedGunShop();
				localPlayer->grenadeBought[grenadeIndex]++;

				if (!allPartyModes[currentPartyMode].infiniteMoney)
				{
					reducePlayerMoney(0, AllGrenades[grenadeIndex].Price);
				}
				break;
			}
		}
	}
	else if (ShopCategory == EQUIPMENT && AllEquipements[equipmentIndex].Price <= localPlayer->Money)
	{
		if (!allPartyModes[currentPartyMode].infiniteMoney)
		{
			reducePlayerMoney(0, AllEquipements[equipmentIndex].Price);
		}

		switch (AllEquipements[equipmentIndex].type)
		{
		case DEFUSER:
			localPlayer->haveDefuseKit = true;
			break;
		case KEVLAR_VEST:
			localPlayer->armor = 100;
			break;
		case KEVLAR_VEST_AND_HELMET:
			localPlayer->armor = 100;
			localPlayer->haveHeadset = true;
			break;
		case C4:
			break;
		}
	}
}

void setNewRoundHandWeapon()
{
	for (int index = 0; index < MaxPlayer; index++)
	{
		Player *player = &AllPlayers[index];
		if (player->Id == UNUSED)
			continue;

		if (index == 0 && player->haveBomb)
		{
			player->currentGunInInventory = 8;
		}
		else if (getPlayerCurrentGunIndex(player) == EMPTY)
		{
			player->currentGunInInventory = 1;
			if (getPlayerCurrentGunIndex(player) == EMPTY)
			{
				player->currentGunInInventory = 0;
			}
		}

		if (index == 0)
			UpdateGunTexture();
	}
}

// Change value "scale" ex map(128, 0,255,0,1024) = 512.0
double map(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Change value "scale" ex map(128, 0,255,0,1024) = 512
int mapInt(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Get player position and set it to float
void CalculatePlayerPosition(int PlayerIndex)
{
	Player *player = &AllPlayers[PlayerIndex];
	player->position.x = player->PlayerModel->x / 4096.0;
	player->position.y = player->PlayerModel->y / 4096.0;
	player->position.z = player->PlayerModel->z / 4096.0;
}

void rumble(int timer)
{
	if (useRumble)
	{
		setRumble(true);
		RumbleTimer = timer;
	}
}

void killPlayer(Player *player)
{
	player->Health = 0;
	// Set client has dead
	player->IsDead = true;
	if (player == localPlayer && (currentMenu == SHOP || currentMenu == SHOPCATEGORIES))
	{
		initGameMenu();
	}
}

void checkAfterDamage(int shooterPlayerIndex, int hittedPlayerIndex, bool CheckScore)
{
	Player *HittedClient = &AllPlayers[hittedPlayerIndex];
	Player *killerClient = &AllPlayers[shooterPlayerIndex];

	if (HittedClient->Health <= 0 && !HittedClient->IsDead)
	{
		killPlayer(HittedClient);
		// HittedClient->Health = 0;
		//  Set client has dead
		// HittedClient->IsDead = true;

		// Reset raycast values on death
		if (hittedPlayerIndex == 0)
		{
			DisableAim();
			Hit = NO_PLAYER;
		}

		if (HittedClient->haveBomb)
		{
			bombDropped = true;
			HittedClient->haveBomb = false;
			SetGunInInventoryForNonLocalPlayer(hittedPlayerIndex, EMPTY, 8);
			//  Set the position of the dropped bomb (so the player position)
			droppedBombPositionAndRotation.x = HittedClient->position.x;
			droppedBombPositionAndRotation.y = HittedClient->position.y - 0.845;
			droppedBombPositionAndRotation.z = HittedClient->position.z;
			droppedBombPositionAndRotation.r = HittedClient->Angle;
			NE_ModelSetCoord(Model[7], droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.y, droppedBombPositionAndRotation.z);
			Model[7]->rz = droppedBombPositionAndRotation.r;
			SetBombTakingZone(droppedBombPositionAndRotation.x, droppedBombPositionAndRotation.z, &bombDefuseZone); // Set zone for taking the bomb
		}

		// HittedClient->ScanFinished = true;
		HittedClient->target = NO_PLAYER;
		HittedClient->bombTimer = 0;
		HittedClient->isPlantingBomb = false;

		// If the killed player was a defuser, free the defuser place for other bots
		if (&AllPlayers[currentDefuserIndex] == HittedClient)
			currentDefuserIndex = NO_PLAYER;

		// Increase player death count
		HittedClient->DeathCount++;

		// If the killer kills the right target, set current target as "null"
		if (killerClient->target == HittedClient->Id)
		{
			killerClient->target = NO_PLAYER;
			killerClient->lastSeenTarget = NO_PLAYER;
		}
		// Stop dead player movements
		if (HittedClient->isAi)
		{
			HittedClient->justCheking = false;
			HittedClient->lastSeenTarget = NO_PLAYER;
			HittedClient->PathCount = 0;
		}

		// If the killer is not null
		if (shooterPlayerIndex != NO_PLAYER)
		{
			// Show a text to show with is the killer
			showKillText(shooterPlayerIndex, hittedPlayerIndex);
			// Add penalties if the killer is in the same team as the dead player
			if (allPartyModes[currentPartyMode].teamDamage && HittedClient->Team == killerClient->Team)
			{
				killerClient->KillCount--;
				reducePlayerMoney(shooterPlayerIndex, allPartyModes[currentPartyMode].killPenalties);
				// killerClient->Money -= allPartyModes[currentPartyMode].killPenalties;
				// if (killerClient->Money < 0)
				// killerClient->Money = 0;
			}
			else
			{
				// Increase killer kill count
				killerClient->KillCount++;
				// if (AllShopElements[killerClient.AllGunsInInventory[killerClient.currentGunInInventory]] is Gun)
				// AddMoneyTo(killerClient, ((Gun)AllShopElements[killerClient.AllGunsInInventory[killerClient.currentGunInInventory]]).KillMoneyBonus[killerClient.ClientParty.PartyType]);
			}
		}

		// Update bottom screen if scoreboard is showed
		if (currentMenu == 2)
			UpdateBottomScreenOneFrame += 8;

		if (!CheckScore)
			return;

		if (roundState == PLAYING)
		{
			// Check if all a team is dead
			int CounterDeadCount = 0, TerroristDeadCount = 0, CounterTerroristsCount = 0, TerroristsCount = 0;

			CheckTeamDeathCount(&TerroristsCount, &CounterTerroristsCount, &TerroristDeadCount, &CounterDeadCount);
			// If all counter terrorists are dead
			if (CounterDeadCount == CounterTerroristsCount)
			{
				// Add a point to terrorists teams
				TerroristsScore++;

				// Set kill bonus count for each teams
				if (LoseCountTerrorists > 0)
					LoseCountTerrorists--;

				if (LoseCountCounterTerrorists < 4)
					LoseCountCounterTerrorists++;

				// Add money to teams
				addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundMoney, TERRORISTS);
				addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountCounterTerrorists, COUNTERTERRORISTS);

				// Set round to finished round state
				PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
				PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
				roundState = END_ROUND;
				showPartyEventText(0);
				CheckAfterRound();
			}
			else if (TerroristDeadCount == TerroristsCount && !bombSet)
			{
				// Add a point to counter terrorists teams
				CounterScore++;

				// Set kill bonus count for each teams
				if (LoseCountCounterTerrorists > 0)
					LoseCountCounterTerrorists--;

				if (LoseCountTerrorists < 4)
					LoseCountTerrorists++;

				// Add money to teams
				addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundMoney, COUNTERTERRORISTS);
				addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountTerrorists, TERRORISTS);

				// Set round to finished round state
				PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
				PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
				roundState = END_ROUND;
				showPartyEventText(1);
				CheckAfterRound();
			}
		}
		else if (roundState == TRAINING) // If the party is in training mode
		{
			// Respawn the dead player
			HittedClient->RespawnTimer = allPartyModes[currentPartyMode].trainingRespawnSecondsDuration * 60;
			HittedClient->NeedRespawn = true;
		}
	}
}

void makeHit(int hitBy, int playerHit)
{
	int hitType = 0;
	int Damage = AllGuns[AllPlayers[hitBy].startGunIdRaycast].Damage;

	Player *hittedPlayer = &AllPlayers[playerHit];
	Player *shooterPlayer = &AllPlayers[hitBy];

	// Apply more or less damage with the location of the bullet
	if (shooterPlayer->IsHeadShot)
	{
		hitType = 1;
		Damage *= 4;
	}
	else if (shooterPlayer->IsLegShot)
	{
		hitType = 2;
		Damage /= 2.0;
	}

	if (shooterPlayer->startGunIdRaycast < GunCount && AllGuns[shooterPlayer->startGunIdRaycast].isKnife)
		hitType = 3;

	bool makeHeadSetSound = false;
	// If an armor was touched
	if ((hitType == 0 && hittedPlayer->armor != 0) || (hitType == 1 && hittedPlayer->haveHeadset))
	{
		// Reduce damage of the bullet
		int oldDamage = Damage;
		Damage *= AllGuns[shooterPlayer->startGunIdRaycast].penetration / 100.0;
		// Remove headset
		if (hitType == 1)
		{
			hittedPlayer->haveHeadset = false;
			makeHeadSetSound = true;
		}
		else // Or reduce armor durability
		{
			hittedPlayer->armor -= oldDamage - Damage;
			if (hittedPlayer->armor < 0)
				hittedPlayer->armor = 0;
		}
	}

	// Set hitted player life
	setPlayerHealth(playerHit, hittedPlayer->Health - Damage);
	checkAfterDamage(hitBy, playerHit, true);

	if (!hittedPlayer->IsDead && hittedPlayer->target == NO_PLAYER && hittedPlayer != localPlayer)
	{
		hittedPlayer->target = hitBy;
		if (hittedPlayer->GunWaitCount >= 0)
			hittedPlayer->GunWaitCount = -60;
	}

	// Get sound volume and panning
	int Panning, Volume;
	GetPanning(hittedPlayer->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);

	if (playerHit == 0)
	{
		rumble(1);
	}

	if (hittedPlayer == localPlayer)
	{
		redHealthTextCounter = 62;

		xOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
		if (rand() % 2 == 0)
			xOffset = -xOffset;

		yOffset = (rand() % ScreenShakeAmount + ScreenShakeMinAmount) / 100.0;
		if (rand() % 2 == 0)
			yOffset = -yOffset;
	}

	// Play sounds
	if (hitType == 0)
		Play3DSound(SFX_FLESH_IMPACT, Volume, Panning); // TODO Check kevlar sound
	else if (hitType == 1)
	{
		if (!makeHeadSetSound)
			Play3DSound(SFX_HEADSHOT1, Volume, Panning);
		else
			Play3DSound(SFX_HIT_HELMET, Volume, Panning);
	}
	else if (hitType == 2)
		Play3DSound(SFX_FLESH_IMPACT, Volume, Panning);
	else
		Play3DSound(SFX_KNIFE_HIT_PLAYER, Volume, Panning);
}