// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "debug.h"
#include "input.h"

// File system
#include <fat.h>

#include <NEMain.h>

// File
FILE *debugFile;

// Enable debug text at top screen?
bool isDebugTopScreen = false;
// Enable debug text at bottom screen?
bool isDebugBottomScreen = false;
bool isDebugMode = true;

void initDebug()
{
    if (isDebugMode)
    {
        // Set debug mode if keys are pressed
        if (keys & KEY_START)
            isDebugTopScreen = true;

        if (keys & KEY_SELECT)
        {
            isDebugBottomScreen = true;
            //  Init debug console
            consoleDemoInit();
        }
    }
}

/**
 * @brief Print a message in the file
 *
 * @param text
 */
void debugPrint(const char *text)
{
    // Open the file
    if (debugFile == NULL)
    {
        debugFile = fopen("fat:/debug_cs.log", "a");
    }

    // Print the message and a new line
    fprintf(debugFile, text);
    fprintf(debugFile, "\n");

    // Close the file
    if (debugFile != NULL)
    {
        fclose(debugFile);
    }
}

/**
 * @brief Print Nitro Engine error in debug mode
 *
 * @param text Text to print
 */
void error_handler(const char *text)
{
    // Simple handler. You could write this to a file instead, for example.
    printf(text);

    // Write in the file
    // debugPrint(text);
}