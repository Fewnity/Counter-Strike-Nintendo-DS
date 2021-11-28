#ifndef UI_H_ /* Include guard */
#define UI_H_
#include <NEMain.h>
#include "main.h"

extern int selectedServer;
extern int ShopCategory;
extern bool isShowingMap;
extern bool isShowingKeyBoard;

enum UiMenu
{
    GAME = 0,
    SCORE = 2,
    SHOPCATEGORIES = 3,
    SETTINGS = 4,
    QUIT = 5,
    SHOP = 6,
    CONTROLLER = 7,
    MAIN = 8,
    SERVERS = 9,
    CHANGENAME = 10,
    CHAT = 11
};

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
void drawButtons();

#endif // UI_H_