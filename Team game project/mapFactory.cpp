/*
* Developed by Michael Jordan 14868336
*/
//Local Includes
#include "mapFactory.h"
#include "map.h"
#include "sprite.h"
#include "backbuffer.h"

//Library Includes
#include <string>

Map* 
MapFactory::GenerateMap(BackBuffer& backBuffer, Map::Style style, float diffScale)
{
	//TODO Generate a Map
	Map* map = new Map();
	map->Initialize(style, backBuffer, diffScale, backBuffer.GetWidth(), backBuffer.GetHeight());
	return map;
}