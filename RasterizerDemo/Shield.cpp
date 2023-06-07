#include "Shield.h"

//Shield::Shield(int hp, int shield, Mesh mesh)
//{
//    this->health = hp;
//    this->shield = shield;
//    setMesh(mesh);
//}

Shield::Shield(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int hp, int Pshield)
	:Temporary(device, "../Meshes/zShield", Position, Rotation), health(hp), defense(Pshield)
{
    setID(ID::shield);

}

Shield::Shield(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int hp, int Pshield)
    :Temporary(device, mesh, Position, Rotation), health(hp), defense(Pshield)
{
    setID(ID::shield);

}

int Shield::decreaseDmg(int dmg)
{
    int decreased = dmg;

    if (dmg < defense)
    {
        int tmp = defense - dmg;
        if (health < tmp)
        {
            decreased -= health;
        }
        else
        {
            decreased -= tmp;
        }
    }
    else
    {
        if (health < defense)
        {
            decreased -= health;
        }
        else
        {
            decreased -= defense;
        }
    }

    health -= (dmg - decreased);
    return decreased;
}

bool Shield::usedUp() const
{
    return health <= 0;
}

bool Shield::collision(Entity& entity)
{
    return false;
}

void Shield::makeVirtual()
{
}
