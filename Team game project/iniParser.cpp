//Local includes
#include "IniParser.h"

//Library includes
#include <string>
#include <fstream>
#include <sstream>
#include <map>

//Static instances:
IniParser* IniParser::sm_pInstance = 0;
std::map<std::string, std::string>* m_Content = 0;

IniParser&
IniParser::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new IniParser();
	}

	return (*sm_pInstance);
}

void
IniParser::DestroyInstance()
{
	if(sm_pInstance != 0) 
		delete sm_pInstance;
}

IniParser::IniParser()
	: m_Content(0)
{
	
}

IniParser::~IniParser()
{
	if (sm_pInstance->m_Content != 0)
	{
		sm_pInstance->m_Content->clear();
		delete m_Content;
	}
}

bool 
IniParser::LoadIniFile(const std::string& filename)
{
	if (m_Content == 0) 
	{
		m_Content = new std::map<std::string, std::string>();
	}
	else 
	{
		m_Content->clear();
	}
	std::ifstream file;
	std::string line;
	std::string section;
	file.open(filename);
	if (file.is_open()) //Check if the file exists
	{
		while (std::getline(file, line))
		{
			if (line.empty()) 
			{
				//Ignore because it is empty.
			}
			else if (line.at(0) == '[')
			{
				section = line.substr(1, line.size() - 2);
			}
			else if (line.at(0) == ';')
			{
				//Ignore because it is a comment.
			}
			else
			{
				int temp = line.find(" ");
				std::string key = line.substr(0, temp);
				std::string value = line.substr(temp + 3, line.size() - 1);
				m_Content->insert(std::pair<std::string, std::string>(section + " | " + key, value));
			}
		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

std::string
IniParser::GetValueAsString(const std::string& iniSection, const std::string& key)
{
	return m_Content->at(iniSection + " | " + key);
}

int
IniParser::GetValueAsInt(const std::string& iniSection, const std::string& key)
{
	std::string temp = m_Content->at(iniSection + " | " + key);
	std::stringstream geek(temp);
	int val = 0;
	geek >> val;
	return val;
}

float
IniParser::GetValueAsFloat(const std::string& iniSection, const std::string& key)
{
	std::string temp = m_Content->at(iniSection + " | " + key);
	return strtof(temp.data(), NULL);
}

bool
IniParser::GetValueAsBoolean(const std::string& iniSection, const std::string& key)
{
	std::string temp = m_Content->at(iniSection + " | " + key);
	if (temp == "true") 
	{
		return true;
	}
	else if(temp == "false")
	{
		return false;
	}
}
