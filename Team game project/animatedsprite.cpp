// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
	: m_frameWidth(0)
	, m_timeElapsed(0.0f)
	, m_currentFrame(0)
	, m_currentRow(0)
	, m_paused(false)
	, m_loop(true)
	, m_animating(true)
	, m_xValues(0)
{

}

AnimatedSprite::~AnimatedSprite()
{

}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	m_frameWidth = 0;

	m_loop = true;
	m_paused = false;
	m_animating = true;
	if (!m_xValues.empty()){
		m_currentFrame = m_xValues[0];
	}
	m_currentFrameNo = 0;
	Sprite::Initialise(texture);

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrame(int x)
{
	m_xValues.push_back(x);
}

void
AnimatedSprite::Process(float deltaTime)
{
	if (!m_paused)
	{
		m_timeElapsed += deltaTime;
		//FIX THIS PROPERLY
		if (m_timeElapsed > m_frameSpeed)
		{
			m_currentFrame = m_xValues[m_currentFrameNo];
			m_timeElapsed = 0;
			if (!(m_xValues.size()<=1)){
				if (m_loop){
					if (m_currentFrameNo >= (static_cast<int>(m_xValues.size()) - 1)){
						m_currentFrameNo = 0;
					}
					else
					{
						m_currentFrameNo++;
					}
				}
				else{
					if (m_currentFrameNo < static_cast<int>(m_xValues.size())){
						m_currentFrameNo++;
					}
					else{
						m_animating = false;
					}
				}
			}
		}
	}
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	backbuffer.DrawAnimatedSprite(*this);
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void
AnimatedSprite::SetFrameWidth(int w)
{
	m_frameWidth = w;
}

int
AnimatedSprite::GetFrameWidth()
{
	return m_frameWidth;
}

void
AnimatedSprite::SetFrameHeight(int h)
{
	m_frameHeight = h;
}

int
AnimatedSprite::GetFrameHeight()
{
	return m_frameHeight;
}

int
AnimatedSprite::GetCurrentFrame()
{
	return m_currentFrame;
}
void
AnimatedSprite::SetCurrentFrame(int currentFrame)
{
	m_currentFrame = currentFrame;
}

void
AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}

int
AnimatedSprite::GetCurrentFrameNo()
{
	return m_currentFrameNo;
}
void
AnimatedSprite::SetCurrentFrameNo(int currentFrameNo)
{
	m_currentFrameNo = currentFrameNo;
	m_currentFrame = m_xValues[m_currentFrameNo];
}
int
AnimatedSprite::GetNoOfFrames()
{
	return m_xValues.size();
}
void
AnimatedSprite::SetCurrentRow(int row)
{
	m_currentRow = row;
}
int
AnimatedSprite::GetCurrentRow()
{
	return m_currentRow;
}