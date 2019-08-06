#pragma once

//Library includes:
#include <string>
#include <map>

class IniParser
{
	//Member methods:
public:
	static IniParser& GetInstance();
	static void DestroyInstance();
	~IniParser();

	bool LoadIniFile(const std::string& filename);

	std::string GetValueAsString(const std::string& iniSection, const std::string& key);
	int GetValueAsInt(const std::string& iniSection, const std::string& key);
	float GetValueAsFloat(const std::string& iniSection, const std::string& key);
	bool GetValueAsBoolean(const std::string& iniSection, const std::string& key);
protected:

private:
	IniParser();
	IniParser(const IniParser& logManager);
	IniParser& operator=(const IniParser& logManager);

	//Member data:
public:

protected:
	static IniParser* sm_pInstance;
	std::map<std::string, std::string>* m_Content;

private:

};