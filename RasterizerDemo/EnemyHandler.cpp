#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(ID3D11Device* device, int maxArrSize)
	:arrSize(maxArrSize)
{
	//Initiate arrays
	for (int i = 0; i < this->arrSize; i++)
	{
		float* whdSkull = new float[6]{ 1, 1.5, 1, 0, 1, 0 };
		float* whdPile = new float[6]{ 2, 2, 2, 0, 1.65f, 0 };
		float* whdNecro = new float[6]{ 1, 2, 1, 0, 0, 0 };

		LspMesh skull;		skull.sceneFileName = "../LspMeshes/Skull";						LoadMesh(skull);		skullArr.push_back(new Skull(device, skull, resetPosition, XMFLOAT3(0, 0, 0), 0, whdSkull, 1, 1, 0.2f, 30));
		LspMesh pile;		pile.sceneFileName = "../LspMeshes/Pile";						LoadMesh(pile);			pileArr.push_back(new Pile(device, pile, resetPosition, XMFLOAT3(0, 0, 0), 0, whdPile, 15/*1*/, 1, 2.5f,30));
		LspMesh wraith;		wraith.sceneFileName = "../LspMeshes/Wraith";					LoadMesh(wraith);		wraithArr.push_back(new Wraith(device, wraith, resetPosition, XMFLOAT3(0, 0, 0), 0, nullptr, 10/*1*/, 1, 0.025f));
		LspMesh necromancer;necromancer.sceneFileName = "../LspMeshes/necroStillPose";		LoadMesh(necromancer);	necroArr.push_back(new Necromancer(device, necromancer, resetPosition, XMFLOAT3(0, 0, 0), 4, whdNecro, 40/*1*/, 1, 0.25f, 30));
	}
}

EnemyHandler::~EnemyHandler()
{
	for (int i = 0; i < arrSize; i++)
	{
		delete skullArr.at(i);
		delete	pileArr.at(i);
		delete	wraithArr.at(i);
		delete necroArr.at(i);
	}
}

Enemy * EnemyHandler::getEnemy(ID enemyID)
{
	//Assumes ptr is actually added to external array thus Index++.
	Enemy * ptrToReturn = nullptr;
	if (enemyID == ID::skull && skIndex < arrSize)
	{
		ptrToReturn = skullArr.at(skIndex);
		skIndex++;
	}
	else if (enemyID == ID::pile && piIndex < arrSize)
	{
		ptrToReturn = pileArr.at(piIndex);
		piIndex++;
	}
	else if (enemyID == ID::wraith && wrIndex < arrSize)
	{
		ptrToReturn = wraithArr.at(wrIndex);
		wrIndex++;
	}
	else if (enemyID == ID::necromancer && necIndex < arrSize)
	{
		ptrToReturn = necroArr.at(necIndex);
		necIndex++;
	}
	return ptrToReturn;
}

void EnemyHandler::removeEnemy(ID enemyID)
{
	//Sets enemy position (so it cant be seen, ie enemy becomes inactive) & decreses index.
	if (enemyID == ID::skull && skIndex <= arrSize)
	{
		skullArr.at(skIndex-1)->setPosition(resetPosition);
		skIndex--;
	}
	else if (enemyID == ID::pile && piIndex <= arrSize)
	{
		pileArr.at(piIndex-1)->setPosition(resetPosition);
		piIndex--;
	}
	else if (enemyID == ID::wraith && wrIndex <= arrSize)
	{
		wraithArr.at(wrIndex-1)->setPosition(resetPosition);
		wrIndex--;
	}
	else if (enemyID == ID::necromancer && necIndex <= arrSize)
	{
		necroArr.at(necIndex-1)->setPosition(resetPosition);
		necIndex--;
	}
}
void EnemyHandler::removeWraith()
{

	vector<Wraith*> temp;
	int index;
	for (size_t i = 0; i < wraithArr.size(); i++)
	{
		if (wraithArr.at(i)->getHealth() > 0)
		{
			temp.push_back(wraithArr.at(i));
		}
		else
		{
			index = i;
		}
	}
	temp.push_back(wraithArr.at(index));

	wraithArr = temp;

}


bool EnemyHandler::isEnemyFree(ID enemyid)
{
	bool isFree = false;

	switch (enemyid)
	{
	case ID::skull:
		if (skIndex < arrSize)
		{
			isFree = true;
		}
			break;
	case ID::wraith:
		if (wrIndex < arrSize)
		{
			isFree = true;
		}
		break;
	case ID::pile:
		if (piIndex < arrSize)
		{
			isFree = true;
		}
		break;

	}
	return isFree;
}

void EnemyHandler::reviveEnemies()
{
	for (size_t i = 0; i < skullArr.size(); i++)
	{
		skullArr.at(i)->SetDead(false);
		skullArr.at(i)->setHpToMax();
	}
	for (size_t i = 0; i < pileArr.size(); i++)
	{
		pileArr.at(i)->SetDead(false);
		pileArr.at(i)->setHpToMax();
	}
	for (size_t i = 0; i < wraithArr.size(); i++)
	{
		wraithArr.at(i)->SetDead(false);
		wraithArr.at(i)->setHpToMax();
	}
	for (size_t i = 0; i < necroArr.size(); i++)
	{
		necroArr.at(i)->SetDead(false);
		necroArr.at(i)->setHpToMax();
	}
	skIndex = 0;
	piIndex = 0;
	wrIndex = 0;
	necIndex = 0;

}
