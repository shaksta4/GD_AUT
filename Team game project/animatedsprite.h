#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

#include "sprite.h"

#include <vector>

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	// Methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture);
	void AddFrame(int x);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetFrameSpeed(float f);
	void SetFrameWidth(int w);
	int GetFrameWidth();
	void SetFrameHeight(int h);
	int GetFrameHeight();
	int GetCurrentFrame();
	void SetCurrentFrame(int currentFrame);
	int GetCurrentFrameNo();
	void SetCurrentFrameNo(int currentFrameNo);
	int GetNoOfFrames();
	void SetCurrentRow(int row);
	int GetCurrentRow();

	void Pause();
	bool IsPaused();

	bool IsAnimating();
	void StartAnimating();

	bool IsLooping();
	void SetLooping(bool b);

protected:

private:

	// Fields:
public:

protected:
	std::vector<int> m_xValues;

	float m_frameSpeed;
	int m_frameWidth;
	int m_frameHeight;

	float m_timeElapsed;
	int m_currentFrame;
	int m_currentFrameNo;
	int m_currentRow;

	bool m_paused;
	bool m_loop;
	bool m_animating;

private:

};

#endif // __ANIMATEDSPRITE_H__
