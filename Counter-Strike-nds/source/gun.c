#include "main.h"
#include "gun.h"

Gun AllGuns[GunCount];
Equipement AllEquipements[equipementCount];

int cheapestGunsCostCounter[4];
int cheapestGunsCostTerrorists[4];
int gunCountCounter[4];
int gunCountTerrorists[4];

// Data sources https://counterstrike.fandom.com/wiki/Category:Weapons

void AddGuns()
{
    int gunIndex = 0;
    // M4A4
    /*AllGuns[gunIndex].fireRate = 5; //60 * 0.09
    AllGuns[gunIndex].DamageFalloff = 3;
    AllGuns[gunIndex].Damage = 33;
    AllGuns[gunIndex].texture = (void *)gunSprite13_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3100;
    AllGuns[gunIndex].gunSound = SFX_M4A1;
    AllGuns[gunIndex].WalkSpeed = 200;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 3;            //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 186; //3.1 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 0;
    AllGuns[gunIndex].yMuzzleFlashOffset = 0;
    AllGuns[gunIndex].isBigGun = true;
    strncpy(AllGuns[gunIndex].name, "M4A4", 10);
    gunIndex++;

    //Glock
    AllGuns[gunIndex].fireRate = 9; //60 * 0.150
    AllGuns[gunIndex].DamageFalloff = 20;
    AllGuns[gunIndex].Damage = 15;
    AllGuns[gunIndex].texture = (void *)gunSprite5_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 200;
    AllGuns[gunIndex].gunSound = SFX_GLOCK;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;            //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 136; //2.27 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 12;
    AllGuns[gunIndex].yMuzzleFlashOffset = 0;
    AllGuns[gunIndex].isBigGun = false;
    strncpy(AllGuns[gunIndex].name, "Glock", 10);
    gunIndex++;

    //USP-S
    AllGuns[gunIndex].fireRate = 10; //60 * 0.170
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 35;
    AllGuns[gunIndex].texture = (void *)gunSprite7_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 200;
    AllGuns[gunIndex].gunSound = SFX_USP;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;            //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 12;
    AllGuns[gunIndex].MaxAmmoCount = 24;
    AllGuns[gunIndex].ReloadTime = 132; //2.2 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    strncpy(AllGuns[gunIndex].name, "USP-S", 10);
    gunIndex++;

    //MP9
    AllGuns[gunIndex].fireRate = 4; //60 * 0.070
    AllGuns[gunIndex].DamageFalloff = 13;
    AllGuns[gunIndex].Damage = 26;
    AllGuns[gunIndex].texture = (void *)gunSprite18_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1250;
    AllGuns[gunIndex].gunSound = SFX_MP9;
    AllGuns[gunIndex].WalkSpeed = 215;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;            //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 126; //2.1 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 3;
    AllGuns[gunIndex].yMuzzleFlashOffset = 6;
    AllGuns[gunIndex].isBigGun = true;
    strncpy(AllGuns[gunIndex].name, "MP9", 10);
    gunIndex++;

    //MP7
    AllGuns[gunIndex].fireRate = 5; //60 * 0.080
    AllGuns[gunIndex].DamageFalloff = 15;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)gunSprite18_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1500;
    AllGuns[gunIndex].gunSound = SFX_MP7;
    AllGuns[gunIndex].WalkSpeed = 215;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;             //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 186; //3.1 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 13;
    AllGuns[gunIndex].yMuzzleFlashOffset = 13;
    AllGuns[gunIndex].isBigGun = true;
    strncpy(AllGuns[gunIndex].name, "MP7", 10);
    gunIndex++;

    //Shootgun XM1014
    AllGuns[gunIndex].fireRate = 21; //60 * 0.350
    AllGuns[gunIndex].DamageFalloff = 30;
    AllGuns[gunIndex].Damage = 100;
    AllGuns[gunIndex].texture = (void *)gunSprite16_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 2000;
    AllGuns[gunIndex].gunSound = SFX_XM1014;
    AllGuns[gunIndex].WalkSpeed = 185;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 1;             //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 7;
    AllGuns[gunIndex].MaxAmmoCount = 32;
    AllGuns[gunIndex].ReloadTime = 168; //2.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 9;
    AllGuns[gunIndex].yMuzzleFlashOffset = 8;
    AllGuns[gunIndex].isBigGun = true;
    strncpy(AllGuns[gunIndex].name, "XM1014", 10);
    gunIndex++;

    //Shootgun 2 Nova
    AllGuns[gunIndex].fireRate = 51; //60 * 0.850
    AllGuns[gunIndex].DamageFalloff = 30;
    AllGuns[gunIndex].Damage = 234;
    AllGuns[gunIndex].texture = (void *)gunSprite15_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 1050;
    AllGuns[gunIndex].gunSound = SFX_NOVA;
    AllGuns[gunIndex].WalkSpeed = 200;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 1;             //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 8;
    AllGuns[gunIndex].MaxAmmoCount = 32;
    AllGuns[gunIndex].ReloadTime = 108; //1.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 9;
    AllGuns[gunIndex].yMuzzleFlashOffset = 8;
    AllGuns[gunIndex].isBigGun = true;
    strncpy(AllGuns[gunIndex].name, "Nova", 10);
    gunIndex++;

    //Sniper AWP
    AllGuns[gunIndex].fireRate = 87; //60 * 1.455
    AllGuns[gunIndex].DamageFalloff = 80;
    AllGuns[gunIndex].Damage = 115;
    AllGuns[gunIndex].texture = (void *)gunSprite22_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 4750;
    AllGuns[gunIndex].gunSound = SFX_AWP;
    AllGuns[gunIndex].WalkSpeed = 180;
    AllGuns[gunIndex].scopeId = 0;
    AllGuns[gunIndex].gunCategory = 3;             //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 10;
    AllGuns[gunIndex].MaxAmmoCount = 30;
    AllGuns[gunIndex].ReloadTime = 186; //2.1 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = -13;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    strncpy(AllGuns[gunIndex].name, "AWP", 10);
    gunIndex++;

    //Knife
    AllGuns[gunIndex].fireRate = 24; //60 * 0.4
    AllGuns[gunIndex].Damage = 40;
    AllGuns[gunIndex].texture = (void *)gunSprite1_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].gunSound = SFX_KNIFE_HIT_WALL;
    AllGuns[gunIndex].WalkSpeed = 220;
    AllGuns[gunIndex].scopeId = 0;
    AllGuns[gunIndex].gunCategory = -1;            //0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].ReloadTime = 0;              //2.1 * 60
    AllGuns[gunIndex].isKnife = true;
    strncpy(AllGuns[gunIndex].name, "Knife", 10);
    gunIndex++;*/

    // A revoir, scopeId, gunCategory, holdFireButton, MuzzleFlashOffset

    //
    AllGuns[gunIndex].fireRate = 24; // 60 * 0.4
    AllGuns[gunIndex].Damage = 40;
    AllGuns[gunIndex].texture = (void *)knifect_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].gunSound = SFX_KNIFE_HIT_WALL;
    AllGuns[gunIndex].WalkSpeed = 220;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = -1;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].ReloadTime = 0;              // 2.1 * 60
    AllGuns[gunIndex].isKnife = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.07;
    AllGuns[gunIndex].penetration = 85;
    strncpy(AllGuns[gunIndex].name, "Knife", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 13; // 1 / 267 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 53;
    AllGuns[gunIndex].texture = (void *)deagle_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 650;
    AllGuns[gunIndex].gunSound = SFX_DEAGLE;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 7;
    AllGuns[gunIndex].MaxAmmoCount = 35;
    AllGuns[gunIndex].ReloadTime = 132; // 2.2 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    strncpy(AllGuns[gunIndex].name, "Desert Eagle", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 5; // 1 / 750 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 43;
    AllGuns[gunIndex].texture = (void *)elite_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 800;
    AllGuns[gunIndex].gunSound = SFX_ELITE;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 226; // 3.77 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 52;
    strncpy(AllGuns[gunIndex].name, "Dual Berettas", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 24;
    AllGuns[gunIndex].texture = (void *)fivesevent_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 750;
    AllGuns[gunIndex].gunSound = SFX_FIVESEVEN;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 192; // 3.2 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    strncpy(AllGuns[gunIndex].name, "Five SeveN", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 24;
    AllGuns[gunIndex].texture = (void *)glock18_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 400;
    AllGuns[gunIndex].gunSound = SFX_GLOCK18;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 136; // 2.27 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 52;
    strncpy(AllGuns[gunIndex].name, "Glock-18", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 31;
    AllGuns[gunIndex].texture = (void *)p228_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 600;
    AllGuns[gunIndex].gunSound = SFX_P228;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 13;
    AllGuns[gunIndex].MaxAmmoCount = 52;
    AllGuns[gunIndex].ReloadTime = 162; // 2.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 62.5;
    strncpy(AllGuns[gunIndex].name, "P228", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 9; // 1 / 400 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)uspsilencer_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 500;
    AllGuns[gunIndex].gunSound = SFX_USP;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 0;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 12;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 162; // 2.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = false;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    strncpy(AllGuns[gunIndex].name, "USP", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 53; // 1 / 68 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 171;
    AllGuns[gunIndex].texture = (void *)m3super90_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 1700;
    AllGuns[gunIndex].gunSound = SFX_M3;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 1;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 8;
    AllGuns[gunIndex].MaxAmmoCount = 48;
    AllGuns[gunIndex].ReloadTime = 276; // 4.6 * 60 from empty
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    strncpy(AllGuns[gunIndex].name, "M3", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 15; // 1 / 240 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 171;
    AllGuns[gunIndex].texture = (void *)xm1014_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 3000;
    AllGuns[gunIndex].gunSound = SFX_XM1014;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 1;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 7;
    AllGuns[gunIndex].MaxAmmoCount = 32;
    AllGuns[gunIndex].ReloadTime = 168; // 2.8 * 60 from empty
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    strncpy(AllGuns[gunIndex].name, "XM1014", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 5; // 1 / 750 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 35;
    AllGuns[gunIndex].texture = (void *)m249_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 5750;
    AllGuns[gunIndex].gunSound = SFX_M249;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 1;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 100;
    AllGuns[gunIndex].MaxAmmoCount = 200;
    AllGuns[gunIndex].ReloadTime = 342; // 5.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    strncpy(AllGuns[gunIndex].name, "M249", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 4; // 1 / 857  * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 28;
    AllGuns[gunIndex].texture = (void *)mac10_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1400;
    AllGuns[gunIndex].gunSound = SFX_MAC10;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 156; // 2.6 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 47.5;
    strncpy(AllGuns[gunIndex].name, "MAC-10", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 5; // 1 / 750 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 25;
    AllGuns[gunIndex].texture = (void *)mp5_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1500;
    AllGuns[gunIndex].gunSound = SFX_MP5;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 180; // 3 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    strncpy(AllGuns[gunIndex].name, "MP5", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 4; // 1 / 857 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 25;
    AllGuns[gunIndex].texture = (void *)p90_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2350;
    AllGuns[gunIndex].gunSound = SFX_P90;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 50;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 198; // 3.3 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 75;
    strncpy(AllGuns[gunIndex].name, "P90", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 4; // 1 / 857  * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 19;
    AllGuns[gunIndex].texture = (void *)tmp_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1250;
    AllGuns[gunIndex].gunSound = SFX_TMP;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 120;
    AllGuns[gunIndex].ReloadTime = 126; // 2.1 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    strncpy(AllGuns[gunIndex].name, "TMP", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 571   * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)ump45_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 1250;
    AllGuns[gunIndex].gunSound = SFX_UMP45;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 2;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 25;
    AllGuns[gunIndex].MaxAmmoCount = 100;
    AllGuns[gunIndex].ReloadTime = 210; // 3.5 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 50;
    strncpy(AllGuns[gunIndex].name, "UMP-45", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 600 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 35;
    AllGuns[gunIndex].texture = (void *)ak47_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2500;
    AllGuns[gunIndex].gunSound = SFX_AK47;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 205; // 2.43 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 77.5;
    strncpy(AllGuns[gunIndex].name, "AK-47", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 31;
    AllGuns[gunIndex].texture = (void *)aug_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3500;
    AllGuns[gunIndex].gunSound = SFX_AUG;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 228; // 3.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    strncpy(AllGuns[gunIndex].name, "AUG", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)famas_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2250;
    AllGuns[gunIndex].gunSound = SFX_FAMAS;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 25;
    AllGuns[gunIndex].MaxAmmoCount = 75;
    AllGuns[gunIndex].ReloadTime = 228; // 3.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    strncpy(AllGuns[gunIndex].name, "FAMAS", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 29;
    AllGuns[gunIndex].texture = (void *)galil_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 2000;
    AllGuns[gunIndex].gunSound = SFX_GALIL;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 35;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 198; // 3.3 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 77;
    strncpy(AllGuns[gunIndex].name, "Galil", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 31;
    AllGuns[gunIndex].texture = (void *)m4a1_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3100;
    AllGuns[gunIndex].gunSound = SFX_M4A1;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = -1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 184; // 3.07 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    strncpy(AllGuns[gunIndex].name, "M4A1", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 6; // 1 / 666 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 32;
    AllGuns[gunIndex].texture = (void *)sg552_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 3500;
    AllGuns[gunIndex].gunSound = SFX_SG552;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 174; // 2.9 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 70;
    strncpy(AllGuns[gunIndex].name, "SG 552", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 15; // 1 / 240  * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 69;
    AllGuns[gunIndex].texture = (void *)sg550_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 4200;
    AllGuns[gunIndex].gunSound = SFX_SG550;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 30;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 228; // 3.8 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 72.5;
    strncpy(AllGuns[gunIndex].name, "SG 550", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 75; // 1 / 48   * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 74;
    AllGuns[gunIndex].texture = (void *)scout_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 2750;
    AllGuns[gunIndex].gunSound = SFX_SCOUT;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = 0;
    AllGuns[gunIndex].gunCategory = 3;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 10;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 174; // 2.9 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 85;
    strncpy(AllGuns[gunIndex].name, "Scout", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 15; // 1 / 240 * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 79;
    AllGuns[gunIndex].texture = (void *)g3sg1_bin;
    AllGuns[gunIndex].holdFireButton = true;
    AllGuns[gunIndex].Price = 5000;
    AllGuns[gunIndex].gunSound = SFX_G3SG1;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = 1;
    AllGuns[gunIndex].gunCategory = 3;            // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = 0; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 20;
    AllGuns[gunIndex].MaxAmmoCount = 90;
    AllGuns[gunIndex].ReloadTime = 282; // 4.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 82.5;
    strncpy(AllGuns[gunIndex].name, "G3SG1", 20);
    gunIndex++;

    //
    AllGuns[gunIndex].fireRate = 88; // 1 / 41  * 3600
    AllGuns[gunIndex].DamageFalloff = 9;
    AllGuns[gunIndex].Damage = 115;
    AllGuns[gunIndex].texture = (void *)awp_bin;
    AllGuns[gunIndex].holdFireButton = false;
    AllGuns[gunIndex].Price = 4750;
    AllGuns[gunIndex].gunSound = SFX_AWP;
    AllGuns[gunIndex].WalkSpeed = 210;
    AllGuns[gunIndex].scopeId = 0;
    AllGuns[gunIndex].gunCategory = 3;             // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
    AllGuns[gunIndex].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[gunIndex].MagazineCapacity = 10;
    AllGuns[gunIndex].MaxAmmoCount = 30;
    AllGuns[gunIndex].ReloadTime = 222; // 3.7 * 60
    AllGuns[gunIndex].xMuzzleFlashOffset = 4;
    AllGuns[gunIndex].yMuzzleFlashOffset = -7;
    AllGuns[gunIndex].isBigGun = true;
    AllGuns[gunIndex].MaxSoundDistance = 0.25;
    AllGuns[gunIndex].penetration = 97.5;
    strncpy(AllGuns[gunIndex].name, "AWP", 20);
    gunIndex++;

    for (int i = 0; i < 4; i++)
    {
        cheapestGunsCostTerrorists[i] = 9999;
        cheapestGunsCostCounter[i] = 9999;
    }

    for (int i = 1; i < GunCount; i++)
    {
        if (AllGuns[i].isForCounterTerrorists == 1 || AllGuns[i].isForCounterTerrorists == -1)
        {
            gunCountCounter[AllGuns[i].gunCategory]++;
            if (AllGuns[i].Price < cheapestGunsCostCounter[AllGuns[i].gunCategory])
            {
                cheapestGunsCostCounter[AllGuns[i].gunCategory] = AllGuns[i].Price;
            }
        }
        if (AllGuns[i].isForCounterTerrorists == 0 || AllGuns[i].isForCounterTerrorists == -1)
        {
            gunCountTerrorists[AllGuns[i].gunCategory]++;
            if (AllGuns[i].Price < cheapestGunsCostTerrorists[AllGuns[i].gunCategory])
            {
                cheapestGunsCostTerrorists[AllGuns[i].gunCategory] = AllGuns[i].Price;
            }
        }

        /*if (AllGuns[i].gunCategory == 0 && AllGuns[i].Price < cheapestPistolCost)
        {
            cheapestPistolCost = AllGuns[i].Price;
        }
        else if (AllGuns[i].gunCategory == 1 && AllGuns[i].Price < cheapestHeavyCost)
        {
            cheapestHeavyCost = AllGuns[i].Price;
        }
        else if (AllGuns[i].gunCategory == 2 && AllGuns[i].Price < cheapestSmgCost)
        {
            cheapestSmgCost = AllGuns[i].Price;
        }
        else if (AllGuns[i].gunCategory == 3 && AllGuns[i].Price < cheapestRifleCost)
        {
            cheapestRifleCost = AllGuns[i].Price;
        }*/
    }

    /// 1 * RPM * 3600
    // 60 = 1 sec
    //

    /*AllGuns[0].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[1].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[2].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[3].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[4].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[5].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[6].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[7].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    AllGuns[8].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams*/
}

void ReloadGun(int playerId)
{
    // Check if the weapon in the player hands is a gun
    if (AllPlayers[playerId].currentGunInInventory == 1 || AllPlayers[playerId].currentGunInInventory == 2)
    {
        int missingAmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[AllPlayers[playerId].currentGunInInventory]].MagazineCapacity - AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount;
        if (missingAmmoCount < AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount)
        {
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount += missingAmmoCount;
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount -= missingAmmoCount;
        }
        else
        {
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount += AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount;
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount = 0;
        }
    }
    /*return;
    if (AllGuns[AllPlayers[playerId].AllGunsInInventory[AllPlayers[playerId].currentGunInInventory]].MagazineCapacity <= AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount)
    {
        AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount -= AllGuns[AllPlayers[playerId].AllGunsInInventory[AllPlayers[playerId].currentGunInInventory]].MagazineCapacity - AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount;
        AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[AllPlayers[playerId].currentGunInInventory]].MagazineCapacity;
    }
    else
    {
        int missingAmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[AllPlayers[playerId].currentGunInInventory]].MagazineCapacity - AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount;
        if (missingAmmoCount < AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount)
        {
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount += missingAmmoCount;
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount -= missingAmmoCount;
        }
        else
        {
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].AmmoCount += AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount;
            AllPlayers[playerId].AllAmmoMagazine[AllPlayers[playerId].currentGunInInventory - 1].TotalAmmoCount = 0;
        }
    }*/
}

void SetAim()
{
    doubleTapTimer = 0;
    if (localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory] < GunCount && AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].scopeId != -1)
    {
        if (CurrentScopeLevel < AllScopeLevels[AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].scopeId].scopeCount)
        {
            CurrentScopeLevel++;
            NE_SetFov(AllScopeLevels[AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].scopeId].fov[CurrentScopeLevel - 1]);
        }
        else
        {
            CurrentScopeLevel = 0;
            NE_SetFov(70);
        }
    }
}

void findGun(int gunCategory, int playerIndex)
{
    int gunFound = -1;
    while (gunFound == -1)
    {
        int random2;
        if (AllPlayers[playerIndex].Team == COUNTERTERRORISTS)
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
            if (AllGuns[i2].gunCategory == gunCategory)
            {
                if (count == random2 && AllGuns[i2].Price <= AllPlayers[playerIndex].Money)
                {
                    gunFound = i2;
                    break;
                }
                else
                    count++;
            }
        }
    }
    AllPlayers[playerIndex].Money -= AllGuns[gunFound].Price;

    if (gunCategory == 0)
        AllPlayers[playerIndex].AllGunsInInventory[1] = gunFound;
    else
        AllPlayers[playerIndex].AllGunsInInventory[2] = gunFound;

    ResetGunsAmmo(playerIndex);
    if (playerIndex == 0)
        UpdateGunTexture();
}

void ResetGunAmmo(int playerId, int inventoryIndex)
{
    if (inventoryIndex < 1 || inventoryIndex > 2)
        return;

    AllPlayers[playerId].AllAmmoMagazine[inventoryIndex - 1].AmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[inventoryIndex]].MagazineCapacity;
    AllPlayers[playerId].AllAmmoMagazine[inventoryIndex - 1].TotalAmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[inventoryIndex]].MaxAmmoCount;
}

void ResetGunsAmmo(int playerId)
{
    for (int i = 0; i < 2; i++)
    {
        if (AllPlayers[playerId].AllGunsInInventory[i + 1] != -1)
        {
            AllPlayers[playerId].AllAmmoMagazine[i].AmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[i + 1]].MagazineCapacity;
            AllPlayers[playerId].AllAmmoMagazine[i].TotalAmmoCount = AllGuns[AllPlayers[playerId].AllGunsInInventory[i + 1]].MaxAmmoCount;
        }
    }
}

void shoot()
{
}