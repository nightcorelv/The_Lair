#include "HealthBuff.h"


//HealthBuff::HealthBuff(int hp, Mesh mesh)
//{
//	health = hp;
//	setMesh(mesh);
//}

HealthBuff::HealthBuff(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int hp)
	:Pickup(device, "../Meshes/tileCube", Position), health(hp)
{
	setID(ID::healthbuff);
}

HealthBuff::HealthBuff(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int hp)
	: Pickup(device, mesh, Position), health(hp)
{
	setID(ID::healthbuff);
}

int HealthBuff::getHealth() const
{
	return health;
}

bool HealthBuff::collision(Entity& entity)
{
	return false;
}

void HealthBuff::makeVirtual()
{
}
