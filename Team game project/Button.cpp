#include "Button.h"
#include "backbuffer.h"
#include <cassert>

/*
This class models a button. The button has an X and Y coordinate, as well as a width and height.
It initialises a sprite that represents it and sets a button ID to identify itself. 

The Isclicked function simply takes in two ints and determines if the two ints are within the button's bounds. 

*/

Button::Button()
: m_x(0)
, m_y(0)
, isToggled(false)
, isEnabled(true)
{
}


Button::~Button()
{
}

bool
Button::Initialise(AnimatedSprite* sprite, int buttonID)
{
	assert(sprite);
	m_pSprite = sprite;
	m_buttonID = buttonID;
	m_h = sprite->GetFrameHeight();
	m_w = sprite->GetFrameWidth();

	return true;
}

void
Button::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	if (isToggled)
	{
		m_pSprite->SetCurrentFrameNo(1);
	}
	else
	{
		m_pSprite->SetCurrentFrameNo(0);
	}
	if(!isEnabled) backBuffer.SetTextureColour(m_pSprite->GetTexture(), 0x00, 0x00, 0x00); //<-- Added by Michael
	else backBuffer.SetTextureColour(m_pSprite->GetTexture(), 0xFF, 0xFF, 0xFF);
	m_pSprite->Draw(backBuffer);
	backBuffer.SetDrawColour(0xFF, 0xFF, 0xFF); //<-- Added by Michael
}

int
Button::GetWidth()
{
	return m_w;
}

int
Button::GetHeight()
{
	return m_h;
}

int
Button::GetX()
{
	return m_x;
}

void 
Button::SetX(int x)
{
	m_x = x;
}

int
Button::GetY()
{
	return m_y;
}

void
Button::SetY(int y)
{
	m_y = y;
}

int
Button::GetButtonID()
{
	return m_buttonID;
}

void
Button::SwitchToggle()
{
	if (isToggled)
	{
		isToggled = false;
	}
	else
	{
		isToggled = true;
	}
}

bool
Button::isClicked(int x, int y)
{
	if (!isEnabled) return false;
	if ((x > m_x) && (x < m_x + m_w))
	{
		if ((y > m_y) && (y < m_y + m_h))
		{
			return true;

		}
	}
		
	return false;
}
void 
Button::SetEnabled(bool isEnabled) //<-- Added by Michael
{
	this->isEnabled = isEnabled;
}
