#include "DamageBoost.h"

DamageBoost::DamageBoost(ID3D11Device * device, XMFLOAT3 Position, XMFLOAT3 Rotation, int boost)
	:Temporary(device, "../Meshes/STpotion", Position, Rotation), boost(boost)
{
    setID(ID::damageboost);
}

DamageBoost::DamageBoost(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int boost)
    : Temporary(device, mesh, Position, Rotation), boost(boost)
{
    setID(ID::damageboost);
}

int DamageBoost::getBoost() const
{
    return boost;
}

void DamageBoost::setBoost(int originalDamage)
{
    boost = originalDamage;
}

bool DamageBoost::collision(Entity& entity)
{
    return false;
}

void DamageBoost::makeVirtual()
{
}
