#include "tanktroop.h"
#include "entity.h"

TankTroop::TankTroop()
{
	this->SetHealth(this->GetHealth() * 3);
}

TankTroop::~TankTroop()
{

}