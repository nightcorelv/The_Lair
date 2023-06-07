#pragma once
#include "Pickup.h"

class HealthBuff : public Pickup
{
private:
	int health;																		//The amount of health this buff permanently adds to Player.

public:

	//HealthBuff(int hp, Mesh mesh);
	HealthBuff(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int hp = 1);
	HealthBuff(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int hp = 1);
	int getHealth() const;															//Increases the player's max health permanently.
	bool collision(Entity& entity);
	void makeVirtual();

protected:

};