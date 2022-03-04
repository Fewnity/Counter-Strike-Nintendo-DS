#include "main.h"
#include "saveManager.h"
#include "keyboard.h"
#include "input.h"

// File system
#include <fat.h>
#include <stdio.h>
#include <math.h>

// For random system
#include <time.h>

FILE *debugFile;

void debugPrint(const char *text)
{
    debugFile = fopen("fat:/debug_cs.log", "a");

    fprintf(debugFile, text);
    fprintf(debugFile, "\n");

    fclose(debugFile);
}