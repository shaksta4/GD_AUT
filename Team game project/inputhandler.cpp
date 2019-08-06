// COMP710 GP 2D Framework 2017

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "MapSelectScreen.h"
#include "MenuScreen.h"
#include "logmanager.h"
#include "Mask.h"
#include "Button.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{

}

bool 
InputHandler::Initialise()
{
	
	
	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	
	std::vector<Button* > spawnButtons = game.GetButtonMask()->GetButtons();
	//Add your own mask to .h and then initialise it.

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		int mouseX = 0, mouseY = 0;

		//If event is a button click, and the button is left
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button = SDL_BUTTON_LEFT)
			{
				mouseX = e.button.x;
				mouseY = e.button.y;

				//spawnButtons[] refers to in game SPAWNING buttons only.
				if (spawnButtons[0]->isClicked(mouseX, mouseY))
				{
					LogManager::GetInstance().Log("Spawning NORMAL troop");
					game.addNormalTroopToActingTroops();
				}
				else if (spawnButtons[1]->isClicked(mouseX, mouseY))
				{
					LogManager::GetInstance().Log("Spawning TANK troop");
					game.addTankTroopToActingTroops();
				}
				else if (spawnButtons[2]->isClicked(mouseX, mouseY))
				{
					LogManager::GetInstance().Log("Spawning PROTECTION troop");
					game.addProtectionTroopToActingTroops();
				}
				//----------------------------------------

			}
		}

		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
	}
}

void
InputHandler::ProcessInput(MapSelectScreen& screen)
{
	std::vector<Button* > spawnButtons = screen.GetButtonMask()->GetButtons();
	//Add your own mask to .h and then initialise it.

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		int mouseX = 0, mouseY = 0;

		//If event is a button click, and the button is left
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button = SDL_BUTTON_LEFT)
			{
				mouseX = e.button.x;
				mouseY = e.button.y;

				//spawnButtons[] refers to in game SPAWNING buttons only.
				std::vector<Button* >::iterator iter = spawnButtons.begin();
				while (iter != spawnButtons.end())
				{
					if((*iter)->isClicked(mouseX, mouseY))
					{
						(*iter)->SwitchToggle();
						screen.ProcessButtonClick((*iter));
						break; //Break while;
					}
					iter++;
				}
				//----------------------------------------

			}
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			if (e.button.button = SDL_BUTTON_LEFT)
			{
				mouseX = e.button.x;
				mouseY = e.button.y;

				//spawnButtons[] refers to in game SPAWNING buttons only.
				std::vector<Button* >::iterator iter = spawnButtons.begin();
				while (iter != spawnButtons.end())
				{
					if ((*iter)->isClicked(mouseX, mouseY))
					{
						(*iter)->SwitchToggle();
						screen.LoadUpGame();
						break; //Break while;
					}
					iter++;
				}
				//----------------------------------------
			}
		}

		if (e.type == SDL_QUIT)
		{
			screen.Quit();
		}
	}
}

void
InputHandler::ProcessInput(MenuScreen& screen)
{
	std::vector<Button* > spawnButtons = screen.GetButtonMask()->GetButtons();
	//Add your own mask to .h and then initialise it.

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		int mouseX = 0, mouseY = 0;

		//If event is a button click, and the button is left
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button = SDL_BUTTON_LEFT)
			{
				mouseX = e.button.x;
				mouseY = e.button.y;

				//spawnButtons[] refers to in game SPAWNING buttons only.
				std::vector<Button* >::iterator iter = spawnButtons.begin();
				while (iter != spawnButtons.end())
				{
					if ((*iter)->isClicked(mouseX, mouseY))
					{
						(*iter)->SwitchToggle();
						
						break; //Break while;
					}
					iter++;
				}
				//----------------------------------------

			}
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			if (e.button.button = SDL_BUTTON_LEFT)
			{
				mouseX = e.button.x;
				mouseY = e.button.y;

				//spawnButtons[] refers to in game SPAWNING buttons only.
				std::vector<Button* >::iterator iter = spawnButtons.begin();
				while (iter != spawnButtons.end())
				{
					if ((*iter)->isClicked(mouseX, mouseY))
					{
						(*iter)->SwitchToggle();
						screen.ProcessButtonClick((*iter));
						break; //Break while;
					}
					iter++;
				}
				//----------------------------------------
			}
		}

		if (e.type == SDL_QUIT)
		{
			screen.Quit();
		}
	}
}