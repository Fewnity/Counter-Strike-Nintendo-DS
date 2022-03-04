#ifndef TUTORIAL_H_ /* Include guard */
#define TUTORIAL_H_

#include <NEMain.h>
#include "main.h"

extern int tutorialStep;
extern bool isInTutorial;
extern bool tutorialDone;

void getTutorialText();
void checkTutorial();
void nextStep();
void endTutorial();
void updateTutorialDialogText();

#endif // TUTORIAL_H_