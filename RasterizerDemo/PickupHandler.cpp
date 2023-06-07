#include "PickupHandler.h"

PickupHandler::PickupHandler(ID3D11Device * device)
{
	LspMesh AURA;		AURA.sceneFileName = "../LspMeshes/scroll";			LoadMesh(AURA);		aura = new Aura(device, AURA, resetPosition);
	LspMesh DBOOST;		DBOOST.sceneFileName = "../LspMeshes/StrPotion";	LoadMesh(DBOOST);	dBoost = new DamageBoost(device, DBOOST, resetPosition);
	LspMesh DBUFF;		DBUFF.sceneFileName = "../LspMeshes/DmgRune";		LoadMesh(DBUFF);	dBuff = new DamageBuff(device, DBUFF, resetPosition);
	LspMesh HBUFF;		HBUFF.sceneFileName = "../LspMeshes/hpRune";		LoadMesh(HBUFF);	hBuff = new HealthBuff(device, HBUFF, resetPosition);
	LspMesh HPOTION;	HPOTION.sceneFileName = "../LspMeshes/HPPotion";	LoadMesh(HPOTION);	hPotion = new HealthPotion(device, HPOTION, resetPosition);
	LspMesh SHIELD;		SHIELD.sceneFileName = "../LspMeshes/shield";		LoadMesh(SHIELD);	shield = new Shield(device, SHIELD, resetPosition);
}

PickupHandler::~PickupHandler()
{
	delete aura;
	delete dBoost;
	delete dBuff;
	delete hBuff;
	delete hPotion;
	delete shield;
}

Pickup * PickupHandler::spawnType(ID pickupID, XMFLOAT3 position)
{
	Pickup * ptrToReturn = nullptr;
	if (pickupID == ID::aura)
	{
		ptrToReturn = aura;
		aura->setPosition(position);
	}
	else if (pickupID == ID::damageboost)
	{
		ptrToReturn = dBoost;
		dBoost->setPosition(position);
	}
	else if (pickupID == ID::damagebuff)
	{
		ptrToReturn = dBuff;
		dBuff->setPosition(position);
	}
	else if (pickupID == ID::healthbuff)
	{
		ptrToReturn = hBuff;
		hBuff->setPosition(position);
	}
	else if (pickupID == ID::healthpotion)
	{
		ptrToReturn = hPotion;
		hPotion->setPosition(position);
	}
	else if (pickupID == ID::shield)
	{
		ptrToReturn = shield;
		shield->setPosition(position);
	}
	return ptrToReturn;
}

Pickup * PickupHandler::spawnRandomType(XMFLOAT3 position)
{
	Pickup * ptrToReturn = nullptr;
	int pickupID = rand() % 7;
	if (pickupID == 0)
	{
		ptrToReturn = aura;
		aura->setPosition(position);
	}
	else if (pickupID == 1)
	{
		ptrToReturn = dBoost;
		dBoost->setPosition(position);
	}
	else if (pickupID == 2)
	{
		ptrToReturn = dBuff;
		dBuff->setPosition(position);
	}
	else if (pickupID == 3)
	{
		ptrToReturn = hBuff;
		hBuff->setPosition(position);
	}
	else if (pickupID == 4)
	{
		ptrToReturn = hPotion;
		hPotion->setPosition(position);
	}
	else if (pickupID == 5)
	{
		ptrToReturn = shield;
		shield->setPosition(position);
	}
	return ptrToReturn;
}

void PickupHandler::removePickup(ID pickupID)
{
	if (pickupID == ID::aura)
	{
		aura->setPosition(resetPosition);
	}
	else if (pickupID == ID::damageboost)
	{
		dBoost->setPosition(resetPosition);
	}
	else if (pickupID == ID::damagebuff)
	{
		dBuff->setPosition(resetPosition);
	}
	else if (pickupID == ID::healthbuff)
	{
		hBuff->setPosition(resetPosition);
	}
	else if (pickupID == ID::healthpotion)
	{
		hPotion->setPosition(resetPosition);
	}
	else if (pickupID == ID::shield)
	{
		shield->setPosition(resetPosition);
	}
}

void PickupHandler::resetPickups()
{
	dBoost->resetTimer();
	shield->resetTimer();
}
