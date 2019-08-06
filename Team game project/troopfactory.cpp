/*
* Developed by Dominic 14868762
*/
#include "troopfactory.h"
#include "troop.h"
#include "normaltroop.h"


NormalTroop*
TroopFactory::CreateNormalTroop(){
	NormalTroop* r = new NormalTroop();
	return r;
}

TankTroop*
TroopFactory::CreateTankTroop(){
	TankTroop* r = new TankTroop();
	return r;
}

ProtectionTroop*
TroopFactory::CreateProtectionTroop(){
	ProtectionTroop* r = new ProtectionTroop();
	return r;
}