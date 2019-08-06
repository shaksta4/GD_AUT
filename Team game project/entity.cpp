// COMP710 GP 2D Framework 2017

// This include:
#include "entity.h"

// Local includes:
#include "animatedsprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>

Entity::Entity()
: m_pAnimatedSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
{

}

Entity::~Entity()
{

}

bool
Entity::Initialise(AnimatedSprite* sprite)
{
	assert(sprite);
	m_pAnimatedSprite = sprite;

	return (true);
}

void
Entity::Process(float deltaTime)
{
	m_pAnimatedSprite->Process(deltaTime);
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pAnimatedSprite);
	m_pAnimatedSprite->SetX(static_cast<int>(m_x));
	m_pAnimatedSprite->SetY(static_cast<int>(m_y));
	m_pAnimatedSprite->Draw(backBuffer);
}

//ADDED BY SHAKEEL. SIMPLE CIRCLE COLLISION DETECTION. PLEASE NOTE ITS ONLY CHECKING IF DISTANCE BETWEEN SPRITE IS LESS THAN OR EQUAL TO
// SPRITE WIDTH!!!!
bool
Entity::IsCollidingWith(Entity& e)
{
	int px = e.GetPositionX() - this->GetPositionX();
	int py = e.GetPositionY() - this->GetPositionY();
	int distance = std::sqrt((px*px) + (py*py));

	if (distance <= m_pAnimatedSprite->GetFrameWidth()) // <<<<<<<<<<<<<<<<<<<< THIS IS WHAT I MEAN
	{
		return true;
	}

	return (false);
}

bool
Entity::IsDead() const
{
	if (m_dead)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

void
Entity::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetPositionX() const
{
	return (m_x);
}

float 
Entity::GetPositionY() const
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity() const
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity() const
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}

void
Entity::SetFrameRow(int y){
	m_pAnimatedSprite->SetCurrentRow(y);
}

float
Entity::GetWidth()
{
	return m_pAnimatedSprite->GetFrameWidth();
}

float
Entity::GetHeight()
{
	return m_pAnimatedSprite->GetFrameHeight();
}

