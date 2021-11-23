#include "main.h"
#include "movements.h"
#include "collisions.h"
#include "keyboard.h"
#include "network.h"
#include "ui.h"
#include "party.h"
#include "ai.h"
#include "sounds.h"
#include "gun.h"
#include "grenade.h"

Button *AllButtonsRefForUI;
CheckBox *AllCheckBoxsRefForUI;
NE_Sprite **TopScreenSpritesForUI;
NE_Sprite **BottomScreenSpritesForUI;
NE_Material **BottomScreenSpritesMaterialsForUI;
NE_Palette **PalettesForUI;
Player *AllPlayersRefForUI;
Gun *AllGunsRefForUI;

int ShopCategory = -1; // 0 pistols, 1 Heavy, 2 sgm, 3 rifles, 4 equipment, 5 grenades
int WeaponCount;
int selectedServer = 0;
int serverListOffset = 0;

Vector2 LastTouch;

bool isShowingMap = false;
bool isShowingKeyBoard = true;

Vector2 GetLastTouch()
{
    return LastTouch;
}

void SetPlayerForUI()
{
    AllPlayersRefForUI = GetPlayers();
}

void SetButtonsRefForUI()
{
    AllButtonsRefForUI = GetButtons();
}

void SetSpritesForUI()
{
    TopScreenSpritesForUI = GetSpritesTop();
    BottomScreenSpritesForUI = GetSpritesBottom();
    BottomScreenSpritesMaterialsForUI = GetBottomScreenSpritesMaterials();
    PalettesForUI = GetPalettes();
    AllGunsRefForUI = GetGuns();
}

void SetCheckBoxsRefForUI()
{
    AllCheckBoxsRefForUI = GetCheckBoxs();
}

int GetShopCategory()
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
        if (AllGunsRefForUI[i].gunCategory == ShopCategory && (AllPlayersRefForUI[0].IsCounter == AllGunsRefForUI[i].isForCounterTerrorists || AllGunsRefForUI[i].isForCounterTerrorists == -1))
            WeaponCount++;
    }
}

#define maxCharAtLine 32

void printLongText(int minX, int maxX, int y, char *text)
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
            NE_TextPrint(0,                     // Font slot
                         minX, y + currentLine, // Coordinates x(column), y(row)
                         NE_White,              // Color
                         tempText);

            for (int i = 0; i < strlen(tempText); i++)
                tempText[i] = '\0';

            currentLine++;
        }

        sprintf(tempText + strlen(tempText), ptr);
        sprintf(tempText + strlen(tempText), " ");

        ptr = strtok(NULL, " ");
    }
    NE_TextPrint(0,                     // Font slot
                 minX, y + currentLine, // Coordinates x(column), y(row)
                 NE_White,              // Color
                 tempText);
}

void ChangeMenu(int menuId)
{
    int currentMenu = GetCurrentMenu();

    if (currentMenu == 1) // Back button (map menu)
    {
        if (GetAlwaysUpdateBottomScreen())
            SetNeedChangeScreen(true);

        NE_SpriteVisible(BottomScreenSpritesForUI[0], false);
        NE_SpriteVisible(BottomScreenSpritesForUI[1], false);
        NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[1]);
    }
    else if (menuId == 3 && currentMenu == 6) // Back button (map menu)
    {
        NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[6]);
        NE_PaletteDelete(PalettesForUI[10]);
    }
    else if (currentMenu == 7) // Back button (map menu)
    {
        NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[7]);
        NE_PaletteDelete(PalettesForUI[14]);
        NE_MaterialDelete(BottomScreenSpritesMaterialsForUI[8]);
        NE_PaletteDelete(PalettesForUI[15]);
    }

    SetCurrentMenu(menuId); /////REMOVE?
    if (menuId == 0)
    {
        // Show map button
        /*AllButtonsRefForUI[0].xPos = 15;
        AllButtonsRefForUI[0].yPos = 40;
        AllButtonsRefForUI[0].xSize = 100;
        AllButtonsRefForUI[0].ySize = 24;
        AllButtonsRefForUI[0].OnClick = &ChangeMenu;
        AllButtonsRefForUI[0].parameter = 1;*/

        // Show score button
        AllButtonsRefForUI[0].xPos = 15;
        AllButtonsRefForUI[0].yPos = 72;
        AllButtonsRefForUI[0].xSize = 100;
        AllButtonsRefForUI[0].ySize = 24;
        AllButtonsRefForUI[0].OnClick = &ChangeMenu;
        AllButtonsRefForUI[0].parameter = 2;

        // Open shop button
        AllButtonsRefForUI[1].xPos = 15;
        AllButtonsRefForUI[1].yPos = 104;
        AllButtonsRefForUI[1].xSize = 100;
        AllButtonsRefForUI[1].ySize = 24;
        AllButtonsRefForUI[1].OnClick = &ChangeMenu;
        AllButtonsRefForUI[1].parameter = 3;

        // Settings button
        AllButtonsRefForUI[2].xPos = 15;
        AllButtonsRefForUI[2].yPos = 136;
        AllButtonsRefForUI[2].xSize = 100;
        AllButtonsRefForUI[2].ySize = 24;
        AllButtonsRefForUI[2].OnClick = &ChangeMenu;
        AllButtonsRefForUI[2].parameter = 4;

        // Quit game button
        AllButtonsRefForUI[3].xPos = 15;
        AllButtonsRefForUI[3].yPos = 168;
        AllButtonsRefForUI[3].xSize = 100;
        AllButtonsRefForUI[3].ySize = 24;
        AllButtonsRefForUI[3].OnClick = &ChangeMenu;
        AllButtonsRefForUI[3].parameter = 5;

        AllButtonsRefForUI[4].xPos = 141;
        AllButtonsRefForUI[4].yPos = 40;
        AllButtonsRefForUI[4].xSize = 100;
        AllButtonsRefForUI[4].ySize = 24;
        AllButtonsRefForUI[4].OnClick = &ChangeMenu;
        AllButtonsRefForUI[4].parameter = 7;

        NE_SpriteVisible(BottomScreenSpritesForUI[2], false);
        SetButtonToShow(5);
    }
    else if (menuId == 1) // map menu
    {
        BottomScreenSpritesMaterialsForUI[1] = NE_MaterialCreate();
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[1], PalettesForUI[0], (void *)MapUI_bin, 0);
        NE_SpriteSetMaterial(BottomScreenSpritesForUI[0], BottomScreenSpritesMaterialsForUI[1]);

        // Set map update checkbox
        AllCheckBoxsRefForUI[0].xPos = 220;
        AllCheckBoxsRefForUI[0].yPos = 46;
        AllCheckBoxsRefForUI[0].xSize = 20;
        AllCheckBoxsRefForUI[0].ySize = 20;
        AllCheckBoxsRefForUI[0].value = false;

        // Set exit button
        AllButtonsRefForUI[0].xPos = 200;
        AllButtonsRefForUI[0].yPos = 10;
        AllButtonsRefForUI[0].xSize = 56;
        AllButtonsRefForUI[0].ySize = 20;
        AllButtonsRefForUI[0].OnClick = &ChangeMenu;
        AllButtonsRefForUI[0].parameter = 0;

        SetButtonToShow(1);

        NE_SpriteVisible(BottomScreenSpritesForUI[0], true);
        NE_SpriteVisible(BottomScreenSpritesForUI[1], AllCheckBoxsRefForUI[0].value);
    }
    else if (menuId == 2) // Score board screen
    {
        // Set exit button
        AllButtonsRefForUI[0].xPos = 200;
        AllButtonsRefForUI[0].yPos = 2;
        AllButtonsRefForUI[0].xSize = 56;
        AllButtonsRefForUI[0].ySize = 20;
        AllButtonsRefForUI[0].OnClick = &ChangeMenu;
        AllButtonsRefForUI[0].parameter = 0;

        if (AllPlayersRefForUI[0].IsCounter == -1)
        {
            // Join counter button
            AllButtonsRefForUI[1].xPos = 18; // 128-20-90
            AllButtonsRefForUI[1].yPos = 169;
            AllButtonsRefForUI[1].xSize = 90;
            AllButtonsRefForUI[1].ySize = 20;
            AllButtonsRefForUI[1].OnClick = &SetTeam;
            AllButtonsRefForUI[1].parameter = 1;

            // Join terrorists button
            AllButtonsRefForUI[2].xPos = 148; // 128+20
            AllButtonsRefForUI[2].yPos = 169;
            AllButtonsRefForUI[2].xSize = 90;
            AllButtonsRefForUI[2].ySize = 20;
            AllButtonsRefForUI[2].OnClick = &SetTeam;
            AllButtonsRefForUI[2].parameter = 0;

            SetButtonToShow(3);
        }
        else
            SetButtonToShow(1);
    }
    else if (menuId == 3) // Shop categories
    {
        NE_SpriteVisible(BottomScreenSpritesForUI[2], true);

        for (int buttonIndex = 0; buttonIndex < 6; buttonIndex++)
        {
            // Set all categories buttons
            AllButtonsRefForUI[buttonIndex].xPos = (ScreenWidth / 2) * (buttonIndex / 3);
            AllButtonsRefForUI[buttonIndex].yPos = ((198 - 23) / 3) * (buttonIndex % 3) + 23;
            AllButtonsRefForUI[buttonIndex].xSize = ScreenWidth / 2;
            AllButtonsRefForUI[buttonIndex].ySize = (198 - 23) / 3;
            AllButtonsRefForUI[buttonIndex].OnClick = &OpenShopCategory;
            AllButtonsRefForUI[buttonIndex].parameter = buttonIndex;
        }
        SetButtonToShow(6);
    }
    else if (menuId == 4) // Settings menu
    {
        // Quit button
        NE_SpriteVisible(BottomScreenSpritesForUI[2], true);
    }
    else if (menuId == 5) // Quit menu
    {
        // Quit button
        NE_SpriteVisible(BottomScreenSpritesForUI[2], true);
    }
    else if (menuId == 6) // SHOP
    {
        BottomScreenSpritesMaterialsForUI[6] = NE_MaterialCreate();
        PalettesForUI[10] = NE_PaletteCreate();
        if (ShopCategory < 4)
            NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], AllGunsRefForUI[GetSelectedGunShop()].texture, 1);
        if (ShopCategory == 5)
            NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], GetAllGrenades()[GetSelectedGunShop() - GunCount].texture, 1);

        // Shop quit button
        NE_SpriteVisible(BottomScreenSpritesForUI[2], true);

        // Buy button
        AllButtonsRefForUI[0].xPos = 88;
        AllButtonsRefForUI[0].yPos = 170;
        AllButtonsRefForUI[0].xSize = 80;
        AllButtonsRefForUI[0].ySize = 20;
        AllButtonsRefForUI[0].OnClick = &BuyWeapon;
        AllButtonsRefForUI[0].parameter = 0;

        // Left button
        AllButtonsRefForUI[1].xPos = 48;
        AllButtonsRefForUI[1].yPos = 170;
        AllButtonsRefForUI[1].xSize = 30;
        AllButtonsRefForUI[1].ySize = 20;
        AllButtonsRefForUI[1].OnClick = &ChangeWeapon;
        AllButtonsRefForUI[1].parameter = 1;

        // Right button
        AllButtonsRefForUI[2].xPos = 88 + 80 + 10;
        AllButtonsRefForUI[2].yPos = 170;
        AllButtonsRefForUI[2].xSize = 30;
        AllButtonsRefForUI[2].ySize = 20;
        AllButtonsRefForUI[2].OnClick = &ChangeWeapon;
        AllButtonsRefForUI[2].parameter = 0;

        SetButtonToShow(3);
    }
    else if (menuId == 7) // Game pad menu
    {
        // Game pad quit button
        NE_SpriteVisible(BottomScreenSpritesForUI[2], true);

        BottomScreenSpritesMaterialsForUI[7] = NE_MaterialCreate();
        PalettesForUI[14] = NE_PaletteCreate();
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[7], PalettesForUI[14], (void *)JumpArrow_bin, 1);

        BottomScreenSpritesMaterialsForUI[8] = NE_MaterialCreate();
        PalettesForUI[15] = NE_PaletteCreate();
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[8], PalettesForUI[15], (void *)reload_bin, 1);

        // Jump button
        AllButtonsRefForUI[0].xPos = 2;
        AllButtonsRefForUI[0].yPos = 30;
        AllButtonsRefForUI[0].xSize = 36;
        AllButtonsRefForUI[0].ySize = 36;
        AllButtonsRefForUI[0].OnClick = &SetNeedJump;
        AllButtonsRefForUI[0].parameter = 0;

        // Reload button
        AllButtonsRefForUI[1].xPos = 2;
        AllButtonsRefForUI[1].yPos = 72;
        AllButtonsRefForUI[1].xSize = 36;
        AllButtonsRefForUI[1].ySize = 36;
        AllButtonsRefForUI[1].OnClick = &startReloadGun;
        AllButtonsRefForUI[1].parameter = 0;

        // Right button
        AllButtonsRefForUI[2].xPos = 2;
        AllButtonsRefForUI[2].yPos = 114;
        AllButtonsRefForUI[2].xSize = 36;
        AllButtonsRefForUI[2].ySize = 36;
        AllButtonsRefForUI[2].OnClick = &ChangeGunInInventoryForLocalPlayer;
        AllButtonsRefForUI[2].parameter = 0;

        // Left button
        AllButtonsRefForUI[3].xPos = 2;
        AllButtonsRefForUI[3].yPos = 156;
        AllButtonsRefForUI[3].xSize = 36;
        AllButtonsRefForUI[3].ySize = 36;
        AllButtonsRefForUI[3].OnClick = &ChangeGunInInventoryForLocalPlayer;
        AllButtonsRefForUI[3].parameter = 1;

        SetButtonToShow(4);
    }
    else if (menuId == 8) // Main menu
    {
        // Single player button
        AllButtonsRefForUI[0].xPos = 40;
        AllButtonsRefForUI[0].yPos = 40;
        AllButtonsRefForUI[0].xSize = ScreenWidth - 80;
        AllButtonsRefForUI[0].ySize = 24;
        AllButtonsRefForUI[0].OnClick = &StartGame; // TODO Change this
        AllButtonsRefForUI[0].parameter = 0;

        // Multiplayer button
        AllButtonsRefForUI[1].xPos = 40;
        AllButtonsRefForUI[1].yPos = 87;
        AllButtonsRefForUI[1].xSize = ScreenWidth - 80;
        AllButtonsRefForUI[1].ySize = 24;
        AllButtonsRefForUI[1].OnClick = &ChangeMenu;
        AllButtonsRefForUI[1].parameter = 9;

        // Settings button
        AllButtonsRefForUI[2].xPos = 40;
        AllButtonsRefForUI[2].yPos = 135;
        AllButtonsRefForUI[2].xSize = ScreenWidth - 80;
        AllButtonsRefForUI[2].ySize = 24;
        AllButtonsRefForUI[2].OnClick = &ChangeMenu;
        AllButtonsRefForUI[2].parameter = 4;

        // Hide quit button
        NE_SpriteVisible(BottomScreenSpritesForUI[2], false);

        SetButtonToShow(3);
    }
    else if (menuId == 9) // Multiplayer screen
    {
        // Start game button
        AllButtonsRefForUI[0].xPos = 40;
        AllButtonsRefForUI[0].yPos = 160;
        AllButtonsRefForUI[0].xSize = ScreenWidth - 80;
        AllButtonsRefForUI[0].ySize = 24;
        AllButtonsRefForUI[0].OnClick = &connectFromServerList; // TODO Change this
        AllButtonsRefForUI[0].parameter = -1;

        // Up button
        AllButtonsRefForUI[1].xPos = 62;
        AllButtonsRefForUI[1].yPos = 127;
        AllButtonsRefForUI[1].xSize = 40;
        AllButtonsRefForUI[1].ySize = 24;
        AllButtonsRefForUI[1].OnClick = &changeServer; // TODO Change this
        AllButtonsRefForUI[1].parameter = 0;

        // Down button
        AllButtonsRefForUI[2].xPos = 159;
        AllButtonsRefForUI[2].yPos = 127;
        AllButtonsRefForUI[2].xSize = 40;
        AllButtonsRefForUI[2].ySize = 24;
        AllButtonsRefForUI[2].OnClick = &changeServer; // TODO Change this
        AllButtonsRefForUI[2].parameter = 1;

        // Show quit button
        NE_SpriteVisible(BottomScreenSpritesForUI[2], true);

        // NE_SpriteVisible(BottomScreenSpritesForUI[2], false);
        SetButtonToShow(3);
    }
    SetUpdateBottomScreenOneFrame(4);
}

void OpenShopCategory(int categoryId)
{
    SetSelectedGunShop(-1);
    ShopCategory = categoryId;
    SetWeaponCountFromCategory();
    ChangeMenu(6);
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

void BuyWeapon(int unused)
{
    if (Connection != 0)
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
    // int Selected = -1;
    bool FoundNewGun = false;
    int FirstGunFound = -1;
    if (Left == 0)
    {
        for (int i = 0; i < GunCount + equipementCount + shopGrenadeCount; i++)
        {
            if ((ShopCategory < 4 && i < GunCount && (AllGunsRefForUI[i].gunCategory == ShopCategory && (AllPlayersRefForUI[0].IsCounter == AllGunsRefForUI[i].isForCounterTerrorists || AllGunsRefForUI[i].isForCounterTerrorists == -1))) || (ShopCategory == 5 && i >= GunCount && (AllPlayersRefForUI[0].IsCounter == GetAllGrenades()[i - GunCount].isForCounterTerrorists || GetAllGrenades()[i - GunCount].isForCounterTerrorists == -1)))
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
            if ((ShopCategory < 4 && i < GunCount && (AllGunsRefForUI[i].gunCategory == ShopCategory && (AllPlayersRefForUI[0].IsCounter == AllGunsRefForUI[i].isForCounterTerrorists || AllGunsRefForUI[i].isForCounterTerrorists == -1))) || (ShopCategory == 5 && i >= GunCount && (AllPlayersRefForUI[0].IsCounter == GetAllGrenades()[i - GunCount].isForCounterTerrorists || GetAllGrenades()[i - GunCount].isForCounterTerrorists == -1)))
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
    if (ShopCategory < 4)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], AllGunsRefForUI[Selected].texture, 1);
    else if (ShopCategory == 5)
        NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterialsForUI[6], PalettesForUI[10], GetAllGrenades()[Selected - GunCount].texture, 1);

    SetSelectedGunShop(Selected);
    SetUpdateBottomScreenOneFrame(4);
}

void SetTeam(int i)
{
    SetWaitForTeamResponse(true);
    AllButtonsRefForUI[1].isHidden = true;
    AllButtonsRefForUI[2].isHidden = true;
    AllPlayersRefForUI[0].IsCounter = i; //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////REMOVE
    SetTempTeam(i);
    SetSendTeam(true);
    // SetUpdateBottomScreenOneFrame(4);
}

void ReadTouchScreen(uint32 keysdown, uint32 keys, uint32 keysup, int *currentMenu, touchPosition touch, bool *NeedChangeScreen, bool *AlwaysUpdateBottomScreen, int *ButtonToShow, int *UpdateBottomScreenOneFrame, bool *SendTeam)
{
    if (*UpdateBottomScreenOneFrame == 0)
    {
        // Check checkbox
        if ((keysdown & KEY_TOUCH && touch.px >= AllCheckBoxsRefForUI[0].xPos && touch.px <= AllCheckBoxsRefForUI[0].xPos + AllCheckBoxsRefForUI[0].xSize && touch.py >= AllCheckBoxsRefForUI[0].yPos && touch.py <= AllCheckBoxsRefForUI[0].yPos + AllCheckBoxsRefForUI[0].ySize))
        {
            if (*currentMenu == 1) // Update screen button (map menu)
            {
                AllCheckBoxsRefForUI[0].value = !AllCheckBoxsRefForUI[0].value;
                NE_SpriteVisible(BottomScreenSpritesForUI[1], AllCheckBoxsRefForUI[0].value);
                if (*AlwaysUpdateBottomScreen)
                    *NeedChangeScreen = true;
                else
                    *AlwaysUpdateBottomScreen = true;
            }
        }

        // Check for all buttons click
        for (int i = 0; i < *ButtonToShow; i++)
        {
            if (!AllButtonsRefForUI[i].isHidden && (keysdown & KEY_TOUCH && touch.px >= AllButtonsRefForUI[i].xPos && touch.px <= AllButtonsRefForUI[i].xPos + AllButtonsRefForUI[i].xSize && touch.py >= AllButtonsRefForUI[i].yPos && touch.py <= AllButtonsRefForUI[i].yPos + AllButtonsRefForUI[i].ySize))
            {
                AllButtonsRefForUI[i].OnClick(AllButtonsRefForUI[i].parameter);
                return;
            }
        }

        // Check back button click
        if (keysdown & KEY_TOUCH && touch.px >= 236 && touch.px <= 236 + 20 && touch.py >= 1 && touch.py <= 1 + 20)
        {
            if (*currentMenu == 3 || (*currentMenu == 4 && PartyStarted) || *currentMenu == 5 || *currentMenu == 7) // Back to in game main menu
                ChangeMenu(0);
            else if (*currentMenu == 6) // Back button
                ChangeMenu(3);
            else if (*currentMenu == 9 || (*currentMenu == 4 && !PartyStarted)) // Back main menu
                ChangeMenu(8);
        }

        if (*currentMenu == 7 && keys & KEY_TOUCH && touch.px >= 40 && touch.px <= 255 && touch.py >= 30 && touch.py <= 194)
        {
            if (keysdown & KEY_TOUCH)
            {
                if (GetDoubleTapTimer() > 0)
                    SetAim();

                SetDoubleTapTimer(11);
            }
            else if (LastTouch.x != -1)
            {
                int xAngleToAdd = 0;
                int yAngleToAdd = 0;
                if (LastTouch.x != touch.px)
                    xAngleToAdd = touch.px - LastTouch.x;

                if (LastTouch.y != touch.py)
                    yAngleToAdd = touch.py - LastTouch.y;

                AddAnglesToPlayer(-xAngleToAdd / 2.0, yAngleToAdd / 2.0);
            }

            LastTouch.x = touch.px;
            LastTouch.y = touch.py;
        }
        else
        {
            LastTouch.x = -1;
            LastTouch.y = -1;
        }
    }
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
    sprintf(killText, AllPlayers[killerIndex].name); // GET PLAYER NAME

    sprintf(killText + strlen(killText), " killed ");

    sprintf(killText + strlen(killText), AllPlayers[deadPlayerIndex].name); // GET PLAYER NAME

    KillTextShowTimer = 240;
}

void drawTopScreenUI()
{
    // Set view in 2D mode
    NE_2DViewInit();

    // Only for debug
    int TextYOffset = 0;
    if (!isDebugTopScreen)
        TextYOffset = -3;

    // Player *localPlayer = &AllPlayers[0];
    if (isShowingMap)
    {
        // int xPos = map(localPlayer->xPos, -44, 56, 2, 189);
        // int ypos = map(localPlayer->zPos, -36, 67, 12, 190);

        // Pas trop mal
        // int xPos = map(localPlayer->xPos, -44.46, 56.4, -186, 172);
        // int ypos = map(localPlayer->zPos, -36.4, 66.1, -166, 186);

        // int xPos = map(localPlayer->xPos, -44.46, 56.4, -196, 196);
        // int ypos = map(localPlayer->zPos, -45, 68.7, -196, 196);

        NE_2DDrawQuad(0, 0, ScreenWidth, ScreenHeightFixed, 3, RGB15(0, 0, 0)); // Background

        // PAS MAL
        // int xPos = map(localPlayer->xPos, -46, 56.4, -196, 196);
        // int ypos = map(localPlayer->zPos, -45, 68.7, -196, 196);

        // int xPos = map(localPlayer->xPos, -46, 56.4, -173, 173);
        // int ypos = map(localPlayer->zPos, -45, 68.7, -185, 185);

        // xPos += 50;
        //  int xPos = map(localPlayer->xPos, -44.46, 56.4, -173, 173);
        //  int ypos = map(localPlayer->zPos, -36.4, 66.1, -185, 185);

        // NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 196 / 2 + ((float)xPos * zWithoutYForMap - (float)ypos * xWithoutYForMap) / 2, ScreenCenterHeight - 196 / 2 + 2 + ((float)ypos * zWithoutYForMap + (float)xPos * xWithoutYForMap) / 2);
        //  NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 173 / 2 + ((float)xPos * zWithoutYForMap - (float)ypos * xWithoutYForMap) / 2, ScreenCenterHeight - 185 / 2 + 2 + ((float)ypos * zWithoutYForMap + (float)xPos * xWithoutYForMap) / 2);

        //  NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 173 / 2 + ((float)xPos * zWithoutYForMap - (float)ypos * xWithoutYForMap) / 2 + xWithoutYForMap * 3, ScreenCenterHeight - 185 / 2 + 2 + ((float)ypos * zWithoutYForMap + (float)xPos * xWithoutYForMap) / 2 - zWithoutYForMap * 3);
        // NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 196 / 2 + ((float)xPos * zWithoutYForMap - (float)ypos * xWithoutYForMap) / 2 + zWithoutYForMap * 3, ScreenCenterHeight - 196 / 2 + 2 + ((float)ypos * zWithoutYForMap + (float)xPos * xWithoutYForMap) / 2 + xWithoutYForMap * 3);

        // NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 196 / 2, ScreenCenterHeight - 196 / 2);

        // Calculate map position
        float xPos = map(localPlayer->xPos, -46, 57.5, -170, 170);
        float ypos = map(localPlayer->zPos, -41, 67, -177, 177);
        // Set map position and draw sprite
        NE_SpriteSetPos(TopScreenSprites[4], ScreenCenterWidth - 170 / 2 + (xPos * zWithoutYForMap - ypos * xWithoutYForMap) / 2, ScreenCenterHeight - 177 / 2 + 2 + (ypos * zWithoutYForMap + xPos * xWithoutYForMap) / 2);
        NE_SpriteSetRot(TopScreenSprites[4], (int)localPlayer->Angle);
        NE_SpriteDraw(TopScreenSprites[4]);

        for (int i = 1; i < MaxPlayer; i++)
        {
            // If the other player is in game
            if (AllPlayers[i].Id != -1 && AllPlayers[i].IsCounter != -1 && !AllPlayers[i].IsDead)
            {
                // int xPos = map(AllPlayers[i].xPos, -44, 56, 2, 189);
                // int ypos = map(AllPlayers[i].zPos, -36, 67, 12, 190);

                // Calculate other players points
                float xPos2 = map(AllPlayers[i].xPos, -46, 57.5, -170, 170);
                float ypos2 = map(AllPlayers[i].zPos, -41, 67, -177, 177);
                float xPos3 = ScreenCenterWidth - ((xPos2 - xPos) * zWithoutYForMap + (ypos2 - ypos) * -xWithoutYForMap) / 2;
                float ypos3 = ScreenCenterHeight + ((ypos2 - ypos) * -zWithoutYForMap + (xPos2 - xPos) * -xWithoutYForMap) / 2;

                // Set point color and draw it
                if (AllPlayers[i].IsCounter == AllPlayers[0].IsCounter)
                    NE_2DDrawTexturedQuadColor(xPos3 - 4, ypos3 - 2, xPos3 + 2, ypos3 + 4, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 31)); // Friends points
                else
                    NE_2DDrawTexturedQuadColor(xPos3 - 4, ypos3 - 2, xPos3 + 2, ypos3 + 4, 1, BottomScreenSpritesMaterials[2], RGB15(31, 0, 0)); // Enemies points
            }
        }
        NE_2DDrawTexturedQuadColor(ScreenCenterWidth - 4, ScreenCenterHeight - 2, ScreenCenterWidth + 2, ScreenCenterHeight + 4, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 0)); // Background
        // NE_2DDrawTexturedQuadColor(196 / 2, ScreenCenterHeight, 196 / 2 + 6, ScreenCenterHeight + 6, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 0)); // Background

        char PositionText[30];
        // sprintf(PositionText, "%f", zWithoutYForMap);
        sprintf(PositionText, "%f", xWithoutYForMap);
        // sprintf(PositionText, "%f", ((float)xPos * (-xWithoutYForMap)));
        NE_TextPrint(0,        // Font slot
                     25, 10,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     PositionText);

        /*NE_TextPrint(0,        // Font slot
                     27, 10,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     "map");*/
    }
    else
    {
        if (isDebugTopScreen)
        {
            // DEBUG show player position
            char PositionText[30];
            sprintf(PositionText, "%f %f %f", localPlayer->xPos, localPlayer->yPos, localPlayer->zPos);
            NE_TextPrint(0,        // Font slot
                         1, 0,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         PositionText);

            // DEBUG show CPU usage
            char CPU[40];

            // sprintf(CPU, "CPU : %d, Zone : %d", NE_GetCPUPercent(), localPlayer->CurrentOcclusionZone);
            // sprintf(CPU, "CPU : %d%%, Mem : %d%%", NE_GetCPUPercent(), NE_TextureFreeMemPercent());
            // sprintf(CPU, "curDefuserI : %d chkA%d chkB%d", currentDefuserIndex, bombCheckedInA, bombCheckedInB);

            // sprintf(CPU, "Cur gun : %d %d", AllPlayers[CurrentCameraPlayer].currentGunInInventory, AllPlayers[CurrentCameraPlayer].AllGunsInInventory[AllPlayers[CurrentCameraPlayer].currentGunInInventory]);

            // sprintf(CPU, "Dis 0 & 1 : %d, 1 target : %d", GetDistanceBewteenTwoPlayers(0, 1), AllPlayers[1].target);

            sprintf(CPU, "Pos : %f %f %f", localPlayer->xPos, localPlayer->yPos, localPlayer->zPos);
            // sprintf(CPU, "Look : %f %f %f", x, y, z);
            // sprintf(CPU, "Look : %d %f", (int)localPlayer->Angle % 512, CameraAngleY);

            /*int test = (int)localPlayer->Angle % 512;
            if (test < 0)
                test = 512 + test;
            sprintf(CPU, "Look : X%d fixed X%d Y%f", (int)localPlayer->Angle, test, CameraAngleY);*/

            // sprintf(CPU, "Look : %f %f", CameraAngleY, localPlayer->Angle);

            // sprintf(CPU, "Grenade  : %d", grenades[0]->GrenadeType);
            // sprintf(CPU, "Gun : %d %d %d", currentGunInInventory, AllGunsInInventory[currentGunInInventory], GetSelectedGunShop());
            // sprintf(CPU, "Death Camera : %f", deathCameraLookYOffset);

            // sprintf(CPU, "Bomb : %d %d", localPlayer->haveBomb, AllGunsInInventory[8]);
            // sprintf(CPU, "Mem : %d%%, : %d o", NE_TextureFreeMemPercent(), NE_TextureFreeMem());
            // sprintf(CPU, "Mem : %d%%, : %d o", NE_PaletteFreeMemPercent(), NE_PaletteFreeMem());
            // sprintf(CPU, "Poly : %d, : %d", NE_GetPolygonCount(), NE_GetVertexCount());
            // sprintf(CPU, "Jump : %d %d %d", frameCountDuringAir, CanJump, CanJumpReal);
            // sprintf(CPU, "Shop : %d %d %d", GetShopCategory(), GetSelectedGunShop(), GetWeaponCountVar());
            // sprintf(CPU, "lastTouch : %f %f", GetLastTouch().x, GetLastTouch().y);
            // sprintf(CPU, "Size : %d", sizeof(int));
            NE_TextPrint(0,        // Font slot
                         1, 1,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         CPU);

            /*float distance2D = sqrtf(powf(localPlayer->PlayerModel->x - AllPlayers[1].PlayerModel->x, 2.0) + powf(localPlayer->PlayerModel->z - AllPlayers[1].PlayerModel->z, 2.0));
            Vector3 Direction;
            Direction.x = localPlayer->PlayerModel->x - AllPlayers[1].PlayerModel->x;
            Direction.y = localPlayer->PlayerModel->y - (AllPlayers[1].PlayerModel->y + CameraOffsetY * 4096.0);
            Direction.z = localPlayer->PlayerModel->z - AllPlayers[1].PlayerModel->z;
            //AllPlayers[1].AngleDestination = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0;
            float CameraAngleToGrenadeDirection = atan2f(distance2D, Direction.y) * 512.0 / (M_TWOPI);

            float x2 = 0, y2 = 0, z2 = 0;
            UpdateLookRotationAI(CameraAngleToGrenadeDirection, 1, &x2, &y2, &z2);

            CalculatePlayerPosition(1);
            AllPlayers[1].startXPosRaycast = AllPlayers[1].xPos;
            AllPlayers[1].startYPosRaycast = AllPlayers[1].yPos;
            AllPlayers[1].startZPosRaycast = AllPlayers[1].zPos;

            AllPlayers[1].startXRotRaycast = x2;
            AllPlayers[1].startYRotRaycast = y2;
            AllPlayers[1].startZRotRaycast = z2;
            AllPlayers[1].startGunIdRaycast = AllPlayers[1].AllGunsInInventory[AllPlayers[1].currentGunInInventory];

            char CPU2[40];
            sprintf(CPU2, "Look : %f %f %f r%d", x2, y2, z2, Raycast(1));
            NE_TextPrint(0,        // Font slot
                         1, 8,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         CPU2);*/

            char CPU2[120] = "";
            for (int i = 0; i < MaxPlayer; i++)
            {
                // sprintf(CPU2 + strlen(CPU2), "b%d: hB:%d pB%d bT%d +%d\n", i, AllPlayers[i].haveBomb, AllPlayers[i].isPlantingBomb, AllPlayers[i].bombTimer, AllPlayers[i].Health);
                //  sprintf(CPU2 + strlen(CPU2), "b%d: g0:%d g1:%d +%d\n", i, AllPlayers[i].AllGunsInInventory[0], AllPlayers[i].AllGunsInInventory[1], AllPlayers[i].Health);
                sprintf(CPU2 + strlen(CPU2), "b%d: s0:%d s1:%d s2:%d\n", i, AllPlayers[i].PlayerPhysic->xspeed, AllPlayers[i].PlayerPhysic->yspeed, AllPlayers[i].PlayerPhysic->zspeed);
            }

            NE_TextPrint(0,        // Font slot
                         0, 9,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         CPU2);

            // DEBUG show map zone to show
            char Occlusion[20];
            sprintf(Occlusion, "Zone to show : ");
            for (int i = 0; i < AllZones[localPlayer->CurrentOcclusionZone].ZoneCount; i++)
                sprintf(Occlusion + strlen(Occlusion), "%d ", AllZones[localPlayer->CurrentOcclusionZone].AllVisibleZones[i]);

            // for (int i = 0; i < AllZones[17].ZoneCount; i++)
            // sprintf(Occlusion + strlen(Occlusion), "%d ", AllZones[17].AllVisibleZones[i]);

            NE_TextPrint(0,        // Font slot
                         1, 2,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         Occlusion);
        }

        // DEBUG for player show ping
        char PING[30];
        sprintf(PING, "Ping : %d", ping);
        NE_TextPrint(0,                  // Font slot
                     1, 5 + TextYOffset, // Coordinates x(column), y(row)
                     NE_White,           // Color
                     PING);

        // Show player money
        char MoneyText[30];
        sprintf(MoneyText, "%d$", localPlayer->Money);
        NE_TextPrint(0,                  // Font slot
                     1, 6 + TextYOffset, // Coordinates x(column), y(row)
                     NE_White,           // Color
                     MoneyText);

        // Show party score
        char PartyScore[30];
        // CountersT score
        if (CounterScore > 9)
            sprintf(PartyScore, "%d ", CounterScore);
        else
            sprintf(PartyScore, "0%d ", CounterScore);

        // Terrosists score
        if (TerroristsScore > 9)
            sprintf(PartyScore + strlen(PartyScore), "%d", TerroristsScore);
        else
            sprintf(PartyScore + strlen(PartyScore), "0%d", TerroristsScore);

        NE_TextPrint(0,                  // Font slot
                     1, 4 + TextYOffset, // Coordinates x(column), y(row)
                     NE_White,           // Color
                     PartyScore);

        // Show event text
        if (textToShowTimer > 0)
        {
            NE_TextPrint(0,                  // Font slot
                         1, 7 + TextYOffset, // Coordinates x(column), y(row)
                         NE_White,           // Color
                         textToShow);

            textToShowTimer--;
        }

        // Show kill text
        if (KillTextShowTimer > 0)
        {
            NE_TextPrint(0,                  // Font slot
                         0, 8 + TextYOffset, // Coordinates x(column), y(row)
                         NE_White,           // Color
                         killText);

            KillTextShowTimer--;
        }

        if (!BombPlanted)
        // if (true)
        {
            // Show party timer if bomb is not planted
            // Minutes
            char Timer[30];
            if (PartyMinutes > 9)
                sprintf(Timer, "%d:", PartyMinutes);
            else
                sprintf(Timer, "0%d:", PartyMinutes);

            // Seconds
            if (PartySeconds > 9)
                sprintf(Timer + strlen(Timer), "%d", PartySeconds);
            else
                sprintf(Timer + strlen(Timer), "0%d", PartySeconds);

            if (isDebugTopScreen)
                sprintf(Timer + strlen(Timer), " State : %d", RoundState);

            NE_TextPrint(0,                  // Font slot
                         1, 3 + TextYOffset, // Coordinates x(column), y(row)
                         NE_White,           // Color
                         Timer);

            // Show planting bomb animation
            if (localPlayer->bombTimer < 120 && localPlayer->bombTimer > 0)
            {
                // Draw Background
                NE_PolyFormat(20, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                NE_2DDrawQuad(ScreenWidth / 4, 155, ScreenWidth - mapInt(localPlayer->bombTimer, 1, 119, ScreenWidth / 4 * 3, ScreenWidth / 4), 170, 1, RGB15(3, 3, 3));
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
                NE_2DDrawQuad(ScreenWidth / 4, 155, ScreenWidth - mapInt(localPlayer->bombTimer, 1, 599, ScreenWidth / 4 * 3, ScreenWidth / 4), 170, 1, RGB15(3, 3, 3));
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
        Player *selectPlayer = &AllPlayers[CurrentCameraPlayer];
        if (selectPlayer->IsCounter != -1)
        {
            if (!selectPlayer->IsDead)
            {
                // Show health
                char HealthText[16];
                sprintf(HealthText, "+%d arm:%d", selectPlayer->Health, selectPlayer->armor);

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
                    redHealthTextCounter--;
                    NE_TextPrint(0,                                   // Font slot
                                 1, 23,                               // Coordinates x(column), y(row)
                                 RGB15(31, ColorAmount, ColorAmount), // Color
                                 HealthText);
                }

                // Show gun ammo
                if (selectPlayer->currentGunInInventory == 1 || selectPlayer->currentGunInInventory == 2)
                {
                    char AmmoText[16];
                    // sprintf(AmmoText, "%d/%d", localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].AmmoCount, localPlayer->AllAmmoMagazine[localPlayer->currentGunInInventory - 1].TotalAmmoCount);
                    sprintf(AmmoText, "%d/%d", selectPlayer->AllAmmoMagazine[selectPlayer->currentGunInInventory - 1].AmmoCount, selectPlayer->AllAmmoMagazine[selectPlayer->currentGunInInventory - 1].TotalAmmoCount);
                    NE_TextPrint(0,        // Font slot
                                 1, 22,    // Coordinates x(column), y(row)
                                 NE_White, // Color
                                 AmmoText);
                }
            }

            if (CurrentScopeLevel == 0) // Draw gun if player doesn't aim with his gun
            {
                NE_SpriteDraw(TopScreenSprites[0]);
                // Draw gun
                if (!selectPlayer->IsDead)
                {
                    // Set new gun position with head bobbing
                    if (selectPlayer->currentGunInInventory >= GunCount || !AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].isKnife)
                    {
                        gunX = GunPositionX + GunXRecoil + sinf(BobbingOffset) * 4;
                        gunY = GunPositionY + 4 + GunYRecoil + cosf(BobbingOffset * 2) * 6;
                    }
                    else
                    {
                        gunX = GunPositionX - GunXRecoil + GunMaxRecoil * 1.15 + sinf(BobbingOffset) * 4;
                        gunY = GunPositionY + 4 - GunYRecoil + GunMaxRecoil * 1.4 + cosf(BobbingOffset * 2) * 6;
                    }
                    NE_2DDrawTexturedQuad(gunX, gunY, gunX + 96, gunY + 96, 1, TopScreenSpritesMaterials[1]); // Gun
                }

                // Draw gun muzzle flash
                if (selectPlayer->currentGunInInventory < GunCount && ShowMuzzle > 0 && !AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].isKnife)
                {
                    NE_PolyFormat(10 * ShowMuzzle, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                    NE_2DDrawTexturedQuad(gunX + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].xMuzzleFlashOffset, gunY + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].yMuzzleFlashOffset, gunX + 40 + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].xMuzzleFlashOffset, gunY + 42 + AllGuns[selectPlayer->AllGunsInInventory[selectPlayer->currentGunInInventory]].yMuzzleFlashOffset, 2, TopScreenSpritesMaterials[2]); // Gun muzzle flash																																																																																													  //ShowMuzzle--;
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

            // Reduce muzzle timer
            if (ShowMuzzle > 0)
                ShowMuzzle--;
        }
        // TODO MERGE UP AND DONW
        /*if (GetPlayers()[0].IsCounter != -1)
        {
            if (CurrentScopeLevel == 0) // Draw gun if player doesn't aim with his gun
            {
                NE_SpriteDraw(TopScreenSprites[0]);
                // Draw gun
                if (!GetPlayers()[0].IsDead)
                {
                    // Set new gun position with head bobbing
                    if (localPlayer->currentGunInInventory >= GunCount || !AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].isKnife)
                    {
                        gunX = GunPositionX + GunXRecoil + sinf(BobbingOffset) * 4;
                        gunY = GunPositionY + 4 + GunYRecoil + cosf(BobbingOffset * 2) * 6;
                    }
                    else
                    {
                        gunX = GunPositionX - GunXRecoil + GunMaxRecoil * 1.15 + sinf(BobbingOffset) * 4;
                        gunY = GunPositionY + 4 - GunYRecoil + GunMaxRecoil * 1.4 + cosf(BobbingOffset * 2) * 6;
                    }
                    NE_2DDrawTexturedQuad(gunX, gunY, gunX + 96, gunY + 96, 1, TopScreenSpritesMaterials[1]); // Gun
                }

                // Draw gun muzzle flash
                if (localPlayer->currentGunInInventory < GunCount && ShowMuzzle > 0 && !AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].isKnife)
                {
                    NE_PolyFormat(10 * ShowMuzzle, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                    NE_2DDrawTexturedQuad(gunX + AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].xMuzzleFlashOffset, gunY + AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].yMuzzleFlashOffset, gunX + 40 + AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].xMuzzleFlashOffset, gunY + 42 + AllGuns[localPlayer->AllGunsInInventory[localPlayer->currentGunInInventory]].yMuzzleFlashOffset, 2, TopScreenSpritesMaterials[2]); // Gun muzzle flash																																																																																													  //ShowMuzzle--;
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

            // Reduce muzzle timer
            if (ShowMuzzle > 0)
                ShowMuzzle--;
        }*/

        float totalDis = 0;
        int totalEndAnimation = 0;
        for (int i = 0; i < GrenadeCount; i++)
        {
            if (grenades[i] != NULL && grenades[i]->GrenadeType == 1 && grenades[i]->Timer == 0)
            {
                // Calculate distances
                float smokeDistance = sqrtf(powf(GetPlayers()[0].PlayerModel->x - grenades[i]->Model->x, 2.0) + powf(GetPlayers()[0].PlayerModel->y - grenades[i]->Model->y, 2.0) + powf(GetPlayers()[0].PlayerModel->z - grenades[i]->Model->z, 2.0)) / 26000.0; // fFor smoke detection

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

        // If player is flashed, show white screen
        if (flashed)
        {
            flashaAnimation += flashAnimationSpeed;

            int alpha = 31;
            if (flashaAnimation < flashAnimationSpeed * 5.0)
            {
                alpha = (int)((flashaAnimation / (flashAnimationSpeed * 5.0)) * 31.0);
            }
            else if (flashaAnimation >= flashAnimationSpeed * 120.0)
            {
                alpha = map(flashaAnimation, flashAnimationSpeed * 120.0, 1, 31, 0);
            }

            NE_PolyFormat(alpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION); // Set alpha
            NE_2DDrawQuad(0, 0, ScreenWidth, ScreenHeightFixed, 0, RGB15(31, 31, 31));

            if (flashaAnimation >= 1)
            {
                flashed = false;
                flashaAnimation = 0;
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
}

void drawBottomScreenUI()
{
    // Set view in 2D mode
    NE_2DViewInit();

    if (isShowingKeyBoard)
    {
        drawKeyBoard();

        NE_TextPrint(0,        // Font slot
                     27, 10,   // Coordinates x(column), y(row)
                     NE_White, // Color
                     "keyboard");
    }
    else
    {

        // Draw sprites
        for (int i = 0; i < BottomScreenSpriteCount; i++)
            NE_SpriteDraw(BottomScreenSprites[i]);

        // Draw menu background
        NE_2DDrawQuad(0, 0, 256, 196, 20, RGB15(3, 3, 3));
        if (currentMenu == 1) // Map menu
        {
            // Draw checkbox
            int CheckBoxXPosition = AllCheckBoxs[0].xPos, CheckBoxYPosition = AllCheckBoxs[0].yPos, CheckBoxXSize = AllCheckBoxs[0].xSize, CheckBoxYSize = AllCheckBoxs[0].ySize;
            NE_2DDrawTexturedQuadColor(CheckBoxXPosition - 3, CheckBoxYPosition - 3, CheckBoxXPosition + CheckBoxXSize + 3, CheckBoxYPosition + CheckBoxYSize + 3, 2, BottomScreenSpritesMaterials[5], RGB15(4, 4, 4)); // Border
            NE_2DDrawTexturedQuadColor(CheckBoxXPosition - 1, CheckBoxYPosition - 1, CheckBoxXPosition + CheckBoxXSize + 1, CheckBoxYPosition + CheckBoxYSize + 1, 1, BottomScreenSpritesMaterials[5], RGB15(5, 5, 5)); // Background
            if (AllCheckBoxs[0].value)
                NE_2DDrawTexturedQuadColor(CheckBoxXPosition, CheckBoxYPosition, CheckBoxXPosition + CheckBoxXSize, CheckBoxYPosition + CheckBoxYSize, 0, BottomScreenSpritesMaterials[4], RGB15(31, 31, 31)); // CheckMark

            for (int i = 0; i < MaxPlayer; i++)
            {
                if (AllPlayers[i].Id != -1 && AllPlayers[i].IsCounter != -1 && !AllPlayers[i].IsDead)
                {
                    int xPos = map(AllPlayers[i].xPos, -44, 56, 2, 189);
                    int ypos = map(AllPlayers[i].zPos, -36, 67, 12, 190);
                    if (i == 0)
                        NE_2DDrawTexturedQuadColor(xPos, ypos, xPos + 6, ypos + 6, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 0)); // Background
                    else if (AllPlayers[i].IsCounter == AllPlayers[0].IsCounter)
                        NE_2DDrawTexturedQuadColor(xPos, ypos, xPos + 6, ypos + 6, 1, BottomScreenSpritesMaterials[2], RGB15(0, 31, 31)); // Background
                    else
                        NE_2DDrawTexturedQuadColor(xPos, ypos, xPos + 6, ypos + 6, 1, BottomScreenSpritesMaterials[2], RGB15(31, 0, 0)); // Background
                }
            }

            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            // Print texts
            NE_TextPrint(0,        // Font slot
                         26, 2,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Close");

            NE_TextPrint(0,        // Font slot
                         26, 9,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Update");

            NE_TextPrint(0,        // Font slot
                         27, 10,   // Coordinates x(column), y(row)
                         NE_White, // Color
                         "map");
        }
        else if (currentMenu == 0) // Draw main menu
        {
            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            // Print texts
            NE_TextPrint(0,        // Font slot
                         12, 2,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Main menu");

            /*NE_TextPrint(0,        // Font slot
                         3, 6,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Show map");*/

            NE_TextPrint(0,        // Font slot
                         3, 10,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Show Score");

            NE_TextPrint(0,        // Font slot
                         3, 14,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Open shop");

            NE_TextPrint(0,        // Font slot
                         3, 18,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Settings");

            NE_TextPrint(0,        // Font slot
                         3, 22,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Quit game");

            NE_TextPrint(0,        // Font slot
                         21, 6,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Controls");
        }
        else if (currentMenu == 2) // Draw scoreboard menu
        {
            if (!WaitForTeamResponse && localPlayer->IsCounter == -1)
                ButtonToShow = 3;
            else if (!WaitForTeamResponse) /////////////////////////////////////////////////////////////////////////////////////////////CHECK THIS
                ButtonToShow = 1;

            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            // Draw white bars
            NE_2DDrawQuad(0, 25, 256, 28, 19, RGB15(31, 31, 31));
            NE_2DDrawQuad(126, 28, 130, 180, 19, RGB15(31, 31, 31));

            if (!AllButtons[0].isHidden)
            {
                // Write texts
                NE_TextPrint(0,        // Font slot
                             26, 1,    // Coordinates x(column), y(row)
                             NE_White, // Color
                             "Close");
            }

            NE_TextPrint(0,        // Font slot
                         11, 1,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Score table");

            // Counter terrorists text
            char CounterScoreText[30] = "Counter t : ";
            if (CounterScore > 9)
                sprintf(CounterScoreText + strlen(CounterScoreText), "%d ", CounterScore);
            else
                sprintf(CounterScoreText + strlen(CounterScoreText), "0%d ", CounterScore);

            NE_TextPrint(0,        // Font slot
                         1, 4,     // Coordinates x(column), y(row)
                         NE_White, // Color
                         CounterScoreText);

            // Show each counter terrorists player
            int Count1 = 0;
            for (int i = 0; i < MaxPlayer; i++)
            {
                if (AllPlayers[i].IsCounter == 1)
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
                    NE_TextPrint(0,        // Font slot
                                 1, y + 1, // Coordinates x(column), y(row)
                                 NE_White, // Color
                                 playerValues);

                    Count1++;
                }
            }

            // Counter terrorists text
            char TerroristsScoreText[30] = "Terrorists : ";
            if (TerroristsScore > 9)
                sprintf(TerroristsScoreText + strlen(TerroristsScoreText), "%d ", TerroristsScore);
            else
                sprintf(TerroristsScoreText + strlen(TerroristsScoreText), "0%d ", TerroristsScore);

            NE_TextPrint(0,        // Font slot
                         17, 4,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         TerroristsScoreText);

            // Show each terrorists player
            int Count2 = 0;
            for (int i = 0; i < MaxPlayer; i++)
            {
                if (AllPlayers[i].IsCounter == 0)
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
                    NE_TextPrint(0,         // Font slot
                                 17, y + 1, // Coordinates x(column), y(row)
                                 NE_White,  // Color
                                 playerValues);

                    Count2++;
                }
            }

            // Show join team texts
            if (localPlayer->IsCounter == -1 && !WaitForTeamResponse)
            {
                NE_TextPrint(0,        // Font slot
                             6, 22,    // Coordinates x(column), y(row)
                             NE_White, // Color
                             "Join");

                NE_TextPrint(0,        // Font slot
                             22, 22,   // Coordinates x(column), y(row)
                             NE_White, // Color
                             "Join");
            }
        }
        else if (currentMenu == 3) // Draw shop categories
        {
            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            // Draw white bars
            NE_2DDrawQuad(ScreenCenterWidth - 1, 23, ScreenCenterWidth + 1, ScreenHeightFixed, 0, RGB15(31, 31, 31));
            NE_2DDrawQuad(0, (198 - 23) / 3 - 1 + 23, ScreenWidth, (198 - 23) / 3 + 2 + 23, 0, RGB15(31, 31, 31));
            NE_2DDrawQuad(0, (198 - 23) / 3 * 2 - 1 + 23, ScreenWidth, (198 - 23) / 3 * 2 + 2 + 23, 0, RGB15(31, 31, 31));

            // Show texts
            NE_TextPrint(0,        // Font slot
                         14, 1,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Shop");

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
        else if (currentMenu == 6) // Draw shop
        {
            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            NE_2DDrawQuad(160, 26, 255, 144, 1, RGB15(4, 4, 4));                                   // Draw gun details background
            NE_2DDrawTexturedQuad(26, 26, 118 + 26, 118 + 26, 1, BottomScreenSpritesMaterials[6]); // Draw gun

            // Title
            NE_TextPrint(0,        // Font slot
                         14, 1,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Shop");

            // Text for buy button
            NE_TextPrint(0,        // Font slot
                         14, 22,   // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Buy!");

            // Text for lrft button
            NE_TextPrint(0,        // Font slot
                         7, 22,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "<-");

            // Text for right button
            NE_TextPrint(0,        // Font slot
                         23, 22,   // Coordinates x(column), y(row)
                         NE_White, // Color
                         "->");

            if (GetShopCategory() < 4) // If current shop category is guns
            {
                // Gun name
                printLongText(21, 31, 4, AllGuns[SelectedGunShop].name);

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
        else if (currentMenu == 7) // Draw game pad
        {
            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            NE_2DDrawQuad(40, 30, 255, 194, 1, RGB15(4, 4, 4));                                        // touch pad area
            NE_2DDrawTexturedQuad(4, 32, 4 + 36 - 4, 30 + 36 - 2, 1, BottomScreenSpritesMaterials[7]); // Jump button
            NE_2DDrawTexturedQuad(4, 74, 4 + 36 - 4, 72 + 36 - 2, 1, BottomScreenSpritesMaterials[8]); // Reload button

            // Print texts
            NE_TextPrint(0,        // Font slot
                         12, 2,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Game pad");
        }
        else if (currentMenu == 8) // Draw main menu
        {
            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

            // Print texts
            NE_TextPrint(0,        // Font slot
                         10, 2,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Counter Strike");

            NE_TextPrint(0,        // Font slot
                         1, 22,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "1.0");

            NE_TextPrint(0,        // Font slot
                         24, 22,   // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Fewnity");

            NE_TextPrint(0,        // Font slot
                         10, 6,    // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Single Player");

            NE_TextPrint(0,        // Font slot
                         11, 12,   // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Multiplayer");

            NE_TextPrint(0,        // Font slot
                         12, 18,   // Coordinates x(column), y(row)
                         NE_White, // Color
                         "Settings");
        }
        else if (currentMenu == 9) // Draw main menu
        {
            // Draw buttons
            for (int i = 0; i < ButtonToShow; i++)
            {
                if (!AllButtons[i].isHidden)
                {
                    int ButtonXPosition = AllButtons[i].xPos, ButtonYPosition = AllButtons[i].yPos, ButtonXSize = AllButtons[i].xSize, ButtonYSize = AllButtons[i].ySize;
                    NE_2DDrawQuad(ButtonXPosition, ButtonYPosition, ButtonXPosition + ButtonXSize, ButtonYPosition + ButtonYSize, 1, RGB15(5, 5, 5)); // Button background
                }
            }

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
    }
    // Change screen update mode (30 fps with 2 screens update or 60 fps with main game screen update)
    if (NeedChangeScreen)
    {
        NeedChangeScreen = false;
        AlwaysUpdateBottomScreen = !AlwaysUpdateBottomScreen;
        UpdateBottomScreenOneFrame = 4;
    }
}