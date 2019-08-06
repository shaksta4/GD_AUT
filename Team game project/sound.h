#ifndef __SOUND_H__
#define __SOUND_H__

//Made by Jack Boyd 14860648

#include <fmod.hpp>
#include <string>

using namespace std;
class Sound
{

public:
	Sound();
	~Sound();

	FMOD::Sound* GetSound();
	string GetName();
	void SetName(string name);
protected:
private:

public:
	string m_name;
	FMOD::Sound* m_pSound;
protected:
private:

};

#endif // __SOUND_H__