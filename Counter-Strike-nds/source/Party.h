#ifndef PARTY_H_ /* Include guard */
#define PARTY_H_

#include <NEMain.h>
#include "main.h"

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
} PartyMode;

extern PartyMode allPartyModes[2];
extern bool bombCheckedInA;
extern bool bombCheckedInB;
extern int currentDefuserIndex;
extern int bombPlantedAt;

void AddAllPartyModes();
void AddPartyMode(int index, bool MiddlePartyTeamSwap, int MaxRound, int StartMoney, int MaxMoney, int WinTheRoundMoney, int WinTheRoundBombMoney, int LoseTheRoundMoney, int LoseIncrease, int DefuseBombMoneyBonus, int PlantBombMoneyBonus, int PlantedBombLoseMoneyBonus, int KillPenalties, bool NoMoneyOnTimeEnd, bool TeamDamage, bool CanSeeOtherTeamView);

#endif // PARTY_H_