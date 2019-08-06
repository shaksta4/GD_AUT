// COMP710 GP 2D Framework 2017


/*
Shakeel : 16/10/2017 Added towers. Can be placed but havent implemented shooting projectiles just yet. Can acquire targets 

*/

// This includes:
#include "game.h"

// Local includes:
#include "animatedsprite.h"
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "normaltroop.h"
#include "tanktroop.h"
#include "protectiontroop.h"
#include "projectile.h"
#include "map.h"
#include "mapFactory.h"
#include "troopFactory.h"
#include "TowerFactory.h"
#include "Projectile.h"
#include "Mask.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <crtdbg.h>
#include <cmath>
#include <string>
// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_pMap(0)
, m_pVillage(0)
{
	
}

Game::~Game()
{	
	//To be removed:
	std::vector<Troop* >::iterator troopIter = waitingTroops.begin();
	while (troopIter != waitingTroops.end())
	{
		Troop* temp = (*troopIter);
		troopIter = waitingTroops.erase(troopIter);
		delete temp;
	}

	troopIter = usingTroops.begin();
	while (troopIter != usingTroops.end())
	{
		Troop* temp = (*troopIter);
		troopIter = usingTroops.erase(troopIter);
		delete temp;
	}

	troopIter = actingTroops.begin();
	while (troopIter != actingTroops.end())
	{
		Troop* temp = (*troopIter);
		troopIter = actingTroops.erase(troopIter);
		delete temp;
	}

	std::vector<Button* >::iterator butIter = m_Buttons.begin();
	while (butIter != m_Buttons.end())
	{
		Button* temp = (*butIter);
		butIter = m_Buttons.erase(butIter);
		delete temp;
	}

	std::vector<Tower* >::iterator towIter = m_Towers.begin();
	while (towIter != m_Towers.end())
	{
		Tower* temp = (*towIter);
		towIter = m_Towers.erase(towIter);
		delete temp;
	}

	delete m_pInputHandler;
	m_pInputHandler = 0;

}

bool 
Game::Initialise(BackBuffer* backBuffer)
{
	Refresh();
	//NOTE: BACKBUFFER HAS BEEN INITIALISED IN MAIN! ~ Michael Jordan
	assert(backBuffer); //Should only work if backbuffer hasn't died.

	m_pBackBuffer = backBuffer;

	//BUTTONS
	//Create sprites for buttons. USING ONE SPRITE FOR ALL BUTTONS ATM. I suggest a sprite sheet. 
	AnimatedSprite* NormalButtonSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\troopbutton.png");
	NormalButtonSprite->SetFrameWidth(200);
	NormalButtonSprite->SetFrameHeight(200);
	NormalButtonSprite->SetFrameSpeed(1 / 5);
	NormalButtonSprite->AddFrame(0);

	AnimatedSprite* TankButtonSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\troopbuttonTANK.png");
	TankButtonSprite->SetFrameWidth(200);
	TankButtonSprite->SetFrameHeight(200);
	TankButtonSprite->SetFrameSpeed(1 / 5);
	TankButtonSprite->AddFrame(0);

	AnimatedSprite* ProtButtonSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\troopbuttonPROT.png");
	ProtButtonSprite->SetFrameWidth(200);
	ProtButtonSprite->SetFrameHeight(200);
	ProtButtonSprite->SetFrameSpeed(1 / 5);
	ProtButtonSprite->AddFrame(0);

	//Create buttons using the sprite and assign its ID
	Button* SpawnNormalTroop = new Button();
	SpawnNormalTroop->Initialise(NormalButtonSprite, 1);

	Button* SpawnTankTroop = new Button();
	SpawnTankTroop->Initialise(TankButtonSprite, 2);

	Button* SpawnProtectionTroop = new Button();
	SpawnProtectionTroop->Initialise(ProtButtonSprite, 3);

	//Put button into container in Game
	m_Buttons.push_back(SpawnNormalTroop);
	m_Buttons.push_back(SpawnTankTroop);
	m_Buttons.push_back(SpawnProtectionTroop);

	//Create mask which takes in the container of buttons. POSITION OF MASK DETERMINES POSITION OF BUTTONS
	m_pButtonMask = new Mask();
	m_pButtonMask->Initialise(m_Buttons, 300, 500, 900, false);

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	//TO BE REMOVED:
	int style = (rand() % 4);
	Map::Style selection;
	switch (style)
	{
	case 0: 
		selection = Map::PLAIN;
		break;
	case 1:
		selection = Map::SNOWY;
		break;
	case 2:
		selection = Map::DESERT;
		break;
	case 3:
		selection = Map::HELLSCAPE;
		break;

	}

	actingTroops = *new std::vector<Troop* >();
	iniWaitingTroops();
	
	//Troops
	//Making troop sprites.
	AnimatedSprite* normalTroopSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\NormalTroop.png");
	normalTroopSprite->SetFrameWidth(50);
	normalTroopSprite->SetFrameHeight(50);
	normalTroopSprite->SetFrameSpeed(0.03f);
	for (int i = 0; i < 399; i += 50){
		normalTroopSprite->AddFrame(i);
	}
		
	//Towers
	AnimatedSprite* WTowerSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\woodentower.png");
	WTowerSprite->SetFrameWidth(200);
	WTowerSprite->SetFrameHeight(200);
	WTowerSprite->SetFrameSpeed(1 / 5);
	WTowerSprite->AddFrame(0);
	
	std::vector<Map::Coords* > towerPos = m_pMap->GetAllTowerCoords();
	std::vector<Tower*>::iterator towerIter = m_Towers.begin();
	for (int p = 0; p < towerPos.size(); p++)
	{
		towerIter = m_Towers.insert(towerIter, TowerFactory::CreateTower(WTowerSprite, Tower::WOODEN));
		int Tx = towerPos.at(p)->x - (WTowerSprite->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0;
		int Ty = towerPos.at(p)->y - (WTowerSprite->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0;
		(*towerIter)->SetPosition(Tx, Ty);
	}

	//Village by Michael
	AnimatedSprite* villageSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\village.png");
	villageSprite->SetFrameWidth(300);
	villageSprite->SetFrameHeight(300);
	villageSprite->SetFrameSpeed(1.0);
	villageSprite->AddFrame(0);

	m_pVillage = TowerFactory::CreateTower(villageSprite, Tower::VILLAGE);
	m_Towers.push_back(m_pVillage);
	m_pMap->SetUpVillageCoordinates(m_pVillage);

	backBuffer->PlaySound("BackgroundTrack", true);

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	m_looping = m_currentState == Screen::ScreenState::DISPLAY;

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
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	checkChangeStatesOfTroops();

	int tx;
	int ty;
	Map::select tType;

	//m_pNormalTroop->Process(deltaTime);
	for (int i = 0; i < actingTroops.size(); i++){
		if (actingTroops.at(i)->IsDead()){
			Troop* temp = actingTroops.at(i);
			actingTroops.erase(actingTroops.begin() + i);
			delete temp;
		}
		else
		{
			actingTroops.at(i)->Process(deltaTime);
		}
	}
	
	//Following added by Michael: (could be moved to m_pNormalTroop's Process method)
	

	for (int i = 0; i < actingTroops.size(); i++){
		actingTroops.at(i)->GetTarget(tx, ty, tType);
		if (m_pMap->HasReachedTarget(actingTroops.at(i), tType))
		{
			if (tType == Map::select::CENTER)
			{
				tType = Map::select::EXIT;
				m_pMap->GetNextTarget(tx, ty, actingTroops.at(i), tType);
				actingTroops.at(i)->SetTarget(tx, ty, tType);
			}
			else if (tType == Map::select::EXIT)
			{
				actingTroops.at(i)->SetCurrentTile(actingTroops.at(i)->GetCurrentTile() + 1);
				tType = Map::select::CENTER;
				m_pMap->GetNextTarget(tx, ty, actingTroops.at(i), tType);
				actingTroops.at(i)->SetTarget(tx, ty, tType);
			}
		}
	}

	//Towers
	std::vector<Tower*>::iterator towerIter = m_Towers.begin();
	while (towerIter != m_Towers.end())
	{
		if (actingTroops.size() > 0)
		{
			(*towerIter)->SelectTarget(actingTroops[0]); // CHANGE THIS TO BE HIGHEST PRIORITY TROOP IN PRIORITY QUEUE
			(*towerIter)->Process(deltaTime);
		}
		towerIter++;
	}
	if (m_pMap->GetVictoryStatus())
	{
		m_currentState = Screen::POP;
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	m_pMap->Draw(*m_pBackBuffer);
	for (int i = 0; i < actingTroops.size(); i++){
		actingTroops.at(i)->Draw(*m_pBackBuffer);
	}

	//Altered by Michael:
	std::vector<Tower*>::iterator towerIter = m_Towers.begin();
	while (towerIter != m_Towers.end())
	{
		(*towerIter)->Draw(*m_pBackBuffer);
		(*towerIter)->FireProj(*m_pBackBuffer);
		towerIter++;
	}

	m_Buttons = m_pButtonMask->GetButtons();
	std::vector<Button*>::iterator buttonIter = m_Buttons.begin();
	while (buttonIter != m_Buttons.end())
	{
		(*buttonIter)->Draw(*m_pBackBuffer);
		buttonIter++;
	}

//if (!actingTroops.empty()){
//	for (int i = 0; i < actingTroops.size(); i++){
//		actingTroops.at(i)->Draw(backBuffer);
//	}
//}
//if (!usingTroops.empty()){
//	for (int i = 0; i < usingTroops.size(); i++){
//		usingTroops.at(i)->Draw(backBuffer);
//	}
//}
//if (!waitingTroops.empty()){
//	for (int i = 0; i < waitingTroops.size(); i++){
//		waitingTroops.at(i)->Draw(backBuffer);
//	}
//}
	backBuffer.Present();
}

void 
Game::Quit()
{
	m_currentState = Screen::ScreenState::EXIT;
}

void
Game::Refresh()
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

//Testing Normal Troop Sprite work
//Jack Boyd 14860648
void
Game::ChangeDirection(int x)
{
	if (x == 0){
		m_pNormalTroop->SetFrameRow(0);

	}
	else if (x == 1){
		m_pNormalTroop->SetFrameRow(50);

	}
	else if (x == 2){
		m_pNormalTroop->SetFrameRow(100);

	}
	else if (x == 3){
		m_pNormalTroop->SetFrameRow(150);

	}
}

void
Game::StopAllTroops()
{
	m_pNormalTroop->SetHalted(true);
	for (int i = 0; i < actingTroops.size(); i++){
		actingTroops.at(i)->SetHalted(true);
	}
}

void
Game::StartTroops()
{
	m_pNormalTroop->SetHalted(false);
	for (int i = 0; i < actingTroops.size(); i++){
		actingTroops.at(i)->SetHalted(false);
	}
}

void 
Game::DebugRemakeMap()
{
	delete m_pMap;
	std::vector<Tower*>::iterator towerIter = m_Towers.begin();
	while (towerIter != m_Towers.end())
	{
		Tower* temp = *towerIter;
		towerIter = m_Towers.erase(towerIter);
		delete temp;
	}
	delete m_pNormalTroop;
	//TO BE REMOVED:
	int style = (rand() % 4);
	Map::Style selection;
	switch (style)
	{
	case 0:
		selection = Map::PLAIN;
		break;
	case 1:
		selection = Map::SNOWY;
		break;
	case 2:
		selection = Map::DESERT;
		break;
	case 3:
		selection = Map::HELLSCAPE;
		break;

	}
	m_pMap = MapFactory::GenerateMap(*m_pBackBuffer, selection, 2.0f);

	actingTroops = *new std::vector<Troop* >();
	iniWaitingTroops();

	AnimatedSprite* normalTroop = m_pBackBuffer->CreateAnimatedSprite("assets\\NormalTroop.png");
	normalTroop->SetFrameWidth(50);
	normalTroop->SetFrameHeight(50);
	normalTroop->SetFrameSpeed(0.03f);
	for (int i = 0; i < 399; i += 50) {
		normalTroop->AddFrame(i);
	}
	m_pNormalTroop = TroopFactory::CreateNormalTroop();
	m_pNormalTroop->Initialise(normalTroop);
	int x;
	int y;
	m_pMap->GetTrackStartCoords(x, y);
	x -= (normalTroop->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0; //<- Added by Michael
	y -= (normalTroop->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0; //<- Added by Michael
	m_pNormalTroop->SetPosition(x + 100, y);
	m_pNormalTroop->SetCurrentTile(0);
	m_pMap->GetNextTarget(x, y, m_pNormalTroop, Map::select::CENTER); //<- Added by Michael
	m_pNormalTroop->SetTarget(x, y, Map::select::CENTER); //<- Added by Michael
	m_pNormalTroop->SetCurrentTile(0);
	m_pNormalTroop->SetVerticalVelocity(100); //<- Added by Michael
	m_pNormalTroop->SetHorizontalVelocity(100); //<- Added by Michael
	m_pMap->GetNextTarget(x, y, m_pNormalTroop, Map::select::CENTER); //<- Added by Michael
	m_pNormalTroop->SetTarget(x, y, Map::select::CENTER); //<- Added by Michael

	AnimatedSprite* normalTroopSprite1 = m_pBackBuffer->CreateAnimatedSprite("assets\\NormalTroop.png");
	normalTroopSprite1->SetFrameWidth(50);
	normalTroopSprite1->SetFrameHeight(50);
	normalTroopSprite1->SetFrameSpeed(0.03f);
	for (int i = 0; i < 399; i += 50){
		normalTroopSprite1->AddFrame(i);
	}
	m_pNormalTroop1 = TroopFactory::CreateNormalTroop();
	m_pNormalTroop1->Initialise(normalTroopSprite1);
	int x1, y1 = 0;
	m_pMap->GetTrackStartCoords(x1, y1);
	x1 -= (normalTroopSprite1->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0; //<- Added by Michael
	y1 -= (normalTroopSprite1->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0; //<- Added by Michael
	m_pNormalTroop1->SetPosition(x1, y1 + 10);
	m_pNormalTroop1->SetCurrentTile(0);
	m_pNormalTroop1->SetVerticalVelocity(100); //<- Added by Michael
	m_pNormalTroop1->SetHorizontalVelocity(100); //<- Added by Michael
	m_pMap->GetNextTarget(x1, y1, m_pNormalTroop1, Map::select::CENTER); //<- Added by Michael
	m_pNormalTroop1->SetTarget(x1, y1, Map::select::CENTER); //<- Added by Michael
	actingTroops.push_back(m_pNormalTroop1);


	//Towers
	AnimatedSprite* WTowerSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\woodentower.png");
	WTowerSprite->SetFrameWidth(200);
	WTowerSprite->SetFrameHeight(200);
	WTowerSprite->SetFrameSpeed(1 / 5);
	WTowerSprite->AddFrame(0);

	//ADDED by Michael:
	std::vector<Map::Coords* > towerPos = m_pMap->GetAllTowerCoords();
	towerIter = m_Towers.begin();
	for (int p = 0; p < towerPos.size(); p++)
	{
		towerIter = m_Towers.insert(towerIter, TowerFactory::CreateTower(WTowerSprite, Tower::WOODEN));
		int Tx = towerPos.at(p)->x - (WTowerSprite->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0;
		int Ty = towerPos.at(p)->y - (WTowerSprite->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0;
		(*towerIter)->SetPosition(Tx, Ty);
	}
}

Mask* 
Game::GetButtonMask() const
{
	return m_pButtonMask;
}

Map*
Game::GetMap()
{
	return m_pMap;
}

void 
Game::SetMap(Map* map)
{
	m_pMap = map;
}


void
Game::checkChangeStatesOfTroops(){
	for (int i = 0; i < m_Towers.size(); i++){
		for (int j = 0; j < actingTroops.size(); j++){
			if (actingTroops.at(j)->GetPositionX() > m_Towers.at(i)->GetPositionX() - m_Towers.at(i)->GetWidth() / 4 && actingTroops.at(j)->GetPositionX() < m_Towers.at(i)->GetPositionX() + m_Towers.at(i)->GetWidth() / 2 &&
				actingTroops.at(j)->GetPositionY() > m_Towers.at(i)->GetPositionY() - m_Towers.at(i)->GetWidth() / 4 && actingTroops.at(j)->GetPositionY() < m_Towers.at(i)->GetPositionY() + m_Towers.at(i)->GetHeight()){
				actingTroops.at(j)->SetActed(true);
				m_Towers.at(i)->SetHealth(m_Towers.at(i)->GetHealth() - actingTroops.at(j)->GetDamage());
				//actingTroops.at(j)->SetHealth(actingTroops.at(j)->GetHealth() - 50);
				if (actingTroops.at(j)->GetHealth() <= 0){
					actingTroops.at(j)->SetDead(true);
				}
				if (m_Towers.at(i)->GetHealth() <= 0){
					m_Towers.at(i)->SetDead(true);
					actingTroops.at(j)->SetMoved(true);
				}
			}
		}
	}
}



void 
Game::iniWaitingTroops(){
	waitingTroops = * new std::vector<Troop* >();
	for (int i = 0; i < 5; i++){
		waitingTroops.push_back(new NormalTroop());
		waitingTroops.push_back(new TankTroop());
		waitingTroops.push_back(new NormalTroop());
		waitingTroops.push_back(new ProtectionTroop());
		waitingTroops.push_back(new NormalTroop());
	}
}

void 
Game::iniActingTroop(){
	AnimatedSprite* normalTroopSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\NormalTroop.png");
	normalTroopSprite->SetFrameWidth(50);
	normalTroopSprite->SetFrameHeight(50);
	normalTroopSprite->SetFrameSpeed(0.03f);
	for (int i = 0; i < 399; i += 50){
		normalTroopSprite->AddFrame(i);
	}
	Troop* m_pNormalTroop1 = TroopFactory::CreateNormalTroop();
	m_pNormalTroop1->Initialise(normalTroopSprite);
	int x, y = 0;
	m_pMap->GetTrackStartCoords(x, y);
	x -= (normalTroopSprite->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0; //<- Added by Michael
	y -= (normalTroopSprite->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0; //<- Added by Michael

	m_pNormalTroop1->SetPosition(0, 0);
	m_pNormalTroop1->SetCurrentTile(1);
	m_pNormalTroop1->SetVerticalVelocity(50); //<- Added by Michael
	m_pNormalTroop1->SetHorizontalVelocity(50); //<- Added by Michael
	m_pMap->GetNextTarget(x, y, m_pNormalTroop1, Map::select::CENTER); //<- Added by Michael
	m_pNormalTroop1->SetTarget(x, y, Map::select::CENTER); //<- Added by Michael
	actingTroops.push_back(m_pNormalTroop1);

}

void 
Game::addNormalTroopToActingTroops(){
	AnimatedSprite* normalTroopSprite1 = m_pBackBuffer->CreateAnimatedSprite("assets\\NormalTroop.png");
	normalTroopSprite1->SetFrameWidth(50);
	normalTroopSprite1->SetFrameHeight(50);
	normalTroopSprite1->SetFrameSpeed(0.03f);
	for (int i = 0; i < 399; i += 50){
		normalTroopSprite1->AddFrame(i);
	}
	Troop* m_pNormalTroop1 = TroopFactory::CreateNormalTroop();
	m_pNormalTroop1->Initialise(normalTroopSprite1);
	int x1, y1 = 0;
	m_pMap->GetTrackStartCoords(x1, y1);
	x1 -= (normalTroopSprite1->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0; 
	y1 -= (normalTroopSprite1->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0; 
	m_pNormalTroop1->SetPosition(x1, y1 + 10);
	m_pNormalTroop1->SetCurrentTile(0);
	m_pNormalTroop1->SetVerticalVelocity(100); 
	m_pNormalTroop1->SetHorizontalVelocity(100); 
	m_pMap->GetNextTarget(x1, y1, m_pNormalTroop1, Map::select::CENTER); 
	m_pNormalTroop1->SetTarget(x1, y1, Map::select::CENTER); 
	actingTroops.push_back(m_pNormalTroop1);
}

void 
Game::addTankTroopToActingTroops(){
	AnimatedSprite* normalTroopSprite1 = m_pBackBuffer->CreateAnimatedSprite("assets\\TankTroop.png");
	normalTroopSprite1->SetFrameWidth(50);
	normalTroopSprite1->SetFrameHeight(50);
	normalTroopSprite1->SetFrameSpeed(0.03f);
	for (int i = 0; i < 99; i += 50){
		normalTroopSprite1->AddFrame(i);
	}
	TankTroop* m_pNormalTroop1 = TroopFactory::CreateTankTroop();
	m_pNormalTroop1->Initialise(normalTroopSprite1);
	int x1, y1 = 0;
	m_pMap->GetTrackStartCoords(x1, y1);
	x1 -= (normalTroopSprite1->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0;
	y1 -= (normalTroopSprite1->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0;
	m_pNormalTroop1->SetPosition(x1, y1 + 10);
	m_pNormalTroop1->SetCurrentTile(0);
	m_pNormalTroop1->SetVerticalVelocity(100);
	m_pNormalTroop1->SetHorizontalVelocity(100);
	m_pMap->GetNextTarget(x1, y1, m_pNormalTroop1, Map::select::CENTER);
	m_pNormalTroop1->SetTarget(x1, y1, Map::select::CENTER);
	actingTroops.push_back(m_pNormalTroop1);
}

void 
Game::addProtectionTroopToActingTroops(){
	AnimatedSprite* normalTroopSprite1 = m_pBackBuffer->CreateAnimatedSprite("assets\\ProtectionTroop.png");
	normalTroopSprite1->SetFrameWidth(200);
	normalTroopSprite1->SetFrameHeight(200);
	normalTroopSprite1->SetFrameSpeed(0.03f);
	for (int i = 0; i < 1599; i += 200){
		normalTroopSprite1->AddFrame(i);
	}
	ProtectionTroop* m_pNormalTroop1 = TroopFactory::CreateProtectionTroop();
	m_pNormalTroop1->Initialise(normalTroopSprite1);
	int x1, y1 = 0;
	m_pMap->GetTrackStartCoords(x1, y1);
	x1 -= (normalTroopSprite1->GetFrameWidth() * m_pBackBuffer->GetScaleX()) / 2.0;
	y1 -= (normalTroopSprite1->GetFrameHeight() * m_pBackBuffer->GetScaleY()) / 2.0;
	m_pNormalTroop1->SetPosition(x1, y1 + 10);
	m_pNormalTroop1->SetCurrentTile(0);
	m_pNormalTroop1->SetVerticalVelocity(100);
	m_pNormalTroop1->SetHorizontalVelocity(100);
	m_pMap->GetNextTarget(x1, y1, m_pNormalTroop1, Map::select::CENTER);
	m_pNormalTroop1->SetTarget(x1, y1, Map::select::CENTER);
	actingTroops.push_back(m_pNormalTroop1);
}

void 
Game::setAllTroopSpeed(int verticalVelocity, int horizontalVelocity){
	for (int i = 0; i < actingTroops.size(); i++){
		actingTroops.at(i)->SetVerticalVelocity(verticalVelocity);
		actingTroops.at(i)->SetHorizontalVelocity(horizontalVelocity);
	}
}

void 
Game::setAllTroopDamage(float damage){
	for (int i = 0; i < actingTroops.size(); i++){
		actingTroops.at(i)->SetDamage(damage);
	}
}
