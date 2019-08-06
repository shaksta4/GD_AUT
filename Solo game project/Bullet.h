#pragma once
#include "entity.h"

class sprite;

class Bullet : public Entity
{

public:
	Bullet();
	~Bullet();

	void SetDead(bool dead);

	bool IsDead() const;

	bool IsCollidingWith(Entity& e);

	void Process(float deltaTime, BackBuffer& backBuffer);
	void Draw(BackBuffer& backBuffer);

	void SetPosition(float x, float y);

	float GetPositionX() const;
	float GetPositionY() const;

	float GetVelocity() const;
	void SetVelocity(float x);

	float GetAngle() const;
	void SetAngle(float angle);

	void SetAttachedTo(Entity* myEntity);

private:

protected:

public:

private:

protected:

	float m_angle;
	Entity* attachedTo;
	float m_velocity;

};

