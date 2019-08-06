// COMP710 GP 2D Framework 2017

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, gamestate(1)
{
	
}

Game::~Game()
{
	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	const int width = 800;
	const int height = 800;

	m_pBackBuffer = new BackBuffer();
	m_pBackBuffer->SetClearColour(0, 0, 0);
	m_pBackBuffer->Clear();

	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}


	Sprite* AutLogo = m_pBackBuffer->CreateSprite("assets\\AUTLOGOPNG.png");
	Logo = new Player();
	Logo->Initialise(AutLogo);
	Logo->SetPosition(400, 100);

	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	Sprite* pBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");

	myPlayer = new Player();
	myPlayer->Initialise(pPlayerSprite);

	myBullet = new Bullet();
	myBullet->Initialise(pBulletSprite);

	SpawnEnemy(200, 200, 2);
	SpawnEnemy(300, 200, 1);
	SpawnEnemy(200, 300, 2);
	SpawnEnemy(200, 400, 1);

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}
		
		Draw(*m_pBackBuffer);
	}

//	SDL_Delay(1);

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	if (m_elapsedSeconds > 1)
	{
		gamestate = 2;
	}

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:

	for (Enemy* e : myEnemies)
	{
		e->SetTarget(myPlayer);
	}

	myPlayer->Process(deltaTime, *m_pBackBuffer);
	myBullet->Process(deltaTime, *m_pBackBuffer);

}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	if (gamestate == 1)
	{
		Logo->Draw(backBuffer);
	}

	if (gamestate == 2)
	{
		for (int i = 0; i < myEnemies.size(); i++)
		{
			myEnemies.at(i)->Draw(backBuffer);
		}

		myBullet->Draw(backBuffer);

		myPlayer->Draw(backBuffer);
	}

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void 
Game::MoveSpaceShipLeft()
{
	myPlayer->SetVelocity(-50);
}

void
Game::MoveSpaceShipRight()
{
	myPlayer->SetVelocity(50);
}

void
Game::StopPlayerMovement()
{
	myPlayer->SetVelocity(0);
}

void 
Game::FireSpaceShipBullet()
{
	myBullet->SetAttachedTo(myPlayer);
	myBullet->SetVelocity(200);
	}

void 
Game::SpawnEnemy(int x, int y, int type)
{

	Sprite* pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	Sprite* pEnemyTWOSprite = m_pBackBuffer->CreateSprite("assets\\aliengoon.png");

	Enemy* newEnemy = new Enemy();
	newEnemy->SetType(type);

	if (newEnemy->GetType() == 1)
	{
		newEnemy->Initialise(pEnemySprite);
	}
	else if (newEnemy->GetType() == 2)
	{
		newEnemy->Initialise(pEnemyTWOSprite);
	}
	else
	{
		LogManager::GetInstance().Log("Unsupported type!!");
	}

	newEnemy->SetPosition(x, y);

	myEnemies.push_back(newEnemy);
}

void 
Game::SpawnExplosion(int x, int y)
{
	Sprite* pExplosion = m_pBackBuffer->CreateSprite("assets\\explosion.png");
}
