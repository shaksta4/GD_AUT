// COMP710 GP 2D Framework 2017

// Library includes:
#include <SDL.h>
#include <crtdbg.h>
#include <stack>
#include <memory>
#include <cmath>
#include <ctime>

// Local includes:
#include "game.h"
#include "MapSelectScreen.h"
#include "logmanager.h"
#include "mapFactory.h" //TODO remove this after testing works
#include "map.h" //TODO remove this after testing works
#include "MenuScreen.h"
#include "iniParser.h"
#include "autScreen.h"
#include "backbuffer.h"

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(time(0));

	//Prefered Size:
	const int width = 1920;
	const int height = 1080;

	BackBuffer* backBuffer = new BackBuffer();
	std::stack<std::unique_ptr<Screen>*> display;
	std::unique_ptr<Screen>* temp;

	if (!backBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}
	//Display Logo:
	display.push(new std::unique_ptr<Screen>((&AutScreen::GetInstance())));

	if (!display.top()->get()->Initialise(backBuffer))
	{
		LogManager::GetInstance().Log("Screen Initialise failed!");

		return (1);
	}

	while (display.top()->get()->DoGameLoop())
	{
		//No body.
	}

	if (display.top()->get()->m_currentState == Screen::ScreenState::POP)
	{
		temp = display.top();
		display.pop();
		temp->release();
	}
	AutScreen::DestroyInstance();
	
	//Game:
	display.push(new std::unique_ptr<Screen>((&MenuScreen::GetInstance())));
	if (!display.top()->get()->Initialise(backBuffer))
	{
		LogManager::GetInstance().Log("Screen Initialise failed!");

		return (1);
	}

	while (!display.empty())
	{
		while (display.top()->get()->DoGameLoop())
		{
			//No body.
		}
		if (display.top()->get()->m_currentState == Screen::ScreenState::POP)
		{
			temp = display.top();
			display.pop();
			temp->release();
			Game::DestroyInstance();
			if (!display.empty()) display.top()->get()->Refresh();
		}
		else if (display.top()->get()->m_currentState == Screen::ScreenState::PUSH)
		{
			Map* map = display.top()->get()->GetMap();
			if (map == 0)
			{
				display.push(new std::unique_ptr<Screen>((&MapSelectScreen::GetInstance())));
			}
			else
			{
				display.push(new std::unique_ptr<Screen>((&Game::GetInstance())));
				display.top()->get()->SetMap(map);
			}
			
			if (!display.top()->get()->Initialise(backBuffer))
			{
				LogManager::GetInstance().Log("Screen Initialise failed!");

				return (1);
			}
		}
		else if(display.top()->get()->m_currentState == Screen::ScreenState::EXIT)
		{
			while (!display.empty())
			{
				temp = display.top();
				display.pop();
				temp->release();
			}
		}
	}
	MapSelectScreen::DestroyInstance();
	
	IniParser::DestroyInstance();
	LogManager::DestroyInstance();
	
	delete backBuffer;
	return (0);
}
