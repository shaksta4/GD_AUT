#pragma once

#include <string>
#include <map>

using namespace std; 

class IniParser
{
public:
	IniParser();
	~IniParser();

	bool LoadIniFile(const string& filename);

	string GetValueAsString(const string& iniSection, const string& key);
	int GetValueAsInt(const string& iniSeciton, const string& key);
	float GetValueAsFloat(const string& iniSeciton, const string& key);
	bool GetValueAsBoolean(const string& iniSeciton, const string& key);


private:
	//map<string&, string&> iniMap;

};

