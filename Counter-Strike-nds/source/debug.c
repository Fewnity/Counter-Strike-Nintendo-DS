// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "debug.h"

// File system
#include <fat.h>

#include <NEMain.h>

// File
FILE *debugFile;

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