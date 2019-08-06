#pragma once

/* Shakeel Khan - 14864276
* 16/10/2017 Created
*			 Added member methods and fields*/

/*
This is a class which models the projectile for the towers. It overloads Entity's process
and uses LERP to move the bullet to the target's position. 

It uses initialise from the Entity class for its sprite (which is called in Tower.

It has Get/Set methods for damage values and splash etc. 

CAN ADD ELEMENTS LATER IF NEEDED

*/

#include "entity.h"

//Forward declarations
class AnimatedSprite;
class BackBuffer;
class Troop;
class Tower;

class Projectile : public Entity
{

	//Class functions
public:
	Projectile();
	~Projectile();

	bool Initialise(Tower* tower, AnimatedSprite* sprite);
	void Process(float deltaTime);
	int GetDamage();

	void CheckCollision();
	void Explode();
	void SetTarget(Troop* p_target);
	void SetDamage(int damage);
	void SetSplash(bool splash);
	void SetAngle(int angle);

	int lerp(float start, float end, float time);

private:

protected:

	// Member data
public:

private:

	int m_damage;
	bool splash;
	//AnimatedSprite* sprite;
	Troop* p_target;
	Tower* p_tower;
	float attackspeed;
	float projradius;
	int m_angle;


protected:

};

