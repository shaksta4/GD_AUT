#include "Mask.h"

/*
This class is the Mask for the buttons. It holds a vector of buttons and acts as the anchor point
for moving and adding/removing buttons. It automatically places the buttons within the mask. 

*/

Mask::Mask() :
m_pButtons(0)
, m_x(0)
, m_y(0)
, m_h(0)
, m_w(0)
, m_size(0)
{

}


Mask::~Mask()
{

}

void
Mask::Initialise(std::vector<Button*> myButtons, int width, int x, int y, bool hasCoord) //<- Added by Michael
{
	m_pButtons = myButtons;
	m_h = m_pButtons[0]->GetHeight();
	m_w = width;
	m_x = x;
	m_y = y;
	if (!hasCoord) //<- Added by Michael
	{
		std::vector<Button*>::iterator buttonIter = m_pButtons.begin();
		while (buttonIter != m_pButtons.end())
		{
			(*buttonIter)->SetX(x + ((*buttonIter)->GetButtonID() * (*buttonIter)->GetWidth()));
			(*buttonIter)->SetY(y);
			buttonIter++;
		}
	}
	
}

void
Mask::AddButton(Button* myButton)
{
	m_pButtons.push_back(myButton);
	m_size++;
}

void
Mask::RemoveButton(Button* myButton)
{
	m_pButtons.pop_back();
	m_size--;
}

vector<Button*>
Mask::GetButtons()
{
	return m_pButtons;
}

int
Mask::GetX()
{
	return m_x;
}

int
Mask::GetY()
{
	return m_y;
}

int
Mask::GetWidth()
{
	return m_w;
}

int
Mask::GetHeight()
{
	return m_h;
}