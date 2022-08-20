// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "party.h"
#include "ai.h"
#include "player.h"
#include "tutorial.h"
#include "gun.h"
#include "stats.h"
#include "camera.h"
#include "sounds.h"

// Party modes data
PartyMode allPartyModes[3];

// Waypoint where the bomb is planted
int bombPlantedAt = -1;
// Current player that is a defuser
int currentDefuserIndex = -1;
// Position of the dropped bomb (if any)
Vector4 droppedBombPositionAndRotation;
// Is the bomb dropped?
bool bombDropped = false;

// Party minutes timer
int PartyMinutes = 0;
// Party seconds timer
int PartySeconds = 0;
// Bomb seconds timer
int BombSeconds = 0;
// Current round state
enum RoundState roundState = TRAINING;
// Score of Counter Terrorists
int CounterScore = 0;
// Score of Terrorists
int TerroristsScore = 0;
// Party mode (index of allPartyModes)
int currentPartyMode = 2;
// Current map (index of allMaps)
int currentMap = 0;
// Is the party finished?
bool partyFinished = false;

/**
 * @brief Load party modes datas
 *
 */
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
    allPartyModes[0].infiniteGunAmmo = false;

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
    allPartyModes[1].infiniteGunAmmo = false;

    // Training
    AddPartyMode(2, false, 0, 10000, 10000, 0, 0, 0, 0, 0, 0, 0, 0, false, false, false);

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
    allPartyModes[2].trainingRespawnSecondsDuration = 2;

    allPartyModes[2].spawnWithArmor = true;

    allPartyModes[2].infiniteMoney = true;
    allPartyModes[2].infiniteTimer = true;
    allPartyModes[2].limitedShopByZoneAndTimer = false;
    allPartyModes[2].noScore = true;
    allPartyModes[2].infiniteGunAmmo = true;
}

/**
 * @brief "Constructor" of a party mode
 *
 * @param index Index of the party mode
 * @param MiddlePartyTeamSwap If true, at the middle of the party, teams will swap
 * @param MaxRound Number of rounds in the party
 * @param StartMoney Starting money of the players
 * @param MaxMoney Maximum money of the players
 * @param WinTheRoundMoney Money won by winners of the round
 * @param WinTheRoundBombMoney Money won by winners of the round if the bomb is planted
 * @param LoseTheRoundMoney Money won by losers of the round
 * @param LoseIncrease
 * @param DefuseBombMoneyBonus Money won by defuser of the round
 * @param PlantBombMoneyBonus Money won by planter of the round
 * @param PlantedBombLoseMoneyBonus
 * @param KillPenalties Money lost by each kill
 * @param NoMoneyOnTimeEnd If true, the players will not win money when the time is over
 * @param TeamDamage If true, players in same teams will take damage from each other
 * @param CanSeeOtherTeamView If true, players in other teams can see each other
 */
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

/**
 * @brief To call on each new round
 *
 */
void onNewRoundStart()
{
    // Reset some values
    bombDropped = false;
    bombSet = false;
    bombPlantedAt = -1;
    BombPlanted = false;
    BombDefused = false;
    bombBipTimer = 0;
    BombWillExplode = false;
    IsExplode = false;
    BombExplosionScale = 0;
    currentDefuserIndex = NO_PLAYER;

    // Delete grenades
    DeleteAllGrenade();
    // Disable player aim
    DisableAim();
}

/**
 * @brief Party tick
 *
 */
void partyTimerTick()
{
    if (!partyFinished)
    {
        // Reduce seconds every 60 frames
        changeSecondTimer--;
        if (changeSecondTimer == 0)
        {
            // Reduce bomb seconds
            if (BombPlanted && !BombDefused && BombSeconds > 0 && !isInTutorial)
                BombSeconds--;

            if (!allPartyModes[currentPartyMode].infiniteTimer)
            {
                // Reduce seconds
                PartySeconds--;
                if (PartySeconds == -1)
                {
                    // Reduce minutes
                    PartyMinutes--;
                    PartySeconds = 59;

                    if (PartyMinutes == -1) // At the end of the round timer
                    {
                        // Change round state
                        if (!PartyStarted)
                        {
                            // Start party
                            PartyStarted = true;
                            onNewRoundStart();

                            // Set new timer
                            PartyMinutes = allPartyModes[currentPartyMode].startRoundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].startRoundSecondsDuration;
                            shopDisableTimer = SHOP_DISABLE_TIMER;
                            roundState = WAIT_START;

                            // Reset players
                            for (int i = 0; i < MaxPlayer; i++)
                            {
                                if (AllPlayers[i].Id == UNUSED)
                                    continue;
                                resetPlayer(i);
                                setPlayerMoney(i, allPartyModes[currentPartyMode].startMoney);
                                AllPlayers[i].KillCount = 0;
                                AllPlayers[i].DeathCount = 0;
                            }

                            // Update screen if needed
                            if (currentMenu == SCORE_BOARD)
                                UpdateBottomScreenFrameCount += 8;

                            // Buy guns for guns
                            checkShopForBots();
                            // Give the bomb to a player
                            setBombForARandomPlayer();
                            // Set players position to their spawns
                            setPlayersPositionAtSpawns();
                            // Put a weapons in the hands of each player
                            setNewRoundHandWeapon();
                        }
                        else if (roundState == WAIT_START)
                        {
                            // Start round
                            // Set new timer
                            PartyMinutes = allPartyModes[currentPartyMode].roundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].roundSecondsDuration;
                            roundState = PLAYING;
                        }
                        else if (roundState == PLAYING)
                        {
                            // Stop the round
                            setEndRound();

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
                            // Restart round
                            onNewRoundStart();
                            PartyMinutes = allPartyModes[currentPartyMode].startRoundMinutesDuration;
                            PartySeconds = allPartyModes[currentPartyMode].startRoundSecondsDuration;
                            roundState = WAIT_START;
                            onNewRoundStart();

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
                            initFinalScoreMenu();
                            // initGameFinishedMenu();
                        }
                    }
                }
            }
            changeSecondTimer = 60;
        }
    }
}

/**
 * @brief End the round
 *
 */
void setEndRound()
{
    PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
    PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
    roundState = END_ROUND;
    currentDefuserIndex = NO_PLAYER;
}

/**
 * @brief Party tick online (no game rules gestion)
 *
 */
void partyTimerTickOnline()
{
    if (!partyFinished)
    {
        changeSecondTimer--;
        if (changeSecondTimer == 0)
        {
            // Reduce bomb seconds
            if (BombPlanted && !BombDefused && BombSeconds > 0)
                BombSeconds--;

            changeSecondTimer = 60;
        }
    }
}

/**
 * @brief Set party as finished
 *
 */
void finishParty()
{
    // Set end timer
    PartyMinutes = allPartyModes[currentPartyMode].endRoundMinutesDuration;
    PartySeconds = allPartyModes[currentPartyMode].endRoundSecondsDuration;
    roundState = END;
}

/**
 * @brief Check the party score to do some event (swap teams, finish the party)
 *
 */
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

            UpdatePlayerTexture(i);
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

/**
 * @brief Return the number of players and of dead players in both teams
 *
 * @param TerroristsCount Out number of terrosits
 * @param CounterTerroristsCount Out number of counter terrorists
 * @param TerroristDeadCount Out number of dead terrosits
 * @param CounterDeadCount Out number of dead counter terrorists
 */
void CheckTeamDeathCount(int *TerroristsCount, int *CounterTerroristsCount, int *TerroristDeadCount, int *CounterDeadCount)
{
    // Reset values
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

/**
 * @brief Check player health after damage
 *
 * @param shooterPlayerIndex Shooter player index
 * @param hittedPlayerIndex Hitted player index
 * @param CheckScore Need to check score
 */
void checkAfterDamage(int shooterPlayerIndex, int hittedPlayerIndex, bool CheckScore)
{
    Player *HittedClient = &AllPlayers[hittedPlayerIndex];
    Player *killerClient = NULL;
    if (shooterPlayerIndex != NO_PLAYER)
        killerClient = &AllPlayers[shooterPlayerIndex];

    if (HittedClient->Health <= 0 && !HittedClient->IsDead)
    {
        //  Set client has dead
        killPlayer(HittedClient);

        if (killerClient == localPlayer)
        {
            if (applyRules)
            {
                totalBotsKillCount++;
            }
            else
            {
                totalOnlinePlayersKillCount++;
            }
        }

        // Reset raycast values on death
        if (hittedPlayerIndex == 0)
        {
            DisableAim();
        }

        if (HittedClient->haveBomb)
        {
            dropBomb(HittedClient, hittedPlayerIndex);
        }

        HittedClient->target = NO_PLAYER;
        HittedClient->bombTimer = 0;
        HittedClient->isPlantingBomb = false;

        // If the killed player was a defuser, free the defuser place for other bots
        if (&AllPlayers[currentDefuserIndex] == HittedClient)
            currentDefuserIndex = NO_PLAYER;

        // Increase player death count
        HittedClient->DeathCount++;

        // If the killer kills the right target, set current target as "null"
        if (killerClient != NULL && killerClient->target == HittedClient->Id)
        {
            killerClient->target = NO_PLAYER;
            killerClient->lastSeenTarget = NO_PLAYER;
        }
        // Stop dead player movements
        if (HittedClient->isAi)
        {
            HittedClient->justCheking = false;
            HittedClient->lastSeenTarget = NO_PLAYER;
            HittedClient->PathCount = 0;
        }

        // If the killer is not null
        if (shooterPlayerIndex != NO_PLAYER)
        {
            // Show a text to show with is the killer
            showKillText(shooterPlayerIndex, hittedPlayerIndex);
            // Add penalties if the killer is in the same team as the dead player
            if (allPartyModes[currentPartyMode].teamDamage && HittedClient->Team == killerClient->Team)
            {
                killerClient->KillCount--;
                reducePlayerMoney(shooterPlayerIndex, allPartyModes[currentPartyMode].killPenalties);
            }
            else
            {
                // Increase killer kill count
                killerClient->KillCount++;
                // if (AllShopElements[killerClient.AllGunsInInventory[killerClient.currentGunInInventory]] is Gun)
                // AddMoneyTo(killerClient, ((Gun)AllShopElements[killerClient.AllGunsInInventory[killerClient.currentGunInInventory]]).KillMoneyBonus[killerClient.ClientParty.PartyType]);
            }
        }

        // Update bottom screen if scoreboard is showed
        if (currentMenu == SCORE_BOARD)
            UpdateBottomScreenFrameCount += 8;

        if (currentPartyMode == 2)
        {
            setNeedRespawn(HittedClient);
        }

        if (!CheckScore)
            return;

        if (roundState == PLAYING)
        {
            // Check if all a team is dead
            int CounterDeadCount = 0, TerroristDeadCount = 0, CounterTerroristsCount = 0, TerroristsCount = 0;

            CheckTeamDeathCount(&TerroristsCount, &CounterTerroristsCount, &TerroristDeadCount, &CounterDeadCount);
            // If all counter terrorists are dead
            if (CounterDeadCount == CounterTerroristsCount)
            {
                // Add a point to terrorists teams
                TerroristsScore++;

                // Set kill bonus count for each teams
                if (LoseCountTerrorists > 0)
                    LoseCountTerrorists--;

                if (LoseCountCounterTerrorists < 4)
                    LoseCountCounterTerrorists++;

                // Add money to teams
                addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundMoney, TERRORISTS);
                addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountCounterTerrorists, COUNTERTERRORISTS);

                // Set round to finished round state
                setEndRound();
                showPartyEventText(0);
                CheckAfterRound();
            }
            else if (TerroristDeadCount == TerroristsCount && !bombSet)
            {
                // Add a point to counter terrorists teams
                CounterScore++;

                // Set kill bonus count for each teams
                if (LoseCountCounterTerrorists > 0)
                    LoseCountCounterTerrorists--;

                if (LoseCountTerrorists < 4)
                    LoseCountTerrorists++;

                // Add money to teams
                addMoneyToTeam(allPartyModes[currentPartyMode].winTheRoundMoney, COUNTERTERRORISTS);
                addMoneyToTeam(allPartyModes[currentPartyMode].loseTheRoundMoney + allPartyModes[currentPartyMode].loseIncrease * LoseCountTerrorists, TERRORISTS);

                // Set round to finished round state
                setEndRound();
                showPartyEventText(1);
                CheckAfterRound();
            }
        }
        else if (roundState == TRAINING) // If the party is in training mode
        {
            setNeedRespawn(HittedClient);
        }
    }
}

/**
 * @brief Quit party
 *
 * @param option Unused
 */
void QuitParty(int option)
{
    initMainMenu();
    OnPartyQuit();
}

/**
 * @brief Event on quit party
 *
 */
void OnPartyQuit()
{
    setCameraMapPosition();
    DisableAim();
    uiTimer = 8;
    actionOfUiTimer = SAVE;
}

/**
 * @brief Start a single player party
 *
 */
void StartSinglePlayer(int partyMode)
{
    Connection = OFFLINE;
    currentPartyMode = partyMode;
}

/**
 * @brief Prepare party to start a new one
 *
 * @param multiplayerMode true for an online party
 * mode 0 single player, 1 online, 2 nifi
 */
void prepareParty(/*bool multiplayerMode*/ int mode)
{
    if (mode == 0)
    {
        applyRules = true;
        PartyStarted = false;
    }
    else
    {
        applyRules = false;
        PartyStarted = true;
        SetNeedChangeScreen(true);
    }

    bombDropped = false;
    bombSet = false;
    BombDefused = false;
    BombPlanted = false;
    bombPlantedAt = -1;

    partyFinished = false;
    initScoreMenu();
    textToShowTimer = 0;
    if (mode == 0 || mode == 2)
    {
        ResetTakenBotsNames();
        roundState = TRAINING;
        localPlayer->CurrentOcclusionZone = 15;
        TerroristsScore = 0;
        CounterScore = 0;
        PartySeconds = allPartyModes[currentPartyMode].trainingSecondsDuration;
        PartyMinutes = allPartyModes[currentPartyMode].trainingMinutesDuration;
        changeSecondTimer = 1;
        changeMinuteTimer = 60;
        bombBipTimer = 0;
        BombWillExplode = false;
        currentDefuserIndex = NO_PLAYER;
    }
    SetCurrentCameraPlayer(0);
    DisableAim();
    resetFrameCount();
    stopMusic();
}