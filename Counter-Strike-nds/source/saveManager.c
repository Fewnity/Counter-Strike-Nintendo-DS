// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "saveManager.h"
#include "keyboard.h"
#include "input.h"
#include "tutorial.h"
#include "stats.h"

// File system
#include <fat.h>
#include <stdio.h>
#include <math.h>

// For random system
#include <time.h>

// File system
FILE *savefile;

/**
 * @brief Save data
 *
 */
void Save()
{
    savefile = fopen("fat:/counter_strike_ds.sav", "wb"); // Try to read or create a save on DS

    if (savefile == NULL) // If there is no file, try to create in another location (3DS/2DS)
    {
        savefile = fopen("counter_strike_ds.sav", "wb"); // Try to read or create a save on 3DS/2DS
        if (savefile == NULL)                            // If there is no file, try to create in another location (SD)
        {
            savefile = fopen("sd:/counter_strike_ds.sav", "wb"); // Try to read or create a save on the sd card
        }
    }

    char saveText[512];
    sprintf(saveText, "game_version %s\n", GAME_VERSION);
    sprintf(saveText + strlen(saveText), "player_name %s\n", localPlayer->name);
    sprintf(saveText + strlen(saveText), "use_rumble %d\n", useRumble);
    sprintf(saveText + strlen(saveText), "is_azerty %d\n", isAzerty);
    sprintf(saveText + strlen(saveText), "is_left_controls %d\n", isLeftControls);
    sprintf(saveText + strlen(saveText), "sensitivity %f\n", sensitivity);
    sprintf(saveText + strlen(saveText), "tutorial_done %d\n", tutorialDone);
    sprintf(saveText + strlen(saveText), "show_ping %d\n", showPing);
    sprintf(saveText + strlen(saveText), "inputs %d\n", INPUT_COUNT);
    for (int i = 0; i < INPUT_COUNT; i++)
    {
        sprintf(saveText + strlen(saveText), "k%d %d\n", i, inputs[i].value);
    }
    sprintf(saveText + strlen(saveText), "cursor_transparency %f\n", cursorTransparency);
    sprintf(saveText + strlen(saveText), "buttons_sensitivity %f\n", buttonsSensitivity);

    // Stats
    sprintf(saveText + strlen(saveText), "total_bots_kill_count %d\n", totalBotsKillCount);
    sprintf(saveText + strlen(saveText), "total_online_players_kill_count %d\n", totalOnlinePlayersKillCount);
    sprintf(saveText + strlen(saveText), "total_death_count %d\n", totalDeathCount);
    sprintf(saveText + strlen(saveText), "total_finished_party %d\n", totalFinishedParty);
    sprintf(saveText + strlen(saveText), "total_played_seconds %d\n", totalPlayedSeconds);
    sprintf(saveText + strlen(saveText), "total_played_minutes %d\n", totalPlayedMinutes);
    sprintf(saveText + strlen(saveText), "total_played_hours %d\n", totalPlayedHours);
    sprintf(saveText + strlen(saveText), "total_wins %d\n", totalWins);

    if (savefile != NULL)
    {
        fwrite(saveText, 1, strlen(saveText), savefile);
        fclose(savefile);
    }
}

/**
 * @brief Load data
 *
 */
void Load()
{
    savefile = fopen("fat:/counter_strike_ds.sav", "rb"); // Try to read a save on DS
    if (savefile == NULL)                                 // If there is no file, try to read in another location (3DS/2DS)
    {
        savefile = fopen("counter_strike_ds.sav", "rb"); // Try to read a save on 3DS / 2DS
        if (savefile == NULL)                            // If there is no file, try to read in another location (SD)
        {
            savefile = fopen("sd:/counter_strike_ds.sav", "rb"); // Try to read a save on the sd card
        }
    }

    if (savefile != NULL)
    {
        int inputsFound = 14;
        int inputCount = 0;
        while (1)
        {
            char word[128];
            // read the next word of the file
            int res = fscanf(savefile, "%s", word);

            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            if (strcmp(word, "game_version") == 0) // Read save game version
            {
                int tmpX;
                int tmpY;
                int tmpZ;
                fscanf(savefile, "%d.%d.%d", &tmpX, &tmpY, &tmpZ);
            }
            else if (strcmp(word, "player_name") == 0) // Read player name
            {
                fscanf(savefile, "%s", localPlayer->name);
            }
            else if (strcmp(word, "use_rumble") == 0) // Read use rumble
            {
                int tmpUseRumble;
                fscanf(savefile, "%d", &tmpUseRumble);
                useRumble = tmpUseRumble;
            }
            else if (strcmp(word, "is_azerty") == 0) // Read is azerty
            {
                int tmpIsAzerty;
                fscanf(savefile, "%d", &tmpIsAzerty);
                isAzerty = tmpIsAzerty;
            }
            else if (strcmp(word, "is_left_controls") == 0) // Read is left controls
            {
                int tmpIsLeftControls;
                fscanf(savefile, "%d", &tmpIsLeftControls);
                isLeftControls = tmpIsLeftControls;
            }
            else if (strcmp(word, "sensitivity") == 0) // Read sensitivity
            {
                float tmpSensitivity;
                fscanf(savefile, "%f", &tmpSensitivity);
                sensitivity = tmpSensitivity;
            }
            else if (strcmp(word, "tutorial_done") == 0) // Read tutorial done
            {
                int tmpTutorialDone;
                fscanf(savefile, "%d", &tmpTutorialDone);
                tutorialDone = tmpTutorialDone;
            }
            else if (strcmp(word, "show_ping") == 0) //
            {
                int tmpShowPing;
                fscanf(savefile, "%d", &tmpShowPing);
                showPing = tmpShowPing;
            }
            else if (strcmp(word, "inputs") == 0) //
            {
                int tmpinputsFound;
                fscanf(savefile, "%d", &tmpinputsFound);
                inputsFound = tmpinputsFound;
            }
            else if (strcmp(word, "cursor_transparency") == 0)
            {
                float tmpcursorTransparency;
                fscanf(savefile, "%f", &tmpcursorTransparency);
                cursorTransparency = tmpcursorTransparency;
            }
            else if (strcmp(word, "buttons_sensitivity") == 0)
            {
                float tmpbuttonsSensitivity;
                fscanf(savefile, "%f", &tmpbuttonsSensitivity);
                buttonsSensitivity = tmpbuttonsSensitivity;
            }
            else if (strcmp(word, "total_bots_kill_count") == 0)
            {
                int tmptotalBotsKillCount;
                fscanf(savefile, "%d", &tmptotalBotsKillCount);
                totalBotsKillCount = tmptotalBotsKillCount;
            }
            else if (strcmp(word, "total_online_players_kill_count") == 0)
            {
                int tmptotalOnlinePlayersKillCount;
                fscanf(savefile, "%d", &tmptotalOnlinePlayersKillCount);
                totalOnlinePlayersKillCount = tmptotalOnlinePlayersKillCount;
            }
            else if (strcmp(word, "total_death_count") == 0)
            {
                int tmptotalDeathCount;
                fscanf(savefile, "%d", &tmptotalDeathCount);
                totalDeathCount = tmptotalDeathCount;
            }
            else if (strcmp(word, "total_finished_party") == 0)
            {
                int tmptotalFinishedParty;
                fscanf(savefile, "%d", &tmptotalFinishedParty);
                totalFinishedParty = tmptotalFinishedParty;
            }
            else if (strcmp(word, "total_played_seconds") == 0)
            {
                int tmptotalPlayedSeconds;
                fscanf(savefile, "%d", &tmptotalPlayedSeconds);
                totalPlayedSeconds = tmptotalPlayedSeconds;
            }
            else if (strcmp(word, "total_played_minutes") == 0)
            {
                int tmptotalPlayedMinutes;
                fscanf(savefile, "%d", &tmptotalPlayedMinutes);
                totalPlayedMinutes = tmptotalPlayedMinutes;
            }
            else if (strcmp(word, "total_played_hours") == 0)
            {
                int tmptotalPlayedHours;
                fscanf(savefile, "%d", &tmptotalPlayedHours);
                totalPlayedHours = tmptotalPlayedHours;
            }
            else if (strcmp(word, "total_wins") == 0)
            {
                int tmptotalWins;
                fscanf(savefile, "%d", &tmptotalWins);
                totalWins = tmptotalWins;
            }
            else if (inputsFound != inputCount) // Read inputs
            {
                char inputIndex[13];
                sprintf(inputIndex, "k%d", inputCount);
                if (strcmp(word, inputIndex) == 0)
                {
                    int tmpInput;
                    fscanf(savefile, "%d", &tmpInput);
                    inputs[inputCount].value = tmpInput;
                    if (tmpInput == -1)
                    {
                        inputs[inputCount].nameIndex = 12;
                    }
                    else
                    {
                        for (int i = 0; i < 12; i++)
                        {
                            if (tmpInput == BIT(i))
                            {
                                inputs[inputCount].nameIndex = i;
                                break;
                            }
                        }
                    }
                }
                inputCount++;
            }
        }
        fclose(savefile);
    }
    else
    {
        strncpy(localPlayer->name, "Player", PLAYER_MAX_LENGTH);
        Save();
    }
}