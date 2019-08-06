#define _USE_MATH_DEFINES

//this include
#include "Player.h"

//local includes
#include "backbuffer.h"
#include "sprite.h"
#include "entity.h"
#include "logmanager.h"


//library includes 
#include <cassert>
#include <math.h>

Player::Player()
: m_velocity(0)
, m_angle(90)
, railRadius(350.0f)
{
}

Player::~Player()
{
}

void
Player::SetDead(bool dead)
{
	m_isDead = dead;
}

bool
Player::IsDead() const
{
	return (m_isDead);
}

bool
Player::IsCollidingWith(Entity& e)
{

	return (false); 
}

void
Player::Process(float deltaTime, BackBuffer& backBuffer)
{
	SetOriginPoints(backBuffer.GetWidth() / 2, backBuffer.GetHeight() / 2);
	float spriteAngle = 0;

	float dx = originPointX - m_positionX; 
	float dy = originPointY - m_positionY; 

	spriteAngle = atan2(dy, dx) *(180.0 / M_PI) + 90;
	this->SetAngle(spriteAngle);

	m_angle += (m_velocity*deltaTime) / (180 / M_PI);
	
	m_positionX = (originPointX + ( railRadius *cos(m_angle)));
	m_positionY = (originPointY + ( railRadius *sin(m_angle)));

}

void
Player::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);

	m_pSprite->SetX(static_cast<int>(m_positionX));
	m_pSprite->SetY(static_cast<int>(m_positionY));

	m_pSprite->Draw(backBuffer);
}

void
Player::SetPosition(float x, float y)
{
	m_positionX = x;
	m_positionY = y;
}

float
Player::GetPositionX() const
{
	return (m_positionX);
}

float
Player::GetPositionY() const
{
	return (m_positionY);
}

float
Player::GetVelocity() const
{
	return (m_velocity);
}

void
Player::SetVelocity(float x) 
{
	m_velocity = x;
}

void
Player::SetOriginPoints(float x, float y)
{
	originPointX = x;
	originPointY = y;
}

float 
Player::GetAngle() const
{
	return m_angle;
}

void
Player::SetAngle(float angle)
{
	m_pSprite->SetAngle(angle);
}