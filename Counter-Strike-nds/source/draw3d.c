#include "main.h"
#include "grenade.h"
#include "collisions.h"
#include "ui.h"
#include "map.h"
#include "draw3d.h"
#include "debug.h"
#include "party.h"

int t1x = 0;
int t1z = 0;
int t2x = 0;
int t2z = 0;

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
                // char Occlusion[20];
                // sprintf(Occlusion, "Zone to show : %d", AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone);
                // debugPrint(Occlusion);

                // char visibleMapPart[20];
                // sprintf(visibleMapPart, "visibleMapPart : %d", map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]);
                // debugPrint(visibleMapPart);

                // char shadowed[20];
                // sprintf(shadowed, "shadowed : %d", map->models[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]].shadowed);
                // debugPrint(shadowed);

                if (!map->models[map->AllZones[AllPlayers[CurrentCameraPlayer].CurrentOcclusionZone].visibleMapPart[i]].shadowed)
                {
                    GroundMaterial->diffuse = RGB15(0, 0, 0);
                    GroundMaterial->emission = RGB15(11, 11, 11);
                    GroundMaterial->specular = RGB15(7, 7, 7);
                    // NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                }
                else
                {
                    GroundMaterial->diffuse = RGB15(1, 1, 1);
                    GroundMaterial->emission = RGB15(3, 3, 3);
                    GroundMaterial->specular = RGB15(3, 3, 3);
                    // NE_PolyFormat(31, 0, NE_LIGHT_1, NE_CULL_BACK, NE_MODULATION);
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
                // Draw grenade
                if (grenades[i]->isVisible)
                {
                    // Grenade clipping
                    bool inFov = PointInTriangleInt(grenades[i]->Model->x, grenades[i]->Model->z, AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z, t1x, t1z, t2x, t2z);

                    if (inFov)
                        NE_ModelDraw(grenades[i]->Model);
                }
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
        if (ShowWallHitFlash > 0)
        {
            ShowWallHitFlash--;
            NE_ModelDraw(Model[8]);
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

            // Reset polygons Alpha/Light/Effect
            NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
        }

        DrawPlayers();
    }

    // if (UpdateBottomScreenOneFrame != 0)
    // return;

    // Draw UI
    drawTopScreenUI();
}

/**
 * @brief Draws players
 *
 */
void DrawPlayers()
{
    if (UpdateBottomScreenOneFrame != 0)
        return;

    Map *map = &allMaps[currentMap];

    // for each players
    for (int playerIndex = 1; playerIndex < MaxPlayer; playerIndex++)
    {
        // Check if he is not dead, in game and if the camera is not on this player
        if (AllPlayers[playerIndex].Id != UNUSED && !AllPlayers[playerIndex].IsDead && CurrentCameraPlayer != playerIndex)
        {
            for (int i3 = 0; i3 < allMaps[currentMap].AllZones[AllPlayers[playerIndex].CurrentOcclusionZone].ZoneCount; i3++)
            {
                // If the player is in a visible map part
                if (checkZoneForOcclusion(&map->AllOcclusionZone[allMaps[currentMap].AllZones[AllPlayers[playerIndex].CurrentOcclusionZone].visibleMapPart[i3]], AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z))
                {
                    // If the player is in the field of view of the camera, draw ot
                    bool inFov = PointInTriangleInt(AllPlayers[playerIndex].PlayerModel->x, AllPlayers[playerIndex].PlayerModel->z, AllPlayers[CurrentCameraPlayer].PlayerModel->x, AllPlayers[CurrentCameraPlayer].PlayerModel->z, t1x, t1z, t2x, t2z);

                    if (inFov)
                    {
                        // Draw player's skin
                        NE_ModelDraw(AllPlayers[playerIndex].PlayerModel);
                        // Draw player's shadow
                        NE_PolyFormat(15, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
                        NE_ModelDraw(AllPlayers[playerIndex].PlayerShadow);
                        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_MODULATION);
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
        NE_ModelDraw(map->models[0].Model);
        NE_ModelDraw(map->models[1].Model);
    }

    NE_2DViewInit();
    if (isShowingKeyBoard)
    {
        drawNameChanging();
    }

    // drawTopScreenUI();
}