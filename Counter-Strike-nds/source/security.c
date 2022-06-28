// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "security.h"

// For random system
#include <time.h>

// Key sent by the server
int serverKey = 0;

// For debug, set to true to simulate an wrong key
bool failKey = false;


// Public version
/**
 * @brief Get a key response from the server key
 *
 * @param key Server key
 * @return int Key response
 */
int getKeyResponse(int key)
{
    // Here there is a security key generator from a server key
    // Secret code, create you own
    // Same code as the server's security code
    // Generate a key from a base key, this new key is used to check if the game is an official version
    return -1;
}
