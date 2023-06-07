#pragma once
#include "Enemy.h"
#include "Player.h"
#include"Projectile.h"
#include "projectileHandeler.h"

class Wraith : public Enemy
{
private:
	float chargeTime;										//The amount of time it has already charged.
	float maxChargeTime;									//The amount of time it takes to charge a shot.
	float internalTimer;
	projectileHandeler* PH = nullptr;
	void charge();											//Before shooting, the wraith should charge it's shot for a certain amount of time.
	int indexProjectile = -1;

public:
	//Wraith(int damage, float attackTime, float chargeTime, float maxTime, int health, Mesh& mesh);
	Wraith(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 0.5f, float chargeTime = 0.5f, float maxTime = 0.5f);
	Wraith(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 0.5f, float chargeTime = 0.5f, float maxTime = 0.5f);
	//Wraith(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 0.5f, float chargeTime = 0.5f, float maxTime = 0.5f);
	~Wraith();
	projectileHandeler* getProjectileHandeler();
	virtual void move(Entity* bellamy, Entity** cull, int size, Entity** cull2, int size2); //moves by teleporting and then turning towards the player. how?
	void shoot(Player*& charakter);											//Shoots toward the player, creates a projectile (maybe the func should return a projectile?)
	bool collision(Entity& entity);
	void behaviour(Player*& charakter, Entity** cull, int size, Entity** cull2, int size2);
	void teleport(Player*& charakter, Entity** cull, int size);
	int returnProjectileIndex();

protected:

};