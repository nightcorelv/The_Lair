#include "HealthPotion.h"


//HealthPotion::HealthPotion(int hp, Mesh mesh)
//{
//    health = hp;
//    setMesh(mesh);
//}


HealthPotion::HealthPotion(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int hp)
	:Pickup(device, "../Meshes/HPpotion", Position), health(hp)
{
    setID(ID::healthpotion);
}

HealthPotion::HealthPotion(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int hp)
    : Pickup(device, mesh, Position), health(hp)
{
    setID(ID::healthpotion);
}

int HealthPotion::getHealth() const
{
    return health;
}

bool HealthPotion::collision(Entity& entity)
{
    return false;
}

void HealthPotion::makeVirtual()
{
}
