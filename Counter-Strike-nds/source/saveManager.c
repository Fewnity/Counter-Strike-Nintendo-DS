#include "main.h"
#include "saveManager.h"
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
} SaveFile;

// File system
FILE *savefile;

// Save data
void Save()
{
    strncpy(SaveFile.playerName, localPlayer->name, playerNameLength);
    SaveFile.useRumble = useRumble;
    SaveFile.is3dsMode = is3dsMode;

    /*savefile = fopen("fat:/Counter_Strike.sav", "wb");
    if (savefile == NULL)
    {
        savefile = fopen("/roms/nds/saves/Counter_Strike.sav", "wb");
    }*/
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
    /*if (!is3dsMode)
    {
        savefile = fopen("fat:/Counter_Strike.sav", "rb");
    }
    else
    {
        savefile = fopen("/roms/nds/saves/Counter_Strike.sav", "rb");
    }*/

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
    if (strlen(localPlayer->name) == 0)
    {
        strncpy(localPlayer->name, "Player", playerNameLength);
    }
}