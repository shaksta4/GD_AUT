/*
 * Developed by Michael Jordan 14868336
 */

//Local Includes
#include "tile.h"
#include "backbuffer.h"
#include "sprite.h"

//Library Includes

Tile::Tile()
	: m_pSprite(0)
	, m_startX(-1)
	, m_startY(-1)
	, m_centerX(-1)
	, m_centerY(-1)
	, m_exitX(-1)
	, m_exitY(-1)
{

}

Tile::~Tile()
{
	delete m_pSprite;
}

bool 
Tile::Initialize(Sprite* sprite)
{
	m_pSprite = sprite;
	return true;
}

void 
Tile::Draw(BackBuffer& backBuffer)
{
	m_pSprite->Draw(backBuffer);
}

void
Tile::SetSpriteCoords(int x, int y)
{
	m_pSprite->SetX(x);
	m_pSprite->SetY(y);
}


void
Tile::SetStartCoords(int x, int y)
{
	m_startX = x;
	m_startY = y;
}

void 
Tile::SetCenterCoords(int x, int y)
{
	m_centerX = x;
	m_centerY = y;
}

void 
Tile::SetExitCoords(int x, int y)
{
	m_exitX = x;
	m_exitY = y;
}

void 
Tile::GetSpriteCoords(int& x, int& y)
{
	x = m_pSprite->GetX();
	y = m_pSprite->GetY();
}

void
Tile::GetStartCoords(int& x, int& y)
{
	x = m_startX;
	y = m_startY;
}

void 
Tile::GetCenterCoords(int& x, int& y)
{
	x = m_centerX;
	y = m_centerY;
}

void
Tile::GetExitCoords(int& x, int& y)
{
	x = m_exitX;
	y = m_exitY;
}