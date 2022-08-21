// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#ifndef UI_H_ /* Include guard */
#define UI_H_
#include <NEMain.h>
#include "main.h"

#define DISCORD_LINK "Discord : dsc.gg/fewnity"
#define DEV_NAME "Fewnity"
#define GAME_NAME "Counter Strike"

#define maxCharAtScreen 32
#define maxCharAtScreenDivided2 16 // 32/2
#define ScreenWidth 256
#define ScreenWidthDivided4 64
#define ScreenHeight 192
#define ScreenHeightFixed 196
#define ScreenCenterWidth 128 // 256/2
#define ScreenCenterHeight 96 // 192/2

#define ButtonCount 8
#define CheckBoxCount 3
#define SliderCount 2

#define ColorTerroristsTeam RGB15(26, 26, 13)
#define ColorCounterTeam RGB15(14, 20, 29)
#define ColorCounterTeamLiter RGB15(19, 25, 31)
#define maxCharAtLine 32
#define CONTROLS_PAGE_COUNT 3

#define ERROR_TEXT_LENGTH 100
#define PLAYER_MAX_LENGTH 14

extern int selectedServer;
extern enum shopCategory ShopCategory;
extern bool isShowingMap;
extern bool isShowingKeyBoard;
extern bool isLeftControls;
extern bool useQuitButton;
extern void (*lastOpenedMenu)();
extern int checkBoxToShow;
extern int sliderToShow;
extern int trace;
extern int currentSelectionMap;
extern float debugValue1;
extern float debugValue2;
extern float debugValue3;
extern char errorText[ERROR_TEXT_LENGTH];
extern bool showFinalScore;
extern bool showPing;
extern int showShootFriendMessage;
extern float cursorTransparency;

enum UiMenu
{
    GAME = 0,
    NIFI_MULTIPLAYER = 1, // Unused
    SCORE_BOARD = 2,
    SHOPCATEGORIES = 3,
    SETTINGS = 4,
    QUIT = 5,
    SHOP = 6,
    CONTROLLER = 7,
    MAIN = 8,
    MULTIPLAYER_NIFI_WIFI = 9,
    CHANGENAME = 10,
    CHAT = 11,
    CONTROLSSETTINGS = 12,
    CONTROLSCHANGE = 12,
    MAP_SELECTION_IMAGE = 13,
    MAP_SELECTION_LIST = 14,
    PARTY_MODE_SELECTION_LIST = 15,
    FINAL_SCORE = 16,
    JOIN_CREATE_PARTY = 17,
    ENTER_CODE = 18,
    ONLINE_ERROR = 19,
    STATS = 20,
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

enum TextEnum
{
    TERRORISTS_WIN = 0,
    COUNTER_TERRORISTS_WIN = 1,
    BOMB_PLANTED = 2,
    BOMB_DEFUSED = 3,
};

typedef struct //
{
    int xPos;
    int yPos;
    int xSize;
    int ySize;
    // bool value;
    bool *value;
} CheckBox;

typedef struct //
{
    int xPos;
    int yPos;
    int xSize;
    int ySize;
    float *value;
    float min;
    float max;
    float step;
} Slider;

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

enum shopCategory GetShopCategory();
int GetWeaponCountVar();
void SetCheckBoxToShow(int value);
void startChangeMenu(enum UiMenu menuToShow);
void ChangeMenu(int menuId);
void SetPlayerForUI();
int printLongText(int minX, int maxX, int y, char *text);
int printLongConstChar(int minX, int maxX, int y, const char *text);
void SetTeam(int i);
void OpenShopCategory(int categoryId);
void ChangeShopElement(int Left);
void BuyWeapon(int unused);
void Jump();
void changeServer(int up);
void showPartyEventText(int event);
void showKillText(int killerIndex, int deadPlayerIndex);
void showConnectedText(int connectedPlayerIndex);
void showDisconnectedText(int disconnectedPlayerIndex);
int centerPositionOfAText(int xMin, int xMax, int textLength);
void stopScanForInput();

void changeMapState();
void closeMap();
void setQuitButton(bool value);
void startScanForInput(int inputIndex);
void showDialog();
void setDialogText(char *text);

void initGameMenu();
void initScoreMenu();
void initShopCategoriesMenu();
void initSettingsMenu();
void initChangeNameMenu();
void initQuitMenu();
void initShopMenu();
void initControllerMenu();
void initMainMenu();
void initChatMenu();
void initControlsSettingsMenu();
void initControlsChangeMenu();
void initSelectionMapImageMenu();
void initSelectionMapListMenu();
void initPartyModeSelectionMenu();
void initFinalScoreMenu();
void initJoinCreatePartyMenu();
void initEnterCodeMenu();
void initOnlineErrorMenu();
void initStatsMenu();
void initMultiplayerNifiWifiMenu();
void initNifiMultiplayerMenu();

void drawKeyboardInput();

void drawTopScreenUI();
void drawBottomScreenUI();
void drawButtons();
void drawCheckboxs();
void drawSliders();
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
void drawControlsChangeMenu();
void drawSelectionMapImageMenu();
void drawSelectionMapListMenu();
void drawPartyModeSelectionMenu();
void drawJoinCreatePartyMenu();
void drawEnterCodeMenu();
void drawOnlineErrorMenu();
void drawStatsMenu();
void drawMultiplayerNifiWifiMenu();
void drawNifiMultiplayerMenu();

void unloadShopMenu();
void unloadControllerMenu();
void unloadSelectionMapImageMenu();
void unloadChangeNameMenu();
void unloadEnterCodeMenu();
void unloadControlsChangeMenu();

#endif // UI_H_