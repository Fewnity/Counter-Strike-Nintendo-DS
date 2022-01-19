#ifndef UI_H_ /* Include guard */
#define UI_H_
#include <NEMain.h>
#include "main.h"

#define maxCharAtScreen 32
#define maxCharAtScreenDivided2 16 // 32/2
#define ScreenWidth 256
#define ScreenWidthDivided4 64
#define ScreenHeight 192
#define ScreenHeightFixed 196
#define ScreenCenterWidth 128 // 256/2
#define ScreenCenterHeight 96 // 192/2

#define CheckBoxCount 2

#define ColorTerroristsTeam RGB15(26, 26, 13)
#define ColorCounterTeam RGB15(14, 20, 29)
#define ColorCounterTeamLiter RGB15(19, 25, 31)
#define maxCharAtLine 32

extern int selectedServer;
extern enum shopCategory ShopCategory;
extern bool isShowingMap;
extern bool isShowingKeyBoard;

enum UiMenu
{
    GAME = 0,
    GAME_FINISHED = 1,
    SCORE = 2,
    SHOPCATEGORIES = 3,
    SETTINGS = 4,
    QUIT = 5,
    SHOP = 6,
    CONTROLLER = 7,
    MAIN = 8,
    SERVERS = 9,
    CHANGENAME = 10,
    CHAT = 11,
    CONTROLSSETTINGS = 12
};

enum shopCategory
{
    PISTOLS = 0,
    HEAVY = 1,
    SGM = 2,
    RIFLES = 3,
    EQUIPMENT = 4,
    GRENADES = 5
};

typedef struct //
{
    int xPos;
    int yPos;
    int xSize;
    int ySize;
    bool value;
} CheckBox;

typedef struct //
{
    const char *text;
    int xTextPos;
    int yTextPos;

    int xPos;
    int yPos;
    int xSize;
    int ySize;
    int parameter;
    bool isHidden;
    void (*OnClick)(int);
} Button;

void SetButtonsRefForUI();
void SetSpritesForUI();
void SetCheckBoxsRefForUI();
enum shopCategory GetShopCategory();
int GetWeaponCountVar();

void startChangeMenu(enum UiMenu menuToShow);
void ChangeMenu(int menuId);
void SetPlayerForUI();
void printLongText(int minX, int maxX, int y, char *text);
void ReadTouchScreen(uint32 keysdown, uint32 keys, uint32 keysup, int *currentMenu, touchPosition touch, bool *NeedChangeScreen, bool *AlwaysUpdateBottomScreen, int *ButtonToShow, int *UpdateBottomScreenOneFrame, bool *SendTeam, bool forceCheck);
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
void changeMapState();
void closeMap();

void initGameMenu();
void initGameFinishedMenu();
void initScoreMenu();
void initShopCategoriesMenu();
void initSettingsMenu();
void initChangeNameMenu();
void initQuitMenu();
void initShopMenu();
void initControllerMenu();
void initMainMenu();
void initServerMenu();
void initChatMenu();
void initControlsSettingsMenu();

void drawGameMenu();
void drawGameFinishedMenu();
void drawScoreMenu();
void drawShopCategoriesMenu();
void drawSettingsMenu();
void drawQuitMenu();
void drawShopMenu();
void drawControllerMenu();
void drawMainMenu();
void drawServersMenu();
void drawChangeNameMenu();
void drawChatMenu();
void drawControlsSettingsMenu();

void unloadShopMenu();
void unloadControllerMenu();

#endif // UI_H_