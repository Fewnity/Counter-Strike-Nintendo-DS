#include "main.h"
#include "equipment.h"

int cheapestEquipmentCost = 9999;

void AddEquipment(Equipement *equipement)
{
    int equipementIndex = 0;

    //equipement[equipementIndex].type = 0;
    equipement[equipementIndex].Price = 0;
    equipement[equipementIndex].texture = (void *)c4_bin;
    equipement[equipementIndex].isForCounterTerrorists = -1;
    equipement[equipementIndex].usageSound = SFX_HEGRENADE_BOUNCE;
    strncpy(equipement[equipementIndex].name, "", 20);
    strncpy(equipement[equipementIndex].description, "", 50);
    equipementIndex++;

    for (int i = 0; i < equipementCount; i++)
    {
        equipement[i].isForCounterTerrorists = -1; //0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    }
}
