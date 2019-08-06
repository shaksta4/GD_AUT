#include "protectiontroop.h"
#include "entity.h"

ProtectionTroop::ProtectionTroop()
{
	this->armor = 2000;
}

ProtectionTroop::~ProtectionTroop()
{

}

int 
ProtectionTroop::getArmor(){
	return armor;
}

void
ProtectionTroop::getHurt(int hurt){
	if (armor > hurt){
		armor -= hurt;
	}
	else{
		m_health -= (hurt - armor);
	}
}

void 
ProtectionTroop::setArmor(int armor){
	this->armor = armor;
}