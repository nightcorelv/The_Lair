#pragma once
#include "Enemy.h"
#include "Player.h"
#include"Entity.h"
#include<vector>
#include"Skull.h"

class Pile : public Enemy
{
private:
	int bonesRequired;								//The amount of bones required for the pile to grow.							
	std::vector<Entity*> assimilatedSkuls;           //vector that contains pointers to dead absorbed skulls
	float degrees = 0;

public:
	//Pile(int damage, float attackTime, int health, Mesh& mesh);
	Pile(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 0.5f, float attackTime = 0.5f, int requiredToGrow = 3);
	Pile(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 0.5f, float attackTime = 0.5f, int requiredToGrow = 3);
	void absorb(Entity* skull);					//Absorb bone when passing over/through bones.
	//bool readyToGrow() const;						//Has the pile picked up enough bones?
	//void grow(Entity* skull);									//Increase damage (and size?)
	virtual void move(Entity* bellamy, Entity** cull, int size) override;
	bool overlap(Entity* player);				//The player should be able to walk through a pile, but will take damage and be slowed down.
	void moveThowards(Player*& charakter, Entity** cull, int size);
	bool collision(Entity& entity);
	void moveSkulls();
	float hitDetection(Player*& player, XMFLOAT3*& dir);
	void clearAssim();



protected:

};