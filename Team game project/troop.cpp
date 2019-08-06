/*
* Developed by Dominic 14868762
*/
#include "troop.h"
#include "entity.h"
#include "animatedsprite.h"

Troop::Troop()
: m_state(MOVING)
, m_currentTile(0)
, m_isHalted(false)
, m_isActing(false)
, m_isMoving(true)
, m_speed(100)
, m_health(1000)// ADDED BY SHAKEEL
{
	damage = 200;
}

Troop::~Troop()
{

}

void
Troop::Process(float deltaTime){
	//Moving(deltaTime); //<- Added by Michael
	//test
	//This method is used to be used in driven classes with overriding
	//There is no need to have content inside
	//If you are going to make a object, don't create one with this class
	//Using normaltroop, tanktroop or protectiontroop
	
	UpdateState();

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
}

void
Troop::UpdateState(){
	//The idea of this method is to contain the conditions which check if a certian state should take over. 
	if (m_isHalted)
	{
		m_state = Troop::HALTED;
	}
	else if (m_isActing)
	{
		m_state = Troop::ACTING;
	}
	else if (m_isMoving)
	{
		m_state = Troop::MOVING;
	}
}

void
Troop::Moving(float deltaTime)
{
	this->SetVerticalVelocity(m_speed);
	this->SetHorizontalVelocity(m_speed);
	this->SetFrameRow(0);
	
	if (m_pAnimatedSprite->IsPaused())
	{
		m_pAnimatedSprite->Pause();
	}

	int x = m_x + m_pAnimatedSprite->GetFrameWidth() / 2.0; //<- added by Michael
	int y = m_y + m_pAnimatedSprite->GetFrameHeight() / 2.0; //<- added by Michael
	if (x != t_x)
	{
		if (x > t_x && x + m_velocityX*deltaTime < t_x)
		{
			m_x = t_x - m_pAnimatedSprite->GetFrameWidth() / 2.0;
		}
		else if (m_x < t_x && m_x + m_velocityX*deltaTime > t_x)
		{
			m_x = t_x - m_pAnimatedSprite->GetFrameWidth() / 2.0;
		}
		else
		{
			m_x += m_velocityX * deltaTime;
		}
	}
	else if (y != t_y)
	{
		if (y > t_y && y + m_velocityY*deltaTime < t_y)
		{
			m_y = t_y - m_pAnimatedSprite->GetFrameHeight() / 2.0;
		}
		else if (y < t_y && y + m_velocityY*deltaTime > t_y)
		{
			m_y = t_y - m_pAnimatedSprite->GetFrameHeight() / 2.0;
		}
		else
		{
			if (y < t_y) m_y += m_velocityY * deltaTime;
			else if (y > t_y) m_y -= m_velocityY * deltaTime;
		}
	}
}

void
Troop::Halting(float deltaTime)
{
	if (!m_pAnimatedSprite->IsPaused())
	{
		m_pAnimatedSprite->Pause();
	}

	this->SetVerticalVelocity(0);
	this->SetHorizontalVelocity(0);
}

int
Troop::GetCurrentTile() const
{
	return m_currentTile;
}

void 
Troop::SetCurrentTile(int tile)
{
	m_currentTile = tile;
}

void
Troop::SetTarget(int x, int y, Map::select type){
	t_x = x;
	t_y = y;
	t_type = type;
}

void
Troop::GetTarget(int& x, int& y, Map::select& type)
{
	x = t_x;
	y = t_y;
	type = t_type;
}

void
Troop::SetHalted(bool isHalted)
{
	m_isHalted = isHalted;
	m_isActing = !isHalted;
	m_isMoving = !isHalted;
}

void 
Troop::SetActed(bool isActed){
	m_isHalted = !isActed;
	m_isActing = isActed;
	m_isMoving = !isActed;
}

void 
Troop::SetMoved(bool isMoved){
	m_isHalted = !isMoved;
	m_isActing = !isMoved;
	m_isMoving = isMoved;
}

int
Troop::GetHealth()
{
	return m_health;
}

void
Troop::SetHealth(float health)
{
	m_health = health;
}

int 
Troop::GetDamage(){
	return damage;
}

void 
Troop::SetDamage(float damage){
	this->damage = damage;
}
