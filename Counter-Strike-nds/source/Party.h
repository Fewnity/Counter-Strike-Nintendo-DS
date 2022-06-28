// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef PARTY_H_ /* Include guard */
#define PARTY_H_

#include <NEMain.h>
#include "main.h"

enum RoundState
{
    TRAINING = -1,
    WAIT_START = 0,
    PLAYING = 1,
    END_ROUND = 2,
    END = 3,
};

enum PartyModeName
{
    NO_DEFAULT_PARTY_MODE = -1,
    COMPETITIVE = 0,
    OCCASIONAL = 1,
    TUTORIAL_TRAINING = 2
};

typedef struct //
{
    bool middlePartyTeamSwap;
    int maxRound;
    int startMoney;
    int maxMoney;

    int winTheRoundMoney;
    int winTheRoundBombMoney;
    int loseTheRoundMoney;
    int loseIncrease;
    int defuseBombMoneyBonus;
    int plantBombMoneyBonus;

    int plantedBombLoseMoneyBonus;
    int killPenalties;

    bool teamDamage;
    bool canSeeOtherTeamView;
    bool noMoneyOnTimeEnd;

    int trainingMinutesDuration;
    int trainingSecondsDuration;

    int startRoundMinutesDuration;
    int startRoundSecondsDuration;

    int roundMinutesDuration;
    int roundSecondsDuration;

    int endRoundMinutesDuration;
    int endRoundSecondsDuration;

    int bombWaitingMinutesDuration;
    int bombWaitingSecondsDuration;

    int trainingRespawnMinutesDuration;
    int trainingRespawnSecondsDuration;

    bool spawnWithArmor;
    bool infiniteMoney;
    bool infiniteTimer;
    bool noScore;
    bool limitedShopByZoneAndTimer;
    bool infiniteGunAmmo;
} PartyMode;

extern PartyMode allPartyModes[3];
extern int currentDefuserIndex;
extern int bombPlantedAt;
extern int PartyMinutes;
extern int PartySeconds;
extern enum RoundState roundState;
extern int CounterScore;
extern int TerroristsScore;
extern int BombSeconds;
extern Vector4 droppedBombPositionAndRotation;
extern bool bombDropped;
extern int currentPartyMode;
extern int currentMap;
extern bool partyFinished;

void AddAllPartyModes();
void AddPartyMode(int index, bool MiddlePartyTeamSwap, int MaxRound, int StartMoney, int MaxMoney, int WinTheRoundMoney, int WinTheRoundBombMoney, int LoseTheRoundMoney, int LoseIncrease, int DefuseBombMoneyBonus, int PlantBombMoneyBonus, int PlantedBombLoseMoneyBonus, int KillPenalties, bool NoMoneyOnTimeEnd, bool TeamDamage, bool CanSeeOtherTeamView);
void partyTimerTick();
void partyTimerTickOnline();
void CheckTeamDeathCount(int *TerroristsCount, int *CounterTerroristsCount, int *TerroristDeadCount, int *CounterDeadCount);
void CheckAfterRound();
void finishParty();
void onNewRoundStart();
void setEndRound();

#endif // PARTY_H_