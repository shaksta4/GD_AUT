#pragma once
/*
* Developed by Michael Jordan 14868336
*/

#include "screen.h" 
// Forward Declarations
class BackBuffer;
class Sprite;

class AutScreen : public Screen
{
	//Member Methods:
public:
	static AutScreen& GetInstance();
	static void DestroyInstance();
	~AutScreen();
	bool Initialise(BackBuffer* backBuffer);

	BackBuffer* GetBackBuffer();

	bool DoGameLoop();
	void Quit();

	virtual Map* GetMap();
	virtual void SetMap(Map* map);
	virtual void Refresh();
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	AutScreen(const AutScreen& game);
	AutScreen& operator=(const AutScreen& game);
	AutScreen();

	//Member Data:
public:

protected:
	static AutScreen* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	bool m_looping;

	Sprite* m_pLogo;
	
private:

};