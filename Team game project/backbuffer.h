// COMP710 GP 2D Framework 2017
#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

// Forward Declarations:
class AnimatedSprite;
class SoundManager;
class TextureManager;
class Texture;
class Sprite;
struct SDL_Window;
struct SDL_Renderer;

#include <string>

using namespace std;

class BackBuffer
{
	//Member Methods:
public:
	BackBuffer();
	~BackBuffer();

	bool Initialise(int width, int height);

	void Clear();
	void Present();

	void SetClearColour(unsigned char r, unsigned char g, unsigned char b);

	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b);
	void SetTextureColour(Texture* texture, unsigned char r, unsigned char g, unsigned char b); //Added by Michael
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void DrawSprite(Sprite& sprite);
	void DrawAnimatedSprite(AnimatedSprite& sprite);
	void DrawRectangle(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);

	void CreateSounds();

	float GetScaleX() const;
	float GetScaleY() const;

	Sprite* CreateSprite(const char* pcFilename);
	AnimatedSprite* CreateAnimatedSprite(const char* pcFilename);
	
	int GetWidth() const;
	int GetHeight() const;

	void PlaySound(string string, bool loop);
protected:
	void LogSDLError();

private:
	BackBuffer(const BackBuffer& backBuffer);
	BackBuffer& operator=(const BackBuffer& backBuffer);

	//Member Data:
public:

protected:
	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	
	float m_scaleValX;
	float m_scaleValY;

	int m_prefWidth;
	int m_prefHeight;

	int m_screenWidth;
	int m_screenHeight;

	unsigned char m_clearRed;
	unsigned char m_clearGreen;
	unsigned char m_clearBlue;
	
private:

};

#endif // __BACKBUFFER_H__
