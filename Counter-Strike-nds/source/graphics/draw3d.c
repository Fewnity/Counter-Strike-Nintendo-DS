// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "grenade.h"
#include "collisions.h"
#include "ui.h"
#include "map.h"
#include "draw3d.h"
#include "party.h"

int t1x = 0;
int t1z = 0;
int t2x = 0;
int t2z = 0;
// Text material
NE_Material *TextMaterial = NULL;
// All palettes
NE_Palette *Palettes[17]; // 0 MapUI, 1 Map atlas, 2 text, 3 player, 4 gun sprite, 5 map point

// All materials used for the top screen
NE_Material *TopScreenSpritesMaterials[6];

// All materials used for the bottom screen
NE_Material *BottomScreenSpritesMaterials[9];

void initGraphics()
{
    // create Materials
    GroundMaterial = NE_MaterialCreate();
    GroundMaterialShadowed = NE_MaterialCreate();
    PlayerMaterial = NE_MaterialCreate();
    PlayerMaterialTerrorist = NE_MaterialCreate();
    PlayerShadowMaterial = NE_MaterialCreate();
    TopScreenSpritesMaterials[0] = NE_MaterialCreate();
    TopScreenSpritesMaterials[2] = NE_MaterialCreate();
    TopScreenSpritesMaterials[3] = NE_MaterialCreate();
    TopScreenSpritesMaterials[4] = NE_MaterialCreate();
    TopScreenSpritesMaterials[5] = NE_MaterialCreate();

    BottomScreenSpritesMaterials[0] = NE_MaterialCreate();
    BottomScreenSpritesMaterials[2] = NE_MaterialCreate();
    BottomScreenSpritesMaterials[3] = NE_MaterialCreate();
    BottomScreenSpritesMaterials[4] = NE_MaterialCreate();
    BottomScreenSpritesMaterials[5] = NE_MaterialCreate();

    // Create palettes
    Palettes[0] = NE_PaletteCreate();
    Palettes[2] = NE_PaletteCreate();
    Palettes[1] = NE_PaletteCreate();
    Palettes[3] = NE_PaletteCreate();
    Palettes[5] = NE_PaletteCreate();
    Palettes[6] = NE_PaletteCreate();
    Palettes[7] = NE_PaletteCreate();
    Palettes[8] = NE_PaletteCreate();
    Palettes[9] = NE_PaletteCreate();
    Palettes[11] = NE_PaletteCreate();
    Palettes[12] = NE_PaletteCreate();
    Palettes[13] = NE_PaletteCreate();
    Palettes[14] = NE_PaletteCreate();
    Palettes[15] = NE_PaletteCreate();
    Palettes[16] = NE_PaletteCreate();

    // Load .bin textures

    // Load font
    TextMaterial = NE_MaterialCreate();
    NE_MaterialTexLoadBMPtoRGB256(TextMaterial, Palettes[2], (void *)text_bmp_bin, true); // Load bmp font format

    // Create font
    NE_TextInit(0,            // Font slot
                TextMaterial, // Image
                8, 8);        // Size of one character (x, y)

    NE_MaterialTexLoadBMPtoRGB256(PlayerMaterial, Palettes[3], (void *)gign_skin1_bin, 0);
    NE_MaterialTexLoadBMPtoRGB256(PlayerMaterialTerrorist, Palettes[13], (void *)terrorist_skin1_bin, 0);

    NE_MaterialTexLoadBMPtoRGB256(GroundMaterial, Palettes[1], (void *)Atlas_bin, 1);
    NE_MaterialTexClone(GroundMaterial, GroundMaterialShadowed);

    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[0], Palettes[9], (void *)QuitButton_bin, 1);
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[2], Palettes[5], (void *)MapPointUI_bin, 1);
    NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[5], Palettes[16], (void *)bomb_logo_bin, 1);
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[4], Palettes[7], (void *)CheckMark_bin, 1);
    NE_MaterialTexLoadBMPtoRGB256(BottomScreenSpritesMaterials[5], Palettes[6], (void *)WhiteScareRounded_bin, 1);

    // Load .bmp textures top screen
    NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[0], Palettes[8], (void *)crosshair2_bin, 1);

    NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[2], Palettes[11], (void *)muzzle_bin, 1);
    NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[3], Palettes[12], (void *)scopeImage_bin, 1);
    NE_MaterialTexLoadBMPtoRGB256(TopScreenSpritesMaterials[4], Palettes[14], (void *)MapUI_bin, 0);
    NE_MaterialTexLoadBMPtoRGB256(PlayerShadowMaterial, Palettes[15], (void *)player_shadow_bin, 1);

    // Create crosshair sprite
    TopScreenSprites[0] = NE_SpriteCreate();
    NE_SpriteSetPos(TopScreenSprites[0], ScreenCenterWidth - 20 / 2, ScreenCenterHeight - 20 / 2);
    NE_SpriteSetSize(TopScreenSprites[0], 20, 20);
    NE_SpriteSetPriority(TopScreenSprites[0], 10);
    NE_SpriteSetMaterial(TopScreenSprites[0], TopScreenSpritesMaterials[0]);

    // Create map sprite
    TopScreenSprites[1] = NE_SpriteCreate();
    NE_SpriteSetSize(TopScreenSprites[1], 170, 177);
    NE_SpriteSetPriority(TopScreenSprites[1], 2);
    NE_SpriteSetMaterial(TopScreenSprites[1], TopScreenSpritesMaterials[4]);

    // Create quit button
    BottomScreenSprites[0] = NE_SpriteCreate();
    NE_SpriteSetSize(BottomScreenSprites[0], 20, 20);
    NE_SpriteSetPriority(BottomScreenSprites[0], 0);
    NE_SpriteSetMaterial(BottomScreenSprites[0], BottomScreenSpritesMaterials[0]);
    NE_SpriteSetPos(BottomScreenSprites[0], 236, 2);
    NE_SpriteVisible(BottomScreenSprites[0], false);

    // Create models
    for (int i = 7; i < 7 + 4; i++)
    {
        if (i == 8) // Model 8 is unused
            continue;

        Model[i] = NE_ModelCreate(NE_Static);
        NE_ModelSetMaterial(Model[i], GroundMaterial);
        NE_ModelScaleI(Model[i], 4096, 4096, 4096);
        NE_ModelSetCoord(Model[i], 0, 1.5 + 0.8, 0);
        Model[i]->rx = 128;
        Model[i]->ry = 256;
    }

    // Load bomb model
    NE_ModelLoadStaticMesh(Model[7], (u32 *)bomb_bin);

    // Load flash models
    for (int i = 0; i < FLASH_MODELS_COUNT; i++)
    {
        flashModels[i] = NE_ModelCreate(NE_Static);

        if (i == 0)
            NE_ModelLoadStaticMesh(flashModels[i], (u32 *)plane_bin);
        else
            NE_ModelClone(flashModels[i],  // Destination
                          flashModels[0]); // Source model

        NE_ModelSetMaterial(flashModels[i], TopScreenSpritesMaterials[2]);
        NE_ModelScaleI(flashModels[i], 4096, 4096, 4096);
    }

    createPlayerShadow();

    // Load explision effect model
    NE_ModelLoadStaticMesh(Model[10], (u32 *)explosion_bin);
    Model[10]->rx = 0;
    Model[10]->ry = 256;
    NE_ModelScaleI(Model[10], 0, 0, 0);
}

/**
 * @brief Create shadow for each players
 *
 */
void createPlayerShadow()
{
    // Player shadow
    for (int i = 1; i < MaxPlayer; i++)
    {
        AllPlayers[i].PlayerShadow = NE_ModelCreate(NE_Static);
        NE_ModelScaleI(AllPlayers[i].PlayerShadow, 4096 * 2, 4096 * 2, 4096 * 2);
        NE_ModelSetMaterial(AllPlayers[i].PlayerShadow, PlayerShadowMaterial);
        NE_ModelLoadStaticMesh(AllPlayers[i].PlayerShadow, (u32 *)plane_bin);
        NE_ModelSetCoord(AllPlayers[i].PlayerShadow, 4, -0.845, -13);
    }
}

/**
 * @brief Enanble or disable two screens refresh mode
 *
 * @param value
 */
void SetTwoScreenMode(bool value)
{
    if (AlwaysUpdateBottomScreen && !value && UpdateBottomScreenFrameCount == 0)
    {
        NeedChangeScreen = true;
    }
    else if (!AlwaysUpdateBottomScreen && value)
    {
        NeedChangeScreen = true;
    }
}

/**
 * @brief Draw top screen 3D then UI
 *
 */
void Draw3DScene(void)
{
    // Set camera for drawing
    NE_CameraUse(Camera);

    // Reset polygons Alpha/Light/Effect
    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

    Map *map = &allMaps[currentMap];

    // render 3D if map insn't at screen
    if (!isShowingMap)
    {
        // Field of view end coordinates
        t1x = (xWithoutYForOcclusionSide1 * 500 + AllPlayers[CurrentCameraPlayer].position.x) * 8192.0;
        t1z = (zWithoutYForOcclusionSide1 * 500 + AllPlayers[CurrentCameraPlayer].position.z) * 8192.0;
        t2x = (xWithoutYForOcclusionSide2 * 500 + AllPlayers[CurrentCameraPlayer].position.x) * 8192.0;
        t2z = (zWithoutYForOcclusionSide2 * 500 + AllPlayers[CurrentCameraPlayer].position.z) * 8192.0;

        // Draw map
        for (int i = 0; i < map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].ZoneCount; i++)
        {
            bool inFov = false; // Is the map part in the field of view of the player?

            // Force to render the map part where the player is
            if (checkZoneForOcclusion(&map->AllOcclusionZone[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]], AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z))
                inFov = true;
            else
            {
                // Check if the map part is in the field of view of the player
                for (int i2 = 0; i2 < 4; i2++)
                {
                    if (PointInTriangleInt(map->AllOcclusionZone[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]].anglesInt[i2].x, map->AllOcclusionZone[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]].anglesInt[i2].y, AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z, t1x, t1z, t2x, t2z))
                    {
                        inFov = true;
                        break;
                    }
                }
            }

            // Render map model if needed
            if (inFov)
            {
                if (!map->models[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]].shadowed) // Set the model light like normal
                {
                    GroundMaterial->diffuse = RGB15(0, 0, 0);
                    GroundMaterial->emission = RGB15(11, 11, 11);
                    GroundMaterial->specular = RGB15(7, 7, 7);
                }
                else // Set the model light like shadowed
                {
                    GroundMaterial->diffuse = RGB15(1, 1, 1);
                    GroundMaterial->emission = RGB15(3, 3, 3);
                    GroundMaterial->specular = RGB15(3, 3, 3);
                }
                NE_ModelDraw(map->models[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]].Model);
            }
        }

        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

        // Draw grenades and their effects
        for (int i = 0; i < GrenadeCount; i++)
        {
            if (grenades[i] != NULL)
            {
                //  Draw grenade
                if (grenades[i]->isVisible)
                {
                    // Grenade clipping
                    bool inFov = PointInTriangleInt(grenades[i]->Model->x, grenades[i]->Model->z, AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z, t1x, t1z, t2x, t2z);

                    // Draw grenade if in field of view
                    if (inFov)
                    {
                        NE_PolyFormat(30, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                        NE_ModelDraw(grenades[i]->Model);
                    }
                }

                // Alpha 0 is wireframe mode, put to 1 to see the grenade
                if (grenades[i]->effectAlpha <= 0)
                    grenades[i]->effectAlpha = 1;

                // Set polygons alpha
                NE_PolyFormat(grenades[i]->effectAlpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                // Draw effect if timer effect is on
                if (grenades[i]->EffectTimer != 0)
                {
                    if ((grenades[i]->GrenadeType == SMOKE && isInFullSmoke) || grenades[i]->GrenadeType == FLASH)
                        continue;
                    NE_ModelDraw(grenades[i]->EffectModel);
                }
            }
        }
        isInFullSmoke = false;

        // Reset polygons Alpha/Light/Effect
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

        // If bomb is planted, draw bomb
        if (BombPlanted || bombDropped)
            NE_ModelDraw(Model[7]);

        // Show wall bullet hit flash
        for (int i = 0; i < FLASH_MODELS_COUNT; i++)
        {
            if (ShowWallHitFlash[i] != 0)
            {
                ShowWallHitFlash[i]--;
                NE_ModelDraw(flashModels[i]);
            }
        }

        // Draw bomb explosion
        if (IsExplode)
        {
            // Calculate explosion effect alpha
            int ExplosionAlpha = 31 - (int)(BombExplosionScale / 1.2);
            if (ExplosionAlpha < 0)
            {
                ExplosionAlpha = 0;
                IsExplode = false;
            }
            // Set polygons alpha
            NE_PolyFormat(ExplosionAlpha, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

            // Draw explosion
            NE_ModelDraw(Model[10]);
        }

        DrawPlayers();
    }

    // Draw UI
    drawTopScreenUI();
}

/**
 * @brief Draws players
 *
 */
void DrawPlayers()
{
    if (UpdateBottomScreenFrameCount != 0)
        return;

    Map *map = &allMaps[currentMap];

    // for each players
    for (int playerIndex = 1; playerIndex < MaxPlayer; playerIndex++)
    {
        Player *player = &AllPlayers[playerIndex];
        // Check if he is not dead, in game and if the camera is not on this player
        if (AllPlayers[playerIndex].Id != UNUSED && !AllPlayers[playerIndex].IsDead && CurrentCameraPlayer != playerIndex)
        {
            for (int i3 = 0; i3 < allMaps[currentMap].AllZones[AllPlayers[playerIndex].CurrentOcclusionZone].ZoneCount; i3++)
            {
                // If the player is in a visible map part
                if (checkZoneForOcclusion(&map->AllOcclusionZone[allMaps[currentMap].AllZones[AllPlayers[playerIndex].CurrentOcclusionZone].visibleMapPart[i3]], AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z))
                {
                    // Get if the player is in the field of view of the camera
                    bool inFov = PointInTriangleInt(AllPlayers[playerIndex].PlayerModel->x, AllPlayers[playerIndex].PlayerModel->z, AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z, t1x, t1z, t2x, t2z);

                    if (inFov)
                    {
                        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                        // Draw player's skin
                        NE_ModelDraw(AllPlayers[playerIndex].PlayerModel);

                        // Draw player's shadow
                        if (player->isAi || fabs(player->position.y - player->lerpDestination.y) < 0.05)
                        {
                            NE_PolyFormat(15, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                            NE_ModelDraw(AllPlayers[playerIndex].PlayerShadow);
                        }
                    }
                    break;
                }
            }
        }
    }
}

/**
 * @brief Draw game top screen when a game is not launched
 *
 */
void Draw3DSceneNotInGame(void)
{
    // Set camera for drawing
    NE_CameraUse(Camera);

    // Reset polygons Alpha/Light/Effect
    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);

    Map *map = &allMaps[currentMap];

    // Draw map
    if (currentMap == DUST2)
    {
        NE_ModelDraw(map->models[2].Model);
        NE_ModelDraw(map->models[3].Model);
    }
    else if (currentMap == TUTORIAL)
    {
        for (int i = 0; i < 2; i++)
        {
            // Set the model light like normal
            if (!map->models[i].shadowed)
            {
                GroundMaterial->diffuse = RGB15(0, 0, 0);
                GroundMaterial->emission = RGB15(11, 11, 11);
                GroundMaterial->specular = RGB15(7, 7, 7);
            }
            else // Set the model light like shadowed
            {
                GroundMaterial->diffuse = RGB15(1, 1, 1);
                GroundMaterial->emission = RGB15(3, 3, 3);
                GroundMaterial->specular = RGB15(3, 3, 3);
            }
            NE_ModelDraw(map->models[i].Model);
        }
    }

    NE_2DViewInit();
    // Draw keyboard input screen if needed
    if (isShowingKeyBoard)
    {
        drawKeyboardInput();
    }
}