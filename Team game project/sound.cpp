//Made by Jack Boyd 14860648

#include "sound.h"
#include "logmanager.h"

#include <fmod.hpp>
#include <string>

using namespace std;

Sound::Sound()
: m_pSound(0)
{

}

Sound::~Sound()
{

}

FMOD::Sound*
Sound::GetSound()
{
	return m_pSound;
}

string 
Sound::GetName()
{
	return m_name;
}

void
Sound::SetName(string name)
{
	m_name = name;
}