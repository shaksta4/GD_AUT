/*
* Developed by Dominic 14868762
*/
#ifndef __NORMALTROOP_H__
#define __NORMALTROOP_H__

#include "troop.h"

class AnimatedSprite;

class NormalTroop : public Troop{
public:
	NormalTroop();
	~NormalTroop();
	bool Initialise(AnimatedSprite* sprite); //<- Added by Michael 
	void Acting(float deltaTime);
	void iniAnimation();

	int GetDamage();

	//void Process(float deltaTime); <- Removed by Michael
	//Redundant code
	//void SetTarget(int x, int y);
	//void DrawAnimatedSprite(BackBuffer& backBuffer);
	//void setAnimatedSprite(AnimatedSprite* m_animatedSprite);
	//AnimatedSprite* getAnimatedSprite();
protected:
private:

public:

protected:

private:

	int m_damage;
};

#endif __NORMALTROOP_H__