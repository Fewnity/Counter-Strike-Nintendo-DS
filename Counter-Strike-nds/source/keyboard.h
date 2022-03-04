#ifndef KEYBOARD_H_ /* Include guard */
#define KEYBOARD_H_

#include <NEMain.h>
#include "main.h"

extern char tempText[21];
extern int returnToMenuOnCancel;
extern int returnToMenuOnSucces;
extern bool isAzerty;

void initKeyboard();
void drawKeyBoard();
void readKeyboard();
void makeSpace();
void makeDelete();
void confirmKeyboard();
void cancelKeyboard();
void shiftKeyboard();
void changeKeyboardMode();

#endif // KEYBOARD_H_