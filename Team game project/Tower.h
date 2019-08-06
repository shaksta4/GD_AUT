#pragma once

/* Shakeel Khan - 1486426
*  11/10/2017 created
   16/10/2017 Added projectile as a member, and projectile sprite
			  Added backbuffer as another parameter to init. Used for setting projectile 
			  sprite.

   17/10/2017 Added logic for selecting a target.
			  Added projectile as a class member. 
			  FireProj draws the projectile for the time being.

			  */

#include "entity.h"

//Forward declarations
class AnimatedSprite;
class sprite;
class Troop; 
class BackBuffer;
class Projectile;
class Explosion;

#include <vector>

class Tower : public Entity
{

//Class functions
public:
	Tower();
	~Tower();

	static enum TowerType{ WOODEN, STONE, VILLAGE, CASTLE };

	bool Initialise(AnimatedSprite* sprite, TowerType myType); //Altered by Michael

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer); //Added by Michael;
	void SelectTarget(Troop* target); // Checks if enemies are within radius
	bool FireProj(BackBuffer& backBuffer); // Shoots projectile to troop in environment

	void PlayDeathAnimation(BackBuffer& backBuffer);

	int GetHealth();
	void SetHealth(int health);

	int GetType();
	void SetType(TowerType myType);

	void SetSprite(AnimatedSprite* mySprite);
 
private:

protected:

// Class member data
public:

private:

protected:

	int m_health;
	int m_type;
	int m_radius;
	std::vector<Projectile* > projectiles; //Altered by Michael
	Troop* m_pTarget;
	AnimatedSprite* m_rockSprite;
	AnimatedSprite* m_arrowSprite;
	AnimatedSprite* m_towerExplosionSprite;
	AnimatedSprite* deadTowerSprite;

	Explosion* m_explosion;
	float delay;
};

