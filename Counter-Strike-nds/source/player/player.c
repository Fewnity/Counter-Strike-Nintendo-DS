// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "player.h"
#include "party.h"
#include "ai.h"
#include "gun.h"
#include "map.h"
#include "equipment.h"
#include "collisions.h"
#include "sounds.h"
#include "stats.h"
#include "network.h"

/**
 * @brief Kill a player
 *
 * @param player Player pointer
 */
void killPlayer(Player *player)
{
    player->Health = 0;
    // Set client has dead
    player->IsDead = true;
    if (player == localPlayer && (currentMenu == SHOP || currentMenu == SHOPCATEGORIES))
    {
        initGameMenu();
    }
    if (player == localPlayer)
    {
        totalDeathCount++;
    }
}

/**
 * @brief Hit a player (Apply damage)
 *
 * @param hitBy Shooter player index
 * @param playerHit Hitted player index
 * @param distance Hit distance
 */
void makeHit(int hitBy, int playerHit, float distance, int shootIndex)
{
    Player *hittedPlayer = &AllPlayers[playerHit];
    Player *shooterPlayer = &AllPlayers[hitBy];

    int hitType = 0;
    int Damage = (int)(AllGuns[shooterPlayer->startGunIdRaycast].Damage * pow(AllGuns[shooterPlayer->startGunIdRaycast].DamageFalloff, distance / 500.0));
    // Apply more or less damage with the location of the bullet
    if (shooterPlayer->IsHeadShot[shootIndex])
    {
        hitType = 1;
        Damage *= 4;
    }
    else if (shooterPlayer->IsLegShot[shootIndex])
    {
        hitType = 2;
        Damage /= 2.0;
    }

    // Reduce damage when clients are in the same team
    if (hittedPlayer->Team == shooterPlayer->Team)
    {
        Damage = (int)(Damage / 3.0);
    }

    if (shooterPlayer->startGunIdRaycast < GunCount && AllGuns[shooterPlayer->startGunIdRaycast].isKnife)
        hitType = 3;

    bool makeHeadSetSound = false;
    // If an armor was touched
    if ((hitType == 0 && hittedPlayer->armor != 0) || (hitType == 1 && hittedPlayer->haveHeadset))
    {
        // Reduce damage of the bullet
        int oldDamage = Damage;
        Damage = (int)(Damage * AllGuns[shooterPlayer->startGunIdRaycast].penetration / 100.0);
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
        hittedPlayer->lastSeenTarget = hitBy;
        if (hittedPlayer->GunWaitCount >= 0)
        {
            int randomWait = 40 + rand() % 20;

            hittedPlayer->GunWaitCount = -randomWait;
        }
        // hittedPlayer->GunWaitCount = -60;
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
        Play3DSound(SFX_FLESH_IMPACT, Volume, Panning, hittedPlayer); // TODO Check kevlar sound
    else if (hitType == 1)
    {
        if (!makeHeadSetSound)
            Play3DSound(SFX_HEADSHOT1, Volume, Panning, hittedPlayer);
        else
            Play3DSound(SFX_HIT_HELMET, Volume, Panning, hittedPlayer);
    }
    else if (hitType == 2)
        Play3DSound(SFX_FLESH_IMPACT, Volume, Panning, hittedPlayer);
    else
        Play3DSound(SFX_KNIFE_HIT_PLAYER, Volume, Panning, hittedPlayer);
}

/**
 * @brief Buy select gun from shop for local player
 *
 */
void buyGun()
{
    int grenadeIndex = GetSelectedGunShop() - GunCount;
    int equipmentIndex = GetSelectedGunShop() - GunCount - shopGrenadeCount;
    if (ShopCategory < EQUIPMENT && AllGuns[GetSelectedGunShop()].Price <= localPlayer->Money)
    {
        // Check if the player already have this gun
        if (localPlayer->AllGunsInInventory[1] == GetSelectedGunShop() || localPlayer->AllGunsInInventory[2] == GetSelectedGunShop())
        {
        }
        else
        {
            if (!allPartyModes[currentPartyMode].infiniteMoney)
            {
                reducePlayerMoney(0, AllGuns[GetSelectedGunShop()].Price);
            }

            // Set inventory slot
            if (ShopCategory == PISTOLS)
            {
                SetGunInInventory(GetSelectedGunShop(), 1);
                setSelectedGunInInventory(0, 1);
            }
            else
            {
                SetGunInInventory(GetSelectedGunShop(), 2);
                setSelectedGunInInventory(0, 2);
            }
        }
    }
    else if (ShopCategory == GRENADES && AllGrenades[grenadeIndex].Price <= localPlayer->Money)
    {
        // Search for the first empty slot for grenade
        for (int grenadeCheckIndex = InventoryGrenadeStartPosition; grenadeCheckIndex < InventoryEquipementStartPosition; grenadeCheckIndex++)
        {
            if (localPlayer->AllGunsInInventory[grenadeCheckIndex] == -1 && localPlayer->grenadeBought[grenadeIndex] < AllGrenades[grenadeIndex].maxQuantity[currentPartyMode])
            {
                localPlayer->AllGunsInInventory[grenadeCheckIndex] = GetSelectedGunShop();
                if (currentPartyMode != 2)
                    localPlayer->grenadeBought[grenadeIndex]++;

                if (!allPartyModes[currentPartyMode].infiniteMoney)
                {
                    reducePlayerMoney(0, AllGrenades[grenadeIndex].Price);
                }
                setSelectedGunInInventory(0, grenadeCheckIndex);
                break;
            }
        }
    }
    else if (ShopCategory == EQUIPMENT && allEquipments[equipmentIndex].Price <= localPlayer->Money)
    {
        bool canBuy = true;

        switch (allEquipments[equipmentIndex].type)
        {
        case DEFUSER:
            if (localPlayer->haveDefuseKit)
            {
                canBuy = false;
            }
            else
            {
                localPlayer->haveDefuseKit = true;
            }
            break;

        case KEVLAR_VEST:
            if (localPlayer->armor == 100)
            {
                canBuy = false;
            }
            else
            {
                localPlayer->armor = 100;
            }
            break;

        case KEVLAR_VEST_AND_HELMET:
            if (localPlayer->haveHeadset)
            {
                canBuy = false;
            }
            else
            {
                localPlayer->armor = 100;
                localPlayer->haveHeadset = true;
            }
            break;

        case C4:
            canBuy = false;
            break;
        }

        if (!allPartyModes[currentPartyMode].infiniteMoney && canBuy)
        {
            reducePlayerMoney(0, allEquipments[equipmentIndex].Price);
        }
    }
}

/**
 * @brief Set on new round the most powerful gun in the hand or the bomb
 *
 */
void setNewRoundHandWeapon()
{
    for (int playerIndex = 0; playerIndex < MaxPlayer; playerIndex++)
    {
        Player *player = &AllPlayers[playerIndex];
        if (player->Id == UNUSED)
            continue;

        if (playerIndex == 0 && player->haveBomb)
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

        if (playerIndex == 0)
            UpdateGunTexture();
    }
}

/**
 * @brief Set a gun to the local player
 *
 * @param Value Gun id
 * @param inventoryIndex Inventory slot index
 */
void SetGunInInventory(int Value, int inventoryIndex)
{
    localPlayer->AllGunsInInventory[inventoryIndex] = Value;
    if (getPlayerCurrentGunIndex(localPlayer) == UNUSED) // If the player has no guns in hands
        ChangeGunInInventoryForLocalPlayer(0);           // Switch to a gun in inventory
    else
    {
        // Refresh the gun texture and reset ammo
        ResetGunAmmo(0, inventoryIndex);
        UpdateGunTexture();
    }
}

/**
 * @brief  Set a gun to a player
 *
 * @param playerIndex Player index
 * @param Value Gun id
 * @param inventoryIndex Inventory slot index
 */
void SetGunInInventoryForNonLocalPlayer(int playerIndex, int Value, int inventoryIndex)
{
    AllPlayers[playerIndex].AllGunsInInventory[inventoryIndex] = Value;
    if (playerIndex == 0 && getPlayerCurrentGunIndex(localPlayer) == UNUSED) // If the player has no guns in hands
        ChangeGunInInventoryForLocalPlayer(0);                               // Switch to a gun in inventory
    else                                                                     // Reset ammo
        ResetGunAmmo(playerIndex, inventoryIndex);
}

/**
 * @brief Change current gun of the local player
 *
 * @param Left Way (1 - left, 0 - right)
 */
void ChangeGunInInventoryForLocalPlayer(int Left)
{
    DisableAim();
    ChangeGunInInventory(0, Left);
}

/**
 * @brief Change current gun of a player
 *
 * @param playerIndex Player index
 * @param Left Way (1 - left, 0 - right)
 */
void ChangeGunInInventory(int playerIndex, int Left)
{
    Player *player = &AllPlayers[playerIndex];

    // Retain the current gun in inventory if there is no other gun in the inventory
    int oldCurrentGun = player->currentGunInInventory;
    // Find a gun in the inventory
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

    // If no another gun was found, use the old gun index
    if (playerIndex == 0 && !canChangeGun && player->AllGunsInInventory[oldCurrentGun] != -1)
    {
        player->currentGunInInventory = oldCurrentGun;
        return;
    }

    if (oldCurrentGun != player->currentGunInInventory)
    {
        setSelectedGunInInventory(playerIndex, player->currentGunInInventory);
    }
}

/**
 * @brief Set the Selected Gun In Inventory object
 *
 * @param playerIndex
 * @param gunIndex
 */
void setSelectedGunInInventory(int playerIndex, int gunIndex)
{
    Player *player = &AllPlayers[playerIndex];
    if (player->AllGunsInInventory[gunIndex] != EMPTY)
    {
        player->currentGunInInventory = gunIndex;
        // Stop the reloading animation/action
        StopReloading(playerIndex);
        if (playerIndex == 0)
        {
            // Send to the server
            SendSelectedGun = true;
            // Update gun texture at screen
            UpdateGunTexture();
        }
    }
}

/**
 * @brief Set player health
 *
 * @param playerIndex Player index
 * @param health New health
 */
void setPlayerHealth(int playerIndex, int health)
{
    Player *player = &AllPlayers[playerIndex];
    if (player->Id == NO_PLAYER)
        return;

    player->Health = health;

    // if the player is dead
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
            // Do death sound
            PlayBasicSound(SFX_DEATH);
            // Hide crosshair
            NE_SpriteVisible(TopScreenSprites[0], false);
        }
        else
        {
            // Do death sound
            int Panning, Volume;
            GetPanning(player->Id, &Panning, &Volume, xWithoutYForAudio, zWithoutYForAudio, 0.10);
            Play3DSound(SFX_DEATH, Volume, Panning, player);
        }
    }
    else if (player->Health == 100)
    {
        // Re enable crosshair
        if (playerIndex == 0)
        {
            NE_SpriteVisible(TopScreenSprites[0], true);
            deathCameraAnimation = 0;
            deathCameraYOffset = 0;
            redHealthTextCounter = 0;
        }

        player->IsDead = FALSE;
    }
}

/**
 * @brief Delete all players
 *
 */
void removeAllPlayers()
{
    PlayerCount = 0;
    for (int i = 0; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];
        player->Id = UNUSED;
        player->Team = SPECTATOR;

        // Delete model
        if (player->PlayerModel != NULL)
        {
            NE_ModelDelete(player->PlayerModel);
        }
        // Delete physics component
        if (player->PlayerPhysic != NULL)
        {
            NE_PhysicsDelete(player->PlayerPhysic);
        }
    }
}

/**
 * @brief Set players position to their spawn points
 *
 */
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
            // Set player position to a terrorists spawn
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
            // Set player position to a counter terrorists spawn
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
            setShopZone(player);
        }
    }
    NeedUpdateViewRotation = true;
}

/**
 * @brief Set shop zone for a player
 *
 * @param player Player pointer
 */
void setShopZone(Player *player)
{
    SetCollisionBox(player->position.x, player->position.y, player->position.z, 4, 3, 4, &shopZone);
}

/**
 * @brief Get player position and set it to float
 *
 * @param PlayerIndex Player index
 */
void CalculatePlayerPosition(int PlayerIndex)
{
    Player *player = &AllPlayers[PlayerIndex];
    player->position.x = player->PlayerModel->x / 4096.0;
    player->position.y = player->PlayerModel->y / 4096.0;
    player->position.z = player->PlayerModel->z / 4096.0;
}

/**
 * @brief Set a player position to his spawn point
 *
 * @param playerIndex Player index
 */
void setPlayerPositionAtSpawns(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    Map *map = &allMaps[currentMap];
    if (player->Team == TERRORISTS)
    {
        // Set player position to a terrorists spawn
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
        // Set player position to a counter terrorists spawn
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
        setShopZone(player);
    }
    NeedUpdateViewRotation = true;
}

/**
 * @brief Change player skin texture
 *
 * @param playerIndex Player index
 */
void UpdatePlayerTexture(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    if (player->Team == COUNTERTERRORISTS)
        NE_ModelSetMaterial(player->PlayerModel, PlayerMaterial); // Set counter terrorists texture
    else if (player->Team == TERRORISTS)
        NE_ModelSetMaterial(player->PlayerModel, PlayerMaterialTerrorist); // Set terrorists texture
}

/**
 * @brief Add new player to party
 *
 * @param NewId Id of the player
 * @param IsLocalPlayer Is a local player?
 * @param isAI Is an AI?
 */
int AddNewPlayer(int NewId, bool IsLocalPlayer, bool isAI)
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
            // strcpy(player->client.sendBuffer, "");
            //  Create player (3d model, physics, animations)

            // Set player collision size
            player->xSize = 0.35;
            player->ySize = 0.9;
            player->zSize = 0.35;
            player->cameraAngle = 128;

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
                // player->PlayerModel = NE_ModelCreate(NE_Animated); // ANIMATED VERSION
                player->PlayerModel = NE_ModelCreate(NE_Static);
                // NE_ModelLoadNEA(player->PlayerModel, (u32 *)playerAnimNea_bin); // ANIMATED VERSION

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
                    strcpy(player->name, "Player");
                }
            }

            NE_ModelSetCoord(player->PlayerModel, 0, -100, -2);
            player->lerpDestination.x = 0;
            player->lerpDestination.y = -100;
            player->lerpDestination.z = -2;
            player->position.x = 0;
            player->position.y = -100;
            player->position.z = -2;

            player->Id = NewId;

            setPlayerHealth(i, 100);
            player->isAi = isAI;

            // Init raycasting values
            player->ScanForGrenade = EMPTY;
            player->target = NO_PLAYER;
            player->lastSeenTarget = NO_PLAYER;
            player->justCheking = false;

            setPlayerMoney(i, allPartyModes[currentPartyMode].startMoney);

            player->KillCount = 0;
            player->DeathCount = 0;
            player->GunWaitCount = 0;
            player->GunReloadWaitCount = 0;
            player->isReloading = false;
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
            player->mapVisivilityTimer = 0;

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
            return i;
        }
    }
    return -1;
    // If code running here, this is not normal lol XD
    // No player slot available
}

/**
 * @brief Set the bomb to a random terrorist
 *
 */
void setBombForARandomPlayer()
{
    int TerroristCount = 0;

    // Remove the bomb of all terrorists
    for (int i = 0; i < MaxPlayer; i++)
    {
        SetGunInInventoryForNonLocalPlayer(i, EMPTY, 8);
        AllPlayers[i].haveBomb = false;
        // Get the number of terrorists in game
        if (AllPlayers[i].Id != UNUSED && AllPlayers[i].Team == TERRORISTS)
        {
            TerroristCount++;
        }
    }

    // If the local player is a terrorists
    if (localPlayer->Team == TERRORISTS)
    {
        // Give the bomb to the local player
        localPlayer->haveBomb = true;
        SetGunInInventory(28, 8);
        return;
    }

    // Or give the bomb to a random terrorist
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

void onMoneyUpdated(int playerIndex)
{
    // Refresh the screen if needed
    if (playerIndex == 0)
    {
        if (currentMenu == SHOP || currentMenu == SHOPCATEGORIES)
            UpdateBottomScreenFrameCount += 8;
    }
}

/**
 * @brief Give money to a team
 *
 * @param Money Money to give (if negative, the function will do nothing)
 * @param Team Team see teamEnum
 */
void addMoneyToTeam(int Money, enum teamEnum Team)
{
    if (Money <= 0)
        return;

    for (int i = 0; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];
        if (player->Team == Team)
        {
            addPlayerMoney(i, Money);
        }
    }
}

/**
 * @brief Set player money
 *
 * @param playerIndex Player index
 * @param Money Money (if negative, money will be 0)
 */
void setPlayerMoney(int playerIndex, int Money)
{
    if (Money < 0)
        Money = 0;

    Player *player = &AllPlayers[playerIndex];
    player->Money = Money;

    if (player->Money > allPartyModes[currentPartyMode].maxMoney)
        player->Money = allPartyModes[currentPartyMode].maxMoney;

    onMoneyUpdated(playerIndex);
}

/**
 * @brief Give money to a player
 *
 * @param playerIndex Player index
 * @param Money Money to give (if negative, the function will do nothing)
 */
void addPlayerMoney(int playerIndex, int Money)
{
    if (Money <= 0)
        return;

    Player *player = &AllPlayers[playerIndex];
    player->Money += Money;
    if (player->Money > allPartyModes[currentPartyMode].maxMoney)
        player->Money = allPartyModes[currentPartyMode].maxMoney;

    onMoneyUpdated(playerIndex);
}

/**
 * @brief Reduce player money
 *
 * @param playerIndex Player index
 * @param Money Money to reduce (if negative, the function will do nothing)
 */
void reducePlayerMoney(int playerIndex, int Money)
{
    if (Money <= 0)
        return;

    Player *player = &AllPlayers[playerIndex];
    player->Money -= Money;
    if (player->Money < 0)
        player->Money = 0;

    onMoneyUpdated(playerIndex);
}

/**
 * @brief Reset a player (Reset inventory, ammo, health, armor)
 *
 * @param index Player index
 */
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
        player->invincibilityTimer = 60 * 5; // 5 seconds
        player->currentGunInInventory = 1;
    }
    player->isPlantingBomb = false; //
    player->bombTimer = 0;

    ResetGunsAmmo(index);
    if (index == 0)
    {
        SetCurrentCameraPlayer(0);
        frameCountDuringAir = 0;
    }
    else
    {
        player->PathCount = 0;
        player->target = NO_PLAYER;
        player->lastSeenTarget = NO_PLAYER;
    }
}