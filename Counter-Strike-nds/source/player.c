#include "main.h"
#include "player.h"
#include "party.h"
#include "ai.h"
#include "gun.h"
#include "map.h"
#include "collisions.h"
#include "sounds.h"

void playerUpdate()
{
}

void jump()
{
}

void createPlayerShadow()
{
    // Player shadow
    for (int i = 1; i < MaxPlayer; i++)
    {
        AllPlayers[i].PlayerShadow = NE_ModelCreate(NE_Static);
        NE_ModelScaleI(AllPlayers[i].PlayerShadow, 4096 * 2, 4096 * 2, 4096 * 2);
        NE_ModelSetMaterial(AllPlayers[i].PlayerShadow, PlayerShadowMaterial);
        NE_ModelLoadStaticMesh(AllPlayers[i].PlayerShadow, (u32 *)plane_bin);
        NE_ModelSetCoord(AllPlayers[i].PlayerShadow, 4, -0.845, -13);
    }
}

void setPlayerHealth(int playerIndex, int health)
{
    Player *player = &AllPlayers[playerIndex];
    if (player->Id == NO_PLAYER)
        return;

    player->Health = health;

    if (player->Health <= 0)
    {
        if (!applyRules)
        {
            killPlayer(player);
        }
        // If died player is the local player
        if (playerIndex == 0)
        {
            DisableAim();
            PlayBasicSound(SFX_DEATH);
            // Hide crosshair
            NE_SpriteVisible(TopScreenSprites[0], false);
        }
        else
        {
            int Panning, Volume;
            GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);
            Play3DSound(SFX_DEATH, Volume, Panning);
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

void removeAllPlayers()
{
    // return;
    PlayerCount = 0;
    for (int i = 0; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];
        player->Id = UNUSED;
        player->Team = SPECTATOR;

        if (player->PlayerModel != NULL)
        {
            NE_ModelDelete(player->PlayerModel);
        }

        if (player->PlayerPhysic != NULL)
        {
            NE_PhysicsDelete(player->PlayerPhysic);
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
        Map *map = &allMaps[currentMap];
        if (player->Team == TERRORISTS)
        {
            player->PlayerModel->x = map->allTerroristsSpawns[currentTerrorist].x * 4096.0;
            player->PlayerModel->y = map->allTerroristsSpawns[currentTerrorist].y * 4096.0;
            player->PlayerModel->z = map->allTerroristsSpawns[currentTerrorist].z * 4096.0;
            // Add this for non AI players
            player->lerpDestination.x = map->allTerroristsSpawns[currentTerrorist].x;
            player->lerpDestination.y = map->allTerroristsSpawns[currentTerrorist].y;
            player->lerpDestination.z = map->allTerroristsSpawns[currentTerrorist].z;
            currentTerrorist++;

            player->Angle = map->startPlayerAngleTerrorists;
            player->AngleDestination = map->startPlayerAngleTerrorists;
            player->spawnAt = currentTerrorist;
        }
        else if (player->Team == COUNTERTERRORISTS)
        {
            player->PlayerModel->x = map->allCounterTerroristsSpawns[currentCounter].x * 4096.0;
            player->PlayerModel->y = map->allCounterTerroristsSpawns[currentCounter].y * 4096.0;
            player->PlayerModel->z = map->allCounterTerroristsSpawns[currentCounter].z * 4096.0;
            // Add this for non AI players
            player->lerpDestination.x = map->allCounterTerroristsSpawns[currentCounter].x;
            player->lerpDestination.y = map->allCounterTerroristsSpawns[currentCounter].y;
            player->lerpDestination.z = map->allCounterTerroristsSpawns[currentCounter].z;
            currentCounter++;

            player->Angle = map->startPlayerAngleCounterTerrorists;
            player->AngleDestination = map->startPlayerAngleCounterTerrorists;
            player->spawnAt = currentCounter;
        }
        CalculatePlayerPosition(i);
        if (i == 0)
        {
            SetCollisionBox(player->position.x, player->position.y, player->position.z, 4, 3, 4, &shopZone);
        }
    }
    NeedUpdateViewRotation = true;
}

void setPlayerPositionAtSpawns(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    Map *map = &allMaps[currentMap];
    if (player->Team == TERRORISTS)
    {
        player->PlayerModel->x = map->allTerroristsSpawns[player->spawnAt].x * 4096.0;
        player->PlayerModel->y = map->allTerroristsSpawns[player->spawnAt].y * 4096.0;
        player->PlayerModel->z = map->allTerroristsSpawns[player->spawnAt].z * 4096.0;
        // Add this for non AI players
        player->lerpDestination.x = map->allTerroristsSpawns[player->spawnAt].x;
        player->lerpDestination.y = map->allTerroristsSpawns[player->spawnAt].y;
        player->lerpDestination.z = map->allTerroristsSpawns[player->spawnAt].z;

        player->Angle = map->startPlayerAngleTerrorists;
        player->AngleDestination = map->startPlayerAngleTerrorists;
    }
    else if (player->Team == COUNTERTERRORISTS)
    {
        player->PlayerModel->x = map->allCounterTerroristsSpawns[player->spawnAt].x * 4096.0;
        player->PlayerModel->y = map->allCounterTerroristsSpawns[player->spawnAt].y * 4096.0;
        player->PlayerModel->z = map->allCounterTerroristsSpawns[player->spawnAt].z * 4096.0;
        // Add this for non AI players
        player->lerpDestination.x = map->allCounterTerroristsSpawns[player->spawnAt].x;
        player->lerpDestination.y = map->allCounterTerroristsSpawns[player->spawnAt].y;
        player->lerpDestination.z = map->allCounterTerroristsSpawns[player->spawnAt].z;

        player->Angle = map->startPlayerAngleCounterTerrorists;
        player->AngleDestination = map->startPlayerAngleCounterTerrorists;
    }
    CalculatePlayerPosition(playerIndex);
    if (playerIndex == 0)
    {
        SetCollisionBox(player->position.x, player->position.y, player->position.z, 4, 3, 4, &shopZone);
    }
    NeedUpdateViewRotation = true;
}

// Change player skin texture
void UpdatePlayerTexture(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    if (player->Team == COUNTERTERRORISTS)
        NE_ModelSetMaterial(player->PlayerModel, PlayerMaterial);
    else if (player->Team == TERRORISTS)
        NE_ModelSetMaterial(player->PlayerModel, PlayerMaterialTerrorist);
}

// Add new player to party
void AddNewPlayer(int NewId, bool IsLocalPlayer, bool isAI)
{
    for (int i = 1; i < MaxPlayer; i++)
    {
        if (IsLocalPlayer)
            i = 0;
        // printf("STEP 1\n");
        Player *player = &AllPlayers[i];

        // Check for an empty player slot
        if (player->Id == UNUSED)
        {
            iprintf("\nCreating player Id: %d", NewId);
            // Create player (3d model, physics, animations)
            // NE_ModelAnimSetFrame(AllPlayers[i].PlayerModel, 2);

            // Set player collision size
            player->xSize = 0.35;
            player->ySize = 0.9;
            player->zSize = 0.35;

            // Set default weapon in his hand
            player->currentGunInInventory = 1;

            // Init inventory
            player->AllGunsInInventory[0] = 0;
            player->AllGunsInInventory[1] = 1;
            for (int i2 = 2; i2 < inventoryCapacity; i2++)
            {
                player->AllGunsInInventory[i2] = EMPTY;
            }
            ResetGunsAmmo(i);

            player->rightGunXRecoil = GunMinRecoil;
            player->rightGunYRecoil = GunMinRecoil;
            player->leftGunXRecoil = GunMinRecoil;
            player->leftGunYRecoil = GunMinRecoil;

            if (IsLocalPlayer)
            {
                // strcpy(player->name, "LocalPlayer");
                // Load();
                player->PlayerModel = NE_ModelCreate(NE_Static);
                player->PlayerPhysic = NE_PhysicsCreate(NE_BoundingBox);
                NE_PhysicsSetModel(player->PlayerPhysic, (void *)player->PlayerModel); // Physics object and Model assigned to it
                NE_PhysicsEnable(player->PlayerPhysic, IsLocalPlayer);
                NE_PhysicsSetGravity(player->PlayerPhysic, 0.0065);
                NE_PhysicsSetSize(player->PlayerPhysic, player->xSize * 2.0, player->ySize * 2.0, player->zSize * 2.0);
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

                if (i == 1)
                    NE_ModelLoadStaticMesh(player->PlayerModel, (u32 *)GIGNNew_bin);
                else
                    NE_ModelClone(player->PlayerModel,        // Destination
                                  AllPlayers[1].PlayerModel); // Source model

                // NE_ModelLoadNEA(player->PlayerModel, (u32 *)GIGNAnimNea_bin);
                NE_ModelSetMaterial(player->PlayerModel, PlayerMaterial);
                NE_ModelScaleI(player->PlayerModel, 2048, 2090, 2048); // 2048 <- 4096 * 0.5 ANIMATED VERSION
                // NE_ModelScaleI(player->PlayerModel, 700, 700, 700); // 2048 <- 4096 * 0.5 STATIC VERSION
                //  NE_ModelAnimInterpolate(player->PlayerModel, false);
                //  NE_ModelAnimStart(player->PlayerModel, 0, 0, 5, NE_ANIM_LOOP, 3);

                // Select a random name
                if (isAI)
                {
                    player->PathCount = 0;
                    player->CurrentPath = 0;
                    player->LastWayPoint = 0;
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
                    // strcpy(player->name, "Player%d", NewId);
                    strcpy(player->name, "Player");
                }
            }

            NE_ModelSetCoord(player->PlayerModel, 0, 0 /* + i * 5*/, -2);

            player->Id = NewId;

            setPlayerHealth(i, 100);
            player->isAi = isAI;

            // Init raycasting values
            player->StopAt = 30;
            // player->ScanFinished = true;
            player->target = NO_PLAYER;
            player->lastSeenTarget = NO_PLAYER;
            player->justCheking = false;
            player->canShootEnemy = false;
            player->Money = allPartyModes[currentPartyMode].startMoney; //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// TODO
            // player->Money = 8000;

            setPlayerMoney(i, 8000);

            player->KillCount = 0;
            player->DeathCount = 0;
            player->GunWaitCount = 0;
            player->GunReloadWaitCount = 0;
            player->isReloading = false;
            // player->IsDead = false;
            player->haveDefuseKit = false;
            player->haveBomb = false;
            player->isPlantingBomb = false;
            player->bombTimer = 0;

            player->BobbingOffset = 0;
            player->HasBobbed = false;
            player->Step = 0;

            player->RespawnTimer = 0;
            player->NeedRespawn = false;
            player->CurrentOcclusionZone = 0;
            player->Team = SPECTATOR;

            player->lightCoef = 1;
            player->currentShadowCollBox = 0;
            player->inShadow = false;

            if (allPartyModes[currentPartyMode].spawnWithArmor)
            {
                player->armor = 100;
                player->haveHeadset = true;
            }
            else
            {
                player->armor = 0;
                player->haveHeadset = false;
            }

            PlayerCount++;
            return;
        }
    }

    // If code running here, this is not normal lol XD
    printf("No player slot available");
}

void setBombForARandomPlayer()
{
    int TerroristCount = 0;

    for (int i = 0; i < MaxPlayer; i++)
    {
        SetGunInInventoryForNonLocalPlayer(i, EMPTY, 8);
        AllPlayers[i].haveBomb = false;
        if (AllPlayers[i].Id != UNUSED && AllPlayers[i].Team == TERRORISTS)
        {
            TerroristCount++;
        }
    }

    if (localPlayer->Team == TERRORISTS)
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
        if (player->Id != UNUSED && player->Team == TERRORISTS)
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

void addMoneyToTeam(int Money, enum teamEnum Team)
{
    for (int i = 0; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];
        if (player->Team == Team)
        {
            player->Money += Money;
            if (player->Money > allPartyModes[currentPartyMode].maxMoney)
                player->Money = allPartyModes[currentPartyMode].maxMoney;
        }
    }
}

void setPlayerMoney(int playerIndex, int Money)
{
    Player *player = &AllPlayers[playerIndex];
    player->Money = Money;

    if (currentMenu == SHOP || currentMenu == SHOPCATEGORIES)
        UpdateBottomScreenOneFrame += 8;
}

void addPlayerMoney(int playerIndex, int Money)
{
    Player *player = &AllPlayers[playerIndex];
    player->Money += Money;
    if (player->Money > allPartyModes[currentPartyMode].maxMoney)
        player->Money = allPartyModes[currentPartyMode].maxMoney;

    if (currentMenu == SHOP || currentMenu == SHOPCATEGORIES)
        UpdateBottomScreenOneFrame += 8;
}

void reducePlayerMoney(int playerIndex, int Money)
{
    Player *player = &AllPlayers[playerIndex];
    player->Money -= Money;
    if (player->Money < 0)
        player->Money = 0;

    if (currentMenu == SHOP || currentMenu == SHOPCATEGORIES)
        UpdateBottomScreenOneFrame += 8;
}

void resetPlayer(int index)
{
    Player *player = &AllPlayers[index];

    StopReloading(index);

    for (int i = 0; i < grenadeBoughtLength; i++)
    {
        player->grenadeBought[i] = 0;
    }

    if (player->IsDead || (allPartyModes[currentPartyMode].middlePartyTeamSwap && TerroristsScore + CounterScore == floor(allPartyModes[currentPartyMode].maxRound / 2.0)) || TerroristsScore + CounterScore == 0)
    {
        player->haveDefuseKit = false;
        player->armor = 0;
        player->haveHeadset = false;

        if (player->Team == TERRORISTS)
            player->AllGunsInInventory[1] = DEFAULTTERRORISTGUN;
        else if (player->Team == COUNTERTERRORISTS)
            player->AllGunsInInventory[1] = DEFAULTCOUNTERTERRORISTGUN;

        for (int i = 2; i < inventoryCapacity - 1; i++)
        {
            player->AllGunsInInventory[i] = EMPTY;
        }
    }

    setPlayerHealth(index, 100);
    if (allPartyModes[currentPartyMode].spawnWithArmor)
    {
        player->armor = 100;
        player->haveHeadset = true;
    }

    if (roundState == TRAINING)
    {
        player->invisibilityTimer = 60 * 5; // 5 seconds
        player->currentGunInInventory = 1;
    }
    // player->haveBomb = false;
    ResetGunsAmmo(index);
    if (index == 0)
    {
        CurrentCameraPlayer = 0;
        UpdateGunTexture();
        frameCountDuringAir = 0;
        // SetCollisionBox(player->position.x, player->position.y, player->position.z, 4, 3, 4, &shopZone);
    }
    else
    {
        player->PathCount = 0;
        player->target = NO_PLAYER;
        player->lastSeenTarget = NO_PLAYER;
    }
}