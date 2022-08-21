// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef GRENADE_H_ /* Include guard */
#define GRENADE_H_

//#include <NEMain.h>
#include <maxmod9.h>
//#include "main.h"
//#include "main.h"
#define shopGrenadeCount 3

enum GRENADE_ENUM
{
    EXPLOSIVE = 0,
    SMOKE = 1,
    FLASH = 2
};

typedef struct //
{
    void *texture;
    int Price;
    char name[20];
    char description[50];
    enum GRENADE_ENUM type;
    int maxQuantity[3];
    mm_word collisionSound;
    mm_word finalSound;
    int team; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
} Grenade;

typedef struct
{
    NE_Model *Model;
    NE_Physics *Physic;
    NE_Model *EffectModel;
    int GrenadeType;
    int Timer;
    int EffectTimer;
    int effectAlpha;        //
    bool isVisible;         //
    int lastCollisionTimer; //
    int LastStairs;         //
    int ownerId;            //
} PhysicalGrenade;

extern Grenade AllGrenades[shopGrenadeCount];

void initGrenades();
PhysicalGrenade *CreateGrenade(int id, int ownerId);
void DeleteGrenade(int grenadeIndex);
void lanchGrenade(PhysicalGrenade *grenade, float xDirection, float yDirection, float zDirection, int xPosition, int yPosition, int zPosition);
void UpdateGrenades();
void DeleteAllGrenade();

#endif // GRENADE_H_