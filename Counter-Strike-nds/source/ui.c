#include "main.h"
#include "movements.h"
#include "collisions.h"
#include "keyboard.h"
#include "network.h"
#include "ui.h"
#include "party.h"
#include "input.h"
#include "ai.h"
#include "map.h"
#include "sounds.h"
#include "gun.h"
#include "equipment.h"
#include "grenade.h"
#include "tutorial.h"
#include "keyboard.h"
#include <fat.h>

CheckBox *AllCheckBoxsRefForUI;
NE_Sprite **TopScreenSpritesForUI;
NE_Sprite **BottomScreenSpritesForUI;
NE_Material **BottomScreenSpritesMaterialsForUI;
NE_Palette **PalettesForUI;

enum shopCategory ShopCategory = PISTOLS; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
int WeaponCount = 0;
int selectedServer = 0;
int serverListOffset = 0;
int currentSelectionMap = 0;

bool isShowingMap = false;
bool isShowingKeyBoard = false;
bool isLeftControls = false;

void (*renderFunction)();
void (*onCloseMenu)();
void (*lastOpenedMenu)();
bool haveToCallOnCloseMenu = false;
bool useQuitButton = false;
int checkBoxToShow = 0;
int sliderToShow = 0;
int controlsPage = 0; // [0,1]

int trace = 0;
char *dialogText;

void SetSpritesForUI()
{
    TopScreenSpritesForUI = GetSpritesTop();
    BottomScreenSpritesForUI = GetSpritesBottom();
    BottomScreenSpritesMaterialsForUI = GetBottomScreenSpritesMaterials();
    PalettesForUI = GetPalettes();
}

void SetCheckBoxToShow(int value)
{
    checkBoxToShow = value;
}

void SetSliderToShow(int value)
{
    sliderToShow = value;
}

void SetCheckBoxsRefForUI()
{
    AllCheckBoxsRefForUI = GetCheckBoxs();
}

enum shopCategory GetShopCategory()
{
    return ShopCategory;
}

int GetWeaponCountVar()
{
    return WeaponCount;
}

void SetWeaponCountFromCategory()
{
    WeaponCount = 0;

    for (int i = 0; i < GunCount; i++)
    {
        if (AllGuns[i].gunCategory == ShopCategory && (AllPlayers[0].Team == AllGuns[i].team || AllGuns[i].team == BOTH))
            WeaponCount++;
    }
}

int printLongText(int minX, int maxX, int y, char *text)
{
    char tempText[maxX - minX];
    tempText[0] = '\0';

    char DamageText[50];
    sprintf(DamageText, text);

    char *ptr = strtok(DamageText, " ");
    int currentLine = 0;
    while (ptr != NULL)
    {
        if (strlen(tempText) > 0 && strlen(tempText) + 1 + strlen(ptr) > maxX - minX)
        {
            int fixedXPos = centerPositionOfAText(minX, maxX, strlen(tempText));
            NE_TextPrint(0,                          // Font slot
                         fixedXPos, y + currentLine, // Coordinates x(column), y(row)
                         NE_White,                   // Color
                         tempText);

            for (int i = 0; i < strlen(tempText); i++)
                tempText[i] = '\0';

            currentLine++;
        }

        sprintf(tempText + strlen(tempText), ptr);
        sprintf(tempText + strlen(tempText), " ");

        ptr = strtok(NULL, " ");
    }
    int fixedXPos = centerPositionOfAText(minX, maxX, strlen(tempText));
    NE_TextPrint(0,                          // Font slot
                 fixedXPos, y + currentLine, // Coordinates x(column), y(row)
                 NE_White,                   // Color
                 tempText);

    return currentLine + 1;
}

int printLongConstChar(int minX, int maxX, int y, const char *text)
{
    char tempText[maxX - minX];
    tempText[0] = '\0';

    char DamageText[50];
    sprintf(DamageText, text);

    char *ptr = strtok(DamageText, " ");
    int currentLine = 0;
    while (ptr != NULL)
    {
        if (strlen(tempText) > 0 && strlen(tempText) + 1 + strlen(ptr) > maxX - minX)
        {
            int fixedXPos = centerPositionOfAText(minX, maxX, strlen(tempText));
            NE_TextPrint(0,                          // Font slot
                         fixedXPos, y + currentLine, // Coordinates x(column), y(row)
                         NE_White,                   // Color
                         tempText);

            for (int i = 0; i < strlen(tempText); i++)
                tempText[i] = '\0';

            currentLine++;
        }

        sprintf(tempText + strlen(tempText), ptr);
        sprintf(tempText + strlen(tempText), " ");

        ptr = strtok(NULL, " ");
    }
    int fixedXPos = centerPositionOfAText(minX, maxX, strlen(tempText));
    NE_TextPrint(0,                          // Font slot
                 fixedXPos, y + currentLine, // Coordinates x(column), y(row)
                 NE_White,                   // Color
                 tempText);

    return currentLine + 1;
}

int centerPositionOfAText(int xMin, int xMax, int textLength)
{
    return xMin + ceil((xMax - xMin) / 2.0) - floor(textLength / 2.0);
}

void setDialogText(char *text)
{
    dialogText = text;
}

void showDialog()
{
    if (strlen(dialogText) > 0)
    {
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
        int ligneCount = printLongConstChar(1, 30, 1, dialogText);
        NE_PolyFormat(20, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
        NE_2DDrawQuad(0, 0, 256, (ligneCount + 2) * 8, 1, RGB15(0, 0, 0));
    }
}

void startChangeMenu(enum UiMenu menuToShow)
{
    SetButtonToShow(0);
    SetCheckBoxToShow(0);
    SetSliderToShow(0);

    if (haveToCallOnCloseMenu)
    {
        haveToCallOnCloseMenu = false;
        onCloseMenu();
    }

    for (int i = 0; i < 7; i++)
    {
        AllButtons[i].isHidden = false;
    }
    currentMenu = menuToShow;

    UpdateBottomScreenOneFrame += 8;
}

void ChangeMenu(int menuId)
{
    if (menuId == GAME)
    {
        initGameMenu();
    }
    else if (menuId == GAME_FINISHED) // map menu
    {
        initGameFinishedMenu();
    }
    else if (menuId == SCORE) // Score board screen
    {
        initScoreMenu();
    }
    else if (menuId == SHOPCATEGORIES) // Shop categories
    {
        initShopCategoriesMenu();
    }
    else if (menuId == SETTINGS) // Settings menu
    {
        initSettingsMenu();
    }
    else if (menuId == CHANGENAME) // Change name menu
    {
        initChangeNameMenu();
    }
    else if (menuId == QUIT) // Quit menu
    {
        initQuitMenu();
    }
    else if (menuId == SHOP) // SHOP
    {
        initShopMenu();
    }
    else if (menuId == CONTROLLER) // Game pad menu
    {
        initControllerMenu();
    }
    else if (menuId == MAIN) // Main menu
    {
        initMainMenu();
    }
    else if (menuId == SERVERS) // Multiplayer screen
    {
        initServerMenu();
    }
}

void OpenShopCategory(int categoryId)
{
    SetSelectedGunShop(-1);
    ShopCategory = categoryId;
    SetWeaponCountFromCategory();
    initShopMenu();
    ChangeWeapon(0);
}

void changeServer(int up)
{
    if (TotalIpCount != 0)
    {
        if (up == 1)
        {
            selectedServer++;
            if (selectedServer >= TotalIpCount)
            {
                selectedServer = 0;
                serverListOffset = 0;
            }
            else if (selectedServer - serverListOffset == 5)
                serverListOffset++;
        }
        else
        {
            selectedServer--;
            if (selectedServer < 0)
            {
                selectedServer = TotalIpCount - 1;
                serverListOffset = TotalIpCount - 5;
                if (serverListOffset < 0)
                    serverListOffset = 0;
            }
            else if (selectedServer - serverListOffset == -1)
                serverListOffset--;
        }
        // SetUpdateBottomScreenOneFrame(4);
    }
}

void changeMapState()
{
    isShowingMap = !isShowingMap;
    ForceUpdateLookRotation(CameraAngleY);
}

void closeMap()
{
    isShowingMap = false;
}

void BuyWeapon(int unused)
{
    if (Connection != OFFLINE)
    {
        SetSendBuyWeapon(true);
    }
    else
    {
        buyGun();
    }
    // SetGunInInventory(GetSelectedGunShop());
}

void ChangeWeapon(int Left)
{
    int Selected = GetSelectedGunShop();
    bool FoundNewGun = false;
    int FirstGunFound = -1;
    if (Left == 0)
    {
        for (int i = 0; i < GunCount + equipementCount + shopGrenadeCount; i++)
        {
            if ((ShopCategory < EQUIPMENT && i < GunCount && (AllGuns[i].gunCategory == ShopCategory && (AllPlayers[0].Team == AllGuns[i].team || AllGuns[i].team == -1))) || (ShopCategory == GRENADES && i >= GunCount && i < GunCount + shopGrenadeCount && (AllPlayers[0].Team == GetAllGrenades()[i - GunCount].team || GetAllGrenades()[i - GunCount].team == -1)) || (ShopCategory == EQUIPMENT && i >= GunCount + shopGrenadeCount && !AllEquipements[i - GunCount - shopGrenadeCount].isHided && (AllPlayers[0].Team == AllEquipements[i - GunCount - shopGrenadeCount].team || AllEquipements[i - GunCount - shopGrenadeCount].team == -1)))
            {
                if (FirstGunFound == -1)
                    FirstGunFound = i;

                if (i > Selected)
                {
                    Selected = i;
                    FoundNewGun = true;
                    break;
                }
            }
        }
    }
    else
    {
        for (int i = GunCount + equipementCount + shopGrenadeCount; i > -1; i--)
        {
            if ((ShopCategory < EQUIPMENT && i < GunCount && (AllGuns[i].gunCategory == ShopCategory && (AllPlayers[0].Team == AllGuns[i].team || AllGuns[i].team == -1))) || (ShopCategory == GRENADES && i >= GunCount && i < GunCount + shopGrenadeCount && (AllPlayers[0].Team == GetAllGrenades()[i - GunCount].team || GetAllGrenades()[i - GunCount].team == -1)) || (ShopCategory == EQUIPMENT && i >= GunCount + shopGrenadeCount && !AllEquipements[i - GunCount - shopGrenadeCount].isHided && (AllPlayers[0].Team == AllEquipements[i - GunCount - shopGrenadeCount].team || AllEquipements[i - GunCount - shopGrenadeCount].team == -1)))
            {
                if (FirstGunFound == -1)
                    FirstGunFound = i;

                if (i < Selected)
                {
                    Selected = i;
                    FoundNewGun = true;
                    break;
                }
            }
        }
    }

    if (!FoundNewGun)
        Selected = FirstGunFound;

    NE_PaletteDelete(PalettesForUI[10]);
    NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[6]);
    BottomScreenSpritesMaterialsForUI[6] = NE_MaterialCreate();
    PalettesForUI[10] = NE_PaletteCreate();
    if (ShopCategory < EQUIPMENT)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], AllGuns[Selected].texture, 1);
    else if (ShopCategory == GRENADES)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], GetAllGrenades()[Selected - GunCount].texture, 1);
    else if (ShopCategory == EQUIPMENT)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], AllEquipements[Selected - GunCount - shopGrenadeCount].texture, 1);

    SetSelectedGunShop(Selected);
    SetUpdateBottomScreenOneFrame(4);
}

void ChangeMap(int Left)
{
    if (Left == 0)
    {
        currentSelectionMap++;
        if (currentSelectionMap == MAP_COUNT)
            currentSelectionMap = 0;
    }
    else
    {
        currentSelectionMap--;
        if (currentSelectionMap == -1)
            currentSelectionMap = MAP_COUNT - 1;
    }

    NE_PaletteDelete(PalettesForUI[10]);
    NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[6]);
    BottomScreenSpritesMaterialsForUI[6] = NE_MaterialCreate();
    PalettesForUI[10] = NE_PaletteCreate();
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], allMaps[currentSelectionMap].image, 1);
}

void setQuitButton(bool value)
{
    useQuitButton = value;
    // Set button sprite
    NE_SpriteVisible(BottomScreenSpritesForUI[2], value);
}

void SetTeam(int i)
{
    if (applyRules)
    {
        AllPlayers[0].Team = i;
        UpdateBottomScreenOneFrame += 8;
    }
    else
    {
        SetWaitForTeamResponse(true);
        AllButtons[1].isHidden = true;
        AllButtons[2].isHidden = true;
        AllPlayers[0].Team = i; //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////REMOVE
        SetTempTeam(i);
        SetSendTeam(true);
    }
    // SetUpdateBottomScreenOneFrame(4);
}

void changeControlsPage(int isLeft)
{
    if (!isLeft)
    {
        controlsPage++;
        if (controlsPage == CONTROLS_PAGE_COUNT)
        {
            controlsPage = 0;
        }
    }
    else
    {
        controlsPage--;
        if (controlsPage == -1)
        {
            controlsPage = CONTROLS_PAGE_COUNT - 1;
        }
    }
    initControlsChangeMenu();
}

void showPartyEventText(int event)
{
    if (event == 0)
    {
        strncpy(textToShow, "Terrorists win", 30);
        PlayBasicSound(SFX_TERRORISTSWIN);
    }
    else if (event == 1)
    {
        strncpy(textToShow, "Counter Terrorist win", 30);
        PlayBasicSound(SFX_COUNTERTERRORISTWIN);
    }
    else if (event == 2)
    {
        strncpy(textToShow, "Bomb has been planted", 30);
        PlayBasicSound(SFX_BOMBHASBEENPLANTED);
    }
    else if (event == 3)
    {
        strncpy(textToShow, "Bomb has been defused", 30);
        PlayBasicSound(SFX_BOMBHASBEENDEFUSED);
    }

    textToShowTimer = 400;
}

void showKillText(int killerIndex, int deadPlayerIndex)
{
    sprintf(killText, "{%d", (int)AllPlayers[killerIndex].Team);
    sprintf(killText + strlen(killText), AllPlayers[killerIndex].name); // GET PLAYER NAME

    sprintf(killText + strlen(killText), " {2killed ");

    sprintf(killText + strlen(killText), "{%d", (int)AllPlayers[deadPlayerIndex].Team);
    sprintf(killText + strlen(killText), AllPlayers[deadPlayerIndex].name); // GET PLAYER NAME

    KillTextShowTimer = 240;
}

void startScanForInput(int inputIndex)
{
    for (int i = 0; i < INPUT_COUNT; i++)
    {
        if (inputs[i].nameIndex == 14)
        {
            inputs[i].nameIndex = 12;
        }
    }

    if (inputs[inputIndex].value != 0)
    {
        inputs[inputIndex].value = 0;
        inputs[inputIndex].nameIndex = 12;
    }
    else
    {
        scanForInput = true;
        currentInputScanned = inputIndex;
        inputs[inputIndex].nameIndex = 13;
    }
}

void updateKeyboardModeButton()
{
    if (isAzerty)
    {
        AllButtons[2].text = "Azerty";
    }
    else
    {
        AllButtons[2].text = "Qwerty";
    }
}

void drawNameChanging()
{
    NE_2DDrawQuad(0, 0, ScreenWidth, ScreenHeightFixed, 3, RGB15(3, 3, 3)); // Background
    if (currentMenu == CHANGENAME)
    {
        NE_TextPrint(0,        // Font slot
                     11, 2,    // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Change name");

        char editedName[21];

        // char editedName[40];
        // sprintf(editedName, "%s", localPlayer->name);
        sprintf(editedName, "%s", tempText);
        // strncpy(editedName, tempText, 21);

        for (int i = strlen(editedName); i < 20; i++)
        {
            sprintf(editedName + strlen(editedName), "_");
        }

        NE_TextPrint(0,        // Font slot
                     6, 12,    // Coordinates x(column), y(row)
                     NE_White, // Color
                     editedName);
    }
}

void drawTopScreenUI()
{
    Player *selectPlayer = &AllPlayers[CurrentCameraPlayer];

    // Set view in 2D mode
    NE_2DViewInit();

    // Only for debug
    int TextYOffset = 0;
    if (!isDebugTopScreen)
        TextYOffset = -3;

    // Player *localPlayer = &AllPlayers[0];
    if (isShowingKeyBoard)
    {
        drawNameChanging();
    }
    else if (isShowingMap)
    {
        NE_2DDrawQuad(0, 0, ScreenWidth, ScreenHeightFixed, 3, RGB15(0, 0, 0)); // Background
        // Calculate map position
        float mapXPos = map(selectPlayer->position.x, -46, 57.5, -170, 170);
        float mapYPos = map(selectPlayer->position.z, -41, 67, -177, 177);
        // Set map position and draw sprite
        NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 170 / 2 + (mapXPos * zWithoutYForMap - mapYPos * xWithoutYForMap) / 2, ScreenCenterHeight - 177 / 2 + 2 + (mapYPos * zWithoutYForMap + mapXPos * xWithoutYForMap) / 2);
        NE_SpriteSetRot(TopScreenSprites[4], (int)selectPlayer->Angle);
        NE_SpriteDraw(TopScreenSprites[4]);

        for (int i = 1; i < MaxPlayer; i++)
        {
            // If the other player is in game
            if (AllPlayers[i].Id != UNUSED && AllPlayers[i].Team != SPECTATOR && !AllPlayers[i].IsDead)
            {
                // Calculate other players points
                float xPos2 = map(AllPlayers[i].position.x, -46, 57.5, -170, 170);
                float ypos2 = map(AllPlayers[i].position.z, -41, 67, -177, 177);
                float xPoint = ScreenCenterWidth - ((xPos2 - mapXPos) * zWithoutYForMap + (ypos2 - mapYPos) * -xWithoutYForMap) / 2.0;
                float yPoint = ScreenCenterHeight + ((ypos2 - mapYPos) * -zWithoutYForMap + (xPos2 - mapXPos) * -xWithoutYForMap) / 2.0;
                float finalXPoint1 = xPoint - 4, finalYPoint1 = yPoint - 2;
                float finalXPoint2 = xPoint + 2, finalYPoint2 = yPoint + 4;

                // Set point color and draw it
                if (AllPlayers[i].Team == AllPlayers[0].Team)
                    NE_2DDrawTexturedQuadColor(finalXPoint1, finalYPoint1, finalXPoint2, finalYPoint2, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 31)); // Friends points
                else
                    NE_2DDrawTexturedQuadColor(finalXPoint1, finalYPoint1, finalXPoint2, finalYPoint2, 1, BottomScreenSpritesMaterials[2], RGB15(31, 0, 0)); // Enemies points
            }
        }

        if (bombDropped)
        {
            float bombXPos = map(droppedBombPositionAndRotation.x, -46, 57.5, -170, 170);
            float bombYpos = map(droppedBombPositionAndRotation.z, -41, 67, -177, 177);
            float bombXPoint = ScreenCenterWidth - ((bombXPos - mapXPos) * zWithoutYForMap + (bombYpos - mapYPos) * -xWithoutYForMap) / 2.0;
            float bombYPoint = ScreenCenterHeight + ((bombYpos - mapYPos) * -zWithoutYForMap + (bombXPos - mapXPos) * -xWithoutYForMap) / 2.0;
            float bombFinalXPoint1 = bombXPoint - 4, bombFinalYPoint1 = bombYPoint - 2;
            float bombFinalXPoint2 = bombXPoint + 4, bombFinalYPoint2 = bombYPoint + 4;

            NE_2DDrawTexturedQuadColor(bombFinalXPoint1, bombFinalYPoint1, bombFinalXPoint2, bombFinalYPoint2, 1, TopScreenSpritesMaterials[5], RGB15(31, 0, 0)); // Bomb
        }

        NE_2DDrawTexturedQuadColor(ScreenCenterWidth - 4, ScreenCenterHeight - 2, ScreenCenterWidth + 2, ScreenCenterHeight + 4, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 0)); // Background
    }
    else
    {
        if (isDebugTopScreen)
        {
            // DEBUG show player position
            char PositionText[30];
            sprintf(PositionText, "%f %f %f", localPlayer->position.x, localPlayer->position.y, localPlayer->position.z);
            NE_TextPrint(0,        // Font slot
                         1, 0,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         PositionText);

            // DEBUG show CPU usage
            char CPU[40];

            // sprintf(CPU, "CPU : %d, Zone : %d", NE_GetCPUPercent(), localPlayer->CurrentOcclusionZone);
            sprintf(CPU, "CPU : %d%%, Mem : %d%% %d", NE_GetCPUPercent(), NE_TextureFreeMemPercent(), localPlayer->inShadow);

            // sprintf(CPU, "t %d m %d c %d", trace, currentMenu, Connection);

            NE_TextPrint(0,        // Font slot
                         1, 1,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         CPU);

            char CPU2[120] = "";
            for (int i = 0; i < MaxPlayer; i++)
            {
                sprintf(CPU2 + strlen(CPU2), "%s %d %d\n", AllPlayers[i].name, AllPlayers[i].searchForDroppedBomb, AllPlayers[i].haveBomb);
            }

            NE_TextPrint(0,        // Font slot
                         0, 9,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         CPU2);

            // DEBUG show map zone to show
            char Occlusion[20];
            sprintf(Occlusion, "Zone to show : ");
            for (int i = 0; i < allMaps[currentMap].AllZones[localPlayer->CurrentOcclusionZone].ZoneCount; i++)
                sprintf(Occlusion + strlen(Occlusion), "%d ", allMaps[currentMap].AllZones[localPlayer->CurrentOcclusionZone].visibleMapPart[i]);

            // for (int i = 0; i < AllZones[17].ZoneCount; i++)
            // sprintf(Occlusion + strlen(Occlusion), "%d ", AllZones[17].AllVisibleZones[i]);

            NE_TextPrint(0,        // Font slot
                         1, 2,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         Occlusion);
        }

        // if (true) // TODO REMOVE THS
        if (!BombPlanted)
        {
            if (!allPartyModes[currentPartyMode].infiniteTimer && (!localPlayer->IsDead || roundState == TRAINING))
            {
                NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(ScreenCenterWidth - 25, 8, ScreenCenterWidth + 24, 16, 1, RGB15(3, 3, 3));
                NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                // Show party timer if bomb is not planted
                // Minutes
                char Timer[30];
                if (PartyMinutes > 9)
                    sprintf(Timer, "%d::", PartyMinutes);
                else
                    sprintf(Timer, "0%d::", PartyMinutes);

                // Seconds
                if (PartySeconds > 9)
                    sprintf(Timer + strlen(Timer), "%d", PartySeconds);
                else
                    sprintf(Timer + strlen(Timer), "0%d", PartySeconds);

                if (isDebugTopScreen)
                    sprintf(Timer + strlen(Timer), " State : %d", roundState);

                NE_TextPrint(0,                   // Font slot
                             13, 4 + TextYOffset, // Coordinates x(column), y(row)
                             NE_White,            // Color
                             Timer);
            }

            // Show planting bomb animation
            if (localPlayer->bombTimer < bombPlantingTime && localPlayer->bombTimer > 0 && !localPlayer->IsDead)
            {
                // Draw Background
                NE_PolyFormat(20, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(ScreenWidthDivided4, 155, ScreenWidth - mapInt(localPlayer->bombTimer, -2, bombPlantingTime - 0, ScreenWidthDivided4 * 3, ScreenWidthDivided4), 170, 1, RGB15(3, 3, 3));
                NE_PolyFormat(24, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(84, 140, 176, 155, 1, RGB15(3, 3, 3));
                NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

                // Show planting text
                char plantingText[12];
                sprintf(plantingText, "Planting...");
                NE_TextPrint(0,        // Font slot
                             11, 18,   // Coordinates x(column), y(row)
                             NE_White, // Color
                             plantingText);
            }
        }
        else
        {
            // Show defusing bomb animation
            if (localPlayer->bombTimer < bombDefuseTime && localPlayer->bombTimer > 0)
            {
                // Draw Background
                NE_PolyFormat(20, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(ScreenWidthDivided4, 155, ScreenWidth - mapInt(localPlayer->bombTimer, -2, bombDefuseTime, ScreenWidth / 4 * 3, ScreenWidthDivided4), 170, 1, RGB15(3, 3, 3));
                NE_PolyFormat(24, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(84, 140, 176, 155, 1, RGB15(3, 3, 3));
                NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

                // Show defusing text
                char defusingText[12];
                sprintf(defusingText, "Defusing...");
                NE_TextPrint(0,        // Font slot
                             11, 18,   // Coordinates x(column), y(row)
                             NE_White, // Color
                             defusingText);
            }
        }

        if (!allPartyModes[currentPartyMode].noScore)
        {
            NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
            NE_2DDrawQuad(ScreenCenterWidth - 25, 0, ScreenCenterWidth + 24, 8, 1, RGB15(3, 3, 3));
            NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
            // Show party score
            char PartyScore[15];
            //  CountersT score
            if (CounterScore > 9)
                sprintf(PartyScore, "%d  ", CounterScore);
            else
                sprintf(PartyScore, "0%d  ", CounterScore);

            NE_TextPrint(0,                   // Font slot
                         13, 3 + TextYOffset, // Coordinates x(column), y(row)
                         ColorCounterTeam,    // Color
                         PartyScore);

            // Terrosists score
            if (TerroristsScore > 9)
                // sprintf(PartyScoreTerrorist, "%d", TerroristsScore);
                sprintf(PartyScore, "%d", TerroristsScore);
            else
                // sprintf(PartyScoreTerrorist, "0%d", TerroristsScore);
                sprintf(PartyScore, "0%d", TerroristsScore);

            NE_TextPrint(0,                   // Font slot
                         17, 3 + TextYOffset, // Coordinates x(column), y(row)
                         ColorTerroristsTeam, // Color
                         PartyScore);
        }

        if (Connection != OFFLINE)
        {
            // DEBUG for player show ping
            char PING[9];
            sprintf(PING, "Ping : %d", ping);
            NE_TextPrint(0,                  // Font slot
                         1, 5 + TextYOffset, // Coordinates x(column), y(row)
                         NE_White,           // Color
                         PING);
        }
        else
        {
            TextYOffset--;
        }

        // Show player money
        /*char MoneyText[30];
        sprintf(MoneyText, "%d$", localPlayer->Money);
        NE_TextPrint(0,                  // Font slot
                     1, 6 + TextYOffset, // Coordinates x(column), y(row)
                     NE_White,           // Color
                     MoneyText);*/

        // Show event text
        if (textToShowTimer > 0)
        {
            int backgroundSize = strlen(textToShow) * 8;
            int backgroundOffset = 1;
            NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
            NE_2DDrawQuad(backgroundOffset, (7 + TextYOffset) * 8 - 1, backgroundOffset + backgroundSize, (8 + TextYOffset) * 8 + 1, 1, RGB15(3, 3, 3));
            NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

            NE_TextPrint(0,                  // Font slot
                         0, 7 + TextYOffset, // Coordinates x(column), y(row)
                         NE_White,           // Color
                         textToShow);
        }

        // Show kill text
        if (KillTextShowTimer > 0)
        {
            char DamageText[50];
            sprintf(DamageText, killText);

            char *ptr = strtok(DamageText, "{");

            int currentLine = 0;
            while (ptr != NULL)
            {
                if (strncmp(ptr, "1", 1) == 0)
                {
                    NE_TextPrint(0,                            // Font slot
                                 currentLine, 8 + TextYOffset, // Coordinates x(column), y(row)
                                 ColorCounterTeamLiter,        // Color
                                 ptr + 1);
                }
                else if (strncmp(ptr, "0", 1) == 0)
                {
                    NE_TextPrint(0,                            // Font slot
                                 currentLine, 8 + TextYOffset, // Coordinates x(column), y(row)
                                 ColorTerroristsTeam,          // Color
                                 ptr + 1);
                }
                else
                {
                    NE_TextPrint(0,                            // Font slot
                                 currentLine, 8 + TextYOffset, // Coordinates x(column), y(row)
                                 NE_White,                     // Color
                                 ptr + 1);
                }

                currentLine += strlen(ptr) - 1;
                ptr = strtok(NULL, "{");
            }

            int backgroundSize = currentLine * 8;
            int backgroundOffset = 0;
            NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
            NE_2DDrawQuad(backgroundOffset, (8 + TextYOffset) * 8 - 1, backgroundOffset + backgroundSize, (9 + TextYOffset) * 8 + 1, 1, RGB15(3, 3, 3));
            NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
        }

        if (redHealthTextCounter != 0)
        {
            redHealthTextCounter--;
        }

        if (selectPlayer->Team != SPECTATOR)
        {
            if (!selectPlayer->IsDead)
            {
                if (CurrentCameraPlayer != 0)
                {
                    int backgroundSize = strlen(selectPlayer->name) * 8;
                    int textPosition = maxCharAtScreenDivided2 - strlen(selectPlayer->name) / 2;
                    int backgroundOffset = textPosition * 8;
                    NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                    NE_2DDrawQuad(backgroundOffset, 168, backgroundOffset + backgroundSize, 176, 1, RGB15(3, 3, 3));
                    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

                    char ViewedBotName[21] = "";
                    sprintf(ViewedBotName, "%s", selectPlayer->name);
                    NE_TextPrint(0,                // Font slot
                                 textPosition, 21, // Coordinates x(column), y(row)
                                 NE_White,         // Color
                                 ViewedBotName);
                }

                // Show gun ammo
                if (selectPlayer->currentGunInInventory == 1 || selectPlayer->currentGunInInventory == 2)
                {
                    char AmmoText[16];
                    // sprintf(AmmoText, "%d/%d", localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount, localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].TotalAmmoCount);
                    sprintf(AmmoText, "%d/%d", selectPlayer->AllAmmoMagazine[selectPlayer->currentGunInInventory - 1].AmmoCount, selectPlayer->AllAmmoMagazine[selectPlayer->currentGunInInventory - 1].TotalAmmoCount);

                    int backgroundSize = strlen(AmmoText) * 8;
                    int backgroundOffset = 8;
                    NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                    NE_2DDrawQuad(backgroundOffset - 1, 175, backgroundOffset + backgroundSize, 184, 1, RGB15(3, 3, 3));
                    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

                    NE_TextPrint(0,        // Font slot
                                 1, 22,    // Coordinates x(column), y(row)
                                 NE_White, // Color
                                 AmmoText);
                }

                // Show health
                char HealthText[16];
                sprintf(HealthText, "+%d arm:%d", selectPlayer->Health, selectPlayer->armor);

                int backgroundSize = strlen(HealthText) * 8;
                int backgroundOffset = 8;
                NE_PolyFormat(13, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(backgroundOffset - 1, 183, backgroundOffset + backgroundSize, 193, 1, RGB15(3, 3, 3));
                NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

                // Print white text
                if (redHealthTextCounter == 0)
                {
                    NE_TextPrint(0,        // Font slot
                                 1, 23,    // Coordinates x(column), y(row)
                                 NE_White, // Color
                                 HealthText);
                }
                else // Print red text after taking damage
                {
                    int ColorAmount = 31 - redHealthTextCounter / 2;
                    // redHealthTextCounter--;
                    NE_TextPrint(0,                                   // Font slot
                                 1, 23,                               // Coordinates x(column), y(row)
                                 RGB15(31, ColorAmount, ColorAmount), // Color
                                 HealthText);
                }
            }

            if (CurrentScopeLevel == 0) // Draw gun if player doesn't aim with his gun
            {
                // Draw cross air
                NE_SpriteDraw(TopScreenSprites[0]);

                //  Draw gun
                if (!selectPlayer->IsDead)
                {
                    // Set new gun position with head bobbing
                    if (selectPlayer->currentGunInInventory >= GunCount || !AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].isKnife) // TODO Remove first condition?
                    {
                        rightGunX = GunPositionX + selectPlayer->rightGunXRecoil + sinf(BobbingOffset) * 4;
                        rightGunY = GunPositionY + 4 + selectPlayer->rightGunYRecoil + cosf(BobbingOffset * 2) * 6;
                        leftGunX = -selectPlayer->leftGunXRecoil + sinf(BobbingOffset) * 4;
                        leftGunY = GunPositionY + 4 + selectPlayer->leftGunYRecoil + cosf(BobbingOffset * 2) * 6;
                    }
                    else
                    {
                        rightGunX = GunPositionX - selectPlayer->rightGunXRecoil + GunMaxRecoil * 1.15 + sinf(BobbingOffset) * 4;
                        rightGunY = GunPositionY + 4 - selectPlayer->rightGunYRecoil + GunMaxRecoil * 1.4 + cosf(BobbingOffset * 2) * 6;
                    }

                    if (selectPlayer->isReloading)
                    {
                        float valueToAdd = sinf((float)selectPlayer->GunReloadWaitCount / (float)AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].ReloadTime * M_PI) * 70;
                        rightGunY += valueToAdd;
                        leftGunY += valueToAdd;
                    }
                    int lightCoef = 31 * selectPlayer->lightCoef;
                    // NE_2DDrawTexturedQuad(rightGunX, rightGunY, rightGunX + 96, rightGunY + 96, 1, TopScreenSpritesMaterials[1]); // Gun
                    NE_2DDrawTexturedQuadColor(rightGunX, rightGunY, rightGunX + 96, rightGunY + 96, 1, TopScreenSpritesMaterials[1], RGB15(lightCoef, lightCoef, lightCoef));
                    if (AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].isDualGun)
                        NE_2DDrawTexturedQuadColor(leftGunX + 96, leftGunY, leftGunX, leftGunY + 96, 1, TopScreenSpritesMaterials[1], RGB15(lightCoef, lightCoef, lightCoef)); // Gun
                }

                // Draw gun muzzle flash
                if (selectPlayer->currentGunInInventory < GunCount && ShowMuzzle > 0 && !AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].isKnife)
                {
                    NE_PolyFormat(10 * ShowMuzzle, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                    if (selectPlayer->isRightGun)
                        NE_2DDrawTexturedQuad(rightGunX + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].xMuzzleFlashOffset, rightGunY + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].yMuzzleFlashOffset, rightGunX + 40 + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].xMuzzleFlashOffset, rightGunY + 42 + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].yMuzzleFlashOffset, 2, TopScreenSpritesMaterials[2]); // Gun muzzle flash
                    else
                        NE_2DDrawTexturedQuad(leftGunX + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].xMuzzleFlashOffset + 30, leftGunY + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].yMuzzleFlashOffset, leftGunX + 70 + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].xMuzzleFlashOffset, leftGunY + 42 + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].yMuzzleFlashOffset, 2, TopScreenSpritesMaterials[2]); // Gun muzzle flash
                }
            }
            else // Draw scope screen when aiming
            {
                NE_2DDrawQuad(0, 0, 46, 196, 1, RGB15(1, 1, 1));                                        // Left border
                NE_2DDrawQuad(ScreenWidth - 46, 0, ScreenWidth, 196, 1, RGB15(1, 1, 1));                // Right border
                NE_2DDrawQuad(0, 0, ScreenWidth, 12, 1, RGB15(1, 1, 1));                                // Top border
                NE_2DDrawQuad(0, ScreenHeight - 12, ScreenWidth, ScreenHeightFixed, 1, RGB15(1, 1, 1)); // Bottom border

                NE_PolyFormat(10, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);                      // Set alpha
                NE_2DDrawQuad(ScreenCenterWidth, 0, ScreenCenterWidth, 196, 1, NE_Black);           // Top to bottom line
                NE_2DDrawQuad(0, ScreenCenterHeight, ScreenWidth, ScreenCenterHeight, 1, NE_Black); // Left to Right line

                NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION); // Set alpha
                // Draw scope image
                NE_2DDrawTexturedQuad(ScreenCenterWidth - 82, ScreenCenterHeight - 84, ScreenCenterWidth + 82, ScreenCenterHeight + 84, 1, TopScreenSpritesMaterials[3]); // Circle
            }
        }

        float totalDis = 0;
        int totalEndAnimation = 0;
        for (int i = 0; i < GrenadeCount; i++)
        {
            if (grenades[i] != NULL && grenades[i]->GrenadeType == SMOKE && grenades[i]->Timer == 0)
            {
                // Calculate distances
                float smokeDistance = sqrtf(powf(selectPlayer->PlayerModel->x - grenades[i]->Model->x, 2.0) + powf(selectPlayer->PlayerModel->y - grenades[i]->Model->y, 2.0) + powf(selectPlayer->PlayerModel->z - grenades[i]->Model->z, 2.0)) / 26000.0; // fFor smoke detection

                // Set a minimum limit to the smoke detection distance
                if (smokeDistance > 1)
                {
                    smokeDistance = 1;
                }
                // Invert distance (more distance decrease the value)
                smokeDistance = map(1 - smokeDistance, 0, 0.2, 0, 1);
                totalDis += smokeDistance;
                totalEndAnimation += grenades[i]->EffectTimer;

                if (totalDis >= 1)
                {
                    totalDis = 1;
                    isInFullSmoke = true;
                    break;
                }
            }
        }

        // If player is in a smoke area
        if (totalDis > 0)
        {
            if (totalEndAnimation > 20)
            {
                totalEndAnimation = 20;
            }

            // Calculate smoke screen alpha
            totalEndAnimation = map(totalEndAnimation, 0, 20, 31, 0);
            int finalAlpha = 31 * totalDis - totalEndAnimation;
            if (finalAlpha < 0)
                finalAlpha = 0;

            NE_PolyFormat(finalAlpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION); // Set alpha
            // Draw smoke screen
            NE_2DDrawQuad(0, 0, ScreenWidth, ScreenHeightFixed, 0, RGB15(9, 9, 9));
        }
    }
    showDialog();
    for (int i = 0; i < MaxPlayer; i++)
    {
        Player *player = &AllPlayers[i];

        // If player is flashed, show white screen
        if (player->flashed)
        {
            if (player->IsDead || player->Id == NO_PLAYER)
            {
                player->flashed = false;
                player->flashAnimation = 0;
                continue;
            }

            player->flashAnimation += flashAnimationSpeed;

            int alpha = 31;
            if (player->flashAnimation < flashAnimationSpeed * 5.0)
            {
                alpha = (int)((player->flashAnimation / (flashAnimationSpeed * 5.0)) * 31.0);
            }
            else if (player->flashAnimation >= flashAnimationSpeed * 120.0)
            {
                alpha = map(player->flashAnimation, flashAnimationSpeed * 120.0, 1, 31, 0);
            }
            if (alpha < 1)
                alpha = 1;

            if (!isShowingMap && !isShowingKeyBoard && selectPlayer == player)
            {
                NE_PolyFormat(alpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION); // Set alpha
                NE_2DDrawQuad(0, 0, ScreenWidth, ScreenHeightFixed, 0, RGB15(31, 31, 31));
            }

            if (player->flashAnimation >= 1)
            {
                player->flashed = false;
                player->flashAnimation = 0;
            }
        }
    }
    if (KillTextShowTimer > 0)
        KillTextShowTimer--;

    if (textToShowTimer > 0)
        textToShowTimer--;

    // Reduce muzzle timer
    if (ShowMuzzle > 0)
        ShowMuzzle--;
}

void drawBottomScreenUI()
{
    // Set view in 2D mode
    NE_2DViewInit();

    if (isShowingKeyBoard)
    {
        drawKeyBoard();
    }
    else
    {
        // Draw sprites
        for (int i = 0; i < BottomScreenSpriteCount; i++)
            NE_SpriteDraw(BottomScreenSprites[i]);

        // Draw menu background
        NE_2DDrawQuad(0, 0, 256, 196, 20, RGB15(3, 3, 3));

        renderFunction();
        drawButtons();
        drawCheckboxs();
        drawSliders();
    }
    // Change screen update mode (30 fps with 2 screens update or 60 fps with main game screen update)
    if (NeedChangeScreen)
    {
        NeedChangeScreen = false;
        AlwaysUpdateBottomScreen = !AlwaysUpdateBottomScreen;
        UpdateBottomScreenOneFrame = 4;
    }
}

void initGameMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(GAME);

    renderFunction = &drawGameMenu;

    lastOpenedMenu = &initGameMenu;

    setQuitButton(false);

    // Show score button
    AllButtons[0].xPos = 15;
    AllButtons[0].yPos = 72;
    AllButtons[0].xSize = 100;
    AllButtons[0].ySize = 24;
    AllButtons[0].OnClick = &initScoreMenu;
    AllButtons[0].xTextPos = 3;
    AllButtons[0].yTextPos = 10;
    if (localPlayer->Team == SPECTATOR)
        AllButtons[0].text = "Choose team";
    else
        AllButtons[0].text = "Show score";

    // Open shop categories button
    AllButtons[1].xPos = 15;
    AllButtons[1].yPos = 104;
    AllButtons[1].xSize = 100;
    AllButtons[1].ySize = 24;
    AllButtons[1].OnClick = &initShopCategoriesMenu;
    AllButtons[1].xTextPos = 3;
    AllButtons[1].yTextPos = 14;
    AllButtons[1].text = "Open shop";

    // Settings button
    AllButtons[2].xPos = 15;
    AllButtons[2].yPos = 136;
    AllButtons[2].xSize = 100;
    AllButtons[2].ySize = 24;
    AllButtons[2].OnClick = &initSettingsMenu;
    AllButtons[2].xTextPos = 3;
    AllButtons[2].yTextPos = 18;
    AllButtons[2].text = "Settings";

    // Quit game button
    AllButtons[3].xPos = 15;
    AllButtons[3].yPos = 168;
    AllButtons[3].xSize = 100;
    AllButtons[3].ySize = 24;
    AllButtons[3].OnClick = &initQuitMenu;
    AllButtons[3].xTextPos = 3;
    AllButtons[3].yTextPos = 22;
    AllButtons[3].text = "Quit game";

    // Game pad button
    AllButtons[4].xPos = 141;
    AllButtons[4].yPos = 40;
    AllButtons[4].xSize = 100;
    AllButtons[4].ySize = 24;
    AllButtons[4].OnClick = &initControllerMenu;
    AllButtons[4].xTextPos = 19;
    AllButtons[4].yTextPos = 6;
    AllButtons[4].text = "Gamepad";

    if (AllPlayers[0].Team == SPECTATOR)
    {
        AllButtons[1].isHidden = true;
        AllButtons[4].isHidden = true;
    }
    if (isInTutorial)
    {
        AllButtons[0].isHidden = true;
        AllButtons[1].isHidden = true;
    }

    SetButtonToShow(5);
}

void initGameFinishedMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(GAME_FINISHED);

    renderFunction = &drawGameFinishedMenu;

    lastOpenedMenu = &initMainMenu;

    setQuitButton(true);
}

void initScoreMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(SCORE);

    renderFunction = &drawScoreMenu;

    lastOpenedMenu = &initGameMenu;

    setQuitButton(true);

    if (AllPlayers[0].Team == -1)
    {
        // Join counter button
        AllButtons[0].xPos = 18; // 128-20-90
        AllButtons[0].yPos = 169;
        AllButtons[0].xSize = 90;
        AllButtons[0].ySize = 20;
        AllButtons[0].OnClick = &SetTeam;
        AllButtons[0].parameter = COUNTERTERRORISTS;
        AllButtons[0].xTextPos = 6;
        AllButtons[0].yTextPos = 22;
        AllButtons[0].text = "Join";

        // Join terrorists button
        AllButtons[1].xPos = 148; // 128+20
        AllButtons[1].yPos = 169;
        AllButtons[1].xSize = 90;
        AllButtons[1].ySize = 20;
        AllButtons[1].OnClick = &SetTeam;
        AllButtons[1].parameter = TERRORISTS;
        AllButtons[1].xTextPos = 22;
        AllButtons[1].yTextPos = 22;
        AllButtons[1].text = "Join";

        SetButtonToShow(2);
    }
    else
        SetButtonToShow(0);
}

void initShopCategoriesMenu()
{
    if (!localPlayer->IsDead && shopDisableTimer != 0 && isInShopZone)
    {
        SetTwoScreenMode(false);

        startChangeMenu(SHOPCATEGORIES);

        renderFunction = &drawShopCategoriesMenu;

        lastOpenedMenu = &initGameMenu;

        setQuitButton(true);

        for (int buttonIndex = 0; buttonIndex < 6; buttonIndex++)
        {
            // Set all categories buttons
            AllButtons[buttonIndex].xPos = (ScreenWidth / 2) * (buttonIndex / 3);
            AllButtons[buttonIndex].yPos = ((198 - 23) / 3) * (buttonIndex % 3) + 23;
            AllButtons[buttonIndex].xSize = ScreenWidth / 2;
            AllButtons[buttonIndex].ySize = (198 - 23) / 3;
            AllButtons[buttonIndex].OnClick = &OpenShopCategory;
            AllButtons[buttonIndex].parameter = buttonIndex;
            AllButtons[buttonIndex].text = "";
            // if (buttonIndex == 4 || buttonIndex == 5)
            //  AllButtons[buttonIndex].isHidden = true;
        }
        SetButtonToShow(6);
    }
}

void initSettingsMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(SETTINGS);

    renderFunction = &drawSettingsMenu;

    if (Connection == UNSELECTED)
        lastOpenedMenu = &initMainMenu;
    else
        lastOpenedMenu = &initGameMenu;

    setQuitButton(true);

    // Set rumble checkbox
    AllCheckBoxs[0].xPos = 58;
    AllCheckBoxs[0].yPos = 56;
    AllCheckBoxs[0].xSize = 20;
    AllCheckBoxs[0].ySize = 20;
    AllCheckBoxs[0].value = &useRumble;

    // Set 3DS mode checkbox
    AllCheckBoxs[1].xPos = 180;
    AllCheckBoxs[1].yPos = 56;
    AllCheckBoxs[1].xSize = 20;
    AllCheckBoxs[1].ySize = 20;
    AllCheckBoxs[1].value = &is3dsMode;

    // Set change name button
    AllButtons[0].xPos = 15;
    AllButtons[0].yPos = 88;
    AllButtons[0].xSize = 100;
    AllButtons[0].ySize = 24;
    AllButtons[0].OnClick = &initChangeNameMenu;
    AllButtons[0].xTextPos = 3;
    AllButtons[0].yTextPos = 12;
    AllButtons[0].text = "Change name";

    // Set change controls button
    AllButtons[1].xPos = 140;
    AllButtons[1].yPos = 88;
    AllButtons[1].xSize = 100;
    AllButtons[1].ySize = 24;
    AllButtons[1].OnClick = &initControlsSettingsMenu;
    AllButtons[1].xTextPos = 20;
    AllButtons[1].yTextPos = 12;
    AllButtons[1].text = "Controls";

    AllButtons[2].xPos = 140;
    AllButtons[2].yPos = 137;
    AllButtons[2].xSize = 100;
    AllButtons[2].ySize = 23;
    AllButtons[2].OnClick = &changeKeyboardMode;
    AllButtons[2].xTextPos = 21;
    AllButtons[2].yTextPos = 18;
    updateKeyboardModeButton();

    SetButtonToShow(3);
    SetCheckBoxToShow(2);
}

void initChangeNameMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(CHANGENAME);

    renderFunction = &drawChangeNameMenu;

    setQuitButton(true);

    isShowingKeyBoard = true;
    returnToMenuOnCancel = SETTINGS;
    returnToMenuOnSucces = SETTINGS;
    strncpy(tempText, localPlayer->name, 21);
}

void initQuitMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(QUIT);

    renderFunction = &drawQuitMenu;

    setQuitButton(false);

    // No button
    AllButtons[0].xPos = ScreenWidth - 80 - 40;
    AllButtons[0].yPos = 98;
    AllButtons[0].xSize = 80;
    AllButtons[0].ySize = 20;
    AllButtons[0].OnClick = &initGameMenu;
    AllButtons[0].xTextPos = 21;
    AllButtons[0].yTextPos = 13;
    AllButtons[0].text = "No";

    // Yes button
    AllButtons[1].xPos = 40;
    AllButtons[1].yPos = 98;
    AllButtons[1].xSize = 80;
    AllButtons[1].ySize = 20;
    AllButtons[1].OnClick = &initMainMenu;
    AllButtons[1].xTextPos = 9;
    AllButtons[1].yTextPos = 13;
    AllButtons[1].text = "Yes";

    SetButtonToShow(2);
}

void initShopMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(SHOP);

    renderFunction = &drawShopMenu;

    onCloseMenu = &unloadShopMenu;
    haveToCallOnCloseMenu = true;

    lastOpenedMenu = &initShopCategoriesMenu;

    BottomScreenSpritesMaterialsForUI[6] = NE_MaterialCreate();
    PalettesForUI[10] = NE_PaletteCreate();
    if (ShopCategory < EQUIPMENT)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], AllGuns[GetSelectedGunShop()].texture, 1);
    else if (ShopCategory == GRENADES)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], GetAllGrenades()[GetSelectedGunShop() - GunCount].texture, 1);
    else if (ShopCategory == EQUIPMENT)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], GetAllGrenades()[GetSelectedGunShop() - GunCount - shopGrenadeCount].texture, 1);
    setQuitButton(true);

    // Buy button
    AllButtons[0].xPos = 88;
    AllButtons[0].yPos = 170;
    AllButtons[0].xSize = 80;
    AllButtons[0].ySize = 20;
    AllButtons[0].OnClick = &BuyWeapon;
    AllButtons[0].parameter = 0;
    AllButtons[0].xTextPos = 14;
    AllButtons[0].yTextPos = 22;
    AllButtons[0].text = "Buy!";

    // Left button
    AllButtons[1].xPos = 48;
    AllButtons[1].yPos = 170;
    AllButtons[1].xSize = 30;
    AllButtons[1].ySize = 20;
    AllButtons[1].OnClick = &ChangeWeapon;
    AllButtons[1].parameter = 1;
    AllButtons[1].xTextPos = 7;
    AllButtons[1].yTextPos = 22;
    AllButtons[1].text = "<-";

    // Right button
    AllButtons[2].xPos = 88 + 80 + 10;
    AllButtons[2].yPos = 170;
    AllButtons[2].xSize = 30;
    AllButtons[2].ySize = 20;
    AllButtons[2].OnClick = &ChangeWeapon;
    AllButtons[2].parameter = 0;
    AllButtons[2].xTextPos = 23;
    AllButtons[2].yTextPos = 22;
    AllButtons[2].text = "->";

    SetButtonToShow(3);
}

void initControllerMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(CONTROLLER);

    renderFunction = &drawControllerMenu;

    onCloseMenu = &unloadControllerMenu;
    haveToCallOnCloseMenu = true;
    lastOpenedMenu = &initGameMenu;

    setQuitButton(true);

    BottomScreenSpritesMaterialsForUI[7] = NE_MaterialCreate();
    PalettesForUI[14] = NE_PaletteCreate();
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[7], PalettesForUI[14], (void *)JumpArrow_bin, 1);

    BottomScreenSpritesMaterialsForUI[8] = NE_MaterialCreate();
    PalettesForUI[15] = NE_PaletteCreate();
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[8], PalettesForUI[15], (void *)reload_bin, 1);

    int buttonXPosition = 2;
    int mapButtonXPosition = 2;
    int mapTextXPosition = 1;
    int textXPosition = 2;
    if (isLeftControls)
    {
        buttonXPosition = 218; // 256 - 36 - 2
        mapButtonXPosition = 193;
        mapTextXPosition = 25;
        textXPosition = 29;
    }

    // Jump button
    AllButtons[0].xPos = buttonXPosition;
    AllButtons[0].yPos = 30;
    AllButtons[0].xSize = 36;
    AllButtons[0].ySize = 36;
    AllButtons[0].OnClick = &SetNeedJump;
    AllButtons[0].parameter = 0;
    AllButtons[0].text = "";

    // Reload button
    AllButtons[1].xPos = buttonXPosition;
    AllButtons[1].yPos = 72;
    AllButtons[1].xSize = 36;
    AllButtons[1].ySize = 36;
    AllButtons[1].OnClick = &startReloadGun;
    AllButtons[1].parameter = 0;
    AllButtons[1].text = "";

    // Right button
    AllButtons[2].xPos = buttonXPosition;
    AllButtons[2].yPos = 114;
    AllButtons[2].xSize = 36;
    AllButtons[2].ySize = 36;
    AllButtons[2].OnClick = &ChangeGunInInventoryForLocalPlayer;
    AllButtons[2].parameter = 0;
    AllButtons[2].xTextPos = textXPosition;
    AllButtons[2].yTextPos = 16;
    AllButtons[2].text = ">";

    // Left button
    AllButtons[3].xPos = buttonXPosition;
    AllButtons[3].yPos = 156;
    AllButtons[3].xSize = 36;
    AllButtons[3].ySize = 36;
    AllButtons[3].OnClick = &ChangeGunInInventoryForLocalPlayer;
    AllButtons[3].parameter = 1;
    AllButtons[3].xTextPos = textXPosition;
    AllButtons[3].yTextPos = 21;
    AllButtons[3].text = "<";

    // Map button
    AllButtons[4].xPos = mapButtonXPosition;
    AllButtons[4].yPos = 2;
    AllButtons[4].xSize = 36;
    AllButtons[4].ySize = 20;
    AllButtons[4].OnClick = &changeMapState;
    AllButtons[4].parameter = 0;
    AllButtons[4].xTextPos = mapTextXPosition;
    AllButtons[4].yTextPos = 1;
    AllButtons[4].text = "Map";

    SetButtonToShow(5);
}

void initMainMenu()
{
    SetTwoScreenMode(true);

    startChangeMenu(MAIN);

    renderFunction = &drawMainMenu;

    lastOpenedMenu = &initMainMenu;

    Connection = UNSELECTED;
    // return;
    setQuitButton(false);

    removeAllPlayers();

    if (isInTutorial)
    {
        // tutorialDone = true;
        // isInTutorial = false;
        endTutorial();
        uiTimer = 8;
        actionOfUiTimer = SAVE;
    }

    // Single player button
    AllButtons[0].xPos = 40;
    AllButtons[0].yPos = 40;
    AllButtons[0].xSize = ScreenWidth - 80;
    AllButtons[0].ySize = 24;
    // AllButtons[0].OnClick = &StartSinglePlayer;
    AllButtons[0].OnClick = &initSelectionMapImageMenu;
    AllButtons[0].parameter = 0;
    AllButtons[0].xTextPos = 10;
    AllButtons[0].yTextPos = 6;
    AllButtons[0].text = "Single Player";

    // Multiplayer button
    AllButtons[1].xPos = 40;
    AllButtons[1].yPos = 87;
    AllButtons[1].xSize = ScreenWidth - 80;
    AllButtons[1].ySize = 24;
    AllButtons[1].OnClick = &initServerMenu;
    AllButtons[1].isHidden = true;
    AllButtons[1].xTextPos = 11;
    AllButtons[1].yTextPos = 12;
    AllButtons[1].text = "Multiplayer";

    // Settings button
    AllButtons[2].xPos = 40;
    AllButtons[2].yPos = 135;
    AllButtons[2].xSize = ScreenWidth - 80;
    AllButtons[2].ySize = 24;
    AllButtons[2].OnClick = &initSettingsMenu;
    AllButtons[2].isHidden = false;
    AllButtons[2].xTextPos = 12;
    AllButtons[2].yTextPos = 18;
    AllButtons[2].text = "Settings";

    SetButtonToShow(3);
}

void initServerMenu()
{
    SetTwoScreenMode(false);

    startChangeMenu(SERVERS);

    renderFunction = &drawServersMenu;

    setQuitButton(true);

    // Start game button
    AllButtons[0].xPos = 40;
    AllButtons[0].yPos = 160;
    AllButtons[0].xSize = ScreenWidth - 80;
    AllButtons[0].ySize = 24;
    AllButtons[0].OnClick = &connectFromServerList; // TODO Change this
    AllButtons[0].parameter = -1;

    // Up button
    AllButtons[1].xPos = 62;
    AllButtons[1].yPos = 127;
    AllButtons[1].xSize = 40;
    AllButtons[1].ySize = 24;
    AllButtons[1].OnClick = &changeServer; // TODO Change this
    AllButtons[1].parameter = 0;

    // Down button
    AllButtons[2].xPos = 159;
    AllButtons[2].yPos = 127;
    AllButtons[2].xSize = 40;
    AllButtons[2].ySize = 24;
    AllButtons[2].OnClick = &changeServer; // TODO Change this
    AllButtons[2].parameter = 1;

    // NE_SpriteVisible(BottomScreenSpritesForUI[2], false);
    SetButtonToShow(3);
}

void initChatMenu()
{
    // To Fill
}

void initControlsSettingsMenu()
{

    SetTwoScreenMode(true);

    startChangeMenu(CONTROLSSETTINGS);

    renderFunction = &drawControlsSettingsMenu;

    lastOpenedMenu = &initSettingsMenu;

    setQuitButton(true);

    // Set rumble checkbox
    AllCheckBoxs[0].xPos = 58;
    AllCheckBoxs[0].yPos = 56;
    AllCheckBoxs[0].xSize = 20;
    AllCheckBoxs[0].ySize = 20;
    AllCheckBoxs[0].value = &isLeftControls;

    // Set change controls button
    AllButtons[0].xPos = 140;
    AllButtons[0].yPos = 40;
    AllButtons[0].xSize = 100;
    AllButtons[0].ySize = 32;
    AllButtons[0].OnClick = &initControlsChangeMenu;
    AllButtons[0].xTextPos = 17;
    AllButtons[0].yTextPos = 6;
    AllButtons[0].text = "";

    AllSliders[0].xPos = 140;
    AllSliders[0].yPos = 128;
    AllSliders[0].xSize = 100;
    // AllSliders[0].ySize = 32;
    AllSliders[0].min = MIN_SENSITIVITY;
    AllSliders[0].max = MAX_SENSITIVITY;
    AllSliders[0].step = 0.1;

    // int val = 2;
    AllSliders[0].value = &sensitivity;

    SetSliderToShow(1);
    SetButtonToShow(1);
    SetCheckBoxToShow(1);
}

void initControlsChangeMenu()
{
    SetTwoScreenMode(true);

    startChangeMenu(CONTROLSCHANGE);

    renderFunction = &drawControlsChangeMenu;

    lastOpenedMenu = &initControlsSettingsMenu;

    setQuitButton(true);

    // Set change controls button
    AllButtons[0].xPos = 15;
    AllButtons[0].yPos = 167;
    AllButtons[0].xSize = 100;
    AllButtons[0].ySize = 24;
    AllButtons[0].OnClick = &changeControlsPage;
    AllButtons[0].parameter = 1;
    AllButtons[0].xTextPos = 7;
    AllButtons[0].yTextPos = 22;
    AllButtons[0].text = "<-";

    AllButtons[1].xPos = 140;
    AllButtons[1].yPos = 167;
    AllButtons[1].xSize = 100;
    AllButtons[1].ySize = 24;
    AllButtons[1].OnClick = &changeControlsPage;
    AllButtons[1].parameter = 0;
    AllButtons[1].xTextPos = 23;
    AllButtons[1].yTextPos = 22;
    AllButtons[1].text = "->";

    // Set change controls button
    AllButtons[2].xPos = 15;
    AllButtons[2].yPos = 40;
    AllButtons[2].xSize = 100;
    AllButtons[2].ySize = 24;
    AllButtons[2].OnClick = &startScanForInput;
    if (controlsPage == 0)
    {
        AllButtons[2].parameter = FIRE_BUTTON;
        AllButtons[2].xTextPos = 6;
        AllButtons[2].yTextPos = 4;
        AllButtons[2].text = "Fire";

        AllButtons[3].parameter = JUMP_BUTTON;
        AllButtons[3].xTextPos = 22;
        AllButtons[3].yTextPos = 4;
        AllButtons[3].text = "Jump";

        AllButtons[4].parameter = LEFT_BUTTON;
        AllButtons[4].xTextPos = 4;
        AllButtons[4].yTextPos = 9;
        AllButtons[4].text = "Move left";

        AllButtons[5].parameter = RIGHT_BUTTON;
        AllButtons[5].xTextPos = 19;
        AllButtons[5].yTextPos = 9;
        AllButtons[5].text = "Move right";

        AllButtons[6].parameter = UP_BUTTON;
        AllButtons[6].xTextPos = 3;
        AllButtons[6].yTextPos = 14;
        AllButtons[6].text = "Move forward";

        AllButtons[7].parameter = DOWN_BUTTON;
        AllButtons[7].xTextPos = 18;
        AllButtons[7].yTextPos = 14;
        AllButtons[7].text = "Move backward";

        SetButtonToShow(8);
    }
    else if (controlsPage == 1)
    {
        AllButtons[2].parameter = LOOK_LEFT_BUTTON;
        AllButtons[2].xTextPos = 4;
        AllButtons[2].yTextPos = 4;
        AllButtons[2].text = "Look left";

        AllButtons[3].parameter = LOOK_RIGHT_BUTTON;
        AllButtons[3].xTextPos = 19;
        AllButtons[3].yTextPos = 4;
        AllButtons[3].text = "Look right";

        AllButtons[4].parameter = LOOK_UP_BUTTON;
        AllButtons[4].xTextPos = 5;
        AllButtons[4].yTextPos = 9;
        AllButtons[4].text = "Look up";

        AllButtons[5].parameter = LOOK_DOWN_BUTTON;
        AllButtons[5].xTextPos = 19;
        AllButtons[5].yTextPos = 9;
        AllButtons[5].text = "Look down";

        AllButtons[6].parameter = DEFUSE_BUTTON;
        AllButtons[6].xTextPos = 5;
        AllButtons[6].yTextPos = 14;
        AllButtons[6].text = "Defuse";

        AllButtons[7].parameter = SCOPE_BUTTON;
        AllButtons[7].xTextPos = 21;
        AllButtons[7].yTextPos = 14;
        AllButtons[7].text = "Scope";

        SetButtonToShow(8);
    }

    AllButtons[3].xPos = 140;
    AllButtons[3].yPos = 40;
    AllButtons[3].xSize = 100;
    AllButtons[3].ySize = 24;
    AllButtons[3].OnClick = &startScanForInput;

    // Set change controls button
    AllButtons[4].xPos = 15;
    AllButtons[4].yPos = 80;
    AllButtons[4].xSize = 100;
    AllButtons[4].ySize = 24;
    AllButtons[4].OnClick = &startScanForInput;

    AllButtons[5].xPos = 140;
    AllButtons[5].yPos = 80;
    AllButtons[5].xSize = 100;
    AllButtons[5].ySize = 24;
    AllButtons[5].OnClick = &startScanForInput;

    // Set change controls button
    AllButtons[6].xPos = 15;
    AllButtons[6].yPos = 120;
    AllButtons[6].xSize = 100;
    AllButtons[6].ySize = 24;
    AllButtons[6].OnClick = &startScanForInput;

    AllButtons[7].xPos = 140;
    AllButtons[7].yPos = 120;
    AllButtons[7].xSize = 100;
    AllButtons[7].ySize = 24;
    AllButtons[7].OnClick = &startScanForInput;
}

void initSelectionMapImageMenu()
{
    SetTwoScreenMode(true);

    startChangeMenu(MAP_SELECTION_IMAGE);

    renderFunction = &drawSelectionMapImageMenu;

    lastOpenedMenu = &initMainMenu;

    onCloseMenu = &unloadSelectionMapImageMenu;
    haveToCallOnCloseMenu = true;

    setQuitButton(true);

    BottomScreenSpritesMaterialsForUI[6] = NE_MaterialCreate();
    PalettesForUI[10] = NE_PaletteCreate();
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], allMaps[currentSelectionMap].image, 1);

    // Set change controls button
    AllButtons[0].xPos = 35;
    AllButtons[0].yPos = 164;
    AllButtons[0].xSize = 40;
    AllButtons[0].ySize = 32;
    AllButtons[0].OnClick = &ChangeMap;
    AllButtons[0].parameter = 0;
    AllButtons[0].xTextPos = 6;
    AllButtons[0].yTextPos = 22;
    AllButtons[0].text = "<-";

    AllButtons[1].xPos = 256 - 35 - 40;
    AllButtons[1].yPos = 164;
    AllButtons[1].xSize = 40;
    AllButtons[1].ySize = 32;
    AllButtons[1].OnClick = &ChangeMap;
    AllButtons[0].parameter = 1;
    AllButtons[1].xTextPos = 24;
    AllButtons[1].yTextPos = 22;
    AllButtons[1].text = "->";

    AllButtons[2].xPos = 256 / 2 - 40;
    AllButtons[2].yPos = 164;
    AllButtons[2].xSize = 80;
    AllButtons[2].ySize = 32;
    AllButtons[2].OnClick = &StartSinglePlayer;
    AllButtons[2].xTextPos = 14;
    AllButtons[2].yTextPos = 22;
    AllButtons[2].text = "Start";

    SetButtonToShow(3);
}

void initSelectionMapListMenu()
{
    SetTwoScreenMode(true);

    startChangeMenu(MAP_SELECTION_LIST);

    renderFunction = &drawSelectionMapListMenu;

    lastOpenedMenu = &initSelectionMapImageMenu;

    setQuitButton(true);

    // Set change controls button
    AllButtons[0].xPos = 40;
    AllButtons[0].yPos = 40;
    AllButtons[0].xSize = 100;
    AllButtons[0].ySize = 32;
    AllButtons[0].OnClick = &initControlsChangeMenu;
    AllButtons[0].xTextPos = 5;
    AllButtons[0].yTextPos = 6;
    AllButtons[0].text = "<-";

    AllButtons[1].xPos = 140;
    AllButtons[1].yPos = 40;
    AllButtons[1].xSize = 100;
    AllButtons[1].ySize = 32;
    AllButtons[1].OnClick = &initControlsChangeMenu;
    AllButtons[1].xTextPos = 17;
    AllButtons[1].yTextPos = 6;
    AllButtons[1].text = "->";

    SetButtonToShow(2);
}

void drawGameMenu()
{
    // Print texts
    NE_TextPrint(0,        // Font slot
                 12, 2,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Main menu");
}

void drawGameFinishedMenu()
{
    NE_TextPrint(0,        // Font slot
                 9, 1,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Party finished");

    // Draw white bars
    NE_2DDrawQuad(0, 25, 256, 28, 19, RGB15(31, 31, 31));
    NE_2DDrawQuad(126, 28, 130, 180, 19, RGB15(31, 31, 31));

    // Counter terrorists text
    char CounterScoreText[30] = "Counter t";
    if (!allPartyModes[currentPartyMode].noScore)
    {
        if (CounterScore > 9)
            sprintf(CounterScoreText + strlen(CounterScoreText), " : %d ", CounterScore);
        else
            sprintf(CounterScoreText + strlen(CounterScoreText), " : 0%d ", CounterScore);
    }

    NE_TextPrint(0,        // Font slot
                 1, 4,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 CounterScoreText);

    // Show each counter terrorists player
    int Count1 = 0;
    for (int i = 0; i < MaxPlayer; i++)
    {
        if (AllPlayers[i].Team == COUNTERTERRORISTS)
        {
            int y = 6 + Count1 * 3;

            char playerName[12];
            // sprintf(playerName, "id : %d", AllPlayers[i].Id);
            sprintf(playerName, AllPlayers[i].name);
            NE_TextPrint(0,        // Font slot
                         1, y,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         playerName);
            char playerValues[12];
            sprintf(playerValues, "K : %d D : %d", AllPlayers[i].KillCount, AllPlayers[i].DeathCount);
            NE_TextPrint(0,                 // Font slot
                         1, y + 1,          // Coordinates x(column), y(row)
                         RGB15(17, 17, 17), // Color
                         playerValues);

            Count1++;
        }
    }

    // Counter terrorists text
    char TerroristsScoreText[30] = "Terrorists";
    if (!allPartyModes[currentPartyMode].noScore)
    {
        if (TerroristsScore > 9)
            sprintf(TerroristsScoreText + strlen(TerroristsScoreText), " : %d ", TerroristsScore);
        else
            sprintf(TerroristsScoreText + strlen(TerroristsScoreText), " : 0%d ", TerroristsScore);
    }
    NE_TextPrint(0,        // Font slot
                 17, 4,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 TerroristsScoreText);

    // Show each terrorists player
    int Count2 = 0;
    for (int i = 0; i < MaxPlayer; i++)
    {
        if (AllPlayers[i].Team == TERRORISTS)
        {
            int y = 6 + Count2 * 3;

            char playerName[12];
            // sprintf(playerName, "id : %d", AllPlayers[i].Id);
            sprintf(playerName, AllPlayers[i].name);
            NE_TextPrint(0,        // Font slot
                         17, y,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         playerName);
            char playerValues[12];
            sprintf(playerValues, "K : %d D : %d", AllPlayers[i].KillCount, AllPlayers[i].DeathCount);
            NE_TextPrint(0,                 // Font slot
                         17, y + 1,         // Coordinates x(column), y(row)
                         RGB15(17, 17, 17), // Color
                         playerValues);

            Count2++;
        }
    }
}

void drawScoreMenu()
{
    if (!WaitForTeamResponse && localPlayer->Team == SPECTATOR)
        ButtonToShow = 2;
    else if (!WaitForTeamResponse) /////////////////////////////////////////////////////////////////////////////////////////////CHECK THIS
        ButtonToShow = 0;

    // Draw white bars
    NE_2DDrawQuad(0, 25, 256, 28, 19, RGB15(31, 31, 31));
    NE_2DDrawQuad(126, 28, 130, 180, 19, RGB15(31, 31, 31));

    NE_TextPrint(0,        // Font slot
                 11, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Score table");

    // Counter terrorists text
    char CounterScoreText[30] = "Counter t";
    if (!allPartyModes[currentPartyMode].noScore)
    {
        if (CounterScore > 9)
            sprintf(CounterScoreText + strlen(CounterScoreText), " : %d ", CounterScore);
        else
            sprintf(CounterScoreText + strlen(CounterScoreText), " : 0%d ", CounterScore);
    }
    NE_TextPrint(0,        // Font slot
                 1, 4,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 CounterScoreText);

    // Show each counter terrorists player
    int Count1 = 0;
    for (int i = 0; i < MaxPlayer; i++)
    {
        if (AllPlayers[i].Team == COUNTERTERRORISTS)
        {
            int y = 6 + Count1 * 3;

            char playerName[12];
            // sprintf(playerName, "id : %d", AllPlayers[i].Id);
            sprintf(playerName, AllPlayers[i].name);
            NE_TextPrint(0,        // Font slot
                         1, y,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         playerName);
            char playerValues[12];
            sprintf(playerValues, "K : %d D : %d", AllPlayers[i].KillCount, AllPlayers[i].DeathCount);
            NE_TextPrint(0,                 // Font slot
                         1, y + 1,          // Coordinates x(column), y(row)
                         RGB15(17, 17, 17), // Color
                         playerValues);

            Count1++;
        }
    }

    // Counter terrorists text
    char TerroristsScoreText[30] = "Terrorists";
    if (!allPartyModes[currentPartyMode].noScore)
    {
        if (TerroristsScore > 9)
            sprintf(TerroristsScoreText + strlen(TerroristsScoreText), " : %d ", TerroristsScore);
        else
            sprintf(TerroristsScoreText + strlen(TerroristsScoreText), " : 0%d ", TerroristsScore);
    }

    NE_TextPrint(0,        // Font slot
                 17, 4,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 TerroristsScoreText);

    // Show each terrorists player
    int Count2 = 0;
    for (int i = 0; i < MaxPlayer; i++)
    {
        if (AllPlayers[i].Team == TERRORISTS)
        {
            int y = 6 + Count2 * 3;

            char playerName[12];
            // sprintf(playerName, "id : %d", AllPlayers[i].Id);
            sprintf(playerName, AllPlayers[i].name);
            NE_TextPrint(0,        // Font slot
                         17, y,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         playerName);
            char playerValues[12];
            sprintf(playerValues, "K : %d D : %d", AllPlayers[i].KillCount, AllPlayers[i].DeathCount);
            NE_TextPrint(0,                 // Font slot
                         17, y + 1,         // Coordinates x(column), y(row)
                         RGB15(17, 17, 17), // Color
                         playerValues);

            Count2++;
        }
    }
}

void drawShopCategoriesMenu()
{
    // Draw white bars
    NE_2DDrawQuad(ScreenCenterWidth - 1, 23, ScreenCenterWidth + 1, ScreenHeightFixed, 0, RGB15(31, 31, 31));
    NE_2DDrawQuad(0, (198 - 23) / 3 - 1 + 23, ScreenWidth, (198 - 23) / 3 + 2 + 23, 0, RGB15(31, 31, 31));
    NE_2DDrawQuad(0, (198 - 23) / 3 * 2 - 1 + 23, ScreenWidth, (198 - 23) / 3 * 2 + 2 + 23, 0, RGB15(31, 31, 31));

    // Show texts
    NE_TextPrint(0,        // Font slot
                 14, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Shop");

    char MoneyText[30];
    sprintf(MoneyText, "%d$", localPlayer->Money);
    NE_TextPrint(0,        // Font slot
                 1, 1,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 MoneyText);

    NE_TextPrint(0,        // Font slot
                 4, 6,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 "PISTOLS");

    NE_TextPrint(0,        // Font slot
                 5, 13,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "HEAVY");

    NE_TextPrint(0,        // Font slot
                 6, 21,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "SMG");

    NE_TextPrint(0,        // Font slot
                 21, 6,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "RIFLES");

    NE_TextPrint(0,        // Font slot
                 20, 13,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "EQUIPMENT");

    NE_TextPrint(0,        // Font slot
                 20, 21,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "GRENADES");
}

void drawSettingsMenu()
{
    NE_TextPrint(0,        // Font slot
                 12, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Settings");

    NE_TextPrint(0,        // Font slot
                 18, 16,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Keyboard mode");

    printLongConstChar(1, 16, 3, "Use Rumble Pack (cause crash on 3DS)");
    printLongConstChar(18, 32, 4, "3DS mode (for saves)");
}

void drawQuitMenu()
{
    if (isInTutorial)
    {
        NE_TextPrint(0,        // Font slot
                     7, 3,     // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Skip the tutorial ?");
    }
    else
    {
        NE_TextPrint(0,        // Font slot
                     1, 3,     // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Do you want to quit the game ?");
    }
}

void drawShopMenu()
{
    NE_2DDrawQuad(160, 26, 255, 144, 1, RGB15(4, 4, 4));                                   // Draw gun details background
    NE_2DDrawTexturedQuad(26, 26, 118 + 26, 118 + 26, 1, BottomScreenSpritesMaterials[6]); // Draw gun

    // Title
    NE_TextPrint(0,        // Font slot
                 14, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Shop");

    char MoneyText[30];
    sprintf(MoneyText, "%d$", localPlayer->Money);
    NE_TextPrint(0,        // Font slot
                 1, 1,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 MoneyText);

    if (GetShopCategory() < 4) // If current shop category is guns
    {
        // Gun name
        printLongConstChar(21, 31, 4, AllGuns[SelectedGunShop].name);

        // Gun damage
        NE_TextPrint(0,        // Font slot
                     21, 7,    // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Damage");
        char DamageText[4];

        sprintf(DamageText, "%d", AllGuns[SelectedGunShop].Damage);
        NE_TextPrint(0,        // Font slot
                     21, 8,    // Coordinates x(column), y(row)
                     NE_White, // Color
                     DamageText);

        // Gun fire rate
        NE_TextPrint(0,        // Font slot
                     21, 11,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Fire rate");
        char FireRateText[5];
        sprintf(FireRateText, "%d", AllGuns[SelectedGunShop].fireRate);
        NE_TextPrint(0,        // Font slot
                     21, 12,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     FireRateText);

        // Gun price
        NE_TextPrint(0,        // Font slot
                     21, 15,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Price");
        char PriceText[7];
        sprintf(PriceText, "%d$", AllGuns[SelectedGunShop].Price);
        NE_TextPrint(0,        // Font slot
                     21, 16,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     PriceText);
    }
    else if (GetShopCategory() == 4) // If current shop category is equipements
    {
        // Gun name
        printLongText(21, 31, 4, AllEquipements[SelectedGunShop - GunCount - shopGrenadeCount].name);

        // Gun price
        NE_TextPrint(0,        // Font slot
                     21, 15,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Price");

        char PriceText[7];
        sprintf(PriceText, "%d$", AllEquipements[SelectedGunShop - GunCount - shopGrenadeCount].Price);
        NE_TextPrint(0,        // Font slot
                     21, 16,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     PriceText);

        // Print description
        printLongText(21, 31, 7, AllEquipements[SelectedGunShop - GunCount - shopGrenadeCount].description);
    }
    else if (GetShopCategory() == 5) // If current shop category is grenades
    {
        // Gun name
        printLongText(21, 31, 4, AllGrenades[SelectedGunShop - GunCount].name);

        // Gun price
        NE_TextPrint(0,        // Font slot
                     21, 15,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     "Price");

        char PriceText[7];
        sprintf(PriceText, "%d$", AllGrenades[SelectedGunShop - GunCount].Price);
        NE_TextPrint(0,        // Font slot
                     21, 16,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     PriceText);

        // Print description
        printLongText(21, 31, 7, AllGrenades[SelectedGunShop - GunCount].description);
    }
}

void drawControllerMenu()
{
    if (!isLeftControls)
    {
        NE_2DDrawQuad(40, 30, 255, 194, 1, RGB15(4, 4, 4));                                        // touch pad area
        NE_2DDrawTexturedQuad(4, 32, 4 + 36 - 4, 30 + 36 - 2, 1, BottomScreenSpritesMaterials[7]); // Jump button
        NE_2DDrawTexturedQuad(4, 74, 4 + 36 - 4, 72 + 36 - 2, 1, BottomScreenSpritesMaterials[8]); // Reload button
    }
    else
    {
        NE_2DDrawQuad(0, 30, 215, 194, 1, RGB15(4, 4, 4));                                             // touch pad area
        NE_2DDrawTexturedQuad(220, 32, 220 + 36 - 4, 30 + 36 - 2, 1, BottomScreenSpritesMaterials[7]); // Jump button
        NE_2DDrawTexturedQuad(220, 74, 220 + 36 - 4, 72 + 36 - 2, 1, BottomScreenSpritesMaterials[8]); // Reload button
    }

    // Print texts
    NE_TextPrint(0,        // Font slot
                 13, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Gamepad");
}

void drawMainMenu()
{

    // Print texts
    NE_TextPrint(0,        // Font slot
                 10, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Counter Strike");

    NE_TextPrint(0,        // Font slot
                 1, 22,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Alpha 0.6.0");

    NE_TextPrint(0,        // Font slot
                 24, 22,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Fewnity");
}

void drawServersMenu()
{
    /*char textIp[1024] = "";
            sprintf(textIp + strlen(textIp), "ipCount : %d", TotalIpCount);

            NE_TextPrint(0,        // Font slot
                         1, 10,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         textIp);*/

    int maxRowServerList = 5;
    // TotalIpCount = 6;
    for (int i = serverListOffset; i < serverListOffset + maxRowServerList; i++)
    {
        if (i == TotalIpCount)
            break;

        int rowXPosition = 10, rowYPosition = (i - serverListOffset) * 16 + 44, rowXSize = ScreenWidth - 20, rowYSize = 16;
        if (i % 2 == 0)
        {
            NE_2DDrawQuad(rowXPosition, rowYPosition, rowXPosition + rowXSize, rowYPosition + rowYSize, 1, RGB15(5, 5, 5)); // Button background
        }
        else
        {
            NE_2DDrawQuad(rowXPosition, rowYPosition, rowXPosition + rowXSize, rowYPosition + rowYSize, 1, RGB15(4, 4, 4)); // Button background
        }

        if (selectedServer == i)
        {
            // Print texts
            NE_TextPrint(0,                                    // Font slot
                         10, ((i - serverListOffset) * 2) + 6, // Coordinates x(column), y(row)
                         NE_White,                             // Color
                         allIps[i]);
        }
        else
        {
            NE_TextPrint(0,                                    // Font slot
                         10, ((i - serverListOffset) * 2) + 6, // Coordinates x(column), y(row)
                         RGB15(10, 10, 10),                    // Color
                         allIps[i]);
        }
    }

    // Print texts
    NE_TextPrint(0,        // Font slot
                 11, 2,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Multiplayer");

    NE_TextPrint(0,        // Font slot
                 11, 3,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Server list");

    NE_TextPrint(0,        // Font slot
                 10, 17,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "^");

    NE_TextPrint(0,        // Font slot
                 22, 17,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "v");

    NE_TextPrint(0,        // Font slot
                 13, 21,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Connect");
}

void drawChangeNameMenu()
{
}

void drawChatMenu()
{
}

void drawControlsSettingsMenu()
{
    // Print texts
    NE_TextPrint(0,        // Font slot
                 9, 1,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Controls settings");

    printLongConstChar(3, 18, 4, "Left-handed touch mode");
    printLongConstChar(20, 28, 6, "Change controls");

    char MoneyText[26];
    sprintf(MoneyText, "Gamepad sensitivity : %0.1f", *AllSliders[0].value);

    printLongConstChar(17, 32, 11, MoneyText);
}

void drawControlsChangeMenu()
{
    // Print texts
    NE_TextPrint(0,        // Font slot
                 10, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Change controls");

    // Print texts
    NE_TextPrint(0,        // Font slot
                 3, 6,     // Coordinates x(column), y(row)
                 NE_White, // Color
                 inputsNames[inputs[0 + (controlsPage * 6)].nameIndex]);

    // Print texts
    NE_TextPrint(0,        // Font slot
                 19, 6,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 inputsNames[inputs[1 + (controlsPage * 6)].nameIndex]);

    // Print texts
    NE_TextPrint(0,        // Font slot
                 3, 11,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 inputsNames[inputs[2 + (controlsPage * 6)].nameIndex]);

    // Print texts
    NE_TextPrint(0,        // Font slot
                 19, 11,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 inputsNames[inputs[3 + (controlsPage * 6)].nameIndex]);

    // Print texts
    NE_TextPrint(0,        // Font slot
                 3, 16,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 inputsNames[inputs[4 + (controlsPage * 6)].nameIndex]);

    // Print texts
    NE_TextPrint(0,        // Font slot
                 19, 16,   // Coordinates x(column), y(row)
                 NE_White, // Color
                 inputsNames[inputs[5 + (controlsPage * 6)].nameIndex]);
}

void drawSelectionMapImageMenu()
{
    // Print texts
    NE_TextPrint(0,        // Font slot
                 11, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Select map");

    NE_2DDrawTexturedQuad(0 + 32, 20, 192 + 32, 139 + 20, 3, BottomScreenSpritesMaterials[6]); // Draw map image

    // Text background
    NE_PolyFormat(15, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
    NE_2DDrawQuad(0 + 32, 20, 192 + 32, 16 + 20, 2, RGB15(0, 0, 0));
    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

    // Print map name
    printLongConstChar(4, 31 - 4, 3, allMaps[currentSelectionMap].name);
}

void drawSelectionMapListMenu()
{
    // Print texts
    NE_TextPrint(0,        // Font slot
                 11, 1,    // Coordinates x(column), y(row)
                 NE_White, // Color
                 "Select map");
}

void drawButtons()
{
    for (int i = 0; i < ButtonToShow; i++)
    {
        if (!AllButtons[i].isHidden)
        {
            int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
            NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background

            NE_TextPrint(0,                                              // Font slot
                         AllButtons[i].xTextPos, AllButtons[i].yTextPos, // Coordinates x(column), y(row)
                         NE_White,                                       // Color
                         AllButtons[i].text);
        }
    }
}

void drawCheckboxs()
{
    for (int i = 0; i < checkBoxToShow; i++)
    {
        int CheckBoxXPosition = AllCheckBoxs[i].xPos, CheckBoxYPosition = AllCheckBoxs[i].yPos, CheckBoxXSize = AllCheckBoxs[i].xSize, CheckBoxYSize = AllCheckBoxs[i].ySize;
        NE_2DDrawTexturedQuadColor(CheckBoxXPosition - 3, CheckBoxYPosition - 3, CheckBoxXPosition + CheckBoxXSize + 3, CheckBoxYPosition + CheckBoxYSize + 3, 2, BottomScreenSpritesMaterials[5], RGB15(4, 4, 4)); // Border
        NE_2DDrawTexturedQuadColor(CheckBoxXPosition - 1, CheckBoxYPosition - 1, CheckBoxXPosition + CheckBoxXSize + 1, CheckBoxYPosition + CheckBoxYSize + 1, 1, BottomScreenSpritesMaterials[5], RGB15(5, 5, 5)); // Background
        if (*AllCheckBoxs[i].value)
            NE_2DDrawTexturedQuadColor(CheckBoxXPosition, CheckBoxYPosition, CheckBoxXPosition + CheckBoxXSize, CheckBoxYPosition + CheckBoxYSize, 0, BottomScreenSpritesMaterials[4], RGB15(31, 31, 31)); // CheckMark
    }
}

void drawSliders()
{
    for (int i = 0; i < sliderToShow; i++)
    {
        int SliderXPosition = AllSliders[i].xPos, SliderYPosition = AllSliders[i].yPos, SliderXSize = AllSliders[i].xSize;
        NE_2DDrawQuad(SliderXPosition - 1, SliderYPosition - 2, SliderXPosition + SliderXSize + 1, SliderYPosition + 2, 2, RGB15(5, 5, 5));

        int sliderHandXPos = (int)map(*AllSliders[i].value, AllSliders[i].min, AllSliders[i].max, AllSliders[i].xPos, AllSliders[i].xPos + AllSliders[i].xSize);
        NE_2DDrawTexturedQuadColor(sliderHandXPos - 10, SliderYPosition - 10, sliderHandXPos + 10, SliderYPosition + 10, 1, BottomScreenSpritesMaterials[5], RGB15(7, 7, 7));
    }
}

void unloadShopMenu()
{
    // Delete shop gun preview image
    NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[6]);
    NE_PaletteDelete(PalettesForUI[10]);
}

void unloadControllerMenu()
{
    closeMap();

    // Delete button images
    NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[7]);
    NE_PaletteDelete(PalettesForUI[14]);
    NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[8]);
    NE_PaletteDelete(PalettesForUI[15]);
}

void unloadSelectionMapImageMenu()
{
    // Delete shop gun preview image
    NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[6]);
    NE_PaletteDelete(PalettesForUI[10]);
}