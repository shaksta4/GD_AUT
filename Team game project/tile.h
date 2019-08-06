#pragma once
/*
* Developed by Michael Jordan 14868336
*/

//Forward Declarations
class BackBuffer;
class Sprite;

class Tile
{
	//Member Methods
public:
	Tile();
	~Tile();
	bool Initialize(Sprite* sprite);
	void Draw(BackBuffer& backBuffer);
	
	void SetSpriteCoords(int x, int y);
	void SetStartCoords(int x, int y);
	void SetCenterCoords(int x, int y);
	void SetExitCoords(int x, int y);

	void GetSpriteCoords(int& x, int& y);
	void GetStartCoords(int& x, int& y);
	void GetCenterCoords(int& x, int& y);
	void GetExitCoords(int& x, int& y);

protected:

private:

	//Memeber Data
public:

protected:
	int m_startX;
	int m_startY;
	int m_centerX;
	int m_centerY;
	int m_exitX;
	int m_exitY;
	Sprite* m_pSprite;

private:

};