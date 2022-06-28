// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "equipment.h"

// Cost of the cheapest equipment
int cheapestEquipmentCost = 9999;

// Loaded equipment data
Equipment allEquipments[equipementCount];

/**
 * @brief Load equipments data
 * Data source : https://counterstrike.fandom.com/wiki/Category:Equipment
 * @param equipment
 */
void LoadEquipments(Equipment *equipment)
{
    int equipmentIndex = 0;

    // C4
    equipment[equipmentIndex].type = C4;                    // Set equipment type
    equipment[equipmentIndex].Price = 0;                    // Set equipment price
    equipment[equipmentIndex].texture = (void *)c4_bin;     // Set equipment texture for shop
    equipment[equipmentIndex].team = TERRORISTS;            // Set equipment team
    equipment[equipmentIndex].isHided = true;               // Set if the equipment is hided from shop
    strncpy(equipment[equipmentIndex].name, "", 20);        // Set equipment name
    strncpy(equipment[equipmentIndex].description, "", 50); // Set equipment description
    equipmentIndex++;

    // Defusekit
    equipment[equipmentIndex].type = DEFUSER;
    equipment[equipmentIndex].Price = 400;
    equipment[equipmentIndex].texture = (void *)defuser_bin;
    equipment[equipmentIndex].team = COUNTERTERRORISTS;
    strncpy(equipment[equipmentIndex].name, "Defuse kit", 20);
    strncpy(equipment[equipmentIndex].description, "Decrease the defuse time of bombs by half", 50);
    equipmentIndex++;

    // Kevlar vest
    equipment[equipmentIndex].type = KEVLAR_VEST;
    equipment[equipmentIndex].Price = 650;
    equipment[equipmentIndex].texture = (void *)kevlar_bin;
    equipment[equipmentIndex].team = BOTH;
    strncpy(equipment[equipmentIndex].name, "Kevlar vest", 20);
    strncpy(equipment[equipmentIndex].description, "The Kevlar Vest protects the chest", 50);
    equipmentIndex++;

    // Kevlar vest and helmet
    equipment[equipmentIndex].type = KEVLAR_VEST_AND_HELMET;
    equipment[equipmentIndex].Price = 1000;
    equipment[equipmentIndex].texture = (void *)kevlar_helmet_bin;
    equipment[equipmentIndex].team = BOTH;
    strncpy(equipment[equipmentIndex].name, "Kevlar + helmet", 20);
    strncpy(equipment[equipmentIndex].description, "Protects the chest and the head", 50);
    equipmentIndex++;
}
