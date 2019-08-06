#pragma once

#include <vector>
#include "Button.h"

using namespace std;

class Mask
{
//Class methods
public:
	Mask();
	~Mask();

	void Initialise(std::vector<Button* > myButtons, int width, int x, int y, bool hasCoord);
	void AddButton(Button* myButton);
	void RemoveButton(Button* myButton);

	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();

	vector<Button*> GetButtons();

private: 

protected:


//Member data
public:

private:
	vector<Button* > m_pButtons;

	int m_x;
	int m_y;
	int m_h;
	int m_w;

	int m_size;

protected:

};

