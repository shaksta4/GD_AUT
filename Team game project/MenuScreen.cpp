/*
* Developed by Michael Jordan 14868336
*/

// This includes:
#include "MenuScreen.h"


// Local includes:
#include "backbuffer.h"
#include "animatedSprite.h"
#include "button.h"
#include "mask.h"
#include "inputHandler.h"
#include "LogManager.h"

// Library includes:
#include <cassert>
#include <SDL.h>

MenuScreen* MenuScreen::sm_pInstance = 0;

MenuScreen&
MenuScreen::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new MenuScreen();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
MenuScreen::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

MenuScreen::MenuScreen()
	: m_pBackBuffer(0)
	, m_pInputHandler(0)
	, m_looping(true)
{

}

MenuScreen::~MenuScreen()
{
	delete m_pInputHandler;
	m_pInputHandler = 0;
}

bool
MenuScreen::Initialise(BackBuffer* backBuffer)
{
	Refresh();

	assert(backBuffer); //Should only work if backbuffer hasn't died.

	m_pBackBuffer = backBuffer;
	std::vector<Button* > temp;
	std::vector<Button* >::iterator iter;
	AnimatedSprite* playSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\playButton.png");
	playSprite->SetFrameWidth(200);
	playSprite->SetFrameHeight(100);
	playSprite->AddFrame(0);
	playSprite->AddFrame(200);
	Button* playButton = new Button();
	playButton->Initialise(playSprite, 0);
	iter = temp.end();
	iter = temp.insert(iter, playButton);

	AnimatedSprite* quitSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\quitButton.png");
	quitSprite->SetFrameWidth(200);
	quitSprite->SetFrameHeight(100);
	quitSprite->AddFrame(0);
	quitSprite->AddFrame(200);
	Button* quitButton = new Button();
	quitButton->Initialise(quitSprite, 1);
	iter = temp.end();
	iter = temp.insert(iter, quitButton);

	m_pButtonMask = new Mask();
	int x = m_pBackBuffer->GetWidth()* 9.0 / 16.0;
	int y = m_pBackBuffer->GetHeight() * 7.0 / 8.0;
	m_pButtonMask->Initialise(temp, 100, x, y, false);

	m_pBackground = m_pBackBuffer->CreateSprite("assets\\menuSplash.png");

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	return (true);
}

bool
MenuScreen::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	m_looping = m_currentState == Screen::DISPLAY;
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
MenuScreen::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

}

void
MenuScreen::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	m_pBackground->Draw(backBuffer);

	std::vector<Button* > buttons = m_pButtonMask->GetButtons();
	std::vector<Button* >::iterator iter = buttons.begin();
	while (iter != buttons.end())
	{
		(*iter)->Draw(*m_pBackBuffer);
		iter++;
	}

	backBuffer.Present();
}

void
MenuScreen::Quit()
{
	m_currentState = Screen::EXIT;
}

void
MenuScreen::Refresh()
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
MenuScreen::GetMap()
{
	//DO NOTHING
	return 0;
}

void
MenuScreen::SetMap(Map* map)
{
	//DO NOTHING
}

Mask* 
MenuScreen::GetButtonMask()
{
	return m_pButtonMask;
}

void 
MenuScreen::ProcessButtonClick(Button* button)
{
	switch (button->GetButtonID())
	{
	case 0:
		m_currentState = Screen::PUSH;
		break;
	case 1:
		Quit();
		break;
	}
}