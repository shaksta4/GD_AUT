// COMP710 GP 2D Framework 2017

// This include:
#include "backbuffer.h"

// Local includes:
#include "animatedsprite.h"
#include "logmanager.h"
#include "soundmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "texture.h"

// Library includes:
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pSoundManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_prefWidth(0)
, m_prefHeight(0)
, m_clearRed(0x00)
, m_clearGreen(0x00)
, m_clearBlue(0x00)
{

}

BackBuffer::~BackBuffer()
{
	delete m_pTextureManager;
	m_pTextureManager = 0;

	delete m_pSoundManager;
	m_pSoundManager = 0;

	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = 0;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = 0;

	IMG_Quit();
	SDL_Quit();
}

bool 
BackBuffer::Initialise(int width, int height)
{
	m_prefWidth = width;
	m_prefHeight = height;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("COMP710 2D Game Framework", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN);
		SDL_MaximizeWindow(m_pWindow);
		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_SHOWN);
		SDL_DisplayMode m_DM;
		SDL_GetCurrentDisplayMode(0, &m_DM);
		m_screenWidth = m_DM.w;
		m_screenHeight = m_DM.h;

		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED); 

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	//Added by Michael and Jack
	m_scaleValX = static_cast<float>(m_screenWidth) / m_prefWidth;
	m_scaleValY = static_cast<float>(m_screenHeight) / m_prefHeight;

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	m_pSoundManager = new SoundManager();
	assert(m_pSoundManager);
	m_pSoundManager->Initialise();

	return (true);
}

void 
BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void
BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}
void
BackBuffer::SetTextureColour(Texture* texture, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetTextureColorMod(texture->GetTexture(), r, g, b); //Added by Michael
}
void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void 
BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = sprite.GetWidth() * m_scaleValX;
	dest.h = sprite.GetHeight() * m_scaleValY;
	
	SDL_Point pivot;
	pivot.x = dest.w / 2.0;
	pivot.y = dest.h / 2.0;

	SDL_RenderCopyEx(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest, sprite.GetAngle(), &pivot, SDL_FLIP_NONE);
}

void
BackBuffer::DrawAnimatedSprite(AnimatedSprite& sprite)
{
	SDL_Rect srect;
	SDL_Rect dest;

	srect.x = sprite.GetCurrentFrame();
	srect.y = sprite.GetCurrentRow();
	srect.w = sprite.GetFrameWidth();
	srect.h = sprite.GetFrameHeight();

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = sprite.GetFrameWidth() * m_scaleValX;
	dest.h = sprite.GetFrameHeight() * m_scaleValY;

	SDL_Point pivot;
	pivot.x = dest.w / 2.0;
	pivot.y = dest.h / 2.0;

	SDL_RenderCopyEx(m_pRenderer, sprite.GetTexture()->GetTexture(), &srect, &dest, sprite.GetAngle(), &pivot, SDL_FLIP_NONE);

}
void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;
	
	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(m_pRenderer, &fillRect);
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void 
BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite* 
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}


AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void 
BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}

int 
BackBuffer::GetWidth() const
{
	return m_screenWidth;
}

int 
BackBuffer::GetHeight() const
{
	return m_screenHeight;
}

float 
BackBuffer::GetScaleX() const
{
	return m_scaleValX;
}

float 
BackBuffer::GetScaleY() const
{
	return m_scaleValY;
}
void
BackBuffer::PlaySound(string string, bool loop)
{
	m_pSoundManager->PlaySound(string, loop);
}