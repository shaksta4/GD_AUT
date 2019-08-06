/*
* 16/10/2017 Created
* 17/10/2017 Added Init which initialises the projectile with its respective tower. 
			 It sets the projectiles position as the position of the tower. GOTTA FIX LATER
			 The process uses LERP to interpolate the projectile to the troop target. UNIMPLEMENTED
*/

#define _USE_MATH_DEFINES

#include "Projectile.h"
#include "Tower.h"
#include "troop.h"
#include "logmanager.h"
#include "animatedsprite.h"
#include "math.h"

#include <cstdio>
#include <cstring>
#include <string>

Projectile::Projectile():
m_damage(0),
splash(false),
p_target(0),
p_tower(0),
attackspeed(0),
projradius(10)
{

}


Projectile::~Projectile()
{

}

bool
Projectile::Initialise(Tower* tower, AnimatedSprite* sprite)
{
	Entity::Initialise(sprite);
	p_tower = tower;
	m_x = p_tower->GetPositionX()+(p_tower->GetWidth()-sprite->GetFrameWidth())/2.0f;
	m_y = p_tower->GetPositionY() + (p_tower->GetHeight() - sprite->GetFrameHeight()) / 2.0f;
	m_dead = false;

	return true;
}


void 
Projectile::SetTarget(Troop* target)
{
	p_target = target;
}

void
Projectile::SetDamage(int damage)
{
	m_damage = damage;
}

void
Projectile::Process(float deltaTime)
{	
	//How long it takes for projectile to reach target
	float movementTime = 10.0f;

	attackspeed += deltaTime;
	float lerpFraction = attackspeed / movementTime;

	////MY PROJECTILE LOCATIONS
	float startX = m_x; // p_tower->GetPositionX();
	float startY = m_y; // p_tower->GetPositionY();

    //TARGET LOCATIONS
	float targetX = p_target->GetPositionX();
	float targetY = p_target->GetPositionY();

	m_x = lerp(startX, targetX, lerpFraction);
	m_y = lerp(startY, targetY, lerpFraction);

	//----------------------ROTATION-------------
	float spriteAngle = 0;

	float dx = targetX - m_x;
	float dy = targetY - m_y;

	spriteAngle = atan2(dy, dx) *(180.0 / M_PI) + 180;
	this->SetAngle(spriteAngle);

	Entity::Process(deltaTime);
}

int
Projectile::lerp(float start, float end, float time)
{
	return (((1 - time) * start) + (time * end));
}

void
Projectile::CheckCollision()
{
	if (this->IsCollidingWith(*p_target))
	{
		LogManager::GetInstance().Log("DESTRUCTION!!!!!");
		Explode();
	}
}

void
Projectile::Explode()
{
	p_target->SetHealth(p_target->GetHealth() - m_damage);
	m_dead = true;
}

void
Projectile::SetAngle(int angle)
{
	m_pAnimatedSprite->SetAngle(angle);
}

