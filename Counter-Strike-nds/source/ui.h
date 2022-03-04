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
#define SliderCount 1

#define ColorTerroristsTeam RGB15(26, 26, 13)
#define ColorCounterTeam RGB15(14, 20, 29)
#define ColorCounterTeamLiter RGB15(19, 25, 31)
#define maxCharAtLine 32
#define CONTROLS_PAGE_COUNT 2

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
    CONTROLSSETTINGS = 12,
    CONTROLSCHANGE = 12,
    MAP_SELECTION_IMAGE = 13,
    MAP_SELECTION_LIST = 14,
    PARTY_MODE_SELECTION_LIST = 15
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

void SetButtonsRefForUI();
void SetSpritesForUI();
void SetCheckBoxsRefForUI();
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
void ChangeWeapon(int Left);
void BuyWeapon(int unused);
void Jump();
void changeServer(int up);
void showPartyEventText(int event);
void showKillText(int killerIndex, int deadPlayerIndex);
int centerPositionOfAText(int xMin, int xMax, int textLength);

void changeMapState();
void closeMap();
void setQuitButton(bool value);
void startScanForInput(int inputIndex);
void showDialog();
void setDialogText(char *text);

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
void initControlsChangeMenu();
void initSelectionMapImageMenu();
void initSelectionMapListMenu();

void drawNameChanging();

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

void unloadShopMenu();
void unloadControllerMenu();
void unloadSelectionMapImageMenu();

#endif // UI_H_