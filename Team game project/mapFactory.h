#pragma once
/*
* Developed by Michael Jordan 14868336
*/

//Local includes
#include "map.h"

//Library includes:
#include <string>

//Forward Declarations
//class Map;
class BackBuffer;

class MapFactory
{
	//Member Methods
public:
	static Map* GenerateMap(BackBuffer& backBuffer, Map::Style style, float diffScale);
protected:

private:

	//Member Data
public:

protected:
private:

};