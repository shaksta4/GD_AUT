/*
* Developed by Dominic 14868762
*/
#ifndef __TROOP_H__
#define __TROOP_H__

#include "entity.h"
#include "map.h"//<- Added by Michael (can't be forward delcaration because an enum has to be referenced)

class Troop :public Entity{
public:
	enum State{MOVING, ACTING, HALTED};

	Troop();
	~Troop();
		
	void Process(float deltaTime);
	void UpdateState();
		
	//Moving and Halting should be the same for all troops.
	void Moving(float deltaTime); //<- Added by Michael// Changed by Shak
	void Halting(float deltaTime);

	//Acting is different for each troop.
	virtual void Acting(float deltaTime) = 0;

	void SetTarget(int x, int y, Map::select type); //<- Moved by Michael, From Normal Troop
	void GetTarget(int& x, int& y, Map::select& type); //<- Added by Michael
	int GetCurrentTile() const;
	void SetCurrentTile(int tile);
	void SetHalted(bool isHalted);
	void SetActed(bool isActed);
	void SetMoved(bool isMoved);
	int GetHealth();
	void SetHealth(float health);
	void SetDamage(float damage);
	virtual int GetDamage() = 0;

protected:
private:

public:
protected:
	State m_state;
	bool m_isHalted;
	bool m_isActing;
	bool m_isMoving;

	float m_health;
	int m_speed;
	int m_currentTile;
	int t_x; //<- Moved by Michael, From Normal Troop
	int t_y; //<- Moved by Michael, From Normal Troop
	Map::select t_type; //<- Added by Michael
	std::vector<Tower* > towers;
	int m_trooptype;
	float damage;

private:
};

#endif __TROOP_H__