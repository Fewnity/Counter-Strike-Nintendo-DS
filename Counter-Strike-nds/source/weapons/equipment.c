// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "equipment.h"
#include "data/equipment_data.h"

// Cost of the cheapest equipment
int cheapestEquipmentCost = 9999;

// Loaded equipment data
Equipment allEquipments[equipementCount];

void initEquipments()
{
    LoadEquipments();
}