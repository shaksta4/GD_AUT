#pragma once
/*
 * Developed by Michael Jordan 14868336
 */

//Forward Declarations
class Tile;
class BackBuffer;
class Tower;
class Troop;
class Sprite;

//Library includes
#include <vector>
class Map
{
	//Member Methods
public:
	Map();
	~Map();

	static enum Style //Style of the map
	{
		PLAIN, SNOWY, DESERT, HELLSCAPE
	}; //TODO allow different tile sets in the generateTiles

	bool Initialize(Style style, BackBuffer& backBuffer, float diff, int screenWidth, int screenHeight);
	
	void Draw(BackBuffer& backBuffer);

	static enum select //Selection type for next target
	{
		CENTER, EXIT
	};
	
	struct Coords
	{
		int x;
		int y;
		Coords()
		{

		};
	};

	void GetNextTarget(int& tx, int& ty, Troop* troop, Map::select type);
	void GetTrackStartCoords(int& x, int& y);
	bool HasReachedTarget(Troop* troop, Map::select type);
	std::vector<Coords*> GetAllTowerCoords() const;
	bool IsTroopAtTheEnd(Troop* troop);

	void SetVictoryStatus(bool status);
	bool GetVictoryStatus();
	void SetID(int i);
	int GetID();

	void SetUpVillageCoordinates(Tower* village);
protected:
	bool GenerateTrack();
	bool GenerateTiles(BackBuffer& backBuffer);
	bool GenerateTowers(BackBuffer& backBuffer);
	bool GenerateFlair(BackBuffer& backBuffer);
	Tile* GetTile(int c, int r);
	Tile* GetTile(int t);
	//Tile* GetNextTile(int& w, int& h);
private:
	void GenerateTrack(int x, int y);
	void AddToTrack(Tile* used);
	void AddToCleared(Tile* unUsed);
	std::string StyleToString();
	void DrawFlair(BackBuffer& backBuffer);
	//Memeber Data
public:

protected:
	Style m_tileStyle;
	float m_diffScale;

	int m_id;

	float m_tileWidth;
	float m_tileHeight;

	int m_tilesX;
	int m_tilesY;

	float m_startX;
	float m_startY;
	float m_endX;
	float m_endY;

	Tile** m_pTiles;
	std::vector<Tile* > m_TilesUsed;
	std::vector<Tile* > m_TilesNotUsed;
	std::vector<Coords* > m_pTowerPos;
	std::vector<Sprite* > m_flairs;
	Tower* m_pVillage;
private:

};