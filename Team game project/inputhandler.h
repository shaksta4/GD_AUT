// COMP710 GP 2D Framework 2017
#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

// Library includes:
#include <SDL.h>

// Forward Declarations:
class Game;
class MapSelectScreen;
class Button;
class Mask;
class MenuScreen;

class InputHandler
{
	//Member Methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise();
	void ProcessInput(Game& game);
	void ProcessInput(MapSelectScreen& game);
	void ProcessInput(MenuScreen& game);
protected:

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	//Member Data:
public:

protected:

private:

};

#endif // __INPUTHANDER_H__
