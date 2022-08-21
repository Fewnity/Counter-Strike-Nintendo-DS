// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "equipment.h"

/**
 * @brief Load equipments data
 * Data source : https://counterstrike.fandom.com/wiki/Category:Equipment
 * @param equipment
 */
void LoadEquipments()
{
    int equipmentIndex = 0;

    // C4
    allEquipments[equipmentIndex].type = C4;                // Set equipment type
    allEquipments[equipmentIndex].Price = 0;                // Set equipment price
    allEquipments[equipmentIndex].texture = (void *)c4_bin; // Set equipment texture for shop
    allEquipments[equipmentIndex].team = TERRORISTS;        // Set equipment team
    allEquipments[equipmentIndex].isHided = true;           // Set if the equipment is hided from shop
    strcpy(allEquipments[equipmentIndex].name, "");         // Set equipment name
    strcpy(allEquipments[equipmentIndex].description, "");  // Set equipment description
    equipmentIndex++;

    // Defusekit
    allEquipments[equipmentIndex].type = DEFUSER;
    allEquipments[equipmentIndex].Price = 400;
    allEquipments[equipmentIndex].texture = (void *)defuser_bin;
    allEquipments[equipmentIndex].team = COUNTERTERRORISTS;
    strcpy(allEquipments[equipmentIndex].name, "Defuse kit");
    strcpy(allEquipments[equipmentIndex].description, "Decrease the defuse time of bombs by half");
    equipmentIndex++;

    // Kevlar vest
    allEquipments[equipmentIndex].type = KEVLAR_VEST;
    allEquipments[equipmentIndex].Price = 650;
    allEquipments[equipmentIndex].texture = (void *)kevlar_bin;
    allEquipments[equipmentIndex].team = BOTH;
    strcpy(allEquipments[equipmentIndex].name, "Kevlar vest");
    strcpy(allEquipments[equipmentIndex].description, "The Kevlar Vest protects the chest");
    equipmentIndex++;

    // Kevlar vest and helmet
    allEquipments[equipmentIndex].type = KEVLAR_VEST_AND_HELMET;
    allEquipments[equipmentIndex].Price = 1000;
    allEquipments[equipmentIndex].texture = (void *)kevlar_helmet_bin;
    allEquipments[equipmentIndex].team = BOTH;
    strcpy(allEquipments[equipmentIndex].name, "Kevlar + helmet");
    strcpy(allEquipments[equipmentIndex].description, "Protects the chest and the head");
    equipmentIndex++;
}
