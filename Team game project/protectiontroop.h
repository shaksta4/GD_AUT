#ifndef __PROTECTIONTROOP_H__
#define __PROTECTIONTROOP_H__

#include "map.h"
#include "normaltroop.h"

class ProtectionTroop : public NormalTroop{

public:
	ProtectionTroop();
	~ProtectionTroop();
	int getArmor();
	void setArmor(int armor);
	void getHurt(int hurt);

protected:
	int armor;

};

#endif __PROTECTIONTROOP_H__