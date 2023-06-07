#pragma once
#include "Pickup.h"
#include "Aura.h"			//in progress
#include "DamageBoost.h"	//Klar modell
#include "damagebuff.h"		//ingen modell!
#include "healthbuff.h"		//ingen modell!
#include "healthpotion.h"	//Klar modell
#include "shield.h"			//in progress


class PickupHandler
{
private:
	
	XMFLOAT3 resetPosition = XMFLOAT3(0, 80.0f, 0);
	Aura* aura;
	DamageBoost* dBoost;
	DamageBuff* dBuff;
	HealthBuff* hBuff;
	HealthPotion* hPotion;
	Shield* shield;

public:

	PickupHandler(ID3D11Device* device);
	~PickupHandler();
	Pickup* spawnType(ID pickupID, XMFLOAT3 position);
	Pickup* spawnRandomType(XMFLOAT3 position);
	void removePickup(ID pickupID);
	void resetPickups();
};