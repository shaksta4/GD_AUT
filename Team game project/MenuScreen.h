#pragma once
/*
* Developed by Michael Jordan 14868336
*/
#include "screen.h"

class InputHandler;
class Mask;
class Sprite;
class Button;

class MenuScreen : public Screen
{
public:
	static MenuScreen& GetInstance();
	static void DestroyInstance();
	~MenuScreen();

	virtual bool Initialise(BackBuffer* backBuffer);
	virtual bool DoGameLoop();
	virtual void Quit();

	virtual Map* GetMap();
	virtual void SetMap(Map* map);
	virtual void Refresh();
	Mask* GetButtonMask();
	void ProcessButtonClick(Button* button);
protected:
	virtual void Process(float deltaTime);
	virtual void Draw(BackBuffer& backBuffer);

	void LoadUpGame();
private:
	MenuScreen(const MenuScreen& game);
	MenuScreen& operator=(const MenuScreen& game);

	MenuScreen();
	
	//Member Data:
public:
	
protected:
	static MenuScreen* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	Mask* m_pButtonMask;
	bool m_looping;
	Sprite* m_pBackground;
private:
};