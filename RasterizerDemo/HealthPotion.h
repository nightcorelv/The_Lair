#pragma once
#include "Temporary.h"

class HealthPotion : public Pickup
{
private:
	int health;												//The amount of health points this potion should give Player.

public:
	//HealthPotion(int hp, Mesh mesh);
	HealthPotion(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int hp = 5);
	HealthPotion(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int hp = 5);
	int getHealth() const;										//Increases the player's current health points.
	bool collision(Entity& entity);
	void makeVirtual();

protected:

};