#include "IniParser.h"
#include <fstream>

IniParser::IniParser()
{

}


IniParser::~IniParser()
{

}

bool 
LoadIniFile(const string& filename)
{
	ifstream myFile;
	myFile.open(filename);
	
	// TO IMPLEMENT
	return false;
}

string
GetValueAsString(const string& iniSection, const string& key)
{
	return NULL;
}

int
GetValueAsInt(const string& iniSeciton, const string& key)
{
	return NULL;
}

float
GetValueAsFloat(const string& iniSeciton, const string& key)
{
	return NULL;
}

bool 
GetValueAsBoolean(const string& iniSeciton, const string& key)
{
	return false;
}
