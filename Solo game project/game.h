// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

using namespace std;

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class Player;
class Enemy;
class Bullet;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void MoveSpaceShipLeft();
	void MoveSpaceShipRight();
	void StopPlayerMovement();
	void FireSpaceShipBullet();

	void SpawnEnemy(int x, int y, int type);
	void SpawnExplosion(int x, int y);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

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
	int gamestate;

	Player* Logo;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	// Game Entities:

	Player* myPlayer;
	Bullet* myBullet;

	vector<Enemy*> myEnemies;

private:

};

#endif // __GAME_H__
