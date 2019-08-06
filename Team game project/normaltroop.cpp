/*
* Developed by Dominic 14868762

31/10/2017 Changed code to make more specialised while, troop being more general
Removed redundant code that wasnt being called (in .h file)
*/
#include "animatedsprite.h"
#include "troop.h"
#include "normaltroop.h"
#include <cstdlib>
#include <ctime>
#include <cassert>

NormalTroop::NormalTroop()
: m_damage(25)
{
	m_health = 100;
}

NormalTroop::~NormalTroop()
{	
}

bool
NormalTroop::Initialise(AnimatedSprite* sprite)
{
	return Entity::Initialise(sprite);
}

//MOVED TO TROOP
/*
void
NormalTroop::Moving(float deltaTime)
{
}
*/

void
NormalTroop::iniAnimation()
{
	this->m_pAnimatedSprite->AddFrame(4);
	this->m_pAnimatedSprite->SetFrameWidth(91);
	this->m_pAnimatedSprite->SetHeight(82);
	this->m_pAnimatedSprite->SetWidth(91);
	this->m_pAnimatedSprite->SetLooping(true);
}

/*
void
NormalTroop::SetTarget(int x, int y)
{
	t_x = x;
	t_y = y;
}
*/

/*Removed by Michael
void
NormalTroop::Process(float deltaTime){
	switch (m_state){
	case MOVING:
		Moving(deltaTime);
		break;
	case ACTING:
		Acting(deltaTime);
		break;
	case HALTED:
		Halting(deltaTime);
		break;
	}
	Entity::Process(deltaTime);
}*/

void
NormalTroop::Acting(float deltaTime)
{
	this->SetFrameRow(200);
}

int
NormalTroop::GetDamage()
{
	return m_damage;
}

//Should be in Troop? 
/*
void
NormalTroop::Halting(float deltaTime)
{
	m_animatedSprite->SetLooping(false);
	this->SetVerticalVelocity(0);
	this->SetHorizontalVelocity(0);
}
*/
