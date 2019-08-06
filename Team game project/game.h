// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

#include "screen.h"
#include <vector>  //<- Added by Michael

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class Map;
class Projectile;
class Troop;
class Tower;
class TroopFactory;
class Mask;
class Button;

class Game : public Screen
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	virtual bool Initialise(BackBuffer* backBuffer);
	virtual bool DoGameLoop();
	virtual void Quit();
	virtual void Refresh();

	virtual Map* GetMap();
	virtual void SetMap(Map* map);
	
	void ChangeDirection(int x);
	void PlaySound();

	//Debug Methods:
	void DebugRemakeMap();
	void StopAllTroops();
	void StartTroops();

	void checkChangeStatesOfTroops();
	void iniWaitingTroops();
	void addNormalTroopToActingTroops();
	void addTankTroopToActingTroops();
	void addProtectionTroopToActingTroops();
	void setAllTroopSpeed(int verticalVelocity, int horizontalVelocity);
	void setAllTroopDamage(float damage);

	Mask* GetButtonMask() const;

	void iniActingTroop();
protected:
	virtual void Process(float deltaTime);
	virtual void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	std::vector<Troop* > waitingTroops; //<- Altered by Michael
	std::vector<Troop* > usingTroops;  //<- Altered by Michael
	std::vector<Troop* > actingTroops;//<- Altered by Michael
	Troop* m_pNormalTroop1;
	//TroopFactory* troopFactory;

	Map* m_pMap;
	Troop* m_pNormalTroop;
	std::vector<Tower*> m_Towers; //ADDED FOR TESTING Shakeel and Altered by Michael
	Projectile* m_pRock; //ADDED FOR TESTING Jack
	Mask* m_pButtonMask;
	std::vector<Button*> m_Buttons;
	Tower* m_pVillage;
private:

};

#endif // __GAME_H__
