#pragma once
#include "Temporary.h"

class Shield : public Temporary
{
private:
	int health;												//The amount of health the shield starts out with. Decreases with every attack.
	int defense;												//The amount of damage the shield decreases each attack.

public:
	//Shield(int hp, int shield, Mesh mesh);
	Shield(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int hp = 1, int Pshield = 0);
	Shield(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int hp = 1, int Pshield = 0);
	int decreaseDmg(int dmg);								//Decreases incoming damage toward the player.
	bool usedUp() const;									//The shield only has a certain amount of health.
	bool collision(Entity& entity);
	void makeVirtual();

protected:

};