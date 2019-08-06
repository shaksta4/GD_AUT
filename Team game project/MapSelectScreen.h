#pragma once
/*
* Developed by Michael Jordan 14868336
*/
#include "screen.h"
#include "map.h" //Access the internal enum
#include <vector>
#include <map>

class InputHandler;
class Button;
class Sprite;
class AnimatedSprite;
class Mask;

class MapSelectScreen : public Screen
{
public:
	static MapSelectScreen& GetInstance();
	static void DestroyInstance();
	~MapSelectScreen();

	virtual bool Initialise(BackBuffer* backBuffer);
	virtual bool DoGameLoop();
	virtual void Quit();
	
	virtual Map* GetMap();
	virtual void SetMap(Map* map);
	virtual void Refresh();

	Mask* GetButtonMask() const;
	void ProcessButtonClick(Button* button);
	void LoadUpGame();

	struct Node
	{
		Button* node;
		std::vector<Button* > connections;
	};

protected:
	virtual void Process(float deltaTime);
	virtual void Draw(BackBuffer& backBuffer);
private:
	MapSelectScreen(const MapSelectScreen& game);
	MapSelectScreen& operator=(const MapSelectScreen& game);

	MapSelectScreen();

	void SetUpGrid();
	void SetUpFlair();
	void SetUpEdges();
	void UpdateButtons();

	AnimatedSprite*	MapSelectScreen::SetUpButton(Map::Style style);
	Sprite* CreateLink(int index1, int index2);
	//Member Data:
public:

protected:
	static MapSelectScreen* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;
	Sprite* m_pbackground;
	std::map<Button* , Map::Style>* m_pMapStyles;

	Map* m_pSelection;

	std::vector<Node* > m_nodes;
	Mask* m_pButtonMask;
	std::vector<Sprite* > m_edges;
	std::vector<Sprite* > m_flair;

private:
};