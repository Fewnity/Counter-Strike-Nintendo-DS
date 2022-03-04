#ifndef PLAYER_H_ /* Include guard */
#define PLAYER_H_

#include <NEMain.h>
#include "main.h"

void playerUpdate();
void createPlayerShadow();
void removeAllPlayers();
void AddNewPlayer(int NewId, bool IsLocalPlayer, bool isAI);
void UpdatePlayerTexture(int playerIndex);
void setPlayerHealth(int playerIndex, int health);
void resetPlayer(int index);
void reducePlayerMoney(int playerIndex, int Money);
void addPlayerMoney(int playerIndex, int Money);
void setPlayerMoney(int playerIndex, int Money);
void addMoneyToTeam(int Money, enum teamEnum Team);
void setBombForARandomPlayer();

#endif // PLAYER_H_