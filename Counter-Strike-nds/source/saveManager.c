#include "main.h"
#include "saveManager.h"
#include "keyboard.h"
#include "input.h"
#include "tutorial.h"

// File system
#include <fat.h>
#include <stdio.h>
#include <math.h>

// For random system
#include <time.h>

struct VariablesToSave
{
    char playerName[playerNameLength];
    bool useRumble;
    bool is3dsMode;
    Input inputs[INPUT_COUNT];
    bool isAzerty;
    bool isLeftControls;
    float sensitivity;
    bool tutorialDone;
} SaveFile;

// File system
FILE *savefile;

// Save data
void Save()
{
    strncpy(SaveFile.playerName, localPlayer->name, playerNameLength);
    SaveFile.useRumble = useRumble;
    SaveFile.is3dsMode = is3dsMode;
    SaveFile.isAzerty = isAzerty;
    SaveFile.isLeftControls = isLeftControls;
    SaveFile.sensitivity = sensitivity;
    SaveFile.tutorialDone = tutorialDone;

    for (int i = 0; i < INPUT_COUNT; i++)
    {
        SaveFile.inputs[i] = inputs[i];
    }

    if (!is3dsMode)
    {
        savefile = fopen("fat:/Counter_Strike.sav", "wb");
    }
    else
    {
        savefile = fopen("/roms/nds/saves/Counter_Strike.sav", "wb");
    }
    fwrite(&SaveFile, 1, sizeof(SaveFile), savefile);
    fclose(savefile);
}

// Load data
void Load()
{
    savefile = fopen("fat:/Counter_Strike.sav", "rb");
    if (savefile == NULL)
    {
        savefile = fopen("/roms/nds/saves/Counter_Strike.sav", "rb");
    }
    fread(&SaveFile, 1, sizeof(SaveFile), savefile);
    fclose(savefile);
    strncpy(localPlayer->name, SaveFile.playerName, playerNameLength);
    useRumble = SaveFile.useRumble;
    is3dsMode = SaveFile.is3dsMode;
    isAzerty = SaveFile.isAzerty;
    isLeftControls = SaveFile.isLeftControls;
    sensitivity = SaveFile.sensitivity;
    tutorialDone = SaveFile.tutorialDone;

    if (sensitivity == 0)
    {
        sensitivity = DEFAULT_SENSITIVITY;
    }

    if (SaveFile.inputs[0].value + SaveFile.inputs[1].value != 0)
    {
        for (int i = 0; i < INPUT_COUNT; i++)
        {
            inputs[i] = SaveFile.inputs[i];
        }
    }

    if (strlen(localPlayer->name) == 0)
    {
        strncpy(localPlayer->name, "Player", playerNameLength);
    }
}