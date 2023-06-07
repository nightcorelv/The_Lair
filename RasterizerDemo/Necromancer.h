#pragma once
#include "Enemy.h"
#include"Player.h"
#include <time.h>
#include<vector>
#include"Projectile.h"
#include "projectileHandeler.h"

class projectileHandeler;
class Necromancer : public Enemy
{
private:

	float internalTimer = 0;
	int behaviourint = 0;
	projectileHandeler* PH = nullptr;
	bool spawning = false;
	
public:
	//Necromancer(int damage, float attackTime, int health, Mesh& mesh);
	Necromancer(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 3, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 0.5f);
	Necromancer(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 3, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 0.5f);
	//Necromancer(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 3, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 0.5f);
	void moveThowards(Player*& charakter, Entity** cull, int size);
	void shoot(Player*& charakter);  //Should be used when the necormancer is above half health.							//Should be used when the necromancer is below half health.
	void heal(int hp);							//Should be used in combination with punch.
	bool collision(Entity& entity);
	void behaviour(Player*& charakter, Entity** cull, int size);  //shoses at intervals a random behaviour, move, shot etc
	float GetTimer();                    //gets the necromancers internal timer
	void setTimer(float time);            // sets timer
	void addTime(float time);              //add time
	void setBehaviour(int i);              // sets behaviour int
	int getBehaviour();                     // gets behaviour int
	bool Spawning();
	void hasspawned();
	ID GetenemyID();
	std::vector<Projectile*> getProjectileArray();
	projectileHandeler* getProjectileHandeler();
	~Necromancer();
	virtual void move(Entity* bellamy, Entity** cull, int size) override;


protected:

};