// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef EQUIPMENT_H_ /* Include guard */
#define EQUIPMENT_H_

//#include <NEMain.h>
//#include "main.h"

#define equipementCount 4 // 4 (bomb, defuser, kevlar, kevlar & headset)

enum EQUIPMENT_ENUM
{
    C4 = 0,
    DEFUSER = 1,
    KEVLAR_VEST = 2,
    KEVLAR_VEST_AND_HELMET = 3
};

typedef struct //
{
    void *texture;            // Equipment texture for shop
    int Price;                // Equipment price
    char name[20];            // Equipment name
    char description[50];     // Equipment description
    enum teamEnum team;       // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    bool isHided;             // Is hided from shop?
    enum EQUIPMENT_ENUM type; // Equipment type
} Equipment;

extern Equipment allEquipments[equipementCount];

void initEquipments();

#endif // EQUIPMENT_H_