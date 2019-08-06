// COMP710 GP 2D Framework 2017
#ifndef __ENTITY_H__
#define __ENTITY_H__

// Forward Declarations:
class BackBuffer;
class Sprite;

class Entity
{
	//Member Methods:
public:
	Entity();
	virtual ~Entity();

	virtual bool Initialise(Sprite* sprite);

	virtual void Process(float deltaTime, BackBuffer& backBuffer) = 0;
	virtual void Draw(BackBuffer& backBuffer) = 0;

	virtual void SetPosition(float x, float y) = 0;

	virtual float GetPositionX() const = 0;
	virtual float GetPositionY() const = 0;

	virtual float GetAngle() const = 0;
	virtual void SetAngle(float angle) = 0;

	virtual bool IsCollidingWith(Entity& e) = 0;

protected:
	
private:

	//Member Data:
public:

protected:
	Sprite* m_pSprite;

	float m_positionX;
	float m_positionY;

	bool m_isDead;

	float m_angle;

private:

};

#endif //__ENTITY_H__
