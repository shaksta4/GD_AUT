#define _USE_MATH_DEFINES

#include "Enemy.h"

//local includes
#include "backbuffer.h"
#include "sprite.h"
#include "entity.h"
#include "logmanager.h"
#include "Player.h"

//library includes 
#include <cassert>
#include <math.h>


Enemy::Enemy()
: m_angle(0)
, enemyType(0)
{

}


Enemy::~Enemy()
{
}

void 
Enemy::SetDead(bool dead)
{
	m_isDead = dead;
}

bool
Enemy::IsDead() const
{
	return (m_isDead);
}


bool 
Enemy::IsCollidingWith(Entity& e)
{

	return false;
}

void 
Enemy::Process(float deltaTime, BackBuffer& backBuffer)
{

}

void
Enemy::SetTarget(Player* p1)
{
	float spriteAngle = 0;
	
	float dx = p1->GetPositionX()- m_positionX;
	float dy = p1->GetPositionY()- m_positionY;

	spriteAngle = atan2(dy, dx) * (180.0f / static_cast<float>(M_PI)) + 90.0f;

	this->SetAngle(spriteAngle);
}

void 
Enemy::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);

	m_pSprite->SetX(static_cast<int>(m_positionX));
	m_pSprite->SetY(static_cast<int>(m_positionY));

	m_pSprite->Draw(backBuffer);
}

void 
Enemy::SetPosition(float x, float y)
{
	m_positionX = x;
	m_positionY = y;
}

float 
Enemy::GetPositionX() const
{
	return m_positionX;
}

float 
Enemy::GetPositionY() const
{
	return m_positionY;
}

float 
Enemy::GetAngle() const
{
	return m_angle;
}

void 
Enemy::SetAngle(float angle)
{
	m_pSprite->SetAngle(angle);
}

int
Enemy::GetType() const
{
	return enemyType;
}

void
Enemy::SetType(int type)
{
	enemyType = type;
}
