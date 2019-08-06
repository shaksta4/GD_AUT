#pragma once
#include "entity.h"

class sprite;
class Player;

class Enemy : public Entity
{
	//Member methods
public:
	Enemy();
	~Enemy();

	void SetDead(bool dead);

	bool IsDead() const;

	bool IsCollidingWith(Entity& e);

	void Process(float deltaTime, BackBuffer& backBuffer);
	void Draw(BackBuffer& backBuffer);

	void SetPosition(float x, float y);

	float GetPositionX() const;
	float GetPositionY() const;

	float GetAngle() const;
	void SetAngle(float angle);

	int GetType() const;
	void SetType(int type);

	void SetTarget(Player* p1);

private:

protected:

	//Member data
public:

private:

protected:

	float m_angle;

	float m_targetPositionX;
	float m_targetPositionY;

	int enemyType;
};

