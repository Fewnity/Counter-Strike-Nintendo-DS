////
// Creator : Fewnity
//
// dd/mm/yyyy
// Creation date --/01/2021
//
// Last modification : 19/07/2021
//
// This file need Nitro Engine
////

#include "collisions.h"
#include "movements.h"
#include "sounds.h"
#include "main.h"
#include "network.h"
#include "ui.h"
#include "gun.h"
#include "grenade.h"
#include "equipment.h"
#include "ai.h"
#include "map.h"
#include "party.h"

//
//////Level
// Whole level 3D models and physics
NE_Model *Model[11]; ////////////REMOVE OR CHANGE THIS

Stairs AllStairs[31];
int StairsCount = 0;
// int CurrentOcclusionZone = 0;

Wall AllWallsCollisions[wallCount];

CollisionBox2D AllTriggersCollisions[OcclusionZonesCount]; /////MOVE THIS TO Zone STRUCT
Zone AllZones[OcclusionZonesCount];
CollisionBox2D AllBombsTriggersCollisions[2];

// Materials
NE_Material *GroundMaterial;

//
//////Players
Player AllPlayers[MaxPlayer];
// Player values
int PlayerCount = 0;
float OldxPos, OldyPos, OldzPos = 0;
int Health = 100;
int CanJump = 0;
int CanJumpReal = 2;
bool Aiming;

int frameCountDuringAir;
bool isOnStairs = false;
bool isInDownStairs;
bool NeedJump; // Ask jump from UI button
// Head bobing
float BobbingOffset = 0;
float XBobbingOffset = 0;
float BobbingSpeed = 0.07;
bool HasBobbed;
// Materials
NE_Material *PlayerMaterial;
NE_Material *PlayerMaterialTerrorist;

//////Guns
// Values

int ShowMuzzle;
int ShowWallHitFlash;
int gunX;
int gunY;

Scope AllScopeLevels[2];
int CurrentScopeLevel;

// UI
int GunXRecoil = 6, GunYRecoil = 6;
int GunMaxRecoil = 16;
int GunMinRecoil = 6;

// Materials
NE_Material *GunMaterial;

// Animations

float flashaAnimation = 0;
bool flashed;

//
//////Camera
NE_Camera *Camara;
// Values
float x, y, z;
float xOffset, yOffset;
float CameraAngleY = 128;
float CameraOffsetY = 0.7;
float xWithoutY, zWithoutY, xWithoutYForAudio, zWithoutYForAudio, xWithoutYForMap, zWithoutYForMap;
int CurrentCameraPlayer;
bool NeedUpdateViewRotation = false;

// Animations
#define deathCameraAnimationSpeed 0.025
float deathCameraAnimation = 0;
float deathCameraYOffset = 0;

// Raycasting
int Hit = -1;

int WallHitXPos;
int WallHitYPos;
int WallHitZPos;

// Other
bool isInFullSmoke;

//
//////Inputs
uint32 keys;
uint32 keysdown;
uint32 keysup;
touchPosition touch;

// Other
int tempTeam;

// AI
int checkPlayerDistanceFromAiTimer = 1;

//
//////UI
NE_Material *CrossHairTexture;
NE_Sprite *CrossHairSprite;

NE_Material *GunTexture;
NE_Sprite *GunSprite;

NE_Palette *Palettes[16]; // 0 MapUI, 1 Map atlas, 2 text, 3 player, 4 gun sprite, 5 map point

NE_Palette *roadPalette;
NE_Palette *wallWindowPalette;
NE_Palette *wallPalette;
NE_Material *roadMaterials;
NE_Material *wallWindowMaterials;
NE_Material *wallMaterials;
NE_Material *TextMaterial;

int TopScreenSpriteCount = 1;
NE_Sprite *TopScreenSprites[5];
NE_Material *TopScreenSpritesMaterials[5];
int BottomScreenSpriteCount = 6;
NE_Sprite *BottomScreenSprites[10];
NE_Material *BottomScreenSpritesMaterials[9]; // Remove this
CheckBox AllCheckBoxs[1];
Button AllButtons[7];

char textToShow[30] = "";
char killText[33] = "";
int textToShowTimer = 0;
int KillTextShowTimer = 0;
int SelectedGunShop;

bool AlwaysUpdateBottomScreen = true;
bool NeedChangeScreen = false;
int UpdateBottomScreenOneFrame = 4;
int ButtonToShow;
int redHealthTextCounter;
int doubleTapTimer;

//
//////Party
int currentMenu = -1; // 0 Main menu //1 Map //2 Score //3 Shop categories //4 Settings //5 Quit //6 Shop
bool PartyStarted;
int currentPartyMode = 1;
bool applyRules;
int LoseCountTerrorists;
int LoseCountCounterTerrorists;

int PartyMinutes;
int PartySeconds;
int RoundState = -1;
int CounterScore = 0;
int TerroristsScore = 0;

int changeSecondMTimer = 60;
int changeMinuteTimer = 60;
bool bombSet;

//
//////Bomb
int bombBipTimer = 120;
Vector4 BombPosition;
bool BombPlanted;
bool BombDefused;
bool BombWillExplose;
CollisionBox2D bombDefuseZone;
int BombExplosionScale = 0;
bool IsExplode;

//
//////Debug
bool isDebugTopScreen = false;
bool isDebugBottomScreen = false;

//
//////Rumble pack
int RumbleTimer = 0;
int Connection = -1;

Player *localPlayer = &AllPlayers[0];

// Min x -44.812
// Max x 56.8331

// Min z 66.496
// Max z 41.601

// Draw top screen
void Draw3DScene(void)
{
	// Set camera for drawing
	NE_CameraUse(Camara);

	// Reset polygons Alpha/Light/Effect
	NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

	// Draw map
	for (int i = 0; i < AllZones[AllPlayers[0].CurrentOcclusionZone].ZoneCount; i++)
		NE_ModelDraw(Model[AllZones[AllPlayers[0].CurrentOcclusionZone].AllVisibleZones[i]]);

	// for (int i = 0; i < 7; i++)
	// NE_ModelDraw(Model[i]);

	// Draw grenades and their effects
	for (int i = 0; i < GrenadeCount; i++)
	{
		if (grenades[i] != NULL)
		{
			if (grenades[i]->isVisible)
				NE_ModelDraw(grenades[i]->Model);
			// Set polygons alpha
			NE_PolyFormat(grenades[i]->effectAlpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
			if (grenades[i]->EffectTimer != 0)
			{
				if (grenades[i]->GrenadeType == 1 && isInFullSmoke)
					continue;

				NE_ModelDraw(grenades[i]->EffectModel);
			}
		}
	}
	isInFullSmoke = false;

	// Reset polygons Alpha/Light/Effect
	NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

	// If bomb is planted, draw bomb
	if (BombPlanted)
		NE_ModelDraw(Model[7]);

	// Show wall bullet hit flash
	if (ShowWallHitFlash > 0)
	{
		ShowWallHitFlash--;
		NE_ModelDraw(Model[8]);
	}

	NE_ModelDraw(Model[9]);

	// Draw bomb explosion
	if (IsExplode)
	{
		// Calculate explosion effect alpha
		int ExplosionAlpha = 31 - (int)(BombExplosionScale / 1.2);
		if (ExplosionAlpha < 0)
		{
			ExplosionAlpha = 0;
			IsExplode = false;
		}
		// Set polygons alpha
		NE_PolyFormat(ExplosionAlpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

		// Draw explosion
		NE_ModelDraw(Model[10]);

		// Reset polygons Alpha/Light/Effect
		NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
	}

	// Draw players
	for (int i = 1; i < MaxPlayer; i++)
	{
		if (AllPlayers[i].Id != -1 && !AllPlayers[i].IsDead && CurrentCameraPlayer != i)
		{
			for (int i2 = 0; i2 < AllZones[AllPlayers[0].CurrentOcclusionZone].ZoneCount; i2++)
			{
				for (int i3 = 0; i3 < AllZones[AllPlayers[i].CurrentOcclusionZone].ZoneCount; i3++)
				{
					if (AllZones[AllPlayers[0].CurrentOcclusionZone].AllVisibleZones[i2] == AllZones[AllPlayers[i].CurrentOcclusionZone].AllVisibleZones[i3])
					{
						NE_ModelDraw(AllPlayers[i].PlayerModel);
						i2 = AllZones[AllPlayers[0].CurrentOcclusionZone].ZoneCount;
						break;
					}
				}
			}
		}
	}

	/*for (int i = 1; i < MaxPlayer; i++)
	{
		if (AllPlayers[i].Id != -1 && !AllPlayers[i].IsDead)
		{
			NE_ModelDraw(AllPlayers[i].PlayerModel);
		}
	}*/

	drawTopScreenUI();
}

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

int main(void)
{
	// Need to include this???
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

	if (isDebugBottomScreen)
		consoleDemoInit();

	// Init fat file system
	if (!fatInitDefault())
	{
		// iprintf("Warning: fatInit failure!\n");
	}

	isRumbleInserted();

	// iprintf("Rumble %d\n", isRumbleInserted());
	/*if (!nitroFSInit(NULL))
	{
		iprintf("nitroFSInit failure!\n");
	}*/

	CreateWaypoints();

	initSoundSystem();

	/*FILE *txt = NULL;
	txt = fopen("info.txt", "r");

	char line[50];
	fgets(line, sizeof line, txt);
	printf(line);*/

	// Remove this later
	SetPlayerForCollisions();
	SetPlayersForMovements();
	SetPlayerForUI();
	SetButtonsRefForUI();
	SetSpritesForUI();
	SetCheckBoxsRefForUI();
	setBotsNames();

	// Load guns data
	AddGuns();
	AddGrenades(AllGrenades);
	AddEquipment(AllEquipements);

	// Camera
	Camara = NE_CameraCreate();
	UpdateLookRotation(CameraAngleY, &x, &y, &z, &xWithoutY, &zWithoutY, &xWithoutYForAudio, &zWithoutYForAudio);

	// Set start camera position to look the map
	localPlayer->CurrentOcclusionZone = 15;
	NE_CameraSet(Camara,
				 -3, 8, -14,
				 -12, 3.2, -16,
				 0, 1, 0);

	// create Materials
	GroundMaterial = NE_MaterialCreate();
	PlayerMaterial = NE_MaterialCreate();
	PlayerMaterialTerrorist = NE_MaterialCreate();
	TopScreenSpritesMaterials[0] = NE_MaterialCreate();
	TopScreenSpritesMaterials[2] = NE_MaterialCreate();
	TopScreenSpritesMaterials[3] = NE_MaterialCreate();
	TopScreenSpritesMaterials[4] = NE_MaterialCreate();
	GunMaterial = NE_MaterialCreate();
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
	NE_MaterialTexLoadBMPtoRGB256(PlayerMaterialTerrorist, Palettes[13], (void *)gign_skin1_bin, 0);

	NE_MaterialTexLoadBMPtoRGB256(GroundMaterial, Palettes[1], (void *)Atlas_bin, 1);
	// NE_MaterialTexLoadBMPtoRGB256(GroundMaterial, Palettes[1], (void *)road_bin, 1);

	// Palettes[4] = NE_PaletteCreate();
	// NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], AllGuns[CurrentLittleGun].texture, 1);
	// UpdateGunTexture();

	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[0], Palettes[9], (void *)QuitButton_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[2], Palettes[5], (void *)MapPointUI_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[4], Palettes[7], (void *)CheckMark_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[5], Palettes[6], (void *)WhiteScareRounded_bin, 1);
	// NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[6], Palettes[10], AllGuns[0].texture, 1);

	// Load .bmp textures top screen
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[0], Palettes[8], (void *)crosshair2_bin, 1);

	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[2], Palettes[11], (void *)muzzle_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[3], Palettes[12], (void *)scopeImage_bin, 1);
	NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[4], Palettes[14], (void *)MapUI_bin, 0);

	// Create crosshair sprite
	TopScreenSprites[0] = NE_SpriteCreate();
	NE_SpriteSetPos(TopScreenSprites[0], ScreenCenterWidth - 20 / 2, ScreenCenterHeight - 20 / 2);
	NE_SpriteSetSize(TopScreenSprites[0], 20, 20);
	NE_SpriteSetPriority(TopScreenSprites[0], 10);
	NE_SpriteSetMaterial(TopScreenSprites[0], TopScreenSpritesMaterials[0]);

	// Create map sprite
	TopScreenSprites[4] = NE_SpriteCreate();
	// NE_SpriteSetSize(TopScreenSprites[4], 196, 196);
	//  NE_SpriteSetSize(TopScreenSprites[4], 173, 185);
	NE_SpriteSetSize(TopScreenSprites[4], 170, 177);
	//   NE_SpriteSetSize(TopScreenSprites[4], 196 * 1.5, 196 * 1.5);
	NE_SpriteSetPriority(TopScreenSprites[4], 2);
	NE_SpriteSetMaterial(TopScreenSprites[4], TopScreenSpritesMaterials[4]);
	// NE_SpriteSetRot(TopScreenSprites[4], (int)localPlayer->Angle);
	//  Create map point
	BottomScreenSprites[1] = NE_SpriteCreate();
	NE_SpriteSetSize(BottomScreenSprites[1], 6, 6);
	NE_SpriteSetPriority(BottomScreenSprites[1], 1);
	NE_SpriteSetMaterial(BottomScreenSprites[1], BottomScreenSpritesMaterials[2]);

	NE_SpriteVisible(BottomScreenSprites[0], false);
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
	for (int i = 0; i < MapPartCount + 4; i++)
	{
		Model[i] = NE_ModelCreate(NE_Static);
		NE_ModelSetMaterial(Model[i], GroundMaterial);
		NE_ModelScaleI(Model[i], 4096, 4096, 4096);
		NE_ModelSetCoord(Model[i], 0, 1.5 + 0.8, 0);
		Model[i]->rx = 128;
		Model[i]->ry = 256;
	}

	// Load map models
	NE_ModelLoadStaticMesh(Model[0], (u32 *)DustPart0_bin);
	NE_ModelLoadStaticMesh(Model[1], (u32 *)DustPart1_bin);
	NE_ModelLoadStaticMesh(Model[2], (u32 *)DustPart2_bin);
	NE_ModelLoadStaticMesh(Model[3], (u32 *)DustPart3_bin);
	NE_ModelLoadStaticMesh(Model[4], (u32 *)DustPart4_bin);
	NE_ModelLoadStaticMesh(Model[5], (u32 *)DustPart5_bin);
	NE_ModelLoadStaticMesh(Model[6], (u32 *)DustPart6_bin);
	/*NE_ModelLoadStaticMesh(Model[0], (u32 *)DustPart3_1_3ds_bin);
	NE_ModelSetMaterial(Model[0], roadMaterials);
	NE_ModelLoadStaticMesh(Model[1], (u32 *)DustPart3_2_3ds_bin);
	NE_ModelSetMaterial(Model[1], wallWindowMaterials);
	NE_ModelLoadStaticMesh(Model[2], (u32 *)DustPart3_3_3ds_bin);
	NE_ModelSetMaterial(Model[2], wallMaterials);*/

	// Load bomb model
	NE_ModelLoadStaticMesh(Model[7], (u32 *)bomb_bin);

	// Load Plane model
	NE_ModelSetMaterial(Model[8], TopScreenSpritesMaterials[2]);
	NE_ModelLoadStaticMesh(Model[8], (u32 *)plane_bin);
	NE_ModelSetCoord(Model[8], 0, 0, 1);

	NE_ModelSetMaterial(Model[9], GroundMaterial);
	NE_ModelLoadStaticMesh(Model[9], (u32 *)repeat_bin);
	NE_ModelSetCoord(Model[9], 1.5, 1.5 + 0.8, -1);

	NE_ModelLoadStaticMesh(Model[10], (u32 *)explosion_bin);
	Model[10]->rx = 0;
	Model[10]->ry = 256;
	NE_ModelScaleI(Model[10], 0, 0, 0);

	// GROUNDS & WALLS COLLISIONS
	AddAllCollisions();

	// Add occlusion culling triggers
	CalculateAllTriggerColBoxs();

	// Add stairs
	AddAllStairs();
	StairsCount = 31;

	AddAllSpawnPoints();
	AddAllPartyModes();

	// Set A and B bomb site triggers
	SetBombZone(40.8, -20.8, 5, 5, 0, AllBombsTriggersCollisions);
	SetBombZone(-28.03, -27.07, 4.46785, 4.578236, 1, AllBombsTriggersCollisions);

	// Set scope levels
	AllScopeLevels[0].scopeCount = 2;
	AllScopeLevels[0].fov[0] = 20;
	AllScopeLevels[0].fov[1] = 7;
	AllScopeLevels[0].Speed = 140;

	AllScopeLevels[1].scopeCount = 1;
	AllScopeLevels[1].fov[0] = 20;
	AllScopeLevels[1].Speed = 140;

	// 3D gun
	// Model[7] = NE_ModelCreate(NE_Static);
	// Physics[2] = NE_PhysicsCreate(NE_BoundingBox);
	// NE_ModelLoadStaticMesh(Model[7], (u32 *)gun2_bin);
	// NE_PhysicsSetModel(Physics[2], (void *)Model[1]); //Physics object and Model assigned to it
	// NE_PhysicsSetSize(Physics[2], 100, 2, 100);
	// NE_ModelScaleI(Model[2], 4096 * 0.04, 4096 * 0.04, 4096 * 0.04);
	// NE_ModelScaleI(Model[7], 4096 * 1, 4096 * 1, 4096 * 1);
	// NE_ModelSetCoord(Model[7], 0, 3.8, 0);

	// PhysicalGrenade *newGrenade = CreateGrenade(0);
	// lanchGrenade(newGrenade, x, y, z, AllPlayers[0].PlayerModel->x, AllPlayers[0].PlayerModel->y + 2 * 4096, AllPlayers[0].PlayerModel->z);

	// SetGunInInventory(7, 2);

	sprintf(killText, "PlayerName01 killed PlayerName02");

	// Set all player ID to -1
	for (int i = 0; i < MaxPlayer; i++)
	{
		AllPlayers[i].Id = -1;
		AllPlayers[i].IsCounter = -1;
	}

	// Lights
	NE_LightSet(0, NE_White, -0.5, -0.5, -0.5);

	// Set background color
	NE_ClearColorSet(RGB15(17, 26, 29), 31, 63); // Blue sky

	// Open default menu
	ChangeMenu(8);

	// AllButtons[0].isHidden = true;

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

	while (Connection == -1)
	{
		scanKeys();
		keys = keysHeld();
		keysdown = keysDown();
		keysup = keysUp();
		touchRead(&touch);

		if ((keysdown & KEY_A) == KEY_A)
			Connection = 0;
		else if ((keysdown & KEY_B) == KEY_B)
			Connection = 1;
		else if ((keysdown & KEY_Y) == KEY_Y)
			Connection = 2;
		else if ((keysdown & KEY_X) == KEY_X)
			Connection = 3;

		ReadTouchScreen(keysdown, keys, keysup, &currentMenu, touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenOneFrame, &SendTeam);
		UpdateEngine();
	}

	StartGame(Connection);

	return 0;
}

void StartGame(int connectionType)
{
	SetNeedChangeScreen(true);
	ChangeMenu(2);

	// Play without internet
	if (connectionType == 0)
	{
		// PartyStarted = true;
		applyRules = true;
		ResetTakenBotsNames();
		AddNewPlayer(0, true, false);
		// Add bots
		for (int i = 0; i < MaxPlayer - 1; i++)
			AddNewPlayer(i + 1, false, true);

		iprintf("\n");

		int rnd = random() % 2;
		// int rndForBomb = random() % (PlayerCount / 2);
		// int countForBomb = 0;
		for (int i = 0; i < PlayerCount; i++) // TODO replace
		{
			if (rnd == i % 2)
			{
				AllPlayers[i].IsCounter = 1;
			}
			else
			{
				AllPlayers[i].IsCounter = 0;
				/*if (countForBomb == rndForBomb)
				{
					AllPlayers[i].haveBomb = true;
					AllPlayers[i].AllGunsInInventory[8] = 28;
				}
				countForBomb++;*/
			}
			UpdatePlayerTexture(i);
		}

		setPlayersPositionAtSpawns();

		checkShopForBot();

		PartySeconds = allPartyModes[currentPartyMode].trainingSecondsDuration;
		PartyMinutes = allPartyModes[currentPartyMode].trainingMinutesDuration;
		changeSecondMTimer = 60;
		changeSecondMTimer = 60;

		while (true)
		{
			// Play game code
			GameLoop();
			UpdateEngine();
		}
	}
	else
	{
		AllButtons[0].isHidden = true;
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
			if (connectionType == 1)
				connectToServer("88.127.205.17", false, my_socket, true); // Mère
			else if (connectionType == 2)
				connectToServer("88.127.205.50", false, my_socket, true); // Père
			else if (connectionType == 3)
				connectToServer("", true, my_socket, true); // Local mode (Android phone)
		}
	}
}

// Getters / setters

Stairs *GetStairs()
{
	return AllStairs;
}

Wall *GetWalls()
{
	return AllWallsCollisions;
}

Player *GetPlayers()
{
	return AllPlayers;
}

Button *GetButtons()
{
	return AllButtons;
}

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

Gun *GetGuns()
{
	return AllGuns;
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

void SetTextToShow(char *Value)
{
	// textToShow = Value;
}
char *GetTextToShow()
{
	return textToShow;
}

void SetNeedJump()
{
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

void AskUpdateLookRotation()
{
	UpdateLookRotation(CameraAngleY, &x, &y, &z, &xWithoutY, &zWithoutY, &xWithoutYForAudio, &zWithoutYForAudio);
}

void SetGunInInventory(int Value, int inventoryIndex)
{
	localPlayer->AllGunsInInventory[inventoryIndex] = Value;
	if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] == -1)
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
	if (playerIndex == 0 && localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] == -1)
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
	} while (player->AllGunsInInventory[player->currentGunInInventory] == -1);

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

void setPlayersPositionAtSpawns()
{
	int currentCounter = 0;
	int currentTerrorist = 0;

	for (int i = 0; i < PlayerCount; i++)
	{
		Player *player = &AllPlayers[i];
		if (player->IsCounter == 0)
		{
			player->PlayerModel->x = allMaps[0].allTerroristsSpawns[currentTerrorist].x * 4096.0;
			player->PlayerModel->y = allMaps[0].allTerroristsSpawns[currentTerrorist].y * 4096.0;
			player->PlayerModel->z = allMaps[0].allTerroristsSpawns[currentTerrorist].z * 4096.0;
			// Add this for non AI players
			player->xDestination = allMaps[0].allTerroristsSpawns[currentTerrorist].x;
			player->yDestination = allMaps[0].allTerroristsSpawns[currentTerrorist].y;
			player->zDestination = allMaps[0].allTerroristsSpawns[currentTerrorist].z;
			currentTerrorist++;

			player->Angle = allMaps[0].startPlayerAngleTerrorists;
			player->AngleDestination = allMaps[0].startPlayerAngleTerrorists;
		}
		else if (player->IsCounter == 1)
		{
			player->PlayerModel->x = allMaps[0].allCounterTerroristsSpawns[currentCounter].x * 4096.0;
			player->PlayerModel->y = allMaps[0].allCounterTerroristsSpawns[currentCounter].y * 4096.0;
			player->PlayerModel->z = allMaps[0].allCounterTerroristsSpawns[currentCounter].z * 4096.0;
			// Add this for non AI players
			player->xDestination = allMaps[0].allCounterTerroristsSpawns[currentCounter].x;
			player->yDestination = allMaps[0].allCounterTerroristsSpawns[currentCounter].y;
			player->zDestination = allMaps[0].allCounterTerroristsSpawns[currentCounter].z;
			currentCounter++;

			player->Angle = allMaps[0].startPlayerAngleCounterTerrorists;
			player->AngleDestination = allMaps[0].startPlayerAngleCounterTerrorists;
		}

		CalculatePlayerPosition(i);
	}
	NeedUpdateViewRotation = true;
}

void UpdateGunTexture()
{
	// unload old texture
	if (TopScreenSpritesMaterials[1] != NULL)
	{
		NE_MaterialDelete(TopScreenSpritesMaterials[1]);
		NE_PaletteDelete(Palettes[4]);
	}

	// Reserve memory for new texture
	TopScreenSpritesMaterials[1] = NE_MaterialCreate();
	Palettes[4] = NE_PaletteCreate();

	Player *viewedPlayer = &AllPlayers[CurrentCameraPlayer];

	// TODO UPDATE FOR OTHER CAMERA
	//  Load new texture
	if (viewedPlayer->AllGunsInInventory[viewedPlayer->currentGunInInventory] < GunCount)
		NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], AllGuns[viewedPlayer->AllGunsInInventory[viewedPlayer->currentGunInInventory]].texture, 1);
	else if (viewedPlayer->AllGunsInInventory[viewedPlayer->currentGunInInventory] < GunCount + shopGrenadeCount)
		NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], AllGrenades[viewedPlayer->AllGunsInInventory[viewedPlayer->currentGunInInventory] - GunCount].texture, 1);
	else
		NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], AllEquipements[viewedPlayer->AllGunsInInventory[viewedPlayer->currentGunInInventory] - GunCount - shopGrenadeCount].texture, 1);
}

// Change player skin texture
void UpdatePlayerTexture(int playerIndex)
{
	Player *player = &AllPlayers[playerIndex];
	if (player->IsCounter == 1)
		NE_ModelSetMaterial(player->PlayerModel, PlayerMaterial);
	else if (player->IsCounter == 0)
		NE_ModelSetMaterial(player->PlayerModel, PlayerMaterialTerrorist);
}

void GameLoop()
{
	// Read keys
	scanKeys();
	keys = keysHeld();
	keysdown = keysDown();
	keysup = keysUp();
	touchRead(&touch);

	if (RumbleTimer != 0)
	{
		RumbleTimer--;
		if (RumbleTimer == 0)
			setRumble(false);
	}

	// Get local player position
	CalculatePlayerPosition(0);

	// Check if player can defuse or plant the bomb
	bool CanPutBomb = false, canDefuseBomb = false;
	CheckZones(AllTriggersCollisions, AllBombsTriggersCollisions, bombDefuseZone, &CanPutBomb, &canDefuseBomb);

	UpdateGrenades();

	// Mini map player point
	// Min x -44.812
	// Max x 56.8331

	// Min z 66.496
	// Max z -41.601
	// Move player point
	// NE_SpriteSetPos(BottomScreenSprites[1], map(AllPlayers[0].xPos, -44, 56, 2, 189), map(AllPlayers[0].zPos, -36, 67, 12, 190));

	// NeedUpdateViewRotation = false;

	if (doubleTapTimer > 0)
		doubleTapTimer--;

	// Check touchscreen input for menus
	ReadTouchScreen(keysdown, keys, keysup, &currentMenu, touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenOneFrame, &SendTeam);

	if (localPlayer->IsCounter != -1)
	{
		// Check input and rotate player
		RotatePlayer(keys, &NeedUpdateViewRotation, &SendPosition, &CameraAngleY, CurrentScopeLevel);

		// Update look rotation value
		if (NeedUpdateViewRotation || CurrentCameraPlayer != 0)
		{
			UpdateLookRotation(CameraAngleY, &x, &y, &z, &xWithoutY, &zWithoutY, &xWithoutYForAudio, &zWithoutYForAudio);
			NeedUpdateViewRotation = false;
		}

		isInDownStairs = false;
		// Check if the player is on a stairs
		CheckStairs(StairsCount, &CanJump, &isInDownStairs);

		if (CanJump > 0)
			CanJump--;

		// Check for jump
		if (RoundState != 0 && !localPlayer->IsDead)
		{
			if (localPlayer->PlayerPhysic->yspeed == 0 && CanJumpReal > 0)
				CanJumpReal--;
			else if (localPlayer->PlayerPhysic->yspeed != 0)
				CanJumpReal = 2;

			if (CanJumpReal != 0 && !isInDownStairs)
				frameCountDuringAir++;
			else if ((CanJumpReal == 0 || (isInDownStairs)) && frameCountDuringAir > 20)
			{
				frameCountDuringAir = 0;
				Play2DSound(SFX_LAND, 140);
				NeedJump = false;
			}
			else
				frameCountDuringAir = 0;

			if ((keysdown & KEY_R || NeedJump) && (CanJumpReal == 0 || CanJump > 0))
			{
				NeedJump = false;
				localPlayer->PlayerPhysic->yspeed = JumpForce;
				CanJump = 0;
			}
		}

		// Set aiming view
		if (keysdown & KEY_START)
		{
			SetAim();
		}
	}

	Player *playerWithView = &AllPlayers[CurrentCameraPlayer];
	// Set camera position
	NE_CameraSet(Camara,
				 playerWithView->xPos, playerWithView->yPos + CameraOffsetY - deathCameraYOffset, playerWithView->zPos,
				 playerWithView->xPos + x + xOffset, playerWithView->yPos + CameraOffsetY + y + yOffset + deathCameraYOffset, playerWithView->zPos + z + xOffset,
				 0, 1, 0);

	// Screen shake
	if (xOffset >= 0.005)
		xOffset -= 0.005;
	else if (xOffset < -0.005)
		xOffset += 0.005;

	if (yOffset >= 0.005)
		yOffset -= 0.005;
	else if (yOffset < -0.005)
		yOffset += 0.005;

	// NE_ModelSetRot(Model[2], -128, 128 - CameraAngleY, PlayerRot + 256);

	// Change gun sprite position to normal position if needed
	if (GunXRecoil > GunMinRecoil)
		GunXRecoil--;
	if (GunYRecoil > GunMinRecoil)
		GunYRecoil--;

	if (applyRules && Hit != -1 && localPlayer->ScanFinished)
	{
		makeHit(0, Hit);
		Hit = -1;
	}

	for (int i = 0; i < MaxPlayer; i++)
	{
		Player *player = &AllPlayers[i];
		if (player->Id == -1)
			continue;

		// Gun timer
		if (player->AllGunsInInventory[player->currentGunInInventory] < GunCount && player->GunWaitCount < AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].fireRate)
			player->GunWaitCount++;

		if (player->AllGunsInInventory[player->currentGunInInventory] < GunCount && player->GunReloadWaitCount < AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].ReloadTime && player->isReloading)
			player->GunReloadWaitCount++;

		if (player->GunReloadWaitCount == AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].ReloadTime && player->isReloading)
		{
			player->isReloading = false;
			ReloadGun(i);
		}

		if (player->IsDead)
		{
			if (i == 0)
				Hit = -1;

			player->justCheking = false;
			player->ScanFinished = true;
			player->target = -1;
			player->lastSeenTarget = -1;
		}
		else
		{
			// Continue to check raycast if scan is not finished
			if (!player->ScanFinished && CheckWallHit(i) == -1)
			{
				if (i == 0)
					Hit = -1;
				player->ScanFinished = true;

				// For AI
				if (player->justCheking)
				{
					printf("lose Target 1");
					player->justCheking = false;
					player->target = -1;

					if (player->lastSeenTarget != -1)
					{
						int nearestWaypoint = getNearestWaypoint(AllPlayers[player->lastSeenTarget].xPos, AllPlayers[player->lastSeenTarget].yPos, AllPlayers[player->lastSeenTarget].zPos);
						StartChecking(i, nearestWaypoint);
					}
					else if (player->PathCount == 0)
					{
						StartChecking(i, random() % 57);
					}
					player->lastSeenTarget = -1;

					// TODO get a random one or to a nearest last seen player position waypoint
				}
			}
			else if (!player->ScanFinished)
			{
				player->OldStopAt = player->StopAt;
				player->StopAt += 30;

				if (player->StopAt == 120 + 30)
				{
					player->ScanFinished = true;

					// For AI
					if (player->justCheking)
					{
						printf("lose Target 2");
						player->justCheking = false;
						player->target = -1;

						if (player->lastSeenTarget != -1)
						{
							int nearestWaypoint = getNearestWaypoint(AllPlayers[player->lastSeenTarget].xPos, AllPlayers[player->lastSeenTarget].yPos, AllPlayers[player->lastSeenTarget].zPos);
							StartChecking(i, nearestWaypoint);
						}
						else if (player->PathCount == 0)
						{
							StartChecking(i, random() % 57);
						}
						player->lastSeenTarget = -1;

						// TODO get a random one or to a nearest last seen player position waypoint
					}
				}
			}
		}
	}

	if (localPlayer->IsCounter != -1 && !localPlayer->IsDead && RoundState != 0)
	{
		// Gun shoot
		if ((localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] < GunCount && ((keys & KEY_L && AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].holdFireButton) || (keysdown & KEY_L && !AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].holdFireButton)) && localPlayer->GunWaitCount >= AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].fireRate))
		{
			if (((localPlayer->currentGunInInventory == 1 || localPlayer->currentGunInInventory == 2) && localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount > 0) || AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].isKnife)
			{
				if (!AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].isKnife)
				{
					localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount--;
					// Gun sound
					PlayBasicSound(AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].gunSound);
				}

				// Reset gun timer
				localPlayer->GunWaitCount = 0;

				// Set gun sprite position

				GunXRecoil = GunMaxRecoil;
				GunYRecoil = GunMaxRecoil;

				ShowMuzzle = 3;

				AllPlayers[0].startXPosRaycast = AllPlayers[0].xPos;
				AllPlayers[0].startYPosRaycast = AllPlayers[0].yPos;
				AllPlayers[0].startZPosRaycast = AllPlayers[0].zPos;

				AllPlayers[0].startXRotRaycast = x;
				AllPlayers[0].startYRotRaycast = y;
				AllPlayers[0].startZRotRaycast = z;
				AllPlayers[0].startGunIdRaycast = AllPlayers[0].AllGunsInInventory[AllPlayers[0].currentGunInInventory];

				// Raycast
				Hit = Raycast(0);

				// PhysicalGrenade *newGrenade = CreateGrenade(0);
				// lanchGrenade(newGrenade, x, y, z, AllPlayers[0].PlayerModel->x, AllPlayers[0].PlayerModel->y, AllPlayers[0].PlayerModel->z);

				setRumble(true);
				RumbleTimer = 1;

				// Send shoot on network
				SendShoot = true;
			}
			else
			{
				// ReloadGun(0);
				startReloadGun(0);
				// AllPlayers[0].GunReloadWaitCount = AllGuns[AllPlayers[0].AllGunsInInventory[AllPlayers[0].currentGunInInventory]].ReloadTime;
				// AllPlayers[0].isReloading = true;
			}
		}
		else if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] >= GunCount + shopGrenadeCount && (keys & KEY_L))
		{
			if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] == GunCount + shopGrenadeCount && CanPutBomb && RoundState == 1 && !BombPlanted && (CanJumpReal == 0 || CanJump > 0))
			{
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
					BombPosition.x = localPlayer->xPos;
					BombPosition.y = localPlayer->yPos - 0.845;
					BombPosition.z = localPlayer->zPos;
					BombPosition.r = localPlayer->Angle;

					BombPlanted = true;
					if (applyRules)
					{
						bombSet = true;
						PartyMinutes = allPartyModes[currentPartyMode].bombWaitingMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						localPlayer->haveBomb = false;
						SetGunInInventory(-1, 8);
						NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
						NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
						Model[7]->rz = BombPosition.r;
						bombBipTimer = 120;
						SetBombDefuseZone(BombPosition.x, BombPosition.z, 2, 2, &bombDefuseZone);
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
		else if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] >= GunCount && (keysdown & KEY_L))
		{
			PhysicalGrenade *newGrenade = CreateGrenade(localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]);
			lanchGrenade(newGrenade, x, y, z, localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z);

			localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] = -1;
			ChangeGunInInventoryForLocalPlayer(1);

			SendGrenade = true;
		}
		else if (keys & KEY_SELECT && canDefuseBomb && RoundState == 1 && !BombDefused && BombPlanted && localPlayer->IsCounter)
		{
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

					addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney, 0);
					addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, 1);

					showPartyEventText(3);
					PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
					PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
					RoundState = 2;
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
			if (!BombDefused && BombPlanted) // Set timer
				localPlayer->bombTimer = bombDefuseTime;
			else if (!BombPlanted) // Set timer
				localPlayer->bombTimer = bombPlantingTime;
		}

		if (keysdown & KEY_START)
		{
			isShowingMap = !isShowingMap;
		}
		/*else
		{
			isShowingMap = false;
		}*/

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
		if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] < GunCount)
			CurrentSpeed = AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].WalkSpeed;

		if (RoundState != 0 && !localPlayer->IsDead)
			MovePlayer(CurrentSpeed, xWithoutY, zWithoutY, keys, &NeedBobbing);

		// Gun headbobing
		if (NeedBobbing && (CanJumpReal == 0 || CanJump > 0))
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
				int FirstFound = -1;
				for (int i = 1; i < MaxPlayer; i++)
				{
					Player *player = &AllPlayers[i];
					if (!player->IsDead && player->Id != -1 && (allPartyModes[currentPartyMode].canSeeOtherTeamView || player->IsCounter == localPlayer->IsCounter))
					{
						if (FirstFound == -1)
							FirstFound = i;

						if (i > CurrentCameraPlayer)
						{
							CurrentCameraPlayer = i;
							UpdateGunTexture();
							Found = true;
							break;
						}
					}
				}

				if (!Found && FirstFound != -1)
				{
					CurrentCameraPlayer = FirstFound;
					UpdateGunTexture();
				}
			}
			else if (keysdown & KEY_RIGHT)
			{
				bool Found = false;
				int FirstFound = -1;
				for (int i = MaxPlayer - 1; i > 0; i--)
				{
					Player *player = &AllPlayers[i];
					if (!player->IsDead && player->Id != -1 && (allPartyModes[currentPartyMode].canSeeOtherTeamView || player->IsCounter == localPlayer->IsCounter))
					{
						if (FirstFound == -1)
							FirstFound = i;

						if (i < CurrentCameraPlayer)
						{
							CurrentCameraPlayer = i;
							UpdateGunTexture();
							Found = true;
							break;
						}
					}
				}

				if (!Found && FirstFound != -1)
				{
					CurrentCameraPlayer = FirstFound;
					UpdateGunTexture();
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

		localPlayer->PlayerPhysic->xspeed = 0;
		localPlayer->PlayerPhysic->yspeed = 0;
		localPlayer->PlayerPhysic->zspeed = 0;
	}

	if (applyRules)
	{
		for (int i = 1; i < MaxPlayer; i++)
		{
			Player *player = &AllPlayers[i];
			if (player->PathCount != 0)
			{
				player->BobbingOffset += BobbingSpeed;

				// Do sound every half bobbing cycle
				if (!player->HasBobbed && player->BobbingOffset >= M_PI)
				{
					int Panning, Volume;
					GetPanning(player->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);
					DoStepSound(Volume, Panning, i);
					player->HasBobbed = true;
				}
				else if (player->BobbingOffset >= M_TWOPI)
				{
					int Panning, Volume;
					GetPanning(player->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);
					player->BobbingOffset = 0;
					DoStepSound(Volume, Panning, i);
					player->HasBobbed = false;
				}
			}

			if (player->isPlantingBomb)
			{
				if (player->IsCounter == 0)
				{
					// On bomb planting make a sound
					if (player->bombTimer == bombPlantingTime)
					{
						int Panning, Volume;
						GetPanning(player->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);
						Play3DSound(SFX_BOMBPLANTING, Volume, Panning);
					}
					player->bombTimer--;
					// Set bomb position when planted
					if (player->bombTimer == 0)
					{
						CalculatePlayerPosition(i);
						BombPosition.x = player->xPos;
						BombPosition.y = player->yPos - 0.845;
						BombPosition.z = player->zPos;
						BombPosition.r = player->Angle;

						BombPlanted = true;
						bombSet = true;
						player->isPlantingBomb = false;
						bombPlantedAt = player->LastWayPoint;
						PartyMinutes = allPartyModes[currentPartyMode].bombWaitingMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						player->haveBomb = false;
						SetGunInInventoryForNonLocalPlayer(i, -1, 8);
						NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
						NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
						Model[7]->rz = BombPosition.r;
						bombBipTimer = 120;
						SetBombDefuseZone(BombPosition.x, BombPosition.z, 2, 2, &bombDefuseZone);
						showPartyEventText(2);
						SetRandomDefuser();
					}
				}
				else
				{
					// On bomb defuse make a sound
					if (player->bombTimer == bombDefuseTime)
					{
						int Panning, Volume;
						GetPanning(player->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);
						Play3DSound(SFX_BOMBPLANTING, Volume, Panning);
					}
					player->bombTimer--;
					// Set bomb when defused
					if (player->bombTimer == 0)
					{
						BombDefused = true;
						player->isPlantingBomb = false;
						currentDefuserIndex = -1;
						CounterScore++;

						if (LoseCountTerrorists > 0)
							LoseCountTerrorists--;

						if (LoseCountCounterTerrorists < 4)
							LoseCountCounterTerrorists++;

						addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney, 0);
						addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, 1);

						showPartyEventText(3);
						PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
						RoundState = 2;
						CheckAfterRound();
					}
				}
			}
		}

		changeSecondMTimer--;
		if (changeSecondMTimer == 0)
		{
			PartySeconds--;

			if (PartySeconds == -1)
			{
				PartyMinutes--;
				PartySeconds = 59;

				if (PartyMinutes == -1)
				{
					if (!PartyStarted)
					{
						PartyStarted = true;
						PartyMinutes = allPartyModes[currentPartyMode].startRoundMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].startRoundSecondsDuration;
						RoundState = 0;

						for (int i = 0; i < MaxPlayer; i++)
						{
							resetPlayer(i);
							AllPlayers[i].Money = allPartyModes[currentPartyMode].startMoney;
							AllPlayers[i].KillCount = 0;
							AllPlayers[i].DeathCount = 0;
						}

						if (currentMenu == 2)
							UpdateBottomScreenOneFrame += 8;

						checkShopForBot();
						setBombForARandomPlayer();
						setPlayersPositionAtSpawns();
						setNewRoundHandWeapon();
					}
					else if (RoundState == 0)
					{
						PartyMinutes = allPartyModes[currentPartyMode].roundMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].roundSecondsDuration;
						RoundState = 1;
					}
					else if (RoundState == 1)
					{
						PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
						RoundState = 2;

						if (bombSet)
						{
							TerroristsScore++;

							if (LoseCountCounterTerrorists > 0)
								LoseCountCounterTerrorists--;

							if (LoseCountTerrorists < 4)
								LoseCountTerrorists++;

							addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, 0);
							addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountTerrorists, 1);

							showPartyEventText(0);
						}
						else
						{
							CounterScore++;

							if (LoseCountTerrorists > 0)
								LoseCountTerrorists--;

							if (LoseCountCounterTerrorists < 4)
								LoseCountCounterTerrorists++;

							if (!allPartyModes[currentPartyMode].noMoneyOnTimeEnd)
							{
								addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney, 0);
							}
							addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, 1);

							showPartyEventText(1);
						}
						CheckAfterRound();
					}
					else if (RoundState == 2)
					{
						PartyMinutes = allPartyModes[currentPartyMode].startRoundMinutesDuration;
						PartySeconds = allPartyModes[currentPartyMode].startRoundSecondsDuration;
						RoundState = 0;

						bombSet = false;
						BombPlanted = false;
						currentDefuserIndex = -1;
						for (int i = 0; i < MaxPlayer; i++)
						{
							resetPlayer(i);
						}
						checkShopForBot();
						setBombForARandomPlayer();
						setPlayersPositionAtSpawns();
						setNewRoundHandWeapon();
					}
					else if (RoundState == 3)
					{
					}
				}
			}
			changeSecondMTimer = 60;
		}
	}

	// Make bomb sounds
	if (BombPlanted && !BombDefused)
	{
		if (bombBipTimer > 0)
			bombBipTimer--;

		if (bombBipTimer == 0 && PartySeconds > 0 && !BombWillExplose)
		{
			// make bib sound
			int Panning, Volume;
			GetPanningByPosition(&Panning, &Volume, AllPlayers, BombPosition, xWithoutYForAudio, zWithoutYForAudio, 0.15);
			Play3DSound(SFX_BOMBBIP, Volume, Panning);
			bombBipTimer = (PartySeconds / 40.0) * 120.0 + 5;
		}
		else if (bombBipTimer == 0 && PartySeconds == 0 && !BombWillExplose)
		{
			// Make detonate sound
			int Panning, Volume;
			GetPanningByPosition(&Panning, &Volume, AllPlayers, BombPosition, xWithoutYForAudio, zWithoutYForAudio, 0.15);
			Play3DSound(SFX_DETONATE, Volume, Panning);
			bombBipTimer = 120;
			BombWillExplose = true;
		}
		else if (bombBipTimer == 0 /*&& PartySeconds == 0*/ && BombWillExplose) // TODO Check bomb explosion online without PartySeconds == 0
		{
			// Make explosion sound
			PlayBasicSound(SFX_BOMBEXPLODE);
			BombPlanted = false;
			IsExplode = true;

			if (applyRules)
			{
				for (int i = 0; i < MaxPlayer; i++)
				{
					Player *player = &AllPlayers[i];
					if (player->IsDead || player->Id == -1)
						continue;

					float Distance = (float)sqrt(pow(player->PlayerModel->x - BombPosition.x * 4096.0, 2.0) + pow(player->PlayerModel->y - BombPosition.y * 4096.0, 2.0) + pow(player->PlayerModel->z - BombPosition.z * 4096.0, 2.0)) / 8096.0;
					if (Distance > 21)
						Distance = 0;

					if (Distance > 0)
					{
						int newHealh = player->Health - (int)map(Distance, 0.3, 21, 200, 0);
						setPlayerHealth(i, newHealh);
						checkAfterDamage(-1, i, false);
					}
				}
			}
		}
	}

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
	}

	// Loop using "AllPlayers" array for updating non local player (online players or bots) position smoothly
	SetOnlinelPlayersPositions();

	if (RoundState != 0)
	{
		checkForPlayer();

		checkTarget();
	}
}

void StopReloading(int playerIndx)
{
	AllPlayers[playerIndx].isReloading = false;
}

void startReloadGun(int playerIndx)
{
	Player *player = &AllPlayers[playerIndx];
	if (player->currentGunInInventory == 1 || player->currentGunInInventory == 2)
	{
		if (!player->isReloading && AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].MagazineCapacity != player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount && player->AllAmmoMagazine[player->currentGunInInventory - 1].TotalAmmoCount != 0)
		{
			player->GunReloadWaitCount = 0;
			player->isReloading = true;
		}
	}
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

	// Update physics and animations
	if (localPlayer->Id != -1)
		NE_PhysicsUpdate(localPlayer->PlayerPhysic);
	NE_WaitForVBL(NE_UPDATE_ANIMATIONS | NE_CAN_SKIP_VBL);
}

void buyGun()
{
	if (ShopCategory < 4 && AllGuns[GetSelectedGunShop()].Price <= localPlayer->Money)
	{
		localPlayer->Money -= AllGuns[GetSelectedGunShop()].Price;
		if (ShopCategory == 0)
			SetGunInInventory(GetSelectedGunShop(), 1);
		else
			SetGunInInventory(GetSelectedGunShop(), 2);
	}
	else if (ShopCategory == 5 && AllGrenades[GetSelectedGunShop() - GunCount].Price <= localPlayer->Money)
	{
		localPlayer->Money -= AllGrenades[GetSelectedGunShop() - GunCount].Price;
	}
}

void setBombForARandomPlayer()
{
	int TerroristCount = 0;

	for (int i = 0; i < MaxPlayer; i++)
	{
		SetGunInInventoryForNonLocalPlayer(i, -1, 8);
		AllPlayers[i].haveBomb = false;
		if (AllPlayers[i].Id != -1 && AllPlayers[i].IsCounter == 0)
		{
			// AllPlayers[i].haveBomb = true; /////////////////////////////////////////////////////////////////////////////REMOVE
			TerroristCount++;
		}
	}

	// return; /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////TODO
	if (localPlayer->IsCounter == 0)
	{
		localPlayer->haveBomb = true;
		SetGunInInventory(28, 8);
		return;
	}

	int giveBombTo = rand() % TerroristCount;
	TerroristCount = 0;

	for (int i = 1; i < MaxPlayer; i++)
	{
		Player *player = &AllPlayers[i];
		if (player->Id != -1 && player->IsCounter == 0)
		{
			if (giveBombTo == TerroristCount)
			{
				player->haveBomb = true;
				SetGunInInventoryForNonLocalPlayer(i, 28, 8);
				break;
			}
			TerroristCount++;
		}
	}
}

void addMoneyToTeam(int Money, int Team)
{
	for (int i = 0; i < MaxPlayer; i++)
	{
		Player *player = &AllPlayers[i];
		if (player->IsCounter == Team)
		{
			player->Money += Money;
			if (player->Money > allPartyModes[currentPartyMode].maxMoney)
				player->Money = allPartyModes[currentPartyMode].maxMoney;
		}
	}
}

void resetPlayer(int index)
{
	setPlayerHealth(index, 100);
	Player *player = &AllPlayers[index];

	if (allPartyModes[currentPartyMode].spawnWithArmor)
	{
		player->armor = 100;
		player->haveHeadset = true;
	}

	StopReloading(index);

	if (player->IsDead || (allPartyModes[currentPartyMode].middlePartyTeamSwap && TerroristsScore + CounterScore == floor(allPartyModes[currentPartyMode].maxRound / 2.0)) || TerroristsScore + CounterScore == 0)
	{
		if (player->IsCounter == 0)
			player->AllGunsInInventory[1] = DEFAULTTERRORISTGUN;
		else if (player->IsCounter == 1)
			player->AllGunsInInventory[1] = DEFAULTCOUNTERTERRORISTGUN;

		for (int i = 2; i < inventoryCapacity - 1; i++)
		{
			player->AllGunsInInventory[i] = -1;
		}
	}

	player->IsDead = false;
	// player->currentGunInInventory = 1;
	ResetGunsAmmo(index);
	if (index == 0)
	{
		CurrentCameraPlayer = 0;
		UpdateGunTexture();
	}
	else
	{
		player->PathCount = 0;
		player->target = -1;
		player->lastSeenTarget = -1;
	}
}

void setNewRoundHandWeapon()
{
	for (int index = 0; index < MaxPlayer; index++)
	{
		Player *player = &AllPlayers[index];
		if (index == 0 && player->haveBomb)
		{
			player->currentGunInInventory = 8;
		}
		else if (player->AllGunsInInventory[player->currentGunInInventory] == -1)
		{
			player->currentGunInInventory = 1;
			if (player->AllGunsInInventory[player->currentGunInInventory] == -1)
			{
				player->currentGunInInventory = 0;
			}
		}

		if (index == 0)
			UpdateGunTexture();
	}
}

void setPlayerHealth(int playerIndex, int health)
{
	Player *player = &AllPlayers[playerIndex];
	player->Health = health;

	if (player->Health <= 0)
	{
		if (!applyRules)
		{
			player->Health = 0;
			player->IsDead = TRUE;
		}

		if (playerIndex == 0)
			PlayBasicSound(SFX_DEATH);
		else
		{
			int Panning, Volume;
			GetPanning(player->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);
			Play3DSound(SFX_DEATH, Volume, Panning);
		}
		// If died player is the local player
		if (playerIndex == 0)
		{
			// Hide crosshair
			NE_SpriteVisible(TopScreenSprites[0], false);
		}
	}
	else if (player->Health == 100)
	{
		if (playerIndex == 0)
		{
			NE_SpriteVisible(TopScreenSprites[0], true);
			deathCameraAnimation = 0;
			deathCameraYOffset = 0;
			redHealthTextCounter = 0;
		}

		iprintf("\nRespawn");
		player->IsDead = FALSE;
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
	player->xPos = player->PlayerModel->x / 4096.0;
	player->yPos = player->PlayerModel->y / 4096.0;
	player->zPos = player->PlayerModel->z / 4096.0;
}

void checkAfterDamage(int shooterPlayerIndex, int hittedPlayerIndex, bool CheckScore)
{
	Player *HittedClient = &AllPlayers[hittedPlayerIndex];
	Player *killerClient = &AllPlayers[shooterPlayerIndex];

	if (HittedClient->Health <= 0 && !HittedClient->IsDead)
	{
		HittedClient->Health = 0;
		// Set client has dead
		HittedClient->IsDead = true;
		if (&AllPlayers[currentDefuserIndex] == HittedClient)
			currentDefuserIndex = -1;

		HittedClient->DeathCount++;

		if (killerClient->target == HittedClient->Id)
		{
			killerClient->target = -1;
			killerClient->lastSeenTarget = -1;
		}
		if (HittedClient->isAi)
		{
			HittedClient->PathCount = 0;
		}

		if (shooterPlayerIndex != -1)
		{
			showKillText(shooterPlayerIndex, hittedPlayerIndex);
			if (allPartyModes[currentPartyMode].teamDamage && HittedClient->IsCounter == killerClient->IsCounter)
			{
				killerClient->KillCount--;
				killerClient->Money -= allPartyModes[currentPartyMode].killPenalties;
				if (killerClient->Money < 0)
					killerClient->Money = 0;
			}
			else
			{
				killerClient->KillCount++;
				// if (AllShopElements[killerClient.AllGunsInInventory[killerClient.currentGunInInventory]] is Gun)
				// AddMoneyTo(killerClient, ((Gun)AllShopElements[killerClient.AllGunsInInventory[killerClient.currentGunInInventory]]).KillMoneyBonus[killerClient.ClientParty.PartyType]);
			}
		}

		if (currentMenu == 2)
			UpdateBottomScreenOneFrame += 8;

		if (!CheckScore)
			return;

		if (RoundState == 1)
		{
			// Check if all a team is dead
			int CounterDeadCount = 0, TerroristDeadCount = 0, CounterTerroristsCount = 0, TerroristsCount = 0;

			CheckTeamDeathCount(&TerroristsCount, &CounterTerroristsCount, &TerroristDeadCount, &CounterDeadCount);

			if (CounterDeadCount == CounterTerroristsCount)
			{
				TerroristsScore++;

				if (LoseCountTerrorists > 0)
					LoseCountTerrorists--;

				if (LoseCountCounterTerrorists < 4)
					LoseCountCounterTerrorists++;

				addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundMoney, 0);
				addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountCounterTerrorists, 1);

				// Set round to finished round state
				PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
				PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
				RoundState = 2;
				showPartyEventText(0);
				CheckAfterRound();
			}
			else if (TerroristDeadCount == TerroristsCount && !bombSet)
			{
				CounterScore++;

				if (LoseCountCounterTerrorists > 0)
					LoseCountCounterTerrorists--;

				if (LoseCountTerrorists < 4)
					LoseCountTerrorists++;

				addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundMoney, 1);
				addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountTerrorists, 0);

				// Set round to finished round state
				PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
				PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
				RoundState = 2;
				showPartyEventText(1);
				CheckAfterRound();
			}
		}
		else if (RoundState == -1)
		{
			HittedClient->RespawnTimer = allPartyModes[currentPartyMode].trainingRespawnSecondsDuration * 60;
			HittedClient->NeedRespawn = true;
		}
	}
}

void CheckAfterRound()
{
	// Check if a team won
	if (TerroristsScore == floor(allPartyModes[currentPartyMode].maxRound / 2.0) + 1)
	{
		PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
		PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
		RoundState = 3;
	}
	else if (CounterScore == floor(allPartyModes[currentPartyMode].maxRound / 2.0) + 1)
	{
		PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
		PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
		RoundState = 3;
	}
	else if (TerroristsScore + CounterScore == allPartyModes[currentPartyMode].maxRound)
	{
		PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
		PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
		RoundState = 3;
	}
	else if (allPartyModes[currentPartyMode].middlePartyTeamSwap && TerroristsScore + CounterScore == floor(allPartyModes[currentPartyMode].maxRound / 2.0)) // Or check team swap
	{
		for (int i = 0; i < MaxPlayer; i++)
		{
			Player *player = &AllPlayers[i];
			if (player->IsCounter == 0)
			{
				player->IsCounter = 1;
			}
			else if (player->IsCounter == 1)
			{
				player->IsCounter = 0;
			}

			player->Money = allPartyModes[currentPartyMode].startMoney;
		}
		LoseCountCounterTerrorists = 0;
		LoseCountTerrorists = 0;
		int TempsScoreTerrorists = TerroristsScore;
		TerroristsScore = CounterScore;
		CounterScore = TempsScoreTerrorists;
	}
}

void CheckTeamDeathCount(int *TerroristsCount, int *CounterTerroristsCount, int *TerroristDeadCount, int *CounterDeadCount)
{
	*CounterDeadCount = 0;
	*TerroristDeadCount = 0;
	*TerroristsCount = 0;
	*CounterTerroristsCount = 0;

	for (int iPlayer = 0; iPlayer < MaxPlayer; iPlayer++)
	{
		Player *player = &AllPlayers[iPlayer];
		if (player->Id == -1)
			continue;

		if (player->IsCounter == 1)
		{
			if (player->IsDead) // If a counter is dead, add one to counter dead count
				*CounterDeadCount = *CounterDeadCount + 1;

			*CounterTerroristsCount = *CounterTerroristsCount + 1;
		}
		else if (player->IsCounter == 0) // If a terrorist is dead, add one to terrorist dead count
		{
			if (player->IsDead)
				*TerroristDeadCount = *TerroristDeadCount + 1;

			*TerroristsCount = *TerroristsCount + 1;
		}
	}
}

void makeHit(int hitBy, int playerHit)
{
	int hitType = 0;
	int Damage = AllGuns[AllPlayers[hitBy].startGunIdRaycast].Damage;

	Player *hittedPlayer = &AllPlayers[playerHit];
	Player *shooterPlayer = &AllPlayers[hitBy];

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
	if ((hitType == 0 && hittedPlayer->armor != 0) || (hitType == 1 && hittedPlayer->haveHeadset))
	{
		int oldDamage = Damage;
		Damage *= AllGuns[shooterPlayer->startGunIdRaycast].penetration / 100.0;
		if (hitType == 1)
		{
			hittedPlayer->haveHeadset = false;
			makeHeadSetSound = true;
		}
		else
		{
			hittedPlayer->armor -= oldDamage - Damage;
			if (hittedPlayer->armor < 0)
				hittedPlayer->armor = 0;
		}
	}

	int newHealth = hittedPlayer->Health - Damage;

	setPlayerHealth(playerHit, newHealth);
	checkAfterDamage(hitBy, playerHit, true);

	/*if (AllPlayers[playerHit].Health == 0)
	{
		AllPlayers[0].KillCount++;
		AllPlayers[playerHit].DeathCount++;
		if (currentMenu == 2)
			UpdateBottomScreenOneFrame += 8;
	}*/

	int Panning, Volume;
	GetPanning(hittedPlayer->Id, &Panning, &Volume, AllPlayers, xWithoutYForAudio, zWithoutYForAudio, 0.10);

	if (hitType == 0)
		Play3DSound(SFX_FLESH_IMPACT, Volume, Panning); // Check with kevlar
	else if (hitType == 1)
	{
		if (!makeHeadSetSound)
			Play3DSound(SFX_HEADSHOT1, Volume, Panning);
		// else
		// Play3DSound(SFX, Volume, Panning);
	}
	else if (hitType == 2)
		Play3DSound(SFX_FLESH_IMPACT, Volume, Panning);
	else
		Play3DSound(SFX_KNIFE_HIT_PLAYER, Volume, Panning);
}

// Add new player to party
void AddNewPlayer(int NewId, bool IsLocalPlayer, bool isAI)
{
	for (int i = 1; i < MaxPlayer; i++)
	{
		if (IsLocalPlayer)
			i = 0;

		Player *player = &AllPlayers[i];

		// Check for an empty player slot
		if (player->Id == -1)
		{
			iprintf("\nCreating player Id: %d", NewId);
			// Create player (3d model, physics, animations)
			// NE_ModelAnimSetFrame(AllPlayers[i].PlayerModel, 2);

			player->PlayerCollisionBox.xSize = 0.35;
			player->PlayerCollisionBox.ySize = 0.9;
			player->PlayerCollisionBox.zSize = 0.35;

			player->currentGunInInventory = 1;
			// Init inventory
			player->AllGunsInInventory[0] = 0;
			player->AllGunsInInventory[1] = 1;
			for (int i2 = 2; i2 < inventoryCapacity; i2++)
			{
				player->AllGunsInInventory[i2] = -1;
			}
			ResetGunsAmmo(i);

			if (IsLocalPlayer)
			{
				strcpy(player->name, "LocalPlayer");
				player->PlayerModel = NE_ModelCreate(NE_Static);
				player->PlayerPhysic = NE_PhysicsCreate(NE_BoundingBox);
				NE_PhysicsSetModel(player->PlayerPhysic, (void *)player->PlayerModel); // Physics object and Model assigned to it
				NE_PhysicsEnable(player->PlayerPhysic, IsLocalPlayer);
				NE_PhysicsSetGravity(player->PlayerPhysic, 0.0065);
				NE_PhysicsSetSize(player->PlayerPhysic, player->PlayerCollisionBox.xSize * 2.0, player->PlayerCollisionBox.ySize * 2.0, player->PlayerCollisionBox.zSize * 2.0);
				NE_PhysicsSetFriction(player->PlayerPhysic, 1);
				NE_PhysicsOnCollision(player->PlayerPhysic, NE_ColBounce);
				NE_PhysicsSetBounceEnergy(player->PlayerPhysic, 0);
				UpdateGunTexture();
			}
			else
			{
				// player->PlayerModel = NE_ModelCreate(NE_Animated);
				player->PlayerModel = NE_ModelCreate(NE_Static);
				// NE_ModelLoadNEA(player->PlayerModel, (u32 *)playerAnimNea_bin);
				NE_ModelLoadStaticMesh(player->PlayerModel, (u32 *)GIGNNew_bin);
				// NE_ModelLoadNEA(player->PlayerModel, (u32 *)GIGNAnimNea_bin);
				NE_ModelSetMaterial(player->PlayerModel, PlayerMaterial);
				NE_ModelScaleI(player->PlayerModel, 2048, 2048, 2048); // 2048 <- 4096 * 0.5 ANIMATED VERSION
				// NE_ModelScaleI(player->PlayerModel, 700, 700, 700); // 2048 <- 4096 * 0.5 STATIC VERSION
				//  NE_ModelAnimInterpolate(player->PlayerModel, false);
				//  NE_ModelAnimStart(player->PlayerModel, 0, 0, 5, NE_ANIM_LOOP, 3);

				if (isAI)
				{
					int botName;
					do
					{
						botName = rand() % 10;
					} while (botsNamesTaken[botName]);
					botsNamesTaken[botName] = true;
					strcpy(player->name, botsNames[botName]);
				}
				else
				{
					strcpy(player->name, "Player");
				}
			}

			NE_ModelSetCoord(player->PlayerModel, 0, 0 /* + i * 5*/, -2);

			player->Id = NewId;
			player->Health = 100;
			player->isAi = isAI;

			// Init raycasting values
			player->StopAt = 30;
			player->ScanFinished = true;
			player->target = -1;
			player->lastSeenTarget = -1;
			// player->Money = allPartyModes[currentPartyMode].startMoney; //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// TODO
			player->Money = 8000;

			if (allPartyModes[currentPartyMode].spawnWithArmor)
			{
				player->armor = 100;
				player->haveHeadset = true;
			}

			PlayerCount++;
			return;
		}
	}

	// If code running here, this is not normal lol XD
	iprintf("\nNo player slot available");
}