#include "main.h"
#include "SaveManager.h"
// File system
#include <fat.h>
#include <stdio.h>
#include <math.h>

// For random system
#include <time.h>

struct VariablesToSave
{
    char playerName[21];
} SaveFile;

// File system
FILE *savefile;

// Save data
void Save()
{
    strncpy(SaveFile.playerName, localPlayer->name, 21);
    savefile = fopen("fat:/Counter_Strike.sav", "wb");
    fwrite(&SaveFile, 1, sizeof(SaveFile), savefile);
    fclose(savefile);
}

// Load data
void Load()
{
    savefile = fopen("fat:/Counter_Strike.sav", "rb");
    fread(&SaveFile, 1, sizeof(SaveFile), savefile);
    fclose(savefile);
    strncpy(localPlayer->name, SaveFile.playerName, 21);
}