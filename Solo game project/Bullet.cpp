#define _USE_MATH_DEFINES

#include "Bullet.h"

//local includes
#include "backbuffer.h"
#include "sprite.h"
#include "entity.h"
#include "logmanager.h"
#include "Player.h"

#include <cassert>
#include <math.h>


Bullet::Bullet()
: m_velocity(0)
{
}


Bullet::~Bullet()
{
}

void
Bullet::SetDead(bool dead)
{
	m_isDead = dead;
}

bool
Bullet::IsDead() const
{
	return (m_isDead);
}


bool
Bullet::IsCollidingWith(Entity& e)
{

	return false;
}

void
Bullet::Process(float deltaTime, BackBuffer& backBuffer)
{
	float originPointX = static_cast<float> (backBuffer.GetWidth()/2);
	float originPointY = static_cast<float> (backBuffer.GetHeight()/2);

	float dx = (originPointX - m_positionX);
	float dy = (originPointY - m_positionY);

	float direction = sqrt(dx*dx + dy*dy);

	m_positionX += (m_velocity*deltaTime) * dx/direction;
	m_positionY += (m_velocity*deltaTime) * dy/direction;


}


void
Bullet::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);

	m_pSprite->SetX(static_cast<int>(m_positionX));
	m_pSprite->SetY(static_cast<int>(m_positionY));

	m_pSprite->Draw(backBuffer);
}

void
Bullet::SetPosition(float x, float y)
{
	m_positionX = x;
	m_positionY = y;
}

float
Bullet::GetPositionX() const
{
	return m_positionX;
}

float
Bullet::GetPositionY() const
{
	return m_positionY;
}

float
Bullet::GetVelocity() const
{
	return (m_velocity);
}

void
Bullet::SetVelocity(float x)
{
	m_velocity = x;
}

float
Bullet::GetAngle() const
{
	return m_angle;
}

void
Bullet::SetAngle(float angle)
{
	m_pSprite->SetAngle(angle);
}

void
Bullet::SetAttachedTo(Entity* myEntity)
{
	attachedTo = myEntity;
	this->SetPosition(myEntity->GetPositionX(), myEntity->GetPositionY());
}

