// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "gun.h"

#define HEAVY_WEAPON_WALK_SPEED 175
#define MEDIUM_HIGH_WEAPON_WALK_SPEED 180
#define MEDIUM_MEDIUM_WEAPON_WALK_SPEED 185
#define MEDIUM_WEAPON_WALK_SPEED 195
#define LIGHT_WEAPON_WALK_SPEED 210

/**
 * @brief Load guns data
 * Data source : https://counterstrike.fandom.com/wiki/Category:Weapons
 * @param equipment
 */
void LoadGuns()
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
    AllGuns[gunIndex].maxBotShootDisance = 32;
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
    AllGuns[gunIndex].maxBotShootDisance = 32;
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
    AllGuns[gunIndex].maxBotShootDisance = 32;
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
    AllGuns[gunIndex].maxBotShootDisance = 32;
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
    AllGuns[gunIndex].maxBotShootDisance = 36;
    AllGuns[gunIndex].inaccuracyReductionForBot = 0.7;
    AllGuns[gunIndex].name = "AWP";
    gunIndex++;
}
