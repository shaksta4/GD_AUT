#include "Tower.h"

/* Shakeel Khan - 1486426
*  11/10/2017 Created
*  16/10/2017 Added functionality to init
			  Added projectile init to Tower's init. This is because
			  I assume each tower type would initialise a different projectile
			  Added logic to selecting target UNTESTED
			  */

#include "sprite.h"
#include "troop.h"
#include "backbuffer.h"
#include "Projectile.h"
#include "animatedsprite.h"
#include "logmanager.h"
#include "Explosion.h"

#include <cmath>
#include <vector>

Tower::Tower():
m_health(0)
, m_type(0)
, m_radius(0)
, delay(1.1f)
, m_pTarget(NULL)
, m_explosion(0)
{
}

Tower::~Tower()
{
	m_pTarget = 0;

	std::vector<Projectile* >::iterator iter = projectiles.begin();
	while (iter != projectiles.end())
	{
		Projectile* temp = (*iter);
		iter = projectiles.erase(iter);
		delete temp;
	}
}

bool
Tower::Initialise(AnimatedSprite* sprite, TowerType myType)
{
	Entity::Initialise(sprite);
	SetType(myType);
	m_radius = 500;

	switch (myType)
	{
	case WOODEN: { m_health = 10000; break; }
	case STONE: { m_health = 25000; break; }
	case VILLAGE: { m_health = 50000; break; }
	case CASTLE: { m_health = 100000; break; }
	}

	return true;
}

void
Tower::SelectTarget(Troop* target)
{
	//m_pTarget = target;
	//IF THE DISTANCE BETWEEN THE TROOP AND TOWER IS LESS THAN RADIUS, SET IT AS TARGET
	int px = target->GetPositionX() - this->GetPositionX();
	int py = target->GetPositionY() - this->GetPositionY();
	int distance = std::sqrt((px*px) + (py*py));

	if (distance <= m_radius)
	{
		m_pTarget = target; //<- Altered by Michael
	}
	else
	{
		m_pTarget = NULL;
	}
}

void
Tower::Draw(BackBuffer& backBuffer)
{
	std::vector<Projectile* >::iterator iter = projectiles.begin();
	while (iter != projectiles.end())
	{
		(*iter)->Draw(backBuffer);
		iter++;
	}

	if (m_dead)
	{
		if(m_explosion == 0) PlayDeathAnimation(backBuffer);
		m_explosion->Draw(backBuffer);
	}

	Entity::Draw(backBuffer);
}

//Creates projectile sprite, adds it into the projectiles vector, and initialises the projectile.
bool
Tower::FireProj(BackBuffer& backBuffer)
{
	if (delay > 1 && m_pTarget != NULL)
	{
		m_rockSprite = backBuffer.CreateAnimatedSprite("assets\\rockprojectile.png");
		m_rockSprite->SetFrameWidth(40);
		m_rockSprite->SetFrameHeight(40);
		m_rockSprite->SetFrameSpeed(0.25f);

		for (int i = 0; i < 159; i += 40)
		{
			m_rockSprite->AddFrame(i);
		}

		m_arrowSprite = backBuffer.CreateAnimatedSprite("assets\\arrowprojectile.png");
		m_arrowSprite->SetFrameWidth(70);
		m_arrowSprite->SetFrameHeight(8);
		m_arrowSprite->SetFrameSpeed(0.25f);
		m_arrowSprite->AddFrame(0);

		std::vector<Projectile* >::iterator iter = projectiles.begin();

		iter = projectiles.insert(iter, new Projectile());

		switch (m_type)
		{
		case WOODEN:
		{

					   (*iter)->Initialise(this, m_rockSprite);
					   (*iter)->SetTarget(m_pTarget);
					   (*iter)->SetDamage(40); break;
		}
		case STONE:
		{
					   (*iter)->Initialise(this, m_arrowSprite);
					   (*iter)->SetTarget(m_pTarget);
					   (*iter)->SetDamage(65); break;
		}
		case VILLAGE:
		{

					   (*iter)->Initialise(this, m_arrowSprite);
					   (*iter)->SetTarget(m_pTarget);
					   (*iter)->SetDamage(95); break;
		}
		case CASTLE:
		{
					   (*iter)->Initialise(this, m_arrowSprite);
					   (*iter)->SetTarget(m_pTarget);
					   (*iter)->SetDamage(150); break;
		}
		}

		delay = 0;
	}
	return true;
}

void
Tower::Process(float deltaTime)
{
	delay += deltaTime;

	//Changed to process all projectiles: ~ Michael Jordan.
	//iter is the first projectile, and iter++ goes to the next projectile etc.
	std::vector<Projectile* >::iterator iter = projectiles.begin();

	while (iter != projectiles.end())
	{
		if (!(*iter)->IsDead())
		{
			(*iter)->Process(deltaTime);
			(*iter)->CheckCollision();
			iter++;
		}
		else
		{
			Projectile* temp = (*iter);
			iter = projectiles.erase(iter);
			delete temp;
		}
	}

	if (m_dead)
	{
		m_pTarget = NULL;
		if (m_explosion != 0) m_explosion->Process(deltaTime);
	}

	if (m_health <= 0)
	{
		m_dead = true;
	}
}

void 
Tower::PlayDeathAnimation(BackBuffer& backBuffer)
{
	if (m_dead)
	{
		m_towerExplosionSprite = backBuffer.CreateAnimatedSprite("assets\\explosion.png");
		
		m_towerExplosionSprite->SetFrameWidth(64);
		m_towerExplosionSprite->SetFrameHeight(64);
		m_towerExplosionSprite->SetFrameSpeed(0.03f);

		m_towerExplosionSprite->SetX(this->GetPositionX());
		m_towerExplosionSprite->SetY(this->GetPositionY());

		for (int i = 0; i < 319; i += 64)
		{
			m_towerExplosionSprite->AddFrame(i);
		}

		m_explosion = new Explosion();
		m_explosion->Initialise(m_towerExplosionSprite, this);
		m_explosion->Draw(backBuffer);

		if (this->GetType() == WOODEN || this->GetType() == STONE)
		{
			if (this->GetType() == WOODEN)
			{
				deadTowerSprite = backBuffer.CreateAnimatedSprite("assets\\woodentowerbroken.png");
			}

			else if (this->GetType() == STONE)
			{
				deadTowerSprite = backBuffer.CreateAnimatedSprite("assets\\stonetowerbroken.png");
			}

			deadTowerSprite->SetFrameWidth(200);
			deadTowerSprite->SetFrameHeight(200);
		}

		else if (this->GetType() == VILLAGE || this->GetType() == CASTLE)
		{
			if (this->GetType() == VILLAGE)
			{
				deadTowerSprite = backBuffer.CreateAnimatedSprite("assets\\villagebroken.png");
			}
			else if (this->GetType() == CASTLE)
			{
				//PLACEHOLDER
				deadTowerSprite = backBuffer.CreateAnimatedSprite("assets\\tower.png");
			}

			deadTowerSprite->SetFrameWidth(300);
			deadTowerSprite->SetFrameHeight(300);
		}

		deadTowerSprite->SetFrameSpeed(1.0f / 5.0f);
		deadTowerSprite->AddFrame(0);

		this->SetSprite(deadTowerSprite);
	}
}

void 
Tower::SetSprite(AnimatedSprite* mySprite)
{
	m_pAnimatedSprite = mySprite;
}

int
Tower::GetHealth()
{
	return m_health;
}

void 
Tower::SetHealth(int health)
{
	m_health = health;
}

int
Tower::GetType()
{
	return m_type;
}

void
Tower::SetType(TowerType myType)
{
	m_type = myType;
}



