#pragma once
#include "Enemy.h"
#include"Player.h"

class Skull : public Enemy
{
private:

public:
	//Skull(int damage, float attackTime, int health, Mesh* mesh);
	Skull(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float atkTimer = 20);
	Skull(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float atkTimer = 20);
	//Skull(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float atkTimer = 20);
	virtual void move(Entity* bellamy, Entity** cull, int size) override;
	void moveThowards(Player* charakter, Entity** cull, int size);
	bool collision(Entity& entity);

protected:

};