#include "main.h"
#include "equipment.h"

int cheapestEquipmentCost = 9999;
Equipement AllEquipements[equipementCount];

/**
 * @brief Load equipments data
 * Data source : https://counterstrike.fandom.com/wiki/Category:Equipment
 * @param equipement
 */
void LoadEquipments(Equipement *equipement)
{
    int equipementIndex = 0;

    equipement[equipementIndex].type = C4;
    equipement[equipementIndex].Price = 0;
    equipement[equipementIndex].texture = (void *)c4_bin;
    equipement[equipementIndex].team = TERRORISTS;
    equipement[equipementIndex].isHided = true;
    strncpy(equipement[equipementIndex].name, "", 20);
    strncpy(equipement[equipementIndex].description, "", 50);
    equipementIndex++;

    equipement[equipementIndex].type = DEFUSER;
    equipement[equipementIndex].Price = 400;
    equipement[equipementIndex].texture = (void *)defuser_bin;
    equipement[equipementIndex].team = COUNTERTERRORISTS;
    strncpy(equipement[equipementIndex].name, "Defuse kit", 20);
    strncpy(equipement[equipementIndex].description, "Decrease the defuse time of bombs by half", 50);
    equipementIndex++;

    equipement[equipementIndex].type = KEVLAR_VEST;
    equipement[equipementIndex].Price = 650;
    equipement[equipementIndex].texture = (void *)kevlar_bin;
    equipement[equipementIndex].team = BOTH;
    strncpy(equipement[equipementIndex].name, "Kevlar vest", 20);
    strncpy(equipement[equipementIndex].description, "The Kevlar Vest protects the chest", 50);
    equipementIndex++;

    equipement[equipementIndex].type = KEVLAR_VEST_AND_HELMET;
    equipement[equipementIndex].Price = 1000;
    equipement[equipementIndex].texture = (void *)kevlar_helmet_bin;
    equipement[equipementIndex].team = BOTH;
    strncpy(equipement[equipementIndex].name, "Kevlar + helmet", 20);
    strncpy(equipement[equipementIndex].description, "Protects the chest and the head", 50);
    equipementIndex++;

    // for (int i = 0; i < equipementCount; i++)
    // {
    //     equipement[i].team = -1; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    // }
}
