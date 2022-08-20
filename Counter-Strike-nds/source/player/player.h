// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef PLAYER_H_ /* Include guard */
#define PLAYER_H_

#include "main.h"

void killPlayer(Player *player);
void makeHit(int hitBy, int playerHit, float distance, int shootIndex);
void buyGun();
void CalculatePlayerPosition(int PlayerIndex);
void setPlayerPositionAtSpawns(int playerIndex);
void setNewRoundHandWeapon();
void SetGunInInventory(int Value, int inventoryIndex);
void SetGunInInventoryForNonLocalPlayer(int playerIndex, int Value, int inventoryIndex);
void ChangeGunInInventoryForLocalPlayer(int Left);
void ChangeGunInInventory(int playerIndex, int Left);
void setSelectedGunInInventory(int playerIndex, int gunIndex);
void playerUpdate();
void removeAllPlayers();
int AddNewPlayer(int NewId, bool IsLocalPlayer, bool isAI);
void UpdatePlayerTexture(int playerIndex);
void setPlayerHealth(int playerIndex, int health);
void resetPlayer(int index);
void reducePlayerMoney(int playerIndex, int Money);
void addPlayerMoney(int playerIndex, int Money);
void setPlayerMoney(int playerIndex, int Money);
void addMoneyToTeam(int Money, enum teamEnum Team);
void setBombForARandomPlayer();
void setShopZone(Player *player);

#endif // PLAYER_H_