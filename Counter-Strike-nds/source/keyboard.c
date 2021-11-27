#include "main.h"
#include "sounds.h"
#include "ui.h"

#define keyCount 29
#define otherKeyCount 1

Key allKeys[keyCount];
OtherKey allOtherKeys[otherKeyCount];

void initKeyboard()
{
    int index = 0;

    strncpy(allKeys[index].letter, "a", 2);
    strncpy(allKeys[index].letterUpperCase, "A", 2);
    allKeys[index].xPos = 1 + 3 * 0;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "z", 2);
    strncpy(allKeys[index].letterUpperCase, "Z", 2);
    allKeys[index].xPos = 1 + 3 * 1;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "e", 2);
    strncpy(allKeys[index].letterUpperCase, "E", 2);
    allKeys[index].xPos = 1 + 3 * 2;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "r", 2);
    strncpy(allKeys[index].letterUpperCase, "R", 2);
    allKeys[index].xPos = 1 + 3 * 3;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "t", 2);
    strncpy(allKeys[index].letterUpperCase, "T", 2);
    allKeys[index].xPos = 1 + 3 * 4;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "y", 2);
    strncpy(allKeys[index].letterUpperCase, "Y", 2);
    allKeys[index].xPos = 1 + 3 * 5;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "u", 2);
    strncpy(allKeys[index].letterUpperCase, "U", 2);
    allKeys[index].xPos = 1 + 3 * 6;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "i", 2);
    strncpy(allKeys[index].letterUpperCase, "I", 2);
    allKeys[index].xPos = 1 + 3 * 7;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "o", 2);
    strncpy(allKeys[index].letterUpperCase, "O", 2);
    allKeys[index].xPos = 1 + 3 * 8;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    strncpy(allKeys[index].letter, "p", 2);
    strncpy(allKeys[index].letterUpperCase, "P", 2);
    allKeys[index].xPos = 1 + 3 * 9;
    allKeys[index].yPos = 1 + 3 * 0;
    index++;

    // ROW 2
    strncpy(allKeys[index].letter, "q", 2);
    strncpy(allKeys[index].letterUpperCase, "Q", 2);
    allKeys[index].xPos = 2 + 3 * 0;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "s", 2);
    strncpy(allKeys[index].letterUpperCase, "S", 2);
    allKeys[index].xPos = 2 + 3 * 1;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "d", 2);
    strncpy(allKeys[index].letterUpperCase, "D", 2);
    allKeys[index].xPos = 2 + 3 * 2;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "f", 2);
    strncpy(allKeys[index].letterUpperCase, "F", 2);
    allKeys[index].xPos = 2 + 3 * 3;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "g", 2);
    strncpy(allKeys[index].letterUpperCase, "G", 2);
    allKeys[index].xPos = 2 + 3 * 4;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "h", 2);
    strncpy(allKeys[index].letterUpperCase, "H", 2);
    allKeys[index].xPos = 2 + 3 * 5;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "j", 2);
    strncpy(allKeys[index].letterUpperCase, "J", 2);
    allKeys[index].xPos = 2 + 3 * 6;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "k", 2);
    strncpy(allKeys[index].letterUpperCase, "K", 2);
    allKeys[index].xPos = 2 + 3 * 7;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "l", 2);
    strncpy(allKeys[index].letterUpperCase, "L", 2);
    allKeys[index].xPos = 2 + 3 * 8;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    strncpy(allKeys[index].letter, "m", 2);
    strncpy(allKeys[index].letterUpperCase, "M", 2);
    allKeys[index].xPos = 2 + 3 * 9;
    allKeys[index].yPos = 1 + 2 * 1;
    index++;

    // ROW 3
    strncpy(allKeys[index].letter, "w", 2);
    strncpy(allKeys[index].letterUpperCase, "W", 2);
    allKeys[index].xPos = 3 + 3 * 0;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "x", 2);
    strncpy(allKeys[index].letterUpperCase, "X", 2);
    allKeys[index].xPos = 3 + 3 * 1;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "c", 2);
    strncpy(allKeys[index].letterUpperCase, "C", 2);
    allKeys[index].xPos = 3 + 3 * 2;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "v", 2);
    strncpy(allKeys[index].letterUpperCase, "V", 2);
    allKeys[index].xPos = 3 + 3 * 3;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "b", 2);
    strncpy(allKeys[index].letterUpperCase, "B", 2);
    allKeys[index].xPos = 3 + 3 * 4;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "n", 2);
    strncpy(allKeys[index].letterUpperCase, "N", 2);
    allKeys[index].xPos = 3 + 3 * 5;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "?", 2);
    strncpy(allKeys[index].letterUpperCase, "?", 2);
    allKeys[index].xPos = 3 + 3 * 6;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, ".", 2);
    strncpy(allKeys[index].letterUpperCase, ".", 2);
    allKeys[index].xPos = 3 + 3 * 7;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    strncpy(allKeys[index].letter, "!", 2);
    strncpy(allKeys[index].letterUpperCase, "!", 2);
    allKeys[index].xPos = 3 + 3 * 8;
    allKeys[index].yPos = 1 + 2 * 2;
    index++;

    index = 0;
    strncpy(allOtherKeys[index].name, "space", 6);
    allOtherKeys[index].xPos = 14;
    allOtherKeys[index].yPos = 1 + 2 * 3;
    allOtherKeys[index].xCenter = 19;
    allOtherKeys[index].xSize = 30;
    allOtherKeys[index].ySize = 2;
}

void drawKeyBoard()
{
    // Screen background
    NE_2DDrawQuad(0, 0, 256, 196, 20, RGB15(3, 3, 3));
    // Keyboard background
    NE_2DDrawQuad(2, 2, 256 - 2, 70, 19, RGB15(4, 4, 4));
    for (int i = 0; i < keyCount; i++)
    {
        NE_TextPrint(0,                                // Font slot
                     allKeys[i].xPos, allKeys[i].yPos, // Coordinates x(column), y(row)
                     NE_White,                         // Color
                     allKeys[i].letter);
        NE_2DDrawQuad((allKeys[i].xPos * 8) - 6, (allKeys[i].yPos * 8) - 3, (allKeys[i].xPos * 8) + 13, (allKeys[i].yPos * 8) + 12, 11, RGB15(7, 7, 7));
        NE_2DDrawQuad((allKeys[i].xPos * 8) - 5, (allKeys[i].yPos * 8) - 2, (allKeys[i].xPos * 8) + 12, (allKeys[i].yPos * 8) + 11, 10, RGB15(6, 6, 6));
    }

    for (int i = 0; i < otherKeyCount; i++)
    {
        NE_TextPrint(0,                                          // Font slot
                     allOtherKeys[i].xPos, allOtherKeys[i].yPos, // Coordinates x(column), y(row)
                     NE_White,                                   // Color
                     allOtherKeys[i].name);
        NE_2DDrawQuad((allOtherKeys[i].xPos * 8) - allOtherKeys[i].xSize - 1 + allOtherKeys[i].xCenter, (allOtherKeys[i].yPos * 8) - allOtherKeys[i].ySize - 1, (allOtherKeys[i].xPos * 8) + allOtherKeys[i].xSize + 1 + allOtherKeys[i].xCenter, (allOtherKeys[i].yPos * 8) + 12, 11, RGB15(7, 7, 7));
        NE_2DDrawQuad((allOtherKeys[i].xPos * 8) - allOtherKeys[i].xSize + allOtherKeys[i].xCenter, (allOtherKeys[i].yPos * 8) - allOtherKeys[i].ySize, (allOtherKeys[i].xPos * 8) + allOtherKeys[i].xSize + allOtherKeys[i].xCenter, (allOtherKeys[i].yPos * 8) + 11, 10, RGB15(6, 6, 6));
    }

    // NE_2DDrawQuad(0, 0, 256, 196, 20, RGB15(3, 3, 3));
}

void readKeyboard()
{
    if (isShowingKeyBoard)
    {
        for (int i = 0; i < keyCount; i++)
        {
            if ((keysdown & KEY_TOUCH && touch.px >= (allKeys[i].xPos * 8) - 6 && touch.px <= (allKeys[i].xPos * 8) + 13 && touch.py >= (allKeys[i].yPos * 8) - 2 && touch.py <= (allKeys[i].yPos * 8) + 11))
            {
                sprintf(AllPlayers[0].name + strlen(AllPlayers[0].name), allKeys[i].letter);
                // AllPlayers[0].name;
            }
        }
    }
}