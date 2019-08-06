#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

//Made by Jack Boyd 14860648


#include <fmod.hpp>
#include <array>
#include <map>
#include <string>

using namespace std;

class Sound;

class SoundManager
{

public:
	SoundManager();
	~SoundManager();

	bool Initialise();
	void CreateSounds();
	void PlaySound(string string, bool loop);
	void ReleaseSounds();
protected:
private:

public:
protected:
private:
	map<string, FMOD::Sound*> m_pSounds;
	FMOD::System* m_pSystem;
	FMOD::Channel* m_pChannel;
	FMOD_RESULT m_result;
};

#endif // __SOUNDMANAGER_H__