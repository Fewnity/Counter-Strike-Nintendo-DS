#ifndef GRENADE_H_ /* Include guard */
#define GRENADE_H_

#include <NEMain.h>
#include "main.h"

extern Grenade AllGrenades[shopGrenadeCount];

void AddGrenades(Grenade *grenades);
PhysicalGrenade *CreateGrenade(int Type);
void DeleteGrenade(int grenadeIndex);
void lanchGrenade(PhysicalGrenade *grenade, float xDirection, float yDirection, float zDirection, int xPosition, int yPosition, int zPosition);
void UpdateGrenades();

#endif // GRENADE_H_