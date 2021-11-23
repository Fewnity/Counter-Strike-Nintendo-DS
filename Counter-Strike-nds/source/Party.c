#include "main.h"
#include "party.h"

PartyMode allPartyModes[2];
bool bombCheckedInA;
bool bombCheckedInB;
int bombPlantedAt = -1;
int currentDefuserIndex = -1;

void AddAllPartyModes()
{
    // Competitive
    AddPartyMode(0, true, 30, 800, 16000, 3250, 3500, 1400, 500, 300, 300, 800, 300, true, true, false);
    allPartyModes[0].trainingMinutesDuration = 0;
    allPartyModes[0].trainingSecondsDuration = 20;

    allPartyModes[0].startRoundMinutesDuration = 0;
    allPartyModes[0].startRoundSecondsDuration = 15;

    allPartyModes[0].roundMinutesDuration = 1;
    allPartyModes[0].roundSecondsDuration = 55;

    allPartyModes[0].endRoundMinutesDuration = 0;
    allPartyModes[0].endRoundSecondsDuration = 5;

    allPartyModes[0].bombWaitingMinutesDuration = 0;
    allPartyModes[0].bombWaitingSecondsDuration = 40;

    allPartyModes[0].trainingRespawnMinutesDuration = 0;
    allPartyModes[0].trainingRespawnSecondsDuration = 5;

    allPartyModes[0].spawnWithArmor = false;

    // Casual
    AddPartyMode(1, false, 15, 1000, 10000, 2700, 2700, 2400, 0, 200, 200, 200, 0, false, false, true);
    allPartyModes[1].trainingMinutesDuration = 0;
    allPartyModes[1].trainingSecondsDuration = 20;

    allPartyModes[1].startRoundMinutesDuration = 0;
    allPartyModes[1].startRoundSecondsDuration = 15;

    allPartyModes[1].roundMinutesDuration = 2;
    allPartyModes[1].roundSecondsDuration = 15;

    allPartyModes[1].endRoundMinutesDuration = 0;
    allPartyModes[1].endRoundSecondsDuration = 5;

    allPartyModes[1].bombWaitingMinutesDuration = 0;
    allPartyModes[1].bombWaitingSecondsDuration = 40;

    allPartyModes[1].trainingRespawnMinutesDuration = 0;
    allPartyModes[1].trainingRespawnSecondsDuration = 5;

    allPartyModes[1].spawnWithArmor = true;
}

void AddPartyMode(int index, bool MiddlePartyTeamSwap, int MaxRound, int StartMoney, int MaxMoney, int WinTheRoundMoney, int WinTheRoundBombMoney, int LoseTheRoundMoney, int LoseIncrease, int DefuseBombMoneyBonus, int PlantBombMoneyBonus, int PlantedBombLoseMoneyBonus, int KillPenalties, bool NoMoneyOnTimeEnd, bool TeamDamage, bool CanSeeOtherTeamView)
{
    PartyMode *newPartyMode = &allPartyModes[index];
    newPartyMode->middlePartyTeamSwap = MiddlePartyTeamSwap;
    newPartyMode->maxRound = MaxRound;
    newPartyMode->startMoney = StartMoney;
    newPartyMode->maxMoney = MaxMoney;
    newPartyMode->winTheRoundMoney = WinTheRoundMoney;
    newPartyMode->winTheRoundBombMoney = WinTheRoundBombMoney;
    newPartyMode->loseTheRoundMoney = LoseTheRoundMoney;
    newPartyMode->loseIncrease = LoseIncrease;
    newPartyMode->defuseBombMoneyBonus = DefuseBombMoneyBonus;
    newPartyMode->plantBombMoneyBonus = PlantBombMoneyBonus;
    newPartyMode->plantedBombLoseMoneyBonus = PlantedBombLoseMoneyBonus;
    newPartyMode->killPenalties = KillPenalties;
    newPartyMode->noMoneyOnTimeEnd = NoMoneyOnTimeEnd;
    newPartyMode->teamDamage = TeamDamage;
    newPartyMode->canSeeOtherTeamView = CanSeeOtherTeamView;
}
