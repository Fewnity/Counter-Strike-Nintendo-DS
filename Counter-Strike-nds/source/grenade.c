// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "sounds.h"
#include "grenade.h"
#include "collisions.h"
#include "player.h"

#define HE_GRENADE_DAMAGE 98
#define MOLOTOV_DURATION 480 // 8sec * 60 frames
#define SMOKE_DURATION 1080  // 18sec * 60 frames
#define HE_GRENADE_TIMER 126 // 2.1sec * 60 frames
#define FLASH_TIMER 120      // 2sec * 60 frames

int cheapestGrenadeCost = 9999;
Grenade AllGrenades[shopGrenadeCount];

/**
 * @brief Load grenades data
 *
 * @param grenades
 */
void LoadGrenades(Grenade *grenades)
{
    int grenadeIndex = 0;

    grenades[grenadeIndex].type = EXPLOSIVE;
    grenades[grenadeIndex].Price = 300;
    grenades[grenadeIndex].texture = (void *)fraggrenade_bin;
    grenades[grenadeIndex].team = BOTH;
    grenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_HEGRENADE_EXPLOSE;
    grenades[grenadeIndex].maxQuantity[0] = 1;
    grenades[grenadeIndex].maxQuantity[1] = 1;
    grenades[grenadeIndex].maxQuantity[2] = 1;
    strncpy(grenades[grenadeIndex].name, "Explosive grenade", 20);
    strncpy(grenades[grenadeIndex].description, "The explosive grenade administers high damage", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = SMOKE;
    grenades[grenadeIndex].Price = 300;
    grenades[grenadeIndex].texture = (void *)smokegrenade_bin;
    grenades[grenadeIndex].team = BOTH;
    grenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE; // PAS BON?
    grenades[grenadeIndex].finalSound = SFX_SMOKE_EMIT;
    grenades[grenadeIndex].maxQuantity[0] = 1;
    grenades[grenadeIndex].maxQuantity[1] = 1;
    grenades[grenadeIndex].maxQuantity[2] = 1;
    strncpy(grenades[grenadeIndex].name, "Smoke grenade", 20);
    strncpy(grenades[grenadeIndex].description, "The smoke grenade creates an area smoke screen.", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = FLASH;
    grenades[grenadeIndex].Price = 200;
    grenades[grenadeIndex].texture = (void *)flashthrowed_bin;
    grenades[grenadeIndex].team = BOTH;
    grenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_FLASHBANG_EXPLODE;
    grenades[grenadeIndex].maxQuantity[0] = 2;
    grenades[grenadeIndex].maxQuantity[1] = 1;
    grenades[grenadeIndex].maxQuantity[2] = 1;
    strncpy(grenades[grenadeIndex].name, "Flash", 20);
    strncpy(grenades[grenadeIndex].description, "The flashbang grenade temporarily blinds anybody", 50);
    grenadeIndex++;

    for (int i = 0; i < shopGrenadeCount; i++)
    {
        if (grenades[i].Price < cheapestGrenadeCost)
        {
            cheapestGrenadeCost = grenades[i].Price;
        }
    }

    // for (int i = 0; i < shopGrenadeCount; i++)
    // {
    //     grenades[i].team = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    // }
}

/**
 * @brief Create a grenade
 *
 * @param id Grenade type
 * @param ownerId Player id who sent the grenade
 * @return PhysicalGrenade* Pointer of the new grenade (null if not created)
 */
PhysicalGrenade *CreateGrenade(int id, int ownerId)
{
    for (int i = 0; i < GrenadeCount; i++)
    {
        // If a slot is free
        if (grenades[i] == NULL)
        {
            // Alloc memory for the new grenade
            grenades[i] = malloc(sizeof(PhysicalGrenade));

            PhysicalGrenade *grenade = grenades[i];

            // Create the model and apply material
            grenade->Model = NE_ModelCreate(NE_Static);
            NE_ModelLoadStaticMesh(grenade->Model, (u32 *)grenade_3ds_bin);
            NE_ModelSetMaterial(grenade->Model, GroundMaterial);

            // Set scale and rotation
            NE_ModelScaleI(grenade->Model, 8, 8, 8);
            grenade->Model->rx = 128;
            grenade->Model->ry = 256;

            // Enable physics
            grenade->Physic = NE_PhysicsCreate(NE_BoundingBox);
            NE_PhysicsSetModel(grenade->Physic, (void *)grenade->Model);
            NE_PhysicsEnable(grenade->Physic, true);
            NE_PhysicsSetGravity(grenade->Physic, 0.0065);
            NE_PhysicsSetSize(grenade->Physic, 0.5, 0.5, 0.5);
            grenade->Physic->physicsgroup[0] = 1;
            grenade->Physic->physicsgroupCount = 1;
            NE_PhysicsSetFriction(grenade->Physic, 3);
            NE_PhysicsOnCollision(grenade->Physic, NE_ColBounce);
            NE_PhysicsSetBounceEnergy(grenade->Physic, 30);

            grenade->GrenadeType = id;

            // Set default values
            grenade->LastStairs = 0;
            grenade->isVisible = true;
            grenade->ownerId = ownerId;
            grenade->lastCollisionTimer = 0;
            grenade->effectAlpha = 30;

            if (grenade->GrenadeType == EXPLOSIVE) // If the grenade is an explosive grenade
            {
                // Create the explosion model and apply material
                grenade->EffectModel = NE_ModelCreate(NE_Static);
                NE_ModelLoadStaticMesh(grenade->EffectModel, (u32 *)explosion_bin);
                NE_ModelSetMaterial(grenade->EffectModel, GroundMaterial);

                // Set scale and rotation
                NE_ModelScaleI(grenade->EffectModel, 0, 0, 0);
                grenade->EffectModel->rx = 0;
                grenade->EffectModel->ry = 256;

                // Set grenade values
                grenade->Timer = 240;      // 4s * 60 frames
                grenade->EffectTimer = 30; // 0,5s * 60 frames
            }
            else if (grenade->GrenadeType == SMOKE) // If the grenade is a smoke grenade
            {
                // Create the explosion model and apply material
                grenade->EffectModel = NE_ModelCreate(NE_Static);
                NE_ModelLoadStaticMesh(grenade->EffectModel, (u32 *)smokeSphere_bin);
                NE_ModelSetMaterial(grenade->EffectModel, GroundMaterial);

                // Set scale and rotation
                NE_ModelScaleI(grenade->EffectModel, 0, 0, 0);
                grenade->EffectModel->rx = -128;

                // Set grenade values
                grenade->Timer = 480;
                grenade->EffectTimer = 1080;
            }
            else if (grenade->GrenadeType == FLASH) // If the grenade is a flash grenade
            {
                // Set grenade values
                grenade->Timer = 480;
                grenade->EffectTimer = 1;
            }

            // Return the new grenade
            return grenade;
        }
    }
    // The grenade array is full
    return NULL;
}

/**
 * @brief Launch a grenade
 *
 * @param grenade Grenade pointer
 * @param xDirection X direction of the grenade (INT value)
 * @param yDirection Y direction of the grenade (INT value)
 * @param zDirection Z direction of the grenade (INT value)
 * @param xPosition X position of the grenade (INT value)
 * @param yPosition Y position of the grenade (INT value)
 * @param zPosition Z position of the grenade (INT value)
 */
void lanchGrenade(PhysicalGrenade *grenade, float xDirection, float yDirection, float zDirection, int xPosition, int yPosition, int zPosition)
{
    grenade->Physic->xspeed = xDirection * 2200;
    grenade->Physic->yspeed = yDirection * 2500;
    grenade->Physic->zspeed = zDirection * 2200;

    grenade->Model->x = xPosition + xDirection * 4096;
    grenade->Model->y = yPosition + 0.7 * 4096 + yDirection * 4096;
    grenade->Model->z = zPosition + zDirection * 4096;
}

/**
 * @brief Delete all spawned grenades
 *
 */
void DeleteAllGrenade()
{
    for (int i = 0; i < GrenadeCount; i++)
    {
        DeleteGrenade(i);
    }
}

/**
 * @brief Delete a spawned grenade
 *
 * @param grenadeIndex Grenade index
 */
void DeleteGrenade(int grenadeIndex)
{
    PhysicalGrenade *grenade = grenades[grenadeIndex];
    if (grenade != NULL)
    {
        // Delete the model
        NE_ModelDelete(grenade->Model);
        grenade->Model = NULL;
        // Delete the physics component
        NE_PhysicsDelete(grenade->Physic);
        grenade->Physic = NULL;

        // Delete the effect model
        if (grenade->EffectModel != NULL && (grenade->GrenadeType == EXPLOSIVE || grenade->GrenadeType == SMOKE))
        {
            NE_ModelDelete(grenade->EffectModel);
            grenade->EffectModel = NULL;
        }

        // Free the grenade
        free(grenades[grenadeIndex]);
        grenades[grenadeIndex] = NULL;
    }
}

/**
 * @brief Update all spawned grenades
 *
 */
void UpdateGrenades()
{
    for (int i = 0; i < GrenadeCount; i++)
    {
        PhysicalGrenade *grenade = grenades[i];

        if (grenade == NULL)
            continue;

        // If the physics component is enabled
        if (grenade->Physic->enabled)
        {
            bool isOnStairs = CheckStairsForGrenades(grenade);
            int totalSpeed = abs(grenade->Physic->xspeed) + abs(grenade->Physic->yspeed) + abs(grenade->Physic->zspeed);
            if (totalSpeed == 0 && !isOnStairs) // If the grenade is not moving and is not on stairs
            {
                // Disable the physics component
                NE_PhysicsEnable(grenade->Physic, false);
                // Start Smoke or flash effect
                if (grenade->GrenadeType == SMOKE || grenade->GrenadeType == FLASH)
                {
                    grenade->Timer = 1;
                }
            }
            else
            {
                // Update the physics component
                NE_PhysicsUpdate(grenade->Physic);

                // If the grenade collide with a wall
                if (grenade->Physic->iscollidingTrigger && grenade->lastCollisionTimer == 0)
                {
                    // Do a collision sound
                    grenade->lastCollisionTimer = 10;
                    PlayBasicSound(AllGrenades[0].collisionSound);
                }
                else if (grenade->lastCollisionTimer > 0)
                    grenade->lastCollisionTimer--;
            }

            // Rotate the grenade
            totalSpeed /= 15.0;
            if (totalSpeed >= 6)
                totalSpeed = 6;

            grenade->Model->rx += totalSpeed;
            grenade->Model->ry += totalSpeed;
        }

        // If the timer is 0
        if (grenade->Timer == 0)
        {
            if (grenade->GrenadeType == EXPLOSIVE)
            {
                // Do an explosion

                // Set explosion scale
                int Scale = 1024 * (30 - grenade->EffectTimer);
                NE_ModelScaleI(grenade->EffectModel, Scale, Scale, Scale);

                grenade->effectAlpha = grenade->EffectTimer;

                // At the beginning of the explosion
                if (grenade->EffectTimer == 30)
                {
                    for (int playerIndex = 0; playerIndex < MaxPlayer; playerIndex++)
                    {
                        Player *player = &AllPlayers[playerIndex];
                        if (player->IsDead || player->Id == NO_PLAYER)
                            continue;

                        // Get the distance between the grenade and the player
                        float Distance = (float)sqrt(pow(player->PlayerModel->x - grenade->Model->x, 2.0) + pow(player->PlayerModel->y - grenade->Model->y, 2.0) + pow(player->PlayerModel->z - grenade->Model->z, 2.0)) / 8096.0;

                        // Set a maximum distance
                        if (Distance > 4)
                            Distance = 0;

                        if (Distance > 0)
                        {
                            // Apply damage
                            int newHealh = player->Health - (int)map(Distance, 0.3, 4, 100, 0);
                            setPlayerHealth(playerIndex, newHealh);
                            checkAfterDamage(NO_PLAYER, playerIndex, false);
                        }
                    }
                }
            }
            else if (grenade->GrenadeType == SMOKE)
            {
                // Do a smoke effect
                int coef = grenade->EffectTimer;
                int Scale = 0;
                int alpha = 31;
                if (coef <= 90)
                    alpha = coef / 3.0;
                coef = 1040;

                if (coef >= 1040)
                    Scale = 5 * (1080 - coef);

                // Set effect alpha and scale
                grenade->effectAlpha = alpha;
                NE_ModelScaleI(grenade->EffectModel, Scale, Scale, Scale);
            }
            else if (grenade->GrenadeType == FLASH)
            {
                // Do a flash effect
                for (int playerIndex = 0; playerIndex < MaxPlayer; playerIndex++)
                {
                    Player *player = &AllPlayers[playerIndex];
                    if (player->IsDead || player->Id == NO_PLAYER)
                        continue;

                    // Check if the flash grenade is visible at screen (the code may be difficult to understand, but it works (but not very well ahah))

                    // Get the distance between the grenade and the player
                    float distance3D = sqrtf(powf(player->PlayerModel->x - grenade->Model->x, 2.0) + powf(player->PlayerModel->y - grenade->Model->y, 2.0) + powf(player->PlayerModel->z - grenade->Model->z, 2.0)); // For vertical flash detection (Include player Y positon)
                    float distance2D = sqrtf(powf(player->PlayerModel->x - grenade->Model->x, 2.0) + powf(player->PlayerModel->z - grenade->Model->z, 2.0));                                                         // For horizontal flash detection

                    // Get direction of grenade
                    Vector3 Direction;
                    Direction.x = grenade->Model->x - player->PlayerModel->x;
                    Direction.y = grenade->Model->y - (player->PlayerModel->y + CameraOffsetY * 4096.0);
                    Direction.z = grenade->Model->z - player->PlayerModel->z;

                    // Calculate angle direction (camera & player angles to have the grenade at the center of the screen)
                    float PlayerAngleToGrenadeDirection = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0;
                    float CameraAngleToGrenadeDirection = atan2f(distance2D, Direction.y) * 512.0 / (M_TWOPI);

                    // Use exponential to increase/decrease angle offset more quickly when the player is close to the grenade
                    float horizontalExpDistance = exp(4000.0 / distance2D);
                    if (horizontalExpDistance > 8.5333)
                        horizontalExpDistance = 8.5333;

                    float verticalExpDistance = exp(4000.0 / distance3D);
                    if (verticalExpDistance > 8.5333)
                        verticalExpDistance = 8.5333;

                    // Add offsets from center angles and fix angles (limit angles from 0 to 512 max)
                    // For horizontal check
                    float fixedHorizontalExpDistance = 60.0 * horizontalExpDistance;
                    int xAngleFinalSideA = (int)(PlayerAngleToGrenadeDirection + fixedHorizontalExpDistance) % 512;
                    if (xAngleFinalSideA < 0)
                    {
                        xAngleFinalSideA = 512 + xAngleFinalSideA;
                    }

                    int xAngleFinalSideB = (int)(PlayerAngleToGrenadeDirection + fixedHorizontalExpDistance) % 512;
                    if (xAngleFinalSideB < 0)
                    {
                        xAngleFinalSideB = 512 + xAngleFinalSideB;
                    }

                    // For vertical check
                    float fixedVerticalExpDistance = 50.0 * verticalExpDistance;
                    int xAngleFinalVerticalSideA = (int)(CameraAngleToGrenadeDirection + fixedVerticalExpDistance) % 512;
                    int xAngleFinalVerticalSideB = (int)(CameraAngleToGrenadeDirection - fixedVerticalExpDistance) % 512;

                    // Fix player angle (limit the angle from 0 to 512 max)
                    int fixePlayerAngle = (int)AllPlayers[CurrentCameraPlayer].Angle % 512; // TODO MAYBE USE player->Angle
                    if (fixePlayerAngle < 0)
                        fixePlayerAngle = 512 + fixePlayerAngle;

                    // Temp var
                    bool isAtScreenHorizontalSideA = false;
                    bool isAtScreenHorizontalSideB = false;
                    bool isAtScreenVerticalSideA = false;
                    bool isAtScreenVerticalSideB = false;

                    // Check top and bottom side with camera rotation
                    if (xAngleFinalVerticalSideA >= CameraAngleY)
                        isAtScreenVerticalSideA = true;

                    if (xAngleFinalVerticalSideB <= CameraAngleY)
                        isAtScreenVerticalSideB = true;

                    if (horizontalExpDistance < 4)
                    {
                        // Check left and right side with fixed player rotation
                        if (xAngleFinalSideA > xAngleFinalSideB)
                        {
                            if (xAngleFinalSideA >= fixePlayerAngle)
                                isAtScreenHorizontalSideA = true;

                            if (xAngleFinalSideB <= fixePlayerAngle)
                                isAtScreenHorizontalSideB = true;
                        }
                        else // Change conditions if values AngleB is greater than AngleA
                        {
                            if (xAngleFinalSideA <= fixePlayerAngle)
                                isAtScreenHorizontalSideA = true;

                            if (xAngleFinalSideB <= fixePlayerAngle)
                                isAtScreenHorizontalSideB = true;

                            // If in this case both are false, the player is looking at the grenade (horizontal)
                            if (!isAtScreenHorizontalSideA && !isAtScreenHorizontalSideB)
                            {
                                isAtScreenHorizontalSideA = true;
                                isAtScreenHorizontalSideB = true;
                            }
                        }
                    }
                    else
                    {
                        isAtScreenHorizontalSideA = true;
                        isAtScreenHorizontalSideB = true;
                    }
                    Vector3Int playerCameraPos = {(int)player->PlayerModel->x, (int)player->PlayerModel->y + CameraOffsetYMultiplied, (int)player->PlayerModel->z};
                    Vector3Int grenadeCameraPos = {(int)grenade->Model->x, (int)grenade->Model->y, (int)grenade->Model->z};

                    float x2 = 0, y2 = 0, z2 = 0;
                    getValuesForRaycast(playerCameraPos, grenadeCameraPos, &x2, &y2, &z2);
                    setRaycastValues(player, x2, y2, z2);
                    player->ScanForGrenade = i;
                    float hitDistance = 0;
                    int foundGrenade = Raycast(playerIndex, 0, &hitDistance);
                    player->ScanForGrenade = EMPTY;

                    // Merge all check variables into one
                    player->flashed = isAtScreenHorizontalSideA && isAtScreenHorizontalSideB && isAtScreenVerticalSideA && isAtScreenVerticalSideB && foundGrenade != EMPTY;
                    player->flashAnimation = 0;
                }
            }

            grenade->EffectTimer--;

            // Destroy the grenade at the end of the effect
            if (grenade->EffectTimer == 0)
            {
                DeleteGrenade(i);
            }
        }
        else if (grenade->Timer == 1)
        {
            // Play the effect sound
            PlayBasicSound(AllGrenades[grenade->GrenadeType].finalSound);

            // Set effect model positon
            if (grenade->GrenadeType == SMOKE || grenade->GrenadeType == EXPLOSIVE)
            {
                grenade->EffectModel->x = grenade->Model->x;
                grenade->EffectModel->y = grenade->Model->y;
                grenade->EffectModel->z = grenade->Model->z;
            }

            grenade->Timer--;
            // Hide the grenade
            grenade->isVisible = false;
        }
        else
        {
            grenade->Timer--;
        }
    }
}
