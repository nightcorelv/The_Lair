#include "DamageBuff.h"

DamageBuff::DamageBuff(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int dmgBoost)
	:Pickup(device, "../Meshes/tileCube", Position), boost(dmgBoost)
{
    setID(ID::damagebuff);
}

DamageBuff::DamageBuff(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int dmgBoost)
    : Pickup(device, mesh, Position), boost(dmgBoost)
{
    setID(ID::damagebuff);
}

int DamageBuff::getBoost() const
{
    return boost;
}

bool DamageBuff::collision(Entity& entity)
{
    return false;
}

void DamageBuff::makeVirtual()
{
}
