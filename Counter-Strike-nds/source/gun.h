#ifndef GUN_H_ /* Include guard */
#define GUN_H_

#include <NEMain.h>
#include "main.h"

typedef struct //
{
    int fireRate;
    void *texture;
    int DamageFalloff;
    int Damage;
    float penetration;
    bool holdFireButton;
    int Price;
    // char name[20];
    const char *name;
    bool isKnife;
    mm_word gunSound;
    int WalkSpeed;
    int scopeId;
    int gunCategory;
    int isForCounterTerrorists; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    int MagazineCapacity;
    int MaxAmmoCount;
    int ReloadTime;
    float MaxSoundDistance;
    int xMuzzleFlashOffset;
    int yMuzzleFlashOffset;
    bool isBigGun;
    bool isDualGun;
} Gun;

extern int cheapestGunsCostCounter[4];
extern int cheapestGunsCostTerrorists[4];
extern int gunCountCounter[4];
extern int gunCountTerrorists[4];
extern Gun AllGuns[GunCount];
extern Equipement AllEquipements[equipementCount];

void AddGuns();
void ReloadGun(int playerId);
void SetAim();
void DisableAim();
void ResetGunAmmo(int playerId, int inventoryIndex);
void ResetGunsAmmo(int playerId);
void findGun(int gunCategory, int playerIndex);
Gun getPlayerCurrentGun(Player *player);
int getPlayerCurrentGunIndex(Player *player);
void setGunRecoil(Player *player);

#endif // GUN_H_