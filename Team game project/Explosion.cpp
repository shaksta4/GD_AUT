#include "Explosion.h"
#include "tower.h"

Explosion::Explosion()
{
}


Explosion::~Explosion()
{
}

bool
Explosion::Initialise(AnimatedSprite* sprite, Tower* aTower)
{
	m_tower = aTower;
	m_x = aTower->GetPositionX();
	m_y = aTower->GetPositionY();

	return Entity::Initialise(sprite);
}

void
Explosion::Draw(BackBuffer& backBuffer)
{

	Entity::Draw(backBuffer);
}

void
Explosion::Process(float deltaTime)
{

	Entity::Process(deltaTime);
}