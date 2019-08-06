//Made by Jack Boyd 14860648

// This include:
#include "soundmanager.h"

// Local includes:
#include "sound.h"
#include "logmanager.h"

#include <fmod.hpp>

SoundManager::SoundManager()
: m_pSystem(0)
{

}

SoundManager::~SoundManager()
{
	ReleaseSounds();
}

bool
SoundManager::Initialise()
{
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		LogManager::GetInstance().Log("Sound System failed to create!");
		return false;
	}
	int driverCount = 0;
	m_pSystem->getNumDrivers(&driverCount);
	if (driverCount == 0)
	{
		LogManager::GetInstance().Log("No Audio Drivers!");
		return false;
	}
	// 36 channels
	m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);
	CreateSounds();
}
void
SoundManager::CreateSounds()
{
	FMOD::Sound* backgroundTrack;
	FMOD::Sound* strike;
	FMOD::Sound* dead;
	FMOD::Sound* stoneTowerFired;
	FMOD::Sound* woodenTowerFired;
	FMOD::Sound* towerDead;
	FMOD::Sound* spawnTroop;

	m_pSystem->createSound("assets\\background.wav", FMOD_DEFAULT, 0, &backgroundTrack);
	backgroundTrack->setMode(FMOD_LOOP_NORMAL);
	backgroundTrack->setLoopCount(-1);
	m_pSounds["BackgroundTrack"] = backgroundTrack;

	m_pSystem->createSound("assets\\hurt.wav", FMOD_DEFAULT, 0, &strike);
	strike->setMode(FMOD_LOOP_OFF);
	strike->setLoopCount(-1);
	m_pSounds["TroopStrikesTower"] = strike;

	m_pSystem->createSound("assets\\pain.wav", FMOD_DEFAULT, 0, &dead);
	dead->setMode(FMOD_LOOP_OFF);
	dead->setLoopCount(-1);
	m_pSounds["TroopDies"] = dead;

	m_pSystem->createSound("assets\\arrowfired.wav", FMOD_DEFAULT, 0, &stoneTowerFired);
	stoneTowerFired->setMode(FMOD_LOOP_OFF);
	stoneTowerFired->setLoopCount(-1);
	m_pSounds["StoneTowerFires"] = stoneTowerFired;

	m_pSystem->createSound("assets\\rockfired.wav", FMOD_DEFAULT, 0, &woodenTowerFired);
	woodenTowerFired->setMode(FMOD_LOOP_OFF);
	woodenTowerFired->setLoopCount(-1);
	m_pSounds["WoodenTowerFires"] = woodenTowerFired;

	m_pSystem->createSound("assets\\towerexplosion.wav", FMOD_DEFAULT, 0, &towerDead);
	towerDead->setMode(FMOD_LOOP_OFF);
	towerDead->setLoopCount(-1);
	m_pSounds["TowerDead"] = towerDead;

	m_pSystem->createSound("assets\\troopspawn.wav", FMOD_DEFAULT, 0, &spawnTroop);
	spawnTroop->setMode(FMOD_LOOP_OFF);
	spawnTroop->setLoopCount(-1);
	m_pSounds["SpawnTroop"] = spawnTroop;
}

void
SoundManager::PlaySound(string string, bool loop)
{
	m_pSystem->playSound(m_pSounds[string], 0, false, 0);
}

void
SoundManager::ReleaseSounds()
{
	FMOD::Sound* release;
	release = m_pSounds["BackgroundTrack"];
	release->release();
	release = m_pSounds["TroopStrikesTower"];
	release->release();
	release = m_pSounds["TroopDies"];
	release->release();
	release = m_pSounds["StoneTowerFires"];
	release->release();
	release = m_pSounds["WoodenTowerFires"];
	release->release();
	release = m_pSounds["TowerDead"];
	release->release();
	release = m_pSounds["SpawnTroop"];
	release->release();
	m_pSystem->release();
}