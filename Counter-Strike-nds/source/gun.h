#ifndef GUN_H_ /* Include guard */
#define GUN_H_

#include <NEMain.h>
#include "main.h"

extern int cheapestGunsCostCounter[4];
extern int cheapestGunsCostTerrorists[4];
extern int gunCountCounter[4];
extern int gunCountTerrorists[4];
extern Gun AllGuns[GunCount];
extern Equipement AllEquipements[equipementCount];

void AddGuns();
void ReloadGun(int playerId);
void SetAim();
void ResetGunAmmo(int playerId, int inventoryIndex);
void ResetGunsAmmo(int playerId);
void findGun(int gunCategory, int playerIndex);

#endif // GUN_H_