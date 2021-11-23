#include "main.h"
#include "sounds.h"
#include "collisions.h"

#define HE_GRENADE_DAMAGE 98
#define MOLOTOV_DURATION 480 // 8sec * 60 frames
#define SMOKE_DURATION 1080  // 18sec * 60 frames
#define HE_GRENADE_TIMER 126 // 2.1sec * 60 frames
#define FLASH_TIMER 120      // 2sec * 60 frames

int cheapestGrenadeCost = 9999;
Grenade AllGrenades[shopGrenadeCount];

// MISSING :
// Flash duration
// fake duration
// HE damage
// Molotov damage

void AddGrenades(Grenade *grenades)
{
    int grenadeIndex = 0;

    grenades[grenadeIndex].type = 0;
    grenades[grenadeIndex].Price = 300;
    grenades[grenadeIndex].texture = (void *)fraggrenade_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_HEGRENADE_EXPLOSE;
    strncpy(grenades[grenadeIndex].name, "Explosive grenade", 20);
    strncpy(grenades[grenadeIndex].description, "The explosive grenade administers high damage", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 1;
    grenades[grenadeIndex].Price = 300;
    grenades[grenadeIndex].texture = (void *)smokegrenade_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE; // PAS BON?
    grenades[grenadeIndex].finalSound = SFX_SMOKE_EMIT;
    strncpy(grenades[grenadeIndex].name, "Smoke grenade", 20);
    strncpy(grenades[grenadeIndex].description, "The smoke grenade creates an area smoke screen.", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 2;
    grenades[grenadeIndex].Price = 200;
    grenades[grenadeIndex].texture = (void *)flashthrowed_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_FLASHBANG_EXPLODE;
    strncpy(grenades[grenadeIndex].name, "Flash", 20);
    strncpy(grenades[grenadeIndex].description, "The flashbang grenade temporarily blinds anybody", 50);
    grenadeIndex++;

    /*grenades[grenadeIndex].type = 0;
    grenades[grenadeIndex].Price = 400;
    grenades[grenadeIndex].texture = (void *)gunSprite26_bin;
    grenades[grenadeIndex].isForCounterTerrorists = 0;
    grenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE; //PAS BON
    grenades[grenadeIndex].finalSound = SFX_MOLOTOV_DETONATE;
    strncpy(grenades[grenadeIndex].name, "Molotov", 20);
    strncpy(grenades[grenadeIndex].description, "Used to burn an area", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 0;
    grenades[grenadeIndex].Price = 600;
    grenades[grenadeIndex].texture = (void *)gunSprite26_bin;
    grenades[grenadeIndex].isForCounterTerrorists = 1;
    grenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_MOLOTOV_DETONATE;
    strncpy(grenades[grenadeIndex].name, "Incendiary grenade", 20);
    strncpy(grenades[grenadeIndex].description, "Used to burn an area", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 1;
    grenades[grenadeIndex].Price = 50;
    grenades[grenadeIndex].texture = (void *)gunSprite28_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE; //PAS BON?
    grenades[grenadeIndex].finalSound = SFX_HEGRENADE_EXPLOSE;
    strncpy(grenades[grenadeIndex].name, "Decoy grenade", 20);
    strncpy(grenades[grenadeIndex].description, "the decoy grenade emulates the sound of your gun", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 2;
    grenades[grenadeIndex].Price = 200;
    grenades[grenadeIndex].texture = (void *)gunSprite28_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_FLASHBANG_EXPLODE;
    strncpy(grenades[grenadeIndex].name, "GSS", 20);
    strncpy(grenades[grenadeIndex].description, "The flashbang grenade temporarily blinds anybody", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 3;
    grenades[grenadeIndex].Price = 300;
    grenades[grenadeIndex].texture = (void *)gunSprite27_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_HEGRENADE_BOUNCE;
    grenades[grenadeIndex].finalSound = SFX_HEGRENADE_EXPLOSE;
    strncpy(grenades[grenadeIndex].name, "Explosive grenade", 20);
    strncpy(grenades[grenadeIndex].description, "The explosive grenade administers high damage", 50);
    grenadeIndex++;

    grenades[grenadeIndex].type = 4;
    grenades[grenadeIndex].Price = 300;
    grenades[grenadeIndex].texture = (void *)gunSprite28_bin;
    grenades[grenadeIndex].isForCounterTerrorists = -1;
    grenades[grenadeIndex].collisionSound = SFX_FLASHBANG_BOUNCE; //PAS BON?
    grenades[grenadeIndex].finalSound = SFX_SMOKE_EMIT;
    strncpy(grenades[grenadeIndex].name, "Smoke grenade", 20);
    strncpy(grenades[grenadeIndex].description, "The smoke grenade creates an area smoke screen.", 50);
    grenadeIndex++;*/

    for (int i = 0; i < shopGrenadeCount; i++)
    {
        if (grenades[i].Price < cheapestGrenadeCost)
        {
            cheapestGrenadeCost = grenades[i].Price;
        }
    }

    for (int i = 0; i < shopGrenadeCount; i++)
    {
        grenades[i].isForCounterTerrorists = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    }
}

PhysicalGrenade *CreateGrenade(int id)
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

            grenades[i]->EffectModel = NE_ModelCreate(NE_Static);
            NE_ModelLoadStaticMesh(grenades[i]->EffectModel, (u32 *)explosion_bin);
            NE_ModelSetMaterial(grenades[i]->EffectModel, GroundMaterial);
            NE_ModelScaleI(grenades[i]->EffectModel, 0, 0, 0);
            grenades[i]->GrenadeType = AllGrenades[id - GunCount].type;
            grenades[i]->isVisible = true;

            if (grenades[i]->GrenadeType == 0)
            {
                grenades[i]->Timer = 240;
                grenades[i]->EffectTimer = 30;
            }
            else if (grenades[i]->GrenadeType == 1)
            {
                grenades[i]->Timer = 480;
                grenades[i]->EffectTimer = 1080;
            }
            else if (grenades[i]->GrenadeType == 2)
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
            bool isOnStairs = CheckStairsForGrenades(grenades[i], StairsCount);
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
            if (grenades[i]->GrenadeType == 0)
            {
                int Scale = 1024 * (30 - grenades[i]->EffectTimer);
                NE_ModelScaleI(grenades[i]->EffectModel, Scale, Scale, Scale);
            }
            else if (grenades[i]->GrenadeType == 1)
            {
                int coef = grenades[i]->EffectTimer;
                int Scale = 0;
                int alpha = 31;
                if (coef <= 90)
                    alpha = coef / 3.0;
                if (coef < 1040) // or 30
                    coef = 1040;

                if (coef >= 1040)
                    Scale = 512 * (1080 - coef);

                /*if (coef <= 60)
                    alpha = coef / 2.0;
                if (coef < 1060 && coef > 20) //or 30
                    coef = 1060;

                if (coef >= 1060)
                    Scale = 1024 * (1080 - coef);
                else
                    Scale = 1024 * (coef);*/

                grenades[i]->effectAlpha = alpha;
                NE_ModelScaleI(grenades[i]->EffectModel, Scale, Scale, Scale);
            }
            else if (grenades[i]->GrenadeType == 2)
            {
                // FLASH
                // Check if the flash grenade is visible at screen
                float distance3D = sqrtf(powf(localPlayer->PlayerModel->x - grenades[i]->Model->x, 2.0) + powf(localPlayer->PlayerModel->y - grenades[i]->Model->y, 2.0) + powf(localPlayer->PlayerModel->z - grenades[i]->Model->z, 2.0)); // For vertical flash detection (Include player Y positon)
                float distance2D = sqrtf(powf(localPlayer->PlayerModel->x - grenades[i]->Model->x, 2.0) + powf(localPlayer->PlayerModel->z - grenades[i]->Model->z, 2.0));                                                                  // For horizontal flash detection

                // Get direction of grenade
                Vector3 Direction;
                Direction.x = grenades[i]->Model->x - localPlayer->PlayerModel->x;
                Direction.y = grenades[i]->Model->y - (localPlayer->PlayerModel->y + CameraOffsetY * 4096.0);
                Direction.z = grenades[i]->Model->z - localPlayer->PlayerModel->z;

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
                int xAngleFinalSideA = (int)(PlayerAngleToGrenadeDirection + (60.0 * horizontalExpDistance)) % 512;
                if (xAngleFinalSideA < 0)
                {
                    xAngleFinalSideA = 512 + xAngleFinalSideA;
                }

                int xAngleFinalSideB = (int)(PlayerAngleToGrenadeDirection + (60.0 * -horizontalExpDistance)) % 512;
                if (xAngleFinalSideB < 0)
                {
                    xAngleFinalSideB = 512 + xAngleFinalSideB;
                }

                // For vertical check
                int xAngleFinalVerticalSideA = (int)(CameraAngleToGrenadeDirection + (50.0 * verticalExpDistance)) % 512;
                int xAngleFinalVerticalSideB = (int)(CameraAngleToGrenadeDirection - (50.0 * verticalExpDistance)) % 512;

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
                flashed = isAtScreenHorizontalSideA && isAtScreenHorizontalSideB && isAtScreenVerticalSideA && isAtScreenVerticalSideB;
                flashaAnimation = 0;
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

            grenades[i]->EffectModel->rx = 0;
            grenades[i]->EffectModel->ry = 256;

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
