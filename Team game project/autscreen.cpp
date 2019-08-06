/*
* Developed by Michael Jordan 14868336
*/
// Local includes:
#include "autscreen.h"
#include "backbuffer.h"
#include "logmanager.h"
#include "sprite.h"
#include "IniParser.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>

// Static Members:
AutScreen* AutScreen::sm_pInstance = 0;

AutScreen&
AutScreen::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new AutScreen();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
AutScreen::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

AutScreen::AutScreen()
	: m_pBackBuffer(0)
	, m_looping(true)
	, m_pLogo(0)
{

}

AutScreen::~AutScreen()
{
	//Note: Back buffer is used in game and main menu.
	delete m_pLogo;
	m_pLogo = 0;

}

BackBuffer*
AutScreen::GetBackBuffer() 
{
	return m_pBackBuffer;
}

bool 
AutScreen::Initialise(BackBuffer* backBuffer)
{
	Refresh();
	
	int m_width = backBuffer->GetWidth();
	int m_height = backBuffer->GetHeight();
	
	assert(backBuffer);
	m_pBackBuffer = backBuffer;

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	m_pLogo = m_pBackBuffer->CreateSprite("assets\\AUTLogo.png");
	m_pLogo->SetWidth(m_pLogo->GetWidth() / 2.0);
	m_pLogo->SetHeight(m_pLogo->GetHeight() / 2.0);
	int x = (m_width - m_pLogo->GetWidth() * backBuffer->GetScaleX()) / 2.0;
	int y = (m_height - m_pLogo->GetHeight() * backBuffer->GetScaleY()) / 2.0;
	m_pLogo->SetX(x);
	m_pLogo->SetY(y);
	return (true);
}

bool 
AutScreen::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;
	
	if (m_executionTime > 2)
	{
		Quit();
	}
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
	return (m_looping);
}

void
AutScreen::Quit()
{
	m_looping = false;
	m_currentState = POP;
}

void 
AutScreen::Process(float deltaTime)
{
	m_elapsedSeconds += deltaTime;

	if (m_elapsedSeconds > 1) 
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

}

void 
AutScreen::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	m_pLogo->Draw(backBuffer);

	backBuffer.Present();
}

void
AutScreen::Refresh()
{
	m_currentState = DISPLAY;
	m_elapsedSeconds = 0;
	m_lag = 0;
	m_executionTime = 0;
	m_lastTime = 0;
	m_frameCount = 0;
	m_FPS = 0;
	m_numUpdates = 0;
	m_drawDebugInfo = false;
}
Map*
AutScreen::GetMap()
{
	//DO NOTHING
	return 0;
}

void
AutScreen::SetMap(Map* map)
{
	//DO NOTHING
}