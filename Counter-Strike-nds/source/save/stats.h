// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef STATS_H_ /* Include guard */
#define STATS_H_

extern int totalBotsKillCount;
extern int totalOnlinePlayersKillCount;
extern int totalDeathCount;
extern int totalFinishedParty;
extern int totalWins;
extern int totalPlayedSeconds;
extern int totalPlayedMinutes;
extern int totalPlayedHours;

void statsTimer();

#endif // DEBUG_H_