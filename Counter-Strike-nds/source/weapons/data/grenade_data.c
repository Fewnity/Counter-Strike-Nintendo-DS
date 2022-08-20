// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "grenade.h"

/**
 * @brief Load grenades data
 * Data source : https://counterstrike.fandom.com/wiki/Category:Grenades
 * @param equipment
 */
void LoadGrenades()
{
    int grenadeIndex = 0;

    AllGrenades[grenadeIndex].type = EXPLOSIVE;
    AllGrenades[grenadeIndex].Price = 300;
    AllGrenades[grenadeIndex].texture = (void *)fraggrenade_bin;
    AllGrenades[grenadeIndex].team = BOTH;
    AllGrenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE;
    AllGrenades[grenadeIndex].finalSound = SFX_HEGRENADE_EXPLOSE;
    AllGrenades[grenadeIndex].maxQuantity[0] = 1;
    AllGrenades[grenadeIndex].maxQuantity[1] = 1;
    AllGrenades[grenadeIndex].maxQuantity[2] = 1;
    AllGrenades[grenadeIndex].name = "Explosive grenade";
    AllGrenades[grenadeIndex].description = "The explosive grenade administers high damage";
    grenadeIndex++;

    AllGrenades[grenadeIndex].type = SMOKE;
    AllGrenades[grenadeIndex].Price = 300;
    AllGrenades[grenadeIndex].texture = (void *)smokegrenade_bin;
    AllGrenades[grenadeIndex].team = BOTH;
    AllGrenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE; // PAS BON?
    AllGrenades[grenadeIndex].finalSound = SFX_SMOKE_EMIT;
    AllGrenades[grenadeIndex].maxQuantity[0] = 1;
    AllGrenades[grenadeIndex].maxQuantity[1] = 1;
    AllGrenades[grenadeIndex].maxQuantity[2] = 1;
    AllGrenades[grenadeIndex].name = "Smoke grenade";
    AllGrenades[grenadeIndex].description = "The smoke grenade creates an area smoke screen.";
    grenadeIndex++;

    AllGrenades[grenadeIndex].type = FLASH;
    AllGrenades[grenadeIndex].Price = 200;
    AllGrenades[grenadeIndex].texture = (void *)flashthrowed_bin;
    AllGrenades[grenadeIndex].team = BOTH;
    AllGrenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE;
    AllGrenades[grenadeIndex].finalSound = SFX_FLASHBANG_EXPLODE;
    AllGrenades[grenadeIndex].maxQuantity[0] = 2;
    AllGrenades[grenadeIndex].maxQuantity[1] = 1;
    AllGrenades[grenadeIndex].maxQuantity[2] = 1;
    AllGrenades[grenadeIndex].name = "Flash";
    AllGrenades[grenadeIndex].description = "The flashbang grenade temporarily blinds anybody";
    grenadeIndex++;
}
