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

PhysicalGrenade *CreateGrenade(int id, int ownerId)
{
    for (int i = 0; i < GrenadeCount; i++)
    {
        if (grenades[i] == NULL)
        {
            grenades[i] = malloc(sizeof(PhysicalGrenade));
            grenades[i]->LastStairs = 0;
            grenades[i]->Model = NE_ModelCreate(NE_Static);
            grenades[i]->Physic = NE_PhysicsCreate(NE_BoundingBox);
            NE_PhysicsSetModel(grenades[i]->Physic, (void *)grenades[i]->Model);

            NE_ModelSetMaterial(grenades[i]->Model, GroundMaterial);
            NE_ModelScaleI(grenades[i]->Model, 8, 8, 8);
            NE_ModelSetCoord(grenades[i]->Model, 0, 3 + i * 2, -4);
            grenades[i]->Model->rx = 128;
            grenades[i]->Model->ry = 256;
            NE_ModelLoadStaticMesh(grenades[i]->Model, (u32 *)grenade_3ds_bin);
            NE_PhysicsEnable(grenades[i]->Physic, true);
            NE_PhysicsSetGravity(grenades[i]->Physic, 0.0065);
            NE_PhysicsSetSize(grenades[i]->Physic, 0.5, 0.5, 0.5);
            grenades[i]->Physic->physicsgroup[0] = 1;
            grenades[i]->Physic->physicsgroupCount = 1;
            NE_PhysicsSetFriction(grenades[i]->Physic, 3);
            NE_PhysicsOnCollision(grenades[i]->Physic, NE_ColBounce);
            NE_PhysicsSetBounceEnergy(grenades[i]->Physic, 30);

            grenades[i]->GrenadeType = AllGrenades[id - GunCount].type;
            grenades[i]->isVisible = true;
            grenades[i]->ownerId = ownerId;

            if (grenades[i]->GrenadeType == EXPLOSIVE)
            {
                grenades[i]->EffectModel = NE_ModelCreate(NE_Static);
                NE_ModelLoadStaticMesh(grenades[i]->EffectModel, (u32 *)explosion_bin);
                NE_ModelSetMaterial(grenades[i]->EffectModel, GroundMaterial);
                NE_ModelScaleI(grenades[i]->EffectModel, 0, 0, 0);
                grenades[i]->EffectModel->rx = 0;
                grenades[i]->EffectModel->ry = 256;
                grenades[i]->Timer = 240;
                grenades[i]->EffectTimer = 30;
            }
            else if (grenades[i]->GrenadeType == SMOKE)
            {
                grenades[i]->EffectModel = NE_ModelCreate(NE_Static);
                NE_ModelLoadStaticMesh(grenades[i]->EffectModel, (u32 *)smokeSphere_bin);
                NE_ModelSetMaterial(grenades[i]->EffectModel, GroundMaterial);
                NE_ModelScaleI(grenades[i]->EffectModel, 0, 0, 0);
                grenades[i]->EffectModel->rx = 128;
                grenades[i]->Timer = 480;
                grenades[i]->EffectTimer = 1080;
            }
            else if (grenades[i]->GrenadeType == FLASH)
            {
                grenades[i]->Timer = 480;
                grenades[i]->EffectTimer = 1;
            }

            return grenades[i];
        }
    }
    return NULL;
}

void lanchGrenade(PhysicalGrenade *grenade, float xDirection, float yDirection, float zDirection, int xPosition, int yPosition, int zPosition)
{
    grenade->Physic->xspeed = xDirection * 2200;
    grenade->Physic->yspeed = yDirection * 2500;
    grenade->Physic->zspeed = zDirection * 2200;

    grenade->Model->x = xPosition + xDirection * 4096;
    grenade->Model->y = yPosition + 0.7 * 4096 + yDirection * 4096;
    grenade->Model->z = zPosition + zDirection * 4096;
}

void DeleteGrenade(int grenadeIndex)
{
    if (grenades[grenadeIndex] != NULL)
    {
        NE_ModelDelete(grenades[grenadeIndex]->Model);
        NE_PhysicsDelete(grenades[grenadeIndex]->Physic);

        if (grenades[grenadeIndex]->EffectModel != NULL)
            NE_ModelDelete(grenades[grenadeIndex]->EffectModel);

        free(grenades[grenadeIndex]);
        grenades[grenadeIndex] = NULL;
    }
}

void UpdateGrenades()
{
    for (int i = 0; i < GrenadeCount; i++)
    {
        if (grenades[i] == NULL)
            continue;

        if (grenades[i]->Physic->enabled)
        {
            bool isOnStairs = CheckStairsForGrenades(grenades[i]);
            int totalSpeed = grenades[i]->Physic->xspeed + grenades[i]->Physic->yspeed + grenades[i]->Physic->zspeed;
            if (totalSpeed == 0 && !isOnStairs)
            {
                NE_PhysicsEnable(grenades[i]->Physic, false);
                if (grenades[i]->GrenadeType == 1 || grenades[i]->GrenadeType == 2)
                {
                    grenades[i]->Timer = 1;
                }
            }
            else
            {
                NE_PhysicsUpdate(grenades[i]->Physic);

                if (grenades[i]->Physic->iscollidingTrigger && grenades[i]->lastCollisionTimer == 0)
                {
                    grenades[i]->lastCollisionTimer = 10;
                    PlayBasicSound(AllGrenades[0].collisionSound);
                }
                else if (grenades[i]->lastCollisionTimer > 0)
                    grenades[i]->lastCollisionTimer--;
            }
            grenades[i]->Model->rx += 2;
            grenades[i]->Model->ry += 2;
        }

        if (grenades[i]->Timer == 0)
        {
            if (grenades[i]->GrenadeType == EXPLOSIVE)
            {
                int Scale = 1024 * (30 - grenades[i]->EffectTimer);
                NE_ModelScaleI(grenades[i]->EffectModel, Scale, Scale, Scale);
                grenades[i]->effectAlpha = grenades[i]->EffectTimer;

                // Apply damage
                if (grenades[i]->EffectTimer == 30)
                {
                    for (int i = 0; i < MaxPlayer; i++)
                    {
                        Player *player = &AllPlayers[i];
                        if (player->IsDead || player->Id == NO_PLAYER)
                            continue;
                        float Distance = (float)sqrt(pow(player->PlayerModel->x - grenades[i]->Model->x, 2.0) + pow(player->PlayerModel->y - grenades[i]->Model->y, 2.0) + pow(player->PlayerModel->z - grenades[i]->Model->z, 2.0)) / 8096.0;
                        if (Distance > 4)
                            Distance = 0;

                        if (Distance > 0)
                        {
                            int newHealh = player->Health - (int)map(Distance, 0.3, 4, 100, 0);
                            setPlayerHealth(i, newHealh);
                            checkAfterDamage(NO_PLAYER, i, false);
                        }
                    }
                }
            }
            else if (grenades[i]->GrenadeType == SMOKE)
            {
                int coef = grenades[i]->EffectTimer;
                int Scale = 0;
                int alpha = 31;
                if (coef <= 90)
                    alpha = coef / 3.0;
                if (coef < 1040) // or 30
                    coef = 1040;

                if (coef >= 1040)
                    // Scale = 512 * (1080 - coef);
                    Scale = 5 * (1080 - coef);

                // Alpha 0 is wireframe mode
                if (alpha == 0)
                    alpha = 1;

                grenades[i]->effectAlpha = alpha;
                NE_ModelScaleI(grenades[i]->EffectModel, Scale, Scale, Scale);
            }
            else if (grenades[i]->GrenadeType == FLASH)
            {
                // for (int i = 0; i < -1; i++)
                for (int i = 0; i < MaxPlayer; i++)
                {
                    Player *player = &AllPlayers[i];
                    if (player->IsDead || player->Id == NO_PLAYER)
                        continue;
                    // Check if the flash grenade is visible at screen
                    float distance3D = sqrtf(powf(player->PlayerModel->x - grenades[i]->Model->x, 2.0) + powf(player->PlayerModel->y - grenades[i]->Model->y, 2.0) + powf(player->PlayerModel->z - grenades[i]->Model->z, 2.0)); // For vertical flash detection (Include player Y positon)
                    float distance2D = sqrtf(powf(player->PlayerModel->x - grenades[i]->Model->x, 2.0) + powf(player->PlayerModel->z - grenades[i]->Model->z, 2.0));                                                             // For horizontal flash detection

                    // Get direction of grenade
                    Vector3 Direction;
                    Direction.x = grenades[i]->Model->x - player->PlayerModel->x;
                    Direction.y = grenades[i]->Model->y - (player->PlayerModel->y + CameraOffsetY * 4096.0);
                    Direction.z = grenades[i]->Model->z - player->PlayerModel->z;

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
                    int fixePlayerAngle = (int)localPlayer->Angle % 512;
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

                    // Merge all check variables into one
                    player->flashed = isAtScreenHorizontalSideA && isAtScreenHorizontalSideB && isAtScreenVerticalSideA && isAtScreenVerticalSideB;
                    player->flashAnimation = 0;
                }
            }

            grenades[i]->EffectTimer--;

            if (grenades[i]->EffectTimer == 0)
                DeleteGrenade(i);
        }
        else if (grenades[i]->Timer == 1)
        {
            // A REACTIVER
            PlayBasicSound(AllGrenades[grenades[i]->GrenadeType].finalSound);
            grenades[i]->EffectModel->x = grenades[i]->Model->x;
            grenades[i]->EffectModel->y = grenades[i]->Model->y;
            grenades[i]->EffectModel->z = grenades[i]->Model->z;

            // grenades[i]->EffectModel->rx = 0;
            // grenades[i]->EffectModel->ry = 256;

            grenades[i]->Timer--;
            grenades[i]->isVisible = false;
        }
        else
        {
            //
            grenades[i]->Timer--; // A REACTIVER
        }
    }
}
