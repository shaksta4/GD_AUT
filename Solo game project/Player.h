#pragma once
#include "entity.h"

class Sprite;

class Player : public Entity
{

	//Member methods
public:
	Player();
	~Player();

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

	void SetOriginPoints(float x, float y);
	

private:

protected:

	//Member data
public:

private:

protected:

	float m_angle; 
	float originPointX;
	float originPointY;

	float m_velocity;	
	
	float railRadius;
};

