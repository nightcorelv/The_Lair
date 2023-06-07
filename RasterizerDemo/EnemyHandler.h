#pragma once
#include "Enemy.h"
#include "Skull.h"
#include "Pile.h"
#include "Necromancer.h"
#include "Wraith.h"

class EnemyHandler
{
private:
	XMFLOAT3 resetPosition = XMFLOAT3(0, 80.0f, 0);
	vector<Skull*> skullArr;
	vector<Pile*> pileArr;
	vector<Wraith*> wraithArr;
	vector<Necromancer*> necroArr;
	int arrSize;
	int skIndex = 0;
	int piIndex = 0;
	int wrIndex = 0;
	int necIndex = 0;

public:

	EnemyHandler(ID3D11Device* device, int maxArrSize = 5);
	~EnemyHandler();
	Enemy* getEnemy(ID enemyID);

	void removeEnemy(ID enemyID); 
	void removeWraith();
	bool isEnemyFree(ID  enemyid);
	void reviveEnemies();

};