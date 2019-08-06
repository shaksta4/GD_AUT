/*
* Developed by Dominic 14868762
*/
#ifndef __TROOPFACTORY_H__
#define __TROOPFACTORY_H__

#include "normaltroop.h"
#include "tanktroop.h"
#include "protectiontroop.h"
#include <vector>

class TroopFactory{
public:
	//static Troop* CreateTroop();
	static NormalTroop* CreateNormalTroop();
	static TankTroop* CreateTankTroop();
	static ProtectionTroop* CreateProtectionTroop();
protected:
private:

public:
protected:
private:

};

#endif __TROOPFACTORY_H__