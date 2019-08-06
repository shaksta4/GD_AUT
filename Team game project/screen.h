#pragma once
/*
* Developed by Michael Jordan 14868336
*/

class BackBuffer;
class Map;

class Screen //Interface
{
	//Member Methods
public:
	
	virtual bool Initialise(BackBuffer* backBuffer) = 0;
	virtual bool DoGameLoop() = 0;
	virtual void Quit() = 0;

	enum ScreenState
	{
		DISPLAY,
		PUSH,
		POP,
		EXIT,
	};

	virtual Map* GetMap() = 0;
	virtual void SetMap(Map* map) = 0;
	virtual void Refresh() = 0;
protected:
	virtual void Process(float deltaTime) = 0;
	virtual void Draw(BackBuffer& backBuffer) = 0;
private:
	//Member Data:
public:
	ScreenState m_currentState;
protected:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
private:
	
};