// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "gun.h"
#include "sounds.h"
#include "ui.h"
#include "party.h"
#include "network.h"
#include "equipment.h"
#include "player.h"

// Loaded guns data
Gun AllGuns[GunCount];

// Cheapest gun of each category for counters terrorists
int cheapestGunsCostCounter[4];
// Cheapest gun of each category for terrorists
int cheapestGunsCostTerrorists[4];
// Gun count for each category for counters terrorists
int gunCountCounter[4];
// Gun count for each category for terrorists
int gunCountTerrorists[4];

// Data sources https://counterstrike.fandom.com/wiki/Category:Weapons

#define HEAVY_WEAPON_WALK_SPEED 175         // 180
#define MEDIUM_HIGH_WEAPON_WALK_SPEED 180   // 185
#define MEDIUM_MEDIUM_WEAPON_WALK_SPEED 185 // 190
#define MEDIUM_WEAPON_WALK_SPEED 195        // 200
#define LIGHT_WEAPON_WALK_SPEED 210

/**
 * @brief Load guns data
 *
 */
void AddGuns()
{
    int gunIndex = 0;

    // Knife
    AllGuns[gunIndex].fireRate = 24; // 60 * 0.4
    AllGuns[gunIndex].DamageFalloff = 1;
    AllGuns[gunIndex].Damage = 40;
    AllGuns[gunIndex].texture = (void *)knifect_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].gunSound = SFX_KNIFE_HIT_WALL;
    AllGuns[gunIndex].WalkSpeed = 220;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = -1; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;      // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].ReloadTime = 0;   // 2.1 * 60
    AllGuns[gunIndex].isKnife = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.07;
    AllGuns[gunIndex].penetration = 85;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 3;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "Knife";
    gunIndex++;

    // Desert Eagle
    AllGuns[gunIndex].fireRate = 13; // 1 / 267 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.81;
    AllGuns[gunIndex].Damage = 53;
    AllGuns[gunIndex].texture = (void *)deagle_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 650;
    AllGuns[gunIndex].gunSound = SFX_DEAGLE;
    AllGuns[gunIndex].WalkSpeed = LIGHT_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = PISTOLS; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;           // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 7;
    AllGuns[gunIndex].MaxAmmoCount = 35;
    AllGuns[gunIndex].ReloadTime = 132; // 2.2 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 11;
    AllGuns[gunIndex].yMuzzleFlashOffset = 7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "Desert Eagle";
    gunIndex++;

    // Dual Berettas
    AllGuns[gunIndex].fireRate = 5; // 1 / 750 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.75;
    AllGuns[gunIndex].Damage = 43;
    AllGuns[gunIndex].texture = (void *)elite_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 800;
    AllGuns[gunIndex].gunSound = SFX_ELITE;
    AllGuns[gunIndex].WalkSpeed = LIGHT_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = PISTOLS; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS;     // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 226; // 3.77 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 14;
    AllGuns[gunIndex].yMuzzleFlashOffset = 3;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 52;
    AllGuns[gunIndex].isDualGun = true;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "Dual Berettas";
    gunIndex++;

    // Five SeveN
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.885;
    AllGuns[gunIndex].Damage = 24;
    AllGuns[gunIndex].texture = (void *)fivesevent_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 750;
    AllGuns[gunIndex].gunSound = SFX_FIVESEVEN;
    AllGuns[gunIndex].WalkSpeed = LIGHT_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = PISTOLS;    // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = COUNTERTERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 192; // 3.2 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 16;
    AllGuns[gunIndex].yMuzzleFlashOffset = 10;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "Five SeveN";
    gunIndex++;

    // Glock-18
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.75;
    AllGuns[gunIndex].Damage = 24;
    AllGuns[gunIndex].texture = (void *)glock18_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 400;
    AllGuns[gunIndex].gunSound = SFX_GLOCK18;
    AllGuns[gunIndex].WalkSpeed = LIGHT_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = PISTOLS; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS;     // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 136; // 2.27 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 17;
    AllGuns[gunIndex].yMuzzleFlashOffset = 9;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 52;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "Glock-18";
    gunIndex++;

    // P228
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.8;
    AllGuns[gunIndex].Damage = 31;
    AllGuns[gunIndex].texture = (void *)p228_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 600;
    AllGuns[gunIndex].gunSound = SFX_P228;
    AllGuns[gunIndex].WalkSpeed = LIGHT_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = PISTOLS; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;           // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 13;
    AllGuns[gunIndex].MaxAmmoCount = 52;
    AllGuns[gunIndex].ReloadTime = 162; // 2.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 19;
    AllGuns[gunIndex].yMuzzleFlashOffset = 10;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 62.5;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "P228";
    gunIndex++;

    // USP
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.79;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)uspsilencer_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 500;
    AllGuns[gunIndex].gunSound = SFX_USP;
    AllGuns[gunIndex].WalkSpeed = LIGHT_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = PISTOLS;    // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = COUNTERTERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 12;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 162; // 2.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 9;
    AllGuns[gunIndex].yMuzzleFlashOffset = 6;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "USP";
    gunIndex++;

    // M3
    AllGuns[gunIndex].fireRate = 53; // 1 / 68 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.7;
    AllGuns[gunIndex].Damage = 171 / 6; // 1 shoot is 6 bullets
    AllGuns[gunIndex].texture = (void *)m3super90_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 1700;
    AllGuns[gunIndex].gunSound = SFX_M3;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_HIGH_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = HEAVY; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;         // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 8;
    AllGuns[gunIndex].MaxAmmoCount = 48;
    AllGuns[gunIndex].ReloadTime = 276; // 4.6 * 60 from empty
    AllGuns[gunIndex].xMuzzleFlashOffset = -1;
    AllGuns[gunIndex].yMuzzleFlashOffset = 3;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    AllGuns[gunIndex].bulletCountPerShoot = 6;
    AllGuns[gunIndex].maxBotShootDisance = 14;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "M3";
    gunIndex++;

    // XM1014
    AllGuns[gunIndex].fireRate = 15; // 1 / 240 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.7;
    AllGuns[gunIndex].Damage = 114 / 6; // 1 shoot is 6 bullets
    AllGuns[gunIndex].texture = (void *)xm1014_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 3000;
    AllGuns[gunIndex].gunSound = SFX_XM1014;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_HIGH_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = HEAVY; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;         // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 7;
    AllGuns[gunIndex].MaxAmmoCount = 32;
    AllGuns[gunIndex].ReloadTime = 168; // 2.8 * 60 from empty
    AllGuns[gunIndex].xMuzzleFlashOffset = -8;
    AllGuns[gunIndex].yMuzzleFlashOffset = 1;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    AllGuns[gunIndex].bulletCountPerShoot = 6;
    AllGuns[gunIndex].maxBotShootDisance = 14;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "XM1014";
    gunIndex++;

    // M249
    AllGuns[gunIndex].fireRate = 5; // 1 / 750 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.97;
    AllGuns[gunIndex].Damage = 35;
    AllGuns[gunIndex].texture = (void *)m249_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 5750;
    AllGuns[gunIndex].gunSound = SFX_M249;
    AllGuns[gunIndex].WalkSpeed = HEAVY_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = HEAVY; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;         // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 100;
    AllGuns[gunIndex].MaxAmmoCount = 200;
    AllGuns[gunIndex].ReloadTime = 342; // 5.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -2;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 20;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "M249";
    gunIndex++;

    // MAC-10
    AllGuns[gunIndex].fireRate = 4; // 1 / 857  * 3600
    AllGuns[gunIndex].DamageFalloff = 0.82;
    AllGuns[gunIndex].Damage = 28;
    AllGuns[gunIndex].texture = (void *)mac10_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1400;
    AllGuns[gunIndex].gunSound = SFX_MAC10;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = SGM; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 156; // 2.6 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 11;
    AllGuns[gunIndex].yMuzzleFlashOffset = 10;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 47.5;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 17;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "MAC-10";
    gunIndex++;

    // MP5
    AllGuns[gunIndex].fireRate = 5; // 1 / 750 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.84;
    AllGuns[gunIndex].Damage = 25;
    AllGuns[gunIndex].texture = (void *)mp5_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1500;
    AllGuns[gunIndex].gunSound = SFX_MP5;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = SGM; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;       // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 180; // 3 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 1;
    AllGuns[gunIndex].yMuzzleFlashOffset = 5;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 17;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "MP5";
    gunIndex++;

    // P90
    AllGuns[gunIndex].fireRate = 4; // 1 / 857 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.885;
    AllGuns[gunIndex].Damage = 25;
    AllGuns[gunIndex].texture = (void *)p90_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2350;
    AllGuns[gunIndex].gunSound = SFX_P90;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = SGM; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;       // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 50;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 198; // 3.3 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 5;
    AllGuns[gunIndex].yMuzzleFlashOffset = 6;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 17;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "P90";
    gunIndex++;

    // TMP
    AllGuns[gunIndex].fireRate = 4; // 1 / 857  * 3600
    AllGuns[gunIndex].DamageFalloff = 0.85;
    AllGuns[gunIndex].Damage = 19;
    AllGuns[gunIndex].texture = (void *)tmp_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1250;
    AllGuns[gunIndex].gunSound = SFX_TMP;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = SGM; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;       // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 126; // 2.1 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 2;
    AllGuns[gunIndex].yMuzzleFlashOffset = 1;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 17;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "TMP";
    gunIndex++;

    // UMP-45
    AllGuns[gunIndex].fireRate = 6; // 1 / 571   * 3600
    AllGuns[gunIndex].DamageFalloff = 0.75;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)ump45_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1250;
    AllGuns[gunIndex].gunSound = SFX_UMP45;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = SGM; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;       // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 25;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 210; // 3.5 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 6;
    AllGuns[gunIndex].yMuzzleFlashOffset = 7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 17;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "UMP-45";
    gunIndex++;

    // AK-47
    AllGuns[gunIndex].fireRate = 6; // 1 / 600 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.98;
    AllGuns[gunIndex].Damage = 35;
    AllGuns[gunIndex].texture = (void *)ak47_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2500;
    AllGuns[gunIndex].gunSound = SFX_AK47;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = RIFLES; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS;    // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 205; // 2.43 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 2;
    AllGuns[gunIndex].yMuzzleFlashOffset = 2;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 77.5;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 23;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "AK-47";
    gunIndex++;

    // AUG
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.96;
    AllGuns[gunIndex].Damage = 31;
    AllGuns[gunIndex].texture = (void *)aug_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3500;
    AllGuns[gunIndex].gunSound = SFX_AUG;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = RIFLES;     // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = COUNTERTERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 228; // 3.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -2;
    AllGuns[gunIndex].yMuzzleFlashOffset = 0;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 23;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.5;
    AllGuns[gunIndex].name = "AUG";
    gunIndex++;

    // FAMAS
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.96;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)famas_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2250;
    AllGuns[gunIndex].gunSound = SFX_FAMAS;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = RIFLES;     // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = COUNTERTERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 25;
    AllGuns[gunIndex].MaxAmmoCount = 75;
    AllGuns[gunIndex].ReloadTime = 228; // 3.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -2;
    AllGuns[gunIndex].yMuzzleFlashOffset = 0;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 23;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "FAMAS";
    gunIndex++;

    // Galil
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.98;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)galil_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2000;
    AllGuns[gunIndex].gunSound = SFX_GALIL;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = RIFLES; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS;    // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 35;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 198; // 3.3 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 0;
    AllGuns[gunIndex].yMuzzleFlashOffset = 1;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 77;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 23;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "Galil";
    gunIndex++;

    // M4A1
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.97;
    AllGuns[gunIndex].Damage = 31;
    AllGuns[gunIndex].texture = (void *)m4a1_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3100;
    AllGuns[gunIndex].gunSound = SFX_M4A1;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = RIFLES;     // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = COUNTERTERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 184; // 3.07 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -3;
    AllGuns[gunIndex].yMuzzleFlashOffset = 3;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 23;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0;
    AllGuns[gunIndex].name = "M4A1";
    gunIndex++;

    // SG 552
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.955;
    AllGuns[gunIndex].Damage = 32;
    AllGuns[gunIndex].texture = (void *)sg552_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3500;
    AllGuns[gunIndex].gunSound = SFX_SG552;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = RIFLES; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS;    // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 174; // 2.9 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 0;
    AllGuns[gunIndex].yMuzzleFlashOffset = 4;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 26;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.6;
    AllGuns[gunIndex].name = "SG 552";
    gunIndex++;

    // SG 550
    AllGuns[gunIndex].fireRate = 15; // 1 / 240  * 3600
    AllGuns[gunIndex].DamageFalloff = 0.98;
    AllGuns[gunIndex].Damage = 69;
    AllGuns[gunIndex].texture = (void *)sg550_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 4200;
    AllGuns[gunIndex].gunSound = SFX_SG550;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = RIFLES;     // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = COUNTERTERRORISTS; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 228; // 3.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -8;
    AllGuns[gunIndex].yMuzzleFlashOffset = 2;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 72.5;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 26;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.6;
    AllGuns[gunIndex].name = "SG 550";
    gunIndex++;

    // Scout
    AllGuns[gunIndex].fireRate = 75; // 1 / 48   * 3600
    AllGuns[gunIndex].DamageFalloff = 0.98;
    AllGuns[gunIndex].Damage = 74;
    AllGuns[gunIndex].texture = (void *)scout_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 2750;
    AllGuns[gunIndex].gunSound = SFX_SCOUT;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = 0;
    AllGuns[gunIndex].gunCategory = RIFLES; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;          // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 10;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 174; // 2.9 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -4;
    AllGuns[gunIndex].yMuzzleFlashOffset = 3;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 85;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 26;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.6;
    AllGuns[gunIndex].name = "Scout";
    gunIndex++;

    // G3SG1
    AllGuns[gunIndex].fireRate = 15; // 1 / 240 * 3600
    AllGuns[gunIndex].DamageFalloff = 0.98;
    AllGuns[gunIndex].Damage = 79;
    AllGuns[gunIndex].texture = (void *)g3sg1_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 5000;
    AllGuns[gunIndex].gunSound = SFX_G3SG1;
    AllGuns[gunIndex].WalkSpeed = MEDIUM_MEDIUM_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = RIFLES; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = TERRORISTS;    // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 282; // 4.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -4;
    AllGuns[gunIndex].yMuzzleFlashOffset = 4;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 82.5;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 26;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.6;
    AllGuns[gunIndex].name = "G3SG1";
    gunIndex++;

    // AWP
    AllGuns[gunIndex].fireRate = 88; // 1 / 41  * 3600
    AllGuns[gunIndex].DamageFalloff = 0.99;
    AllGuns[gunIndex].Damage = 115;
    AllGuns[gunIndex].texture = (void *)awp_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 4750;
    AllGuns[gunIndex].gunSound = SFX_AWP;
    AllGuns[gunIndex].WalkSpeed = HEAVY_WEAPON_WALK_SPEED;
    AllGuns[gunIndex].scopeId = 0;
    AllGuns[gunIndex].gunCategory = RIFLES; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].team = BOTH;          // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 10;
    AllGuns[gunIndex].MaxAmmoCount = 30;
    AllGuns[gunIndex].ReloadTime = 222; // 3.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -7;
    AllGuns[gunIndex].yMuzzleFlashOffset = 3;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 97.5;
    AllGuns[gunIndex].bulletCountPerShoot = 1;
    AllGuns[gunIndex].maxBotShootDisance = 30;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.7;
    AllGuns[gunIndex].name = "AWP";
    gunIndex++;

    // Reset cheapest guns cost list
    for (int i = 0; i < 4; i++)
    {
        cheapestGunsCostTerrorists[i] = 9999;
        cheapestGunsCostCounter[i] = 9999;
    }

    // Get cheapest and gun count for each category
    for (int i = 1; i < GunCount; i++)
    {
        Gun *gun = &AllGuns[i];

        if (gun->team == COUNTERTERRORISTS || gun->team == BOTH)
        {
            gunCountCounter[gun->gunCategory]++;
            // If the current gun price is lower than the cheapest gun
            if (gun->Price < cheapestGunsCostCounter[gun->gunCategory])
            {
                // Set the new cheapest gun price
                cheapestGunsCostCounter[gun->gunCategory] = gun->Price;
            }
        }
        if (gun->team == TERRORISTS || gun->team == BOTH)
        {
            gunCountTerrorists[gun->gunCategory]++;
            // If the current gun price is lower than the cheapest gun
            if (gun->Price < cheapestGunsCostTerrorists[gun->gunCategory])
            {
                // Set the new cheapest gun price
                cheapestGunsCostTerrorists[gun->gunCategory] = gun->Price;
            }
        }
    }
}

/**
 * @brief Reload the current gun of a player
 *
 * @param playerIndex Index of the player to reload
 */
void ReloadGun(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    // Check if the weapon in the player hands is a gun
    if (player->currentGunInInventory == 1 || player->currentGunInInventory == 2)
    {
        int ammoIndex = player->currentGunInInventory - 1;
        // Get how many ammo are missing
        int missingAmmoCount = getPlayerCurrentGun(player).MagazineCapacity - player->AllAmmoMagazine[ammoIndex].AmmoCount;
        if (missingAmmoCount < player->AllAmmoMagazine[ammoIndex].TotalAmmoCount)
        {
            player->AllAmmoMagazine[ammoIndex].AmmoCount += missingAmmoCount;
            if (!allPartyModes[currentPartyMode].infiniteGunAmmo)
                player->AllAmmoMagazine[ammoIndex].TotalAmmoCount -= missingAmmoCount;
        }
        else
        {
            player->AllAmmoMagazine[ammoIndex].AmmoCount += player->AllAmmoMagazine[ammoIndex].TotalAmmoCount;
            player->AllAmmoMagazine[ammoIndex].TotalAmmoCount = 0;
        }
    }
    if (player == localPlayer)
    {
        SendReloaded = true;
    }
}

/**
 * @brief Set the aim to the local player
 *
 */
void SetAim()
{
    doubleTapTimer = 0;
    if (getPlayerCurrentGunIndex(localPlayer) < GunCount && getPlayerCurrentGun(localPlayer).scopeId != -1)
    {
        if (CurrentScopeLevel < AllScopeLevels[getPlayerCurrentGun(localPlayer).scopeId].scopeCount)
        {
            // Change fov
            CurrentScopeLevel++;
            NE_SetFov(AllScopeLevels[getPlayerCurrentGun(localPlayer).scopeId].fov[CurrentScopeLevel - 1]);
            // Make scope sound
            PlayBasicSound(SFX_SCOPE);
        }
        else
        {
            DisableAim();
        }
    }
}

/**
 * @brief Apply gun y offset
 *
 */
void applyGunAnimation()
{
    // Set gun sprite offset position
    if (yOffset < 0.1)
        yOffset += (rand() % 2 + 2) / 100.0;
    else if (yOffset < 0.16)
        yOffset += (rand() % 2 + 2) / 150.0;
    else
        yOffset += (rand() % 2 + 2) / 200.0;
}

/**
 * @brief Disable aiming
 *
 */
void DisableAim()
{
    CurrentScopeLevel = 0;
    // Set default fov
    // TODO make const value
    NE_SetFov(70);
}

int getPlayerCurrentGunIndex(Player *player)
{
    return player->AllGunsInInventory[player->currentGunInInventory];
}

Gun getPlayerCurrentGun(Player *player)
{
    return AllGuns[player->AllGunsInInventory[player->currentGunInInventory]];
}

/**
 * @brief Buy a random gun of a category for a player
 *
 * @param gunCategory Gun category to buy
 * @param playerIndex Index of the player to buy the gun for
 */
void findGun(int gunCategory, int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    int gunFound = -1;
    while (gunFound == -1)
    {
        int random2;
        // Get a random index from the gun count of the category
        if (player->Team == COUNTERTERRORISTS)
        {
            random2 = rand() % gunCountCounter[gunCategory];
        }
        else
        {
            random2 = rand() % gunCountTerrorists[gunCategory];
        }

        int count = 0;
        for (int i2 = 0; i2 < GunCount; i2++)
        {
            if (AllGuns[i2].gunCategory == gunCategory && (AllGuns[i2].team == player->Team || AllGuns[i2].team == BOTH))
            {
                // if the player has enough money
                if (count == random2 && AllGuns[i2].Price <= player->Money)
                {
                    // Return the gun index
                    gunFound = i2;
                    break;
                }
                else
                    count++;
            }
        }
    }

    // Reduce money
    reducePlayerMoney(playerIndex, AllGuns[gunFound].Price);

    // Set the gun in the inventory
    if (gunCategory == 0)
    {
        AllPlayers[playerIndex].AllGunsInInventory[1] = gunFound;
        AllPlayers[playerIndex].currentGunInInventory = 1;
    }
    else
    {
        AllPlayers[playerIndex].AllGunsInInventory[2] = gunFound;
        AllPlayers[playerIndex].currentGunInInventory = 2;
    }

    // Reset new gun ammo
    ResetGunsAmmo(playerIndex);

    // Set the gun texture at screen
    if (playerIndex == 0)
    {
        UpdateGunTexture();
    }
}

/**
 * @brief Reset gun ammo (set ammo to max capacity)
 *
 * @param playerIndex Index of the player to reset the ammo for
 * @param inventoryIndex Index of the inventory to reset the ammo for
 */
void ResetGunAmmo(int playerIndex, int inventoryIndex)
{
    if (inventoryIndex < 1 || inventoryIndex > 2)
        return;

    Player *player = &AllPlayers[playerIndex];

    // Set the ammo to max capacity
    int ammoIndex = inventoryIndex - 1;
    player->AllAmmoMagazine[ammoIndex].AmmoCount = AllGuns[AllPlayers[playerIndex].AllGunsInInventory[inventoryIndex]].MagazineCapacity;
    player->AllAmmoMagazine[ammoIndex].TotalAmmoCount = AllGuns[AllPlayers[playerIndex].AllGunsInInventory[inventoryIndex]].MaxAmmoCount;
}

/**
 * @brief
 *
 * @param playerIndex
 */
void ResetGunsAmmo(int playerIndex)
{
    Player *player = &AllPlayers[playerIndex];
    for (int ammoIndex = 0; ammoIndex < 2; ammoIndex++)
    {
        int inventoryIndex = ammoIndex + 1;
        if (player->AllGunsInInventory[inventoryIndex] != -1)
        {
            // Set the ammo to max capacity
            player->AllAmmoMagazine[ammoIndex].AmmoCount = AllGuns[AllPlayers[playerIndex].AllGunsInInventory[inventoryIndex]].MagazineCapacity;
            player->AllAmmoMagazine[ammoIndex].TotalAmmoCount = AllGuns[AllPlayers[playerIndex].AllGunsInInventory[inventoryIndex]].MaxAmmoCount;
        }
    }
}

/**
 * @brief Set gun recoil
 *
 * @param player Player pointer to set the recoil for
 */
void setGunRecoil(Player *player)
{
    // If the gun is a dual gun, swap the recoil
    if (getPlayerCurrentGun(player).isDualGun)
        player->isRightGun = !player->isRightGun;
    else
        player->isRightGun = true;

    // Set the recoil
    if (player->isRightGun)
    {
        player->rightGunXRecoil = GunMaxRecoil;
        player->rightGunYRecoil = GunMaxRecoil;
    }
    else
    {
        player->leftGunXRecoil = GunMaxRecoil;
        player->leftGunYRecoil = GunMaxRecoil;
    }

    // Set muzzle flash and rumble
    if (player == &AllPlayers[CurrentCameraPlayer])
    {
        ShowMuzzle = 3;
        rumble(1);
    }
}

/**
 * @brief Stop gun reloading
 *
 * @param playerIndx Player index
 */
void StopReloading(int playerIndx)
{
    AllPlayers[playerIndx].isReloading = false;
}

/**
 * @brief Start gun reloading
 *
 * @param playerIndx Player index
 */
void startReloadGun(int playerIndx)
{
    Player *player = &AllPlayers[playerIndx];
    if (player->currentGunInInventory == 1 || player->currentGunInInventory == 2)
    {
        if (!player->isReloading && getPlayerCurrentGun(player).MagazineCapacity != player->AllAmmoMagazine[player->currentGunInInventory - 1].AmmoCount && player->AllAmmoMagazine[player->currentGunInInventory - 1].TotalAmmoCount != 0)
        {
            player->GunReloadWaitCount = 0;
            player->isReloading = true;
        }
    }
}

/**
 * @brief Update weapons/grenades texture
 *
 */
void UpdateGunTexture()
{
    Player *viewedPlayer = &AllPlayers[CurrentCameraPlayer];
    if (viewedPlayer->AllGunsInInventory[viewedPlayer->currentGunInInventory] != EMPTY)
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

        //  Load new texture
        if (getPlayerCurrentGunIndex(viewedPlayer) < GunCount)
            NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], getPlayerCurrentGun(viewedPlayer).texture, 1); // Load gun texture
        else if (getPlayerCurrentGunIndex(viewedPlayer) < GunCount + shopGrenadeCount)
            NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], AllGrenades[getPlayerCurrentGunIndex(viewedPlayer) - GunCount].texture, 1); // Load grenade texture
        else
            NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[1], Palettes[4], allEquipments[getPlayerCurrentGunIndex(viewedPlayer) - GunCount - shopGrenadeCount].texture, 1); // Load equipment texture
    }
}