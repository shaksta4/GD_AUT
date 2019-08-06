#pragma once
#include <SDL.h>
#include "animatedsprite.h"

class Button
{
//Class methods
public:
	Button();
	~Button();

	bool isClicked(int x, int y);
	void Draw(BackBuffer& backBuffer);
	bool Initialise(AnimatedSprite* sprite, int buttonID);

	int GetWidth();
	int GetHeight();

	int GetX();
	void SetX(int x);
	int GetY();
	void SetY(int y);

	int GetButtonID();
	void SwitchToggle();

	void SetEnabled(bool isEnabled); //<-- Added by Michael

private:

protected:

//Member data
public:

private:
	//SDL_Rect box;

	int m_x;
	int m_y;
	int m_w;
	int m_h;

	int m_buttonID;
	bool isToggled;
	bool isEnabled; //<-- Added by Michael

	AnimatedSprite* m_pSprite;

protected:


};

