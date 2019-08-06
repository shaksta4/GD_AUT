
/* Shakeel Khan - 1486426
*  11/10/2017 Created
   16/10/2017 Added functionality to CreateTower*/

#include "TowerFactory.h"
#include "Tower.h"
#include "animatedsprite.h"
#include "sprite.h"
#include "backbuffer.h"

Tower*
TowerFactory::CreateTower(AnimatedSprite* sprite, Tower::TowerType type)
{
	Tower* tower = new Tower();
	tower->Initialise(sprite, type);
	return tower;
}
