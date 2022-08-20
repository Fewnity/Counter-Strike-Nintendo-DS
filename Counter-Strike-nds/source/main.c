// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "collisions.h"
#include "raycast.h"
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
#include "ai/data/ai_data.h"
#include "map.h"
#include "party.h"
#include "keyboard.h"
#include "draw3d.h"
#include "saveManager.h"
#include "input.h"
#include "player.h"
#include "tutorial.h"
#include "stats.h"
#include "camera.h"

//
//////Level
// Whole level 3D models and physics
NE_Model *Model[13]; ////////////REMOVE OR CHANGE THIS
// Models for wall flashs for guns
NE_Model *flashModels[FLASH_MODELS_COUNT];

// Materials
NE_Material *GroundMaterial = NULL;
NE_Material *GroundMaterialShadowed = NULL;

//
//////Players
Player AllPlayers[MaxPlayer];
// Player values
int PlayerCount = 0;
// Old local player position
float OldxPos, OldyPos, OldzPos = 0;
// Timer for the player's jump
int CanJump = 0;
// CanJumpRealTimer is used wait a little bit of time before the player can jump
int CanJumpRealTimer = 2;

// Number of frame when the player is in the air
int frameCountDuringAir = 0;
// Is the player on stairs?
bool isOnStairs = false;
// Is the player descending stairs?
bool isInDownStairs = false;
// Ask jump from UI button
bool NeedJump = false;

// Head bobing speed
float BobbingSpeed = 0.07;

// Materials
NE_Material *PlayerMaterial = NULL;
NE_Material *PlayerMaterialTerrorist = NULL;
NE_Material *PlayerShadowMaterial = NULL;

//////Guns
// Number if frame of showing the muzzle flash at screen
int ShowMuzzle = 0;
// Number of frame of showing the wall shit flash (it's a list because it's possible to have more than one at the same time)
int ShowWallHitFlash[FLASH_MODELS_COUNT] = {0, 0, 0, 0, 0, 0};
// Guns sprites positions
int rightGunX = 0;
int rightGunY = 0;
int leftGunX = 0;
int leftGunY = 0;

// All scopes data
Scope AllScopeLevels[2];
// Current scope level used by the player
int CurrentScopeLevel = 0;

// Gun recoil values
int GunMaxRecoil = 16;
int GunMinRecoil = 6;

//
//////Camera
NE_Camera *Camera = NULL;
// [-1;1] values, 3D position of a point in a sphere by using player and camera angle to get the point
float x = 0, y = 0, z = 0;
// [-1;1] values, 2D position of a point in a sphere by using player and camera angle to get the point
float xWithoutY = 0, zWithoutY = 0, xWithoutYForAudio = 0, zWithoutYForAudio = 0, xWithoutYForMap = 0, zWithoutYForMap = 0, xWithoutYForOcclusionSide1 = 0, zWithoutYForOcclusionSide1 = 0, xWithoutYForOcclusionSide2 = 0, zWithoutYForOcclusionSide2 = 0;
// camera view offset
float xOffset = 0, yOffset = 0;
// Camera angle
// float CameraAngleY = 128;

// Current player's view index for the camera
int CurrentCameraPlayer = 0;
// Need to update the sphere positions values of the camera
bool NeedUpdateViewRotation = false;

// Animations
#define DEATH_ANIMATION_SPEED 0.025
// [0 ; 1] progression of the death animation
float deathCameraAnimation = 0;
// Offset of the death animation camera
float deathCameraYOffset = 0;

// Raycasting
// Index of all hitted client (for raycasting)
int hittedClient[FLASH_MODELS_COUNT] = {NO_PLAYER, NO_PLAYER, NO_PLAYER, NO_PLAYER, NO_PLAYER, NO_PLAYER};

// TODO replace by Vector3Int array
// Positions of all wall hit flashs
int WallHitXPos[FLASH_MODELS_COUNT] = {0, 0, 0, 0, 0, 0};
int WallHitYPos[FLASH_MODELS_COUNT] = {0, 0, 0, 0, 0, 0};
int WallHitZPos[FLASH_MODELS_COUNT] = {0, 0, 0, 0, 0, 0};
// Distance of all hits
float hitDistance[FLASH_MODELS_COUNT] = {0, 0, 0, 0, 0, 0};

// Other
// Is the player is fully in a smoke area?
bool isInFullSmoke = false;

// Retain the team of the player to send to the server
int tempTeam = 0;

// All sprites used for the top screen
NE_Sprite *TopScreenSprites[2]; // 0 Map, 1 crosshair
// All sprites used for the bottom screen
NE_Sprite *BottomScreenSprites[1];
// All checkboxes used for the bottom screen
CheckBox AllCheckBoxs[CheckBoxCount];
// All sliders used for the bottom screen
Slider AllSliders[SliderCount];
// All buttons used for the bottom screen
Button AllButtons[ButtonCount];

// Timer to draw the event text
int textToShowTimer = 0;
// Timer to draw the kill event text
int KillTextShowTimer = 0;
// Selected gun in the shop
int SelectedGunShop = 0;

// Is 30 fps mode enabled with bottom screen refresh?
bool AlwaysUpdateBottomScreen = false;
// Ask to enable 30 fps mode with bottom screen refresh
bool NeedChangeScreen = false;
// Number of frame for 30 fps mode
int UpdateBottomScreenFrameCount = 0;
// Button to show count
int ButtonToShow = 0;
// Number of frame to show the health bar with red color
int redHealthTextCounter = 0;
// Number of frame for aiming activation
int doubleTapTimer = 0;

//
//////Party
// Current opened menu
int currentMenu = -1; // 0 Main menu //1 Map //2 Score //3 Shop categories //4 Settings //5 Quit //6 Shop
// Is party started?
bool PartyStarted = false;
// Need to apply game rules? (true in solo mode, false in online mode)
bool applyRules = false;
// Number of lose of terrorists team
int LoseCountTerrorists = 0;
// Number of lose of counter terrorists team
int LoseCountCounterTerrorists = 0;

// Timer to change party timer
int changeSecondTimer = 60;
int changeMinuteTimer = 60;

// Is the bomb set? TODO : maybe to replace by the existing var BombPlanted
bool bombSet = false;

//
//////Bomb
// Bomb bip timer frame count
int bombBipTimer = 120;
// Bomb position (w = angle)
Vector4 BombPosition;
// Is the bomb planted?
bool BombPlanted = false;
// Is the bomb defused?
bool BombDefused = false;
// Will the bomb explode?
bool BombWillExplode = false;
// Trigger area to defuse the bomb
CollisionBox2D bombDefuseZone;
// Bomb effect scale
int BombExplosionScale = 0;
// Is the bomb exploding?
bool IsExplode = false;

// Timer before disable the shop
int shopDisableTimer = SHOP_DISABLE_TIMER;
// Trigger for the shop zone
CollisionBoxF shopZone;
// Is the player in the shop zone
bool isInShopZone = false;

// Rumble pack timer
int RumbleTimer = 0;
// Is rumble enabled?
bool useRumble = false;

// Connection type (offline for solo)
enum connectionType Connection = UNSELECTED;

// Local player pointer
Player *localPlayer = &AllPlayers[0];

// Wait before doing some action (for example, save the party), we want to wait to avoid a screen refresh at the same time and cause a visual bug
int uiTimer = 0;
// Action to do after the ui timer
enum actionAfterUiTimer actionOfUiTimer = SAVE;

// Is the player using bomb? (planting or defusing)
bool isUsingBomb = false;
// Can the player change gun?
bool canChangeGun = true;
// Can the player shoot?
bool canShoot = true;

// How many players want to start the party (vote)
int playerWantToStart = 0;
// How many players are needed to start the party (vote)
int playerWantToStartLimite = 0;

/**
 * @brief Init the game (launch the engine, load textures, sounds, 3D models, init keyboard...)
 *
 * @return int Result
 */
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

	// Error debug handler for Nitro Engine
	NE_DebugSetHandler(error_handler);

	NE_SetFov(70);											  // Set FOV
	NE_ClippingPlanesSetI(floattof32(0.1), floattof32(90.0)); // Set render distance

	readKeys();

	initDebug();

	// Init fat file system
	if (!fatInitDefault())
	{
		// iprintf("Warning: fatInit failure!\n");
	}

	initInputs();

	// Init default player name
	strcpy(localPlayer->name, "Player");

	// Load save file
	Load();

	// Init rumble pack
	isRumbleInserted();

	/*if (!nitroFSInit(NULL))
	{
		iprintf("nitroFSInit failure!\n");
	}*/

	// Init sound system
	initSoundSystem();
	// Init keyboard
	initKeyboard();

	// Set bots names
	loadBotsNames();

	// Load guns/grenades/equipments data
	initGuns();
	initGrenades();
	initEquipments();

	// Load maps data
	LoadMapTextures();
	SetMapCameraPosition();
	AddAllSpawnPoints();
	SetMapNames();
	SetMapPartyMode();

	// Camera
	Camera = NE_CameraCreate();
	ForceUpdateLookRotation(localPlayer->cameraAngle);

	// Set start camera position to look the map
	setCameraMapPosition();

	initGraphics();

	// Load the default map
	LoadMap(currentMap);

	// load party modes
	AddAllPartyModes();

	// Set scope levels
	AllScopeLevels[0].scopeCount = 2;
	AllScopeLevels[0].fov[0] = 20;
	AllScopeLevels[0].fov[1] = 7;
	AllScopeLevels[0].Speed = 140;

	AllScopeLevels[1].scopeCount = 1;
	AllScopeLevels[1].fov[0] = 20;
	AllScopeLevels[1].Speed = 140;

	// Set all player ID to UNUSED
	for (int i = 0; i < MaxPlayer; i++)
	{
		AllPlayers[i].Id = UNUSED;
		AllPlayers[i].Team = SPECTATOR;
	}

	// Lights
	NE_LightSet(0, RGB15(31, 31, 31), -0.5, -0.5, -0.5);

	// Set background color
	NE_ClearColorSet(RGB15(17, 26, 29), 31, 63); // Blue sky

	if (isDebugMode)
	{
		// DEBUG show connection mode
		printf("A : Offline mode\n");				 // 0
		printf("B : %s\n", DEBUG_IP_1_STRING);		 // 1
		printf("Y : %s\n", ONLINE_SERVER_IP_STRING); // 2
		printf("X : %s\n", DEBUG_IP_2_STRING);		 // 3
	}

	// Check if we need to start the tutorial
	if (!tutorialDone)
	{
		// Load sounds
		loadSounds();
		isInTutorial = true;
		currentSelectionMap = TUTORIAL;
		StartSinglePlayer(2);
	}
	else
	{
		initMainMenu();
	}

	// Loop to start parties
	while (true)
	{
		checkStartGameLoop();
	}

	return 0;
}

/**
 * @brief Loop to check if the player want to start a party
 *
 */
void checkStartGameLoop()
{
	checkMusicSteaming();

	// Play without internet
	if (Connection == OFFLINE)
	{
		prepareParty(0);

		// Unload old map and load new one
		if (currentMap != currentSelectionMap)
		{
			UnLoadMap(currentMap);
			currentMap = currentSelectionMap;
			LoadMap(currentMap);
		}

		AddNewPlayer(0, true, false);

		// Add bots
		if (currentPartyMode != 2)
		{
			for (int i = 1; i < MaxPlayer; i++)
				AddNewPlayer(i, false, true);
		}

		if (isInTutorial)
		{
			AllPlayers[0].Team = TERRORISTS;
		}
		else
		{
			while (AllPlayers[0].Team == SPECTATOR)
			{
				readKeys();

				ReadTouchScreen(touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenFrameCount, &SendTeam, true);
				UpdateEngineNotInGame();

				if (Connection == UNSELECTED)
					return;
			}

			SetNeedChangeScreen(true);
		}

		initControllerMenu();

		// Set randomly players in a team

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
			UpdatePlayerTexture(i);
		}
		AllPlayers[2].Team = AllPlayers[1].Team;
		UpdatePlayerTexture(2);

		setPlayersPositionAtSpawns();

		checkShopForBots();

		while (Connection == OFFLINE)
		{
			// Play game code
			GameLoop();
			UpdateEngine();
		}
	}
	else if (Connection != UNSELECTED)
	{
		// FOR ONLINE
		JoinParty(JOIN_RANDOM_PARTY);
	}
	else
	{
		readKeys();
		// For debug, rapid party start
		if (isDebugMode && currentMenu == MAIN)
		{
			if ((keysdown & KEY_A) == KEY_A)
				Connection = OFFLINE;
			else if ((keysdown & KEY_B) == KEY_B)
				Connection = DEBUG_IP_1;
			else if ((keysdown & KEY_Y) == KEY_Y)
				Connection = ONLINE_SERVER_IP;
			else if ((keysdown & KEY_X) == KEY_X)
				Connection = DEBUG_IP_2;
		}
		ReadTouchScreen(touch, &NeedChangeScreen, false, &ButtonToShow, &UpdateBottomScreenFrameCount, &SendTeam, true);
		UpdateEngineNotInGame();
	}
}

/**
 * @brief Join an online party
 *
 * @param option Join option see JoinType enum in network.h
 */
void JoinParty(int option)
{
	if (Connection == UNSELECTED)
	{
		Connection = ONLINE_SERVER_IP;
	}
	// WIFI CODE
	prepareParty(1);

	// END WIFI CODE
	initNetwork(option);
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
	return UpdateBottomScreenFrameCount;
}

void SetUpdateBottomScreenOneFrame(int value)
{
	UpdateBottomScreenFrameCount = value;
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
	UpdateGunTexture();
}

int GetCurrentCameraPlayer()
{
	return CurrentCameraPlayer;
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
	localPlayer->cameraAngle = Value;
}

float GetCameraAngleY()
{
	return localPlayer->cameraAngle;
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

/**
 * @brief Reduce the rumble timer every frame
 *
 */
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

/**
 * @brief Reduce double tap timer every frame for aiming from the gamepad
 *
 */
void reduceDoubleTapTimer()
{
	// Reduce double tab timer
	if (doubleTapTimer > 0)
		doubleTapTimer--;
}

/**
 * @brief Game loop
 *
 */
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

	checkTakingBombZone(bombDefuseZone);

	// Check if the player is in the shop zone
	if (allPartyModes[currentPartyMode].limitedShopByZoneAndTimer)
		checkShopZone(); // Check
	else
		isInShopZone = true; // Enable shop zone every where

	UpdateGrenades();

	reduceDoubleTapTimer();

	// Check touchscreen input for menus
	ReadTouchScreen(touch, &NeedChangeScreen, &AlwaysUpdateBottomScreen, &ButtonToShow, &UpdateBottomScreenFrameCount, &SendTeam, false);

	if (isInTutorial)
		checkTutorial();

	// If local player is not a spectator
	if (localPlayer->Team != SPECTATOR)
	{
		// Change gun in inventory if needed
		if (isKeyDown(LEFT_GUN))
		{
			ChangeGunInInventoryForLocalPlayer(1);
		}
		else if (isKeyDown(RIGHT_GUN))
		{
			ChangeGunInInventoryForLocalPlayer(0);
		}

		// Check input and rotate player
		RotatePlayer(&NeedUpdateViewRotation, &SendPosition, &localPlayer->cameraAngle);

		// Update look rotation value
		if (NeedUpdateViewRotation || CurrentCameraPlayer != 0)
		{
			UpdateLookRotation(localPlayer->cameraAngle);
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

	// Check for each players
	for (int i = 0; i < MaxPlayer; i++)
	{
		Player *player = &AllPlayers[i];
		if (player->Id == UNUSED)
			continue;

		// Check if the playe is in a shadow
		if (player->inShadow)
		{
			// Reduce player light intensity
			if (player->lightCoef > 0.7)
				player->lightCoef -= 0.05;
		}
		else
		{
			// Increase player light intensity
			if (player->lightCoef < 1)
				player->lightCoef += 0.05;
		}

		// Respawn player if needed
		if (player->NeedRespawn)
		{
			// Reduce timer
			player->RespawnTimer--;
			// If timer is 0, respawn player
			if (player->RespawnTimer == 0)
			{
				player->NeedRespawn = false;
				setPlayerPositionAtSpawns(i);
				resetPlayer(i);
				CheckShopForBot(i);
			}
		}

		// Reduce invincibility timer
		if (player->invincibilityTimer > 0)
		{
			player->invincibilityTimer--;
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

	// Check if the player want to vote
	if (roundState == TRAINING && Connection != OFFLINE && PlayerCount != 1 && (keysdown & KEY_START))
	{
		SendVoteStartNow = true;
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
		if (localPlayer->position.y <= -5 && Connection == OFFLINE)
		{
			setPlayerPositionAtSpawns(0);
		}

		// Gun shoot
		if (canShoot && !localPlayer->isReloading && ((getPlayerCurrentGunIndex(localPlayer) < GunCount && ((isKey(FIRE_BUTTON) && getPlayerCurrentGun(localPlayer).holdFireButton) || (isKeyDown(FIRE_BUTTON) && !getPlayerCurrentGun(localPlayer).holdFireButton)) && localPlayer->GunWaitCount >= getPlayerCurrentGun(localPlayer).fireRate)) && !isUsingBomb)
		{
			if (((localPlayer->currentGunInInventory == 1 || localPlayer->currentGunInInventory == 2) && localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount > 0) || getPlayerCurrentGun(localPlayer).isKnife)
			{
				if (!getPlayerCurrentGun(localPlayer).isKnife)
				{
					localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount--;
					// Gun sound
					PlayBasicSound(getPlayerCurrentGun(localPlayer).gunSound);
				}

				// Reset gun timer
				localPlayer->GunWaitCount = 0;

				// Reduce aiming accuracy
				if (yOffset < 0.1)
					yOffset += (rand() % 2 + 2) / 100.0;
				else if (yOffset < 0.16)
					yOffset += (rand() % 2 + 2) / 150.0;
				else
					yOffset += (rand() % 2 + 2) / 200.0;

				speedAimingReCenter = 1;
				speedAimingReCenterTimer = 8;

				// Set gun sprite offset position
				setGunRecoil(localPlayer);

				// Do multiple raycast if needed
				for (int i = 0; i < getPlayerCurrentGun(localPlayer).bulletCountPerShoot; i++)
				{
					if (getPlayerCurrentGun(localPlayer).bulletCountPerShoot != 1)
					{
						float xShoot, yShoot, zShoot;
						float xOffset = ((rand() % 100) - 50) / 3.0;
						float yOffset2 = ((rand() % 100) - 50) / 3.0;
						UpdateLookRotationAI(localPlayer->cameraAngle + yOffset2, localPlayer->Angle + xOffset, &xShoot, &yShoot, &zShoot);
						setRaycastValues(localPlayer, xShoot, yShoot + yOffset, zShoot);
					}
					else
					{
						setRaycastValues(localPlayer, x, y + yOffset, z);
					}

					// Raycast
					hittedClient[i] = Raycast(0, i, &hitDistance[i]);
					if (hittedClient[i] != NO_PLAYER)
					{
						makeHit(0, hittedClient[i], hitDistance[i], i);

						// If the player shoots a friend, show the warning message
						if (allPartyModes[currentPartyMode].teamDamage && AllPlayers[hittedClient[i]].Team == localPlayer->Team)
							showShootFriendMessage = 240;
					}
				}

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

					if (applyRules)
					{
						bombSet = true;
						if (roundState == PLAYING)
						{
							PartyMinutes = allPartyModes[currentPartyMode].bombWaitingMinutesDuration;
							PartySeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;
						}
					}
					else
					{
						SendBombPlace = true;
					}

					localPlayer->haveBomb = false;
					SetGunInInventory(-1, 8);
					NE_ModelSetCoord(Model[7], BombPosition.x, BombPosition.y, BombPosition.z);
					NE_ModelSetCoord(Model[10], BombPosition.x, BombPosition.y, BombPosition.z);
					Model[7]->rz = BombPosition.r;
					BombSeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;

					bombBipTimer = 120;
					BombPlanted = true;
					SetBombDefuseZone(BombPosition.x, BombPosition.z, &bombDefuseZone); // Remove?
					showPartyEventText(2);

					SetRandomDefuser();
				}
			}
		}
		else if (getPlayerCurrentGunIndex(localPlayer) < GunCount + shopGrenadeCount && getPlayerCurrentGunIndex(localPlayer) >= GunCount && isKeyDown(FIRE_BUTTON) && !isUsingBomb) // Launch grenade
		{
			int grenadeType = AllGrenades[getPlayerCurrentGunIndex(localPlayer) - GunCount].type;
			PhysicalGrenade *newGrenade = CreateGrenade(grenadeType, localPlayer->Id);
			if (newGrenade != NULL)
			{
				lanchGrenade(newGrenade, x, y, z, localPlayer->PlayerModel->x, localPlayer->PlayerModel->y, localPlayer->PlayerModel->z);

				localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] = EMPTY;
				ChangeGunInInventoryForLocalPlayer(1);

				SendGrenade = true;
				// Force player to send position
				SendPosition = true;
				SendPositionData = 0;
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
					setEndRound();
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
			ApplyGunWalkAnimation(0);
		}
	}
	else
	{
		if (localPlayer->IsDead)
		{
			if (keysdown & KEY_LEFT)
			{
				changeCameraPlayerView(false);
			}
			else if (keysdown & KEY_RIGHT)
			{
				changeCameraPlayerView(true);
			}

			if (deathCameraAnimation < 1)
			{
				deathCameraAnimation += DEATH_ANIMATION_SPEED;
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
		if (roundState == PLAYING)
		{
			for (int i = 1; i < MaxPlayer; i++)
			{
				Player *player = &AllPlayers[i];
				if (player->Id == UNUSED)
					continue;

				if (player->isPlantingBomb)
				{
					if (player->Team == TERRORISTS)
					{
						// On bomb planting make a sound
						if (player->bombTimer == bombPlantingTime)
						{
							int Panning, Volume;
							GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);
							Play3DSound(SFX_BOMBPLANTING, Volume, Panning, player);
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

							PartyMinutes = allPartyModes[currentPartyMode].bombWaitingMinutesDuration;
							PartySeconds = allPartyModes[currentPartyMode].bombWaitingSecondsDuration;

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
								Play3DSound(SFX_BOMBPLANTING, Volume, Panning, player);
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
								setEndRound();
								CheckAfterRound();
							}
						}
					}
				}
			}
		}

		partyTimerTick();
	}
	else
	{
		partyTimerTickOnline();
	}

	// Make bomb sounds
	if (BombPlanted && !BombDefused)
	{
		if (bombBipTimer > 0)
			bombBipTimer--;

		if (bombBipTimer == 0 && BombSeconds > 0 && !BombWillExplode)
		{
			// make bib sound
			int Panning, Volume;
			GetPanningByPosition(&Panning, &Volume, BombPosition, xWithoutYForAudio, zWithoutYForAudio, 0.12);
			Play3DSound(SFX_BOMBBIP, Volume, Panning, NULL);
			bombBipTimer = (BombSeconds / 40.0) * 120.0 + 5;
			if (bombBipTimer > 50)
			{
				SetRandomDefuser();
			}
		}
		else if (bombBipTimer == 0 && BombSeconds == 0 && !BombWillExplode)
		{
			// Make detonate sound
			int Panning, Volume;
			GetPanningByPosition(&Panning, &Volume, BombPosition, xWithoutYForAudio, zWithoutYForAudio, 0.15);
			Play3DSound(SFX_DETONATE, Volume, Panning, NULL);
			bombBipTimer = 120;
			BombWillExplode = true;
		}
		else if (bombBipTimer == 0 && BombWillExplode)
		{
			// Make explosion sound
			PlayBasicSound(SFX_BOMBEXPLODE);
			BombPlanted = false;
			IsExplode = true;

			rumble(2);

			if (applyRules)
			{
				BombWillExplode = false;
				// Apply explosion damage to all players
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

	if (roundState != WAIT_START)
	{
		AiCheckForAction();

		checkAiShoot();
	}
}

/**
 * @brief Check if the local player is moving
 *
 * @return true
 * @return false
 */
bool isLocalPlayerMoving()
{
	return localPlayer->PlayerPhysic->xspeed + localPlayer->PlayerPhysic->yspeed + localPlayer->PlayerPhysic->zspeed != 0;
}

/**
 * @brief Update game engine for drawing (when not in party)
 *
 */
void UpdateEngineNotInGame()
{
	ScanForInput();

	increaseFrameCount();

	if (UpdateBottomScreenFrameCount > 8)
		UpdateBottomScreenFrameCount--;

	// Draw UI and 3D
	if (!isDebugBottomScreen)
	{
		NE_ProcessDual(Draw3DSceneNotInGame, drawBottomScreenUI);
	}
	else
	{
		NE_Process(Draw3DSceneNotInGame); // For debug
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

	NE_WaitForVBL(NE_CAN_SKIP_VBL);
}

/**
 * @brief Increase the frame count (used for the server to know the speed of actions)
 *
 */
void increaseFrameCount()
{
	frameCount++;
	if (frameCount == 2013265920)
	{
		frameCount = 0;
	}
	statsTimer();
}

/**
 * @brief Reset frame count
 *
 */
void resetFrameCount()
{
	frameCount = 0;
}

/**
 * @brief Update game engine for drawing
 *
 */
void UpdateEngine()
{
	ScanForInput();

	increaseFrameCount();

	if (!isDebugBottomScreen)
	{
		// Draw 3D objects and sprites
		if (!AlwaysUpdateBottomScreen && UpdateBottomScreenFrameCount == 0)
			NE_Process(Draw3DScene);
		else
		{
			if (UpdateBottomScreenFrameCount > 0)
				UpdateBottomScreenFrameCount--;

			NE_ProcessDual(Draw3DScene, drawBottomScreenUI);
		}
	}
	else
	{
		if (UpdateBottomScreenFrameCount > 0)
			UpdateBottomScreenFrameCount--;

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
	if (localPlayer->Id != UNUSED && localPlayer->Team != SPECTATOR)
		NE_PhysicsUpdate(localPlayer->PlayerPhysic);

	NE_WaitForVBL(NE_CAN_SKIP_VBL);
}

/**
 * @brief Change value "scale" ex map(128, 0,255,0,1024) = 512.0
 *
 * @param x The number to map
 * @param in_min The lower bound of the value’s current range
 * @param in_max The upper bound of the value’s current range
 * @param out_min The lower bound of the value’s target range
 * @param out_max The upper bound of the value’s target range
 * @return double The mapped value
 */
double map(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Change value "scale" ex map(128, 0,255,0,1024) = 512
 *
 * @param x The number to map
 * @param in_min The lower bound of the value’s current range
 * @param in_max The upper bound of the value’s current range
 * @param out_min The lower bound of the value’s target range
 * @param out_max The upper bound of the value’s target range
 * @return int The mapped value
 */
int mapInt(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Enable rumble for an amount of frame
 *
 * @param timer Frame count
 */
void rumble(int timer)
{
	if (useRumble)
	{
		setRumble(true);
		RumbleTimer = timer;
	}
}

/**
 * @brief Drop the bomb
 *
 * @param HittedClient Hitted client pointer
 * @param hittedPlayerIndex Hitted client index
 */
void dropBomb(Player *HittedClient, int hittedPlayerIndex)
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

/**
 * @brief Set need respawn for a player
 *
 * @param player Player pointer
 */
void setNeedRespawn(Player *player)
{
	player->RespawnTimer = allPartyModes[currentPartyMode].trainingRespawnSecondsDuration * 60;
	player->NeedRespawn = true;
}