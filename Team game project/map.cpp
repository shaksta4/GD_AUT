/*
* Developed by Michael Jordan 14868336
*/

//Local Includes
#include "map.h"
#include "tile.h"
#include "backbuffer.h"
#include "sprite.h"
#include "tower.h"
#include "iniParser.h"
#include "troop.h"

//Library Includes
#include <cmath>
#include <string>

Map::Map()
	: m_tileWidth(0)
	, m_tileHeight(0)
	, m_tilesX(4)
	, m_tilesY(3)
	, m_diffScale(0)
	, m_pVillage(0)
{ 

}

Map::~Map()
{
	//TODO Delete all tiles
	for (int i = 0; i < m_tilesX * m_tilesY; i++)
	{
		delete m_pTiles[i];
	}

	std::vector<Sprite* >::iterator flairIter = m_flairs.begin();
	while (flairIter != m_flairs.end())
	{
		Sprite* temp = (*flairIter);
		flairIter = m_flairs.erase(flairIter);
		delete temp;
	}

	std::vector<Coords* >::iterator towerIter = m_pTowerPos.begin();
	while (flairIter != m_flairs.end())
	{
		Coords* temp = (*towerIter);
		towerIter = m_pTowerPos.erase(towerIter);
		delete temp;
	}

	delete[] m_pTiles;
	m_pTiles = 0;
}

bool
Map::Initialize(Style style, BackBuffer& backBuffer, float diff, int screenWidth, int screenHeight)
{
	m_tileStyle = style;
	m_tileWidth = screenWidth / m_tilesX;
	m_tileHeight = screenHeight / m_tilesY;
	m_pTiles = new Tile*[m_tilesX * m_tilesY];
	m_diffScale = diff;
	for (int i = 0; i < m_tilesX * m_tilesY; i++)
	{
		m_pTiles[i] = new Tile();
	}

	bool track = GenerateTrack();
	bool tiles = GenerateTiles(backBuffer);
	bool towers = GenerateTowers(backBuffer);
	bool flair = GenerateFlair(backBuffer);

	return track && tiles && towers && flair;
}

bool 
Map::GenerateTrack()
{
	//Creates the track
	m_startX = 0;
	m_startY = (rand() % (m_tilesY)); //0 to Y-1

	m_endX = m_tilesX-1;
	m_endY = (rand() % (m_tilesY)); //0 to Y-1

	GetTile(0, m_startY)->SetStartCoords(0, (m_startY + 0.5)*m_tileHeight);
	GenerateTrack(0, m_startY);
	return true;
}

//Filter cases: 
// - The track never goes backwards or on a tile already visited.
// - Avoid going the wrong way.
void 
Map::GenerateTrack(int x, int y)
{
	Tile* current = GetTile(x, y);
	current->SetCenterCoords((x + 0.5)*m_tileWidth, (y + 0.5)*m_tileHeight);
	if (x == m_endX && y == m_endY)
	{
		current->SetExitCoords(m_endX*m_tileWidth + m_tileWidth, (m_endY + 0.5)*m_tileHeight);
		AddToTrack(current);
		return; //BREAK STEP
	}
	int checkX;
	int checkY;
	int baseX;
	int baseY;
	current->GetCenterCoords(baseX, baseY);
	std::vector<std::pair<int, Tile*>> cases;
	std::vector<std::pair<int, Tile*>>::iterator iter = cases.begin();
	if (y > 0)
	{
		Tile* temp = GetTile(x, y - 1);
		temp->GetCenterCoords(checkX, checkY);
		if (checkX != baseX && !(m_endY > y && x == m_tilesX - 1)) //Filer case
		{
			Tile* temp = GetTile(x, y - 1);
			iter = cases.insert(iter, std::pair<int, Tile*>(0, temp));
		}
	}
	if (y < m_tilesY - 1)
	{
		Tile* temp = GetTile(x, y + 1);
		temp->GetCenterCoords(checkX, checkY);
		if (checkX != baseX && !(m_endY < y && x == m_tilesX - 1)) //Filer case
		{
			iter = cases.insert(iter, std::pair<int, Tile*>(1, temp));
		}
	}
	if (x < m_tilesX - 1)
	{
		Tile* temp = GetTile(x + 1, y);
		temp->GetCenterCoords(checkX, checkY);
		iter = cases.insert(iter, std::pair<int, Tile*>(2, temp));
	}

	int select = (rand() % cases.size()); //0 to size
	switch (cases.at(select).first)
	{
	case 0: //UP
		baseY -= m_tileHeight * 0.5;
		GenerateTrack(x, y - 1); //Next Step
		break;
	case 1: //DOWN
		baseY += m_tileHeight * 0.5;
		GenerateTrack(x, y + 1); //Next Step
		break;
	case 2: //RIGHT
		baseX += m_tileWidth * 0.5;
		GenerateTrack(x + 1, y); //Next Step
		break;
	}
	cases.at(select).second->SetStartCoords(baseX, baseY);
	current->SetExitCoords(baseX, baseY);
	AddToTrack(current);
	return;
}

bool
Map::GenerateTiles(BackBuffer& backBuffer)
{
	//Selects the sprite files
	IniParser::GetInstance().LoadIniFile("assets\\tile.ini");
	std::string style = StyleToString();
	for (int i = 0; i < m_tilesX; i++)
	{
		for (int j = 0; j < m_tilesY; j++)
		{
			Tile* current = GetTile(i, j);
			int x1;
			int y1;
			int x2;
			int y2;
			Sprite* sprite = 0;
			current->GetStartCoords(x1, y1);
			current->GetExitCoords(x2, y2);
			if (x1 == -1 && x2 == -1)
			{
				//Clear Tile
				sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "clear").c_str());
				current->Initialize(sprite);
				AddToCleared(current);
			}
			else
			{
				if (x1 == x2)
				{
					//Vertical Tile
					sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "straightV").c_str());
					current->Initialize(sprite);
				}
				else if (y1 == y2)
				{
					//Horizontal Tile
					sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "straightH").c_str());
					current->Initialize(sprite);
				}
				else
				{
					//All Turn Tiles:
					int floorX = i * m_tileWidth;
					int ceilingX = (i + 1) * m_tileWidth;
					int ceilingY = j * m_tileHeight;
					int floorY = (j + 1) * m_tileHeight;
					if (x1 == floorX)
					{
						//West to...
						if (y2 == floorY)
						{
							//... South
							sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "turnL").c_str());
							sprite->SetAngle(180);
							current->Initialize(sprite);
						}
						else if (y2 == ceilingY)
						{
							//... North
							sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "turnJ").c_str());
							current->Initialize(sprite);
						}
					}
					else if (x2 == ceilingX)
					{
						//... to East
						if (y1 == floorY)
						{
							//South
							sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "turnJ").c_str());
							sprite->SetAngle(180);
							current->Initialize(sprite);
						}
						else if (y1 == ceilingY)
						{
							//North
							sprite = backBuffer.CreateSprite(IniParser::GetInstance().GetValueAsString(style, "turnL").c_str());
							current->Initialize(sprite);
						}
					}
				}
			}
			if (sprite == 0)
			{
				return false;
			}
			else
			{
				int coordX = i * m_tileWidth;
				int coordY = j * m_tileHeight;
				if (sprite->GetAngle() > 0)
				{
					coordX--;
					coordY--;
				}
				current->SetSpriteCoords(coordX, coordY);
			}
		}
	}
	
	return true;
}

bool
Map::GenerateTowers(BackBuffer& backBuffer)
{
	//For every increase by 1, a new tower is created.
	int towerCount = static_cast<int>(m_diffScale - static_cast<int>(m_diffScale) % 1); //currently 2
	int divide = (m_TilesUsed.size() - 2.0) / towerCount; //Minus first tile and last tile
	int track = 1; //1 = minimum
	std::vector<Coords* >::iterator iter = m_pTowerPos.begin();
	for (int t = 0; t < towerCount; t++)
	{
		track = (rand() % divide) + track; //track+1 to track+divide
		Tile* current = m_TilesUsed.at(track);
		Coords* temp = new Coords;
		current->GetCenterCoords(temp->x, temp->y);
		iter = m_pTowerPos.insert(iter, temp);
		track++;
	}
	return true;
}

bool
Map::GenerateFlair(BackBuffer& backBuffer)
{
	std::vector<Tile* >::iterator iter = m_TilesNotUsed.begin();
	std::vector<Sprite* >::iterator flairIter = m_flairs.begin();
	while (iter != m_TilesNotUsed.end())
	{
		int flairCount = (rand() % 5); //0 - 4
		int minX;
		int minY;
		(*iter)->GetSpriteCoords(minX, minY);
		for (int f = 1; f <= flairCount; f++)
		{
			int width = (f * (m_tileWidth / flairCount));
			int height = (f * (m_tileHeight / flairCount));
			int maxX = minX + width;
			int maxY = minY + height;
			int coordX = (rand() % width) + minX; //minX to minX + width
			int coordY = (rand() % height) + minY; //minY to minY + height
			Sprite* sprite = backBuffer.CreateSprite("assets\\stone.png");
			sprite->SetX(coordX);
			sprite->SetY(coordY);
			flairIter = m_flairs.insert(flairIter, sprite);
		}
		iter++;
	}
	return true;
}

void
Map::Draw(BackBuffer& backBuffer)
{
	for (int i = 0; i < m_tilesX; i++)
	{
		for (int j = 0; j < m_tilesY; j++)
		{
			GetTile(i, j)->Draw(backBuffer);
		}
	}
	DrawFlair(backBuffer);
}

void
Map::DrawFlair(BackBuffer& backBuffer)
{
	std::vector<Sprite* >::iterator iter = m_flairs.begin();
	while (iter != m_flairs.end())
	{
		(*iter)->Draw(backBuffer);
		iter++;
	}
}


/* GetTile(int c, int r)
 * Returns the tile at the memory coordinates of w column and h row
 */
Tile*
Map::GetTile(int c, int r)
{
	if (c >= 0 && c < m_tilesX && r >= 0 && r < m_tilesY)
	{
		return m_pTiles[(c * m_tilesY) + r];
	}
	else
	{
		return NULL;
	}
}

/* GetTile(int t)
* Returns the tile within the tiles used vector, at t position.
*/

Tile*
Map::GetTile(int t)
{
	return m_TilesUsed.at(t);
}

void
Map::GetNextTarget(int& tx, int& ty, Troop* troop, Map::select type)
{
	int x = troop->GetPositionX() + 25;
	int y = troop->GetPositionY() + 25;
	int t = troop->GetCurrentTile();
	Tile* current = GetTile(t);

	if (type == Map::CENTER)
	{
		current->GetCenterCoords(tx, ty);
	}
	else if (type == Map::EXIT)
	{
		current->GetExitCoords(tx, ty);
	}
}

void
Map::GetTrackStartCoords(int& x, int& y)
{
	x = 0;
	y = (m_startY + 0.5) * m_tileHeight;
}

bool
Map::HasReachedTarget(Troop* troop, Map::select type)
{
	int x = troop->GetPositionX() + troop->GetWidth() / 2.0;
	int y = troop->GetPositionY() + troop->GetHeight() / 2.0;
	int t = troop->GetCurrentTile();
	
	Tile* current = GetTile(t);

	int tx;
	int ty;

	if (type == Map::CENTER)
	{
		current->GetCenterCoords(tx, ty);
		return (x == tx && y == ty);
	}
	else if (type == Map::EXIT)
	{
		current->GetExitCoords(tx, ty);
		if (t == m_TilesUsed.size() - 1 && (x == tx && y == ty))
		{
			troop->SetHalted(true);
			return false;
		}
		else
		{
			return (x == tx && y == ty);
		}
	}
	
}

void
Map::AddToTrack(Tile* used)
{
	std::vector<Tile* >::iterator iter = m_TilesUsed.begin();
	m_TilesUsed.insert(iter, used);
}

void
Map::AddToCleared(Tile* unUsed)
{
	std::vector<Tile* >::iterator iter = m_TilesNotUsed.begin();
	m_TilesNotUsed.insert(iter, unUsed);
}

std::vector<Map::Coords*>
Map::GetAllTowerCoords() const
{
	return m_pTowerPos;
}

std::string 
Map::StyleToString()
{
	switch (m_tileStyle)
	{
	case PLAIN:
		return "plain";
	case SNOWY:
		return "snowy";
	case DESERT:
		return "desert";
	case HELLSCAPE:
		return "hellscape";
	}
}

bool 
Map::IsTroopAtTheEnd(Troop* troop)
{
	int Ex = 0; 
	int Ey = 0;
	m_TilesUsed.at(m_TilesUsed.size() - 1)->GetExitCoords(Ex, Ey);
	int Tx = 0;
	int Ty = 0;
	Map::select type;
	troop->GetTarget(Tx, Ty, type);
	bool condX = Ex == Tx && Tx == static_cast<int>(troop->GetPositionX() + troop->GetWidth() / 2.0);
	bool condY = Ey == Ty && Ty == static_cast<int>(troop->GetPositionY() + troop->GetHeight() / 2.0);
	return condY && condX;
}

void
Map::SetVictoryStatus(bool status)
{
	m_pVillage->IsDead();
}

bool 
Map::GetVictoryStatus()
{
	if (m_pVillage != 0)
	{
		return m_pVillage->IsDead();
	}
	return false;
}

void
Map::SetID(int id)
{
	m_id = id;
}

int
Map::GetID()
{
	return m_id;
}

void 
Map::SetUpVillageCoordinates(Tower* village)
{
	int x = 0;
	int y = 0;
	m_TilesUsed.at(m_TilesUsed.size() - 1)->GetExitCoords(x, y);

	x -= village->GetWidth() / 2.0;
	y -= village->GetHeight() / 2.0;
	village->SetPosition(x, y);

	m_pVillage = village;
}