#pragma once

/* Shakeel Khan - 1486426
*  11/10/2017 Created */


//Local includes
#include "Tower.h"

//Forward Declarations
class animatedsprite;
class BackBuffer;

class TowerFactory
{

	// Class methods
public:
	static Tower* CreateTower(AnimatedSprite* sprite, Tower::TowerType type);

private: 

protected:

	// Member Data

public:

private:

protected:


};

