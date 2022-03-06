#include "main.h"
#include "party.h"
#include "ai.h"
#include "player.h"
#include "tutorial.h"

PartyMode allPartyModes[3];

int bombPlantedAt = -1;
int currentDefuserIndex = -1;
Vector4 droppedBombPositionAndRotation;
bool bombDropped = false;

int PartyMinutes = 0;
int PartySeconds = 0;
int BombSeconds = 0;
enum RoundState roundState = TRAINING;
int CounterScore = 0;
int TerroristsScore = 0;
int currentPartyMode = 2;
int currentMap = 0;
bool partyFinished = false;

void AddAllPartyModes()
{
    // Competitive
    AddPartyMode(0, true, 30, 800, 16000, 3250, 3500, 1400, 500, 300, 300, 800, 300, true, true, false);
    allPartyModes[0].trainingMinutesDuration = 0;
    // allPartyModes[0].trainingSecondsDuration = 20;
    allPartyModes[0].trainingSecondsDuration = 0;

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

    allPartyModes[0].infiniteMoney = false;
    allPartyModes[0].infiniteTimer = false;
    allPartyModes[0].limitedShopByZoneAndTimer = true;
    allPartyModes[0].noScore = false;

    // Casual
    AddPartyMode(1, false, 15, 1000, 10000, 2700, 2700, 2400, 0, 200, 200, 200, 0, false, false, true);
    // AddPartyMode(1, false, 2, 1000, 10000, 2700, 2700, 2400, 0, 200, 200, 200, 0, false, false, true);
    // allPartyModes[1].trainingMinutesDuration = 2;
    // allPartyModes[1].trainingSecondsDuration = 20;
    allPartyModes[1].trainingMinutesDuration = 0;
    allPartyModes[1].trainingSecondsDuration = 0;

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

    allPartyModes[1].infiniteMoney = false;
    allPartyModes[1].infiniteTimer = false;
    allPartyModes[1].limitedShopByZoneAndTimer = true;
    allPartyModes[1].noScore = false;

    AddPartyMode(2, false, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, false, false, false);

    allPartyModes[2].trainingMinutesDuration = 0;
    allPartyModes[2].trainingSecondsDuration = 0;

    allPartyModes[2].startRoundMinutesDuration = 0;
    allPartyModes[2].startRoundSecondsDuration = 0;

    allPartyModes[2].roundMinutesDuration = 0;
    allPartyModes[2].roundSecondsDuration = 0;

    allPartyModes[2].endRoundMinutesDuration = 0;
    allPartyModes[2].endRoundSecondsDuration = 0;

    allPartyModes[2].bombWaitingMinutesDuration = 0;
    allPartyModes[2].bombWaitingSecondsDuration = 40;

    allPartyModes[2].trainingRespawnMinutesDuration = 0;
    allPartyModes[2].trainingRespawnSecondsDuration = 0;

    allPartyModes[2].spawnWithArmor = true;

    allPartyModes[2].infiniteMoney = true;
    allPartyModes[2].infiniteTimer = true;
    allPartyModes[2].limitedShopByZoneAndTimer = false;
    allPartyModes[2].noScore = true;
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

void partyTimerTick()
{
    if (!partyFinished)
    {
        changeSecondTimer--;
        if (changeSecondTimer == 0)
        {
            if (BombPlanted && !BombDefused && BombSeconds > 0 && !isInTutorial)
                BombSeconds--;

            if (!allPartyModes[currentPartyMode].infiniteTimer)
            {
                PartySeconds--;
                if (PartySeconds == -1)
                {
                    PartyMinutes--;
                    PartySeconds = 59;

                    if (PartyMinutes == -1)
                    {
                        if (!PartyStarted)
                        {
                            PartyStarted = true;
                            PartyMinutes = allPartyModes[currentPartyMode].startRoundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].startRoundSecondsDuration;
                            shopDisableTimer = SHOP_DISABLE_TIMER;
                            roundState = WAIT_START;

                            for (int i = 0; i < MaxPlayer; i++)
                            {
                                if (AllPlayers[i].Id == UNUSED)
                                    continue;
                                resetPlayer(i);
                                setPlayerMoney(i, allPartyModes[currentPartyMode].startMoney);
                                AllPlayers[i].KillCount = 0;
                                AllPlayers[i].DeathCount = 0;
                            }

                            if (currentMenu == SCORE)
                                UpdateBottomScreenOneFrame += 8;

                            checkShopForBots();
                            setBombForARandomPlayer();
                            setPlayersPositionAtSpawns();
                            setNewRoundHandWeapon();
                        }
                        else if (roundState == WAIT_START)
                        {
                            PartyMinutes = allPartyModes[currentPartyMode].roundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].roundSecondsDuration;
                            roundState = PLAYING;
                        }
                        else if (roundState == PLAYING)
                        {
                            PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
                            roundState = END_ROUND;

                            if (bombSet)
                            {
                                TerroristsScore++;

                                if (LoseCountCounterTerrorists > 0)
                                    LoseCountCounterTerrorists--;

                                if (LoseCountTerrorists < 4)
                                    LoseCountTerrorists++;

                                addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, TERRORISTS);
                                addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountTerrorists, COUNTERTERRORISTS);

                                showPartyEventText(0);
                            }
                            else
                            {
                                CounterScore++;

                                if (LoseCountTerrorists > 0)
                                    LoseCountTerrorists--;

                                if (LoseCountCounterTerrorists < 4)
                                    LoseCountCounterTerrorists++;

                                if (!allPartyModes[currentPartyMode].noMoneyOnTimeEnd)
                                {
                                    addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney, TERRORISTS);
                                }
                                addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundBombMoney, COUNTERTERRORISTS);

                                showPartyEventText(1);
                            }

                            CheckAfterRound();
                        }
                        else if (roundState == END_ROUND)
                        {
                            PartyMinutes = allPartyModes[currentPartyMode].startRoundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].startRoundSecondsDuration;
                            roundState = WAIT_START;

                            bombDropped = false;
                            bombSet = false;
                            BombPlanted = false;
                            BombDefused = false;
                            bombBipTimer = 0;
                            BombWillExplose = false;
                            currentDefuserIndex = NO_PLAYER;
                            for (int i = 0; i < MaxPlayer; i++)
                            {
                                resetPlayer(i);
                            }
                            checkShopForBots();
                            setBombForARandomPlayer();
                            setPlayersPositionAtSpawns();
                            setNewRoundHandWeapon();
                            shopDisableTimer = SHOP_DISABLE_TIMER;
                        }
                        else if (roundState == END)
                        {
                            partyFinished = true;
                            initGameFinishedMenu();
                        }
                    }
                }
            }
            changeSecondTimer = 60;
        }
    }
}

void finishParty()
{
    // Set end timer
    PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
    PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
    roundState = END;
}

void CheckAfterRound()
{
    int scoreFloor = floor(allPartyModes[currentPartyMode].maxRound / 2.0);
    int totalScore = TerroristsScore + CounterScore;
    // Check if a team won
    if (TerroristsScore == scoreFloor + 1 || CounterScore == scoreFloor + 1 || totalScore == allPartyModes[currentPartyMode].maxRound)
    {
        finishParty();
    }
    else if (allPartyModes[currentPartyMode].middlePartyTeamSwap && totalScore == scoreFloor) // Or check team swap
    {
        // Swap team
        for (int i = 0; i < MaxPlayer; i++)
        {
            Player *player = &AllPlayers[i];

            if (player->Id == UNUSED)
                continue;

            if (player->Team == TERRORISTS)
            {
                player->Team = COUNTERTERRORISTS;
            }
            else if (player->Team == COUNTERTERRORISTS)
            {
                player->Team = TERRORISTS;
            }

            player->Money = allPartyModes[currentPartyMode].startMoney;
        }
        // Reset lose count money bonus
        LoseCountCounterTerrorists = 0;
        LoseCountTerrorists = 0;
        // Swap score
        int TempsScoreTerrorists = TerroristsScore;
        TerroristsScore = CounterScore;
        CounterScore = TempsScoreTerrorists;
    }
}

// Return the number of players and of dead players in both teams
void CheckTeamDeathCount(int *TerroristsCount, int *CounterTerroristsCount, int *TerroristDeadCount, int *CounterDeadCount)
{
    *CounterDeadCount = 0;
    *TerroristDeadCount = 0;
    *TerroristsCount = 0;
    *CounterTerroristsCount = 0;

    for (int iPlayer = 0; iPlayer < MaxPlayer; iPlayer++)
    {
        Player *player = &AllPlayers[iPlayer];
        if (player->Id == UNUSED)
            continue;

        if (player->Team == COUNTERTERRORISTS)
        {
            if (player->IsDead) // If a counter is dead, add one to counter dead count
                *CounterDeadCount = *CounterDeadCount + 1;

            *CounterTerroristsCount = *CounterTerroristsCount + 1;
        }
        else if (player->Team == TERRORISTS) // If a terrorist is dead, add one to terrorist dead count
        {
            if (player->IsDead)
                *TerroristDeadCount = *TerroristDeadCount + 1;

            *TerroristsCount = *TerroristsCount + 1;
        }
    }
}