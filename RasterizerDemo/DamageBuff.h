#pragma once
#include "Pickup.h"

class DamageBuff : public Pickup
{
private:
	int boost;													//Keeps track of the extra damage output.

public:
	DamageBuff(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int dmgBoost = 1);
	DamageBuff(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int dmgBoost = 1);
	int getBoost() const;										//This pickup permanently boosts the player's damage.
	bool collision(Entity& entity);
	void makeVirtual();

protected:

};