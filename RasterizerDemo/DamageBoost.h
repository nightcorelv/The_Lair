#pragma once
#include "Temporary.h"

class DamageBoost : public Temporary
{
private:
	int boost;											//Keeps track of the extra damage output.

public:
	DamageBoost(ID3D11Device * device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0,0,0), int boost = 1);
	DamageBoost(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int boost = 1);
	int getBoost() const;								//Returns amount of extra damage given by this pickup.
	void setBoost(int originalDamage);					//This pickup should double the player's current damage and also saves the amount of damage to make the removal easier later.
	bool collision(Entity& entity);						//needed for other than making base class abstract?
	void makeVirtual();

protected:

};