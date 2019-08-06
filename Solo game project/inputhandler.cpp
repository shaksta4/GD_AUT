// COMP710 GP 2D Framework 2017

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT: game.Quit(); break;

			//KEYBOARD
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT: LogManager::GetInstance().Log("Key Pressed!!"); game.MoveSpaceShipLeft(); break;
			case SDLK_RIGHT: game.MoveSpaceShipRight(); break;
			case SDLK_SPACE: game.FireSpaceShipBullet(); break;
			} break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT: game.StopPlayerMovement(); break;
			case SDLK_RIGHT: game.StopPlayerMovement(); break;
			} break;

			//END KEYBOARD

		case SDL_JOYBUTTONDOWN:
			switch (e.jbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A: game.FireSpaceShipBullet(); break;
			} break;

		case SDL_JOYHATMOTION:
			switch (e.jhat.value)
			{
			case SDL_HAT_LEFT: game.MoveSpaceShipLeft(); break;
			case SDL_HAT_RIGHT: game.MoveSpaceShipRight(); break;
			case SDL_HAT_CENTERED: game.StopPlayerMovement(); break;
			}break;
		}
	}
}
