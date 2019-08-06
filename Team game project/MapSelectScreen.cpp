/*
* Developed by Michael Jordan 14868336
*/

// This includes:
#include "MapSelectScreen.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "Button.h"
#include "iniparser.h"
#include "Mask.h"
#include "mapFactory.h"
#include "sprite.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cmath>


MapSelectScreen* MapSelectScreen::sm_pInstance = 0;

MapSelectScreen&
MapSelectScreen::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new MapSelectScreen();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
MapSelectScreen::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

MapSelectScreen::MapSelectScreen()
	: m_pBackBuffer(0)
	, m_pInputHandler(0)
	, m_looping(true)
	, m_pMapStyles(0)
	, m_pSelection(0)
{

}

MapSelectScreen::~MapSelectScreen()
{
	delete m_pInputHandler;
	m_pInputHandler = 0;

	if (m_pMapStyles != 0)
	{
		m_pMapStyles->clear();
		delete m_pMapStyles;
	}
	
	if (m_pSelection != 0)
	{
		delete m_pSelection;
		m_pSelection = 0;
	}

	std::vector<Sprite* >::iterator spriteIter = m_edges.begin();
	while (spriteIter != m_edges.end())
	{
		Sprite* temp = *(spriteIter);
		spriteIter = m_edges.erase(spriteIter);
		delete temp;
	}

	spriteIter = m_flair.begin();
	while (spriteIter != m_flair.end())
	{
		Sprite* temp = *(spriteIter);
		spriteIter = m_flair.erase(spriteIter);
		delete temp;
	}

	std::vector<Node* >::iterator nodeIter = m_nodes.begin();
	while (nodeIter != m_nodes.end())
	{
		Node* temp = *(nodeIter);
		nodeIter = m_nodes.erase(nodeIter);
		delete temp->node;
		delete temp;
	}
}

bool
MapSelectScreen::Initialise(BackBuffer* backBuffer)
{
	Refresh();
	
	m_currentState = DISPLAY;
	assert(backBuffer);

	m_pBackBuffer = backBuffer;
	m_pbackground = m_pBackBuffer->CreateSprite("assets\\selectBackGround.png");
	m_pMapStyles = new std::map<Button*, Map::Style>();

	SetUpGrid();
	SetUpEdges();
	SetUpFlair();

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	return (true);
}

bool
MapSelectScreen::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	m_looping = m_currentState == Screen::DISPLAY;

	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;
		m_executionTime += deltaTime;
		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);
			m_lag -= stepSize;
			++m_numUpdates;
		}

		Draw(*m_pBackBuffer);
	}

	//	SDL_Delay(1);

	return (m_looping);
}
void
MapSelectScreen::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	if (m_pSelection != 0)
	{
		if (m_pSelection->GetVictoryStatus())
		{
			UpdateButtons();
			delete m_pSelection;
			m_pSelection = 0;
		}
	}

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

}

void
MapSelectScreen::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	m_pbackground->Draw(backBuffer);

	std::vector<Sprite* >::iterator lineIter = m_edges.begin();
	while (lineIter != m_edges.end())
	{
		(*lineIter)->Draw(*m_pBackBuffer);
		lineIter++;
	}
	std::vector<Button*> buttons = m_pButtonMask->GetButtons();
	std::vector<Button*>::iterator buttonIter = buttons.begin();
	while (buttonIter != buttons.end())
	{
		(*buttonIter)->Draw(*m_pBackBuffer);
		buttonIter++;
	}
	
	std::vector<Sprite*>::iterator flairIter = m_flair.begin();
	while (flairIter != m_flair.end())
	{
		(*flairIter)->Draw(*m_pBackBuffer);
		flairIter++;
	}

	backBuffer.Present();
}

void
MapSelectScreen::Quit()
{
	m_currentState = EXIT;
}

void 
MapSelectScreen::Refresh()
{
	m_currentState = DISPLAY;
	m_elapsedSeconds = 0;
	m_lag = 0;
	m_executionTime = 0;
	m_lastTime = 0;
	m_frameCount = 0;
	m_FPS = 0;
	m_numUpdates = 0;
	m_drawDebugInfo = false;
}

void
MapSelectScreen::SetUpGrid()
{
	std::vector<Node* >::iterator nodeIter;
	std::vector<Button* > buttons;
	std::vector<Button* >::iterator buttonIter = buttons.begin();
	
	m_pButtonMask = new Mask();

	//Start node == plains
	IniParser::GetInstance().LoadIniFile("assets\\tile.ini");
	AnimatedSprite* sprite = SetUpButton(Map::Style::PLAIN);
	Node* temp = new Node();
	temp->node = new Button();
	
	temp->node->Initialise(sprite, 0);
	m_pButtonMask->AddButton(temp->node);
	buttonIter = buttons.end();
	buttonIter = buttons.insert(buttonIter, temp->node);
	nodeIter = m_nodes.end();
	nodeIter = m_nodes.insert(nodeIter, temp);
	m_pMapStyles->insert(std::pair<Button*, Map::Style>(temp->node, Map::Style::PLAIN));

	int x = m_pBackBuffer->GetWidth() * 3.0 / 4.0 - sprite->GetFrameWidth() / 2.0;
	int y = m_pBackBuffer->GetHeight() * 3.0 / 4.0 - sprite->GetFrameHeight() / 2.0;
	temp->node->SetX(x);
	temp->node->SetY(y);
	temp->node->SetEnabled(true);

	sprite = SetUpButton(Map::Style::DESERT);

	temp = new Node();
	temp->node = new Button();
	temp->node->Initialise(sprite, 1);
	m_pButtonMask->AddButton(temp->node);
	buttonIter = buttons.end();
	buttonIter = buttons.insert(buttonIter, temp->node);
	nodeIter = m_nodes.end();
	nodeIter = m_nodes.insert(nodeIter, temp);
	m_pMapStyles->insert(std::pair<Button*, Map::Style>(temp->node, Map::Style::DESERT));

	x = m_pBackBuffer->GetWidth() * 7.0 / 8.0 - sprite->GetFrameWidth() / 2.0;
	y = m_pBackBuffer->GetHeight() * 5.0 / 8.0 - sprite->GetFrameHeight() / 2.0;
	temp->node->SetX(x);
	temp->node->SetY(y);
	temp->node->SetEnabled(false);

	sprite = SetUpButton(Map::Style::SNOWY);

	temp = new Node();
	temp->node = new Button();
	temp->node->Initialise(sprite, 2);
	m_pButtonMask->AddButton(temp->node);
	buttonIter = buttons.end();
	buttonIter = buttons.insert(buttonIter, temp->node);
	nodeIter = m_nodes.end();
	nodeIter = m_nodes.insert(nodeIter, temp);
	m_pMapStyles->insert(std::pair<Button*, Map::Style>(temp->node, Map::Style::SNOWY));

	x = m_pBackBuffer->GetWidth() * 5.0 / 8.0 - sprite->GetFrameWidth() / 2.0;
	y = m_pBackBuffer->GetHeight() * 5.0 / 8.0 - sprite->GetFrameHeight() / 2.0;
	temp->node->SetX(x);
	temp->node->SetY(y);
	temp->node->SetEnabled(false);

	sprite = SetUpButton(Map::Style::HELLSCAPE);

	temp = new Node();
	temp->node = new Button();
	temp->node->Initialise(sprite, 3);
	m_pButtonMask->AddButton(temp->node);
	buttonIter = buttons.end();
	buttonIter = buttons.insert(buttonIter, temp->node);
	nodeIter = m_nodes.end();
	nodeIter = m_nodes.insert(nodeIter, temp);
	m_pMapStyles->insert(std::pair<Button*, Map::Style>(temp->node, Map::Style::HELLSCAPE));

	x = m_pBackBuffer->GetWidth() * 3.0 / 4.0 - sprite->GetFrameWidth() / 2.0;
	y = m_pBackBuffer->GetHeight()/ 2.0 - sprite->GetFrameHeight() / 2.0;
	temp->node->SetX(x);
	temp->node->SetY(y);
	temp->node->SetEnabled(false);

	m_pButtonMask->Initialise(buttons, m_pBackBuffer->GetWidth() / 2.0, m_pBackBuffer->GetWidth() / 2.0, m_pBackBuffer->GetHeight() / 2.0, true);
	//End node == Hellscape
	//Length = 3
}

AnimatedSprite* 
MapSelectScreen::SetUpButton(Map::Style style)
{
	AnimatedSprite* sprite;
	switch (style)
	{
	case Map::Style::PLAIN:
		sprite = m_pBackBuffer->CreateAnimatedSprite(IniParser::GetInstance().GetValueAsString("select", "plain").c_str());
		break;
	case Map::Style::SNOWY:
		sprite = m_pBackBuffer->CreateAnimatedSprite(IniParser::GetInstance().GetValueAsString("select", "snowy").c_str());
		break;
	case Map::Style::DESERT:
		sprite = m_pBackBuffer->CreateAnimatedSprite(IniParser::GetInstance().GetValueAsString("select", "desert").c_str());
		break;
	case Map::Style::HELLSCAPE:
		sprite = m_pBackBuffer->CreateAnimatedSprite(IniParser::GetInstance().GetValueAsString("select", "hellscape").c_str());
		break;
	}
	sprite->SetFrameSpeed(0.03f);
	sprite->SetFrameWidth(50);
	sprite->SetFrameHeight(50);
	sprite->AddFrame(0);
	sprite->AddFrame(50);
	return sprite;
}

void 
MapSelectScreen::SetUpEdges()
{
	std::vector<Sprite* >::iterator iter = m_edges.begin();
	std::vector<Button* >::iterator connectionIter = m_nodes.at(0)->connections.begin();
	//Hardcoded connections:
	connectionIter = m_nodes.at(0)->connections.insert(connectionIter, m_nodes.at(1)->node);
	connectionIter = m_nodes.at(0)->connections.insert(connectionIter, m_nodes.at(2)->node);
	connectionIter = m_nodes.at(1)->connections.begin();
	connectionIter = m_nodes.at(1)->connections.insert(connectionIter, m_nodes.at(3)->node);
	connectionIter = m_nodes.at(2)->connections.begin();
	connectionIter = m_nodes.at(2)->connections.insert(connectionIter, m_nodes.at(3)->node);

	iter = m_edges.insert(iter, CreateLink(0, 1));
	iter = m_edges.insert(iter, CreateLink(0, 2));
	iter = m_edges.insert(iter, CreateLink(1, 3));
	iter = m_edges.insert(iter, CreateLink(2, 3));

}

void 
MapSelectScreen::SetUpFlair()
{
	Sprite* village = m_pBackBuffer->CreateSprite("assets\\villageOverlay.png");
	int x = m_nodes.at(0)->node->GetX();
	int y = m_nodes.at(0)->node->GetY() - village->GetHeight()/2.0;

	village->SetX(x);
	village->SetY(y);
	std::vector<Sprite* >::iterator iter = m_flair.begin();
	iter = m_flair.insert(iter, village);

	village = m_pBackBuffer->CreateSprite("assets\\villageOverlay.png");
	x = m_nodes.at(1)->node->GetX();
	y = m_nodes.at(1)->node->GetY() - village->GetHeight() / 2.0;

	village->SetX(x);
	village->SetY(y);
	iter = m_flair.insert(iter, village);

	village = m_pBackBuffer->CreateSprite("assets\\villageOverlay.png");
	x = m_nodes.at(2)->node->GetX();
	y = m_nodes.at(2)->node->GetY() - village->GetHeight() / 2.0;

	village->SetX(x);
	village->SetY(y);
	iter = m_flair.insert(iter, village);

	village = m_pBackBuffer->CreateSprite("assets\\villageOverlay.png");
	x = m_nodes.at(3)->node->GetX();
	y = m_nodes.at(3)->node->GetY() - village->GetHeight() / 2.0;

	village->SetX(x);
	village->SetY(y);
	iter = m_flair.insert(iter, village);

}

Sprite*
MapSelectScreen::CreateLink(int index1, int index2)
{
	Sprite* line = m_pBackBuffer->CreateSprite("assets\\line.png");

	int cx = m_pBackBuffer->GetWidth() / 2.0;
	int cy = m_pBackBuffer->GetHeight() / 2.0;
	int x1 = (m_nodes.at(index1)->node->GetX() + m_nodes.at(index1)->node->GetWidth() / 2.0);
	int y1 = (m_nodes.at(index1)->node->GetY() + m_nodes.at(index1)->node->GetHeight() / 2.0);
	int x2 = (m_nodes.at(index2)->node->GetX() + m_nodes.at(index2)->node->GetWidth() / 2.0);
	int y2 = (m_nodes.at(index2)->node->GetY() + m_nodes.at(index2)->node->GetHeight() / 2.0);
	int dx = (x2 + x1) / 2.0;
	int dy = (y2 + y1) / 2.0;

	float deg = std::atan2((y2 - cy) - (y1 - cy), (x2 - cx) - (x1 - cx)) * 180.0f / 3.14159f;
	int dis = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
	line->SetWidth(dis);
	line->SetX(dx - line->GetWidth() / 2.0);
	line->SetY(dy - line->GetHeight() / 2.0);
	line->SetAngle(deg);

	return line;
}
void
MapSelectScreen::LoadUpGame()
{
	m_currentState = PUSH;
}

Mask*
MapSelectScreen::GetButtonMask() const
{
	return m_pButtonMask;
}

void MapSelectScreen::ProcessButtonClick(Button* button)
{
	Map::Style style = m_pMapStyles->at(button);
	m_pSelection = MapFactory::GenerateMap(*m_pBackBuffer, style, 2.0f);
	m_pSelection->SetID(button->GetButtonID());
}

Map*
MapSelectScreen::GetMap() 
{
	return m_pSelection;
}

void 
MapSelectScreen::SetMap(Map* map)
{
	m_pSelection = map;
}

void 
MapSelectScreen::UpdateButtons()
{
	int index = m_pSelection->GetID();

	std::vector<Node* >::iterator nodeIter = m_nodes.begin();
	while (nodeIter != m_nodes.end())
	{
		(*nodeIter)->node->SetEnabled(false);
		nodeIter++;
	}

	if (!m_nodes.at(index)->connections.empty())
	{
		std::vector<Button* >::iterator connectionIter = m_nodes.at(index)->connections.begin();
		while (connectionIter != m_nodes.at(index)->connections.end())
		{
			(*connectionIter)->SetEnabled(true);
			connectionIter++;
		}
	}
	else
	{
		//Game has been won
	}
}