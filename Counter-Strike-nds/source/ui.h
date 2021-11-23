#ifndef UI_H_ /* Include guard */
#define UI_H_
#include <NEMain.h>
#include "main.h"

extern int selectedServer;
extern int ShopCategory;
extern bool isShowingMap;

void SetButtonsRefForUI();
void SetSpritesForUI();
void SetCheckBoxsRefForUI();
int GetShopCategory();
int GetWeaponCountVar();
Vector2 GetLastTouch();

void ChangeMenu(int menuId);
void SetPlayerForUI();
void printLongText(int minX, int maxX, int y, char *text);
void ReadTouchScreen(uint32 keysdown, uint32 keys, uint32 keysup, int *currentMenu, touchPosition touch, bool *NeedChangeScreen, bool *AlwaysUpdateBottomScreen, int *ButtonToShow, int *UpdateBottomScreenOneFrame, bool *SendTeam);
void SetTeam(int i);
void OpenShopCategory(int categoryId);
void ChangeWeapon(int Left);
void BuyWeapon(int unused);
void Jump();
void drawTopScreenUI();
void drawBottomScreenUI();
void changeServer(int up);
void showPartyEventText(int event);
void showKillText(int killerIndex, int deadPlayerIndex);

#endif // UI_H_