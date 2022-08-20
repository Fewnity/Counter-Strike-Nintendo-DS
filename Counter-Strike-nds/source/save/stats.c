// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "stats.h"
#include "network.h"

int totalBotsKillCount = 0;
int totalOnlinePlayersKillCount = 0;
int totalDeathCount = 0;
int totalFinishedParty = 0;
int totalPlayedSeconds = 0;
int totalPlayedMinutes = 0;
int totalPlayedHours = 0;
int totalWins = 0;

void statsTimer()
{
    if (frameCount % 60 == 0 && Connection != UNSELECTED)
    {
        totalPlayedSeconds++;
        if (totalPlayedSeconds == 60)
        {
            totalPlayedSeconds = 0;
            totalPlayedMinutes++;
        }
        if (totalPlayedMinutes == 60)
        {
            totalPlayedMinutes = 0;
            totalPlayedHours++;
        }
    }
}