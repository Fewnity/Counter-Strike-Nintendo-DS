#ifndef EQUIPMENT_H_ /* Include guard */
#define EQUIPMENT_H_

#include <NEMain.h>
#include "main.h"

#define equipementCount 4 // 4 (bomb, headset, kevlar, defuser)

enum EQUIPMENT_ENUM
{
    C4 = 0,
    DEFUSER = 1,
    KEVLAR_VEST = 2,
    KEVLAR_VEST_AND_HELMET = 3
};

typedef struct //
{
    void *texture;
    int Price;
    char name[20];
    char description[50];
    enum teamEnum team; // 0 for terrorist only, 1 for counter terrorists only, -1 for both teams
    bool isHided;
    enum EQUIPMENT_ENUM type;
} Equipement;

extern Equipement AllEquipements[equipementCount];

void LoadEquipments();

#endif // EQUIPMENT_H_