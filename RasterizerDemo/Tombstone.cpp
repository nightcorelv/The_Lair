#include "Tombstone.h"


//Tombstone::Tombstone(Mesh mesh)
//{
//	setMesh(mesh);
//}


Tombstone::Tombstone(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation)
	:Movable(device, "../Meshes/tombstoneDummy", Position, Rotation)
{
	setID(ID::tombstone);
}

Tombstone::Tombstone(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation)
	: Movable(device, mesh, Position, Rotation)
{
	setID(ID::tombstone);
}

bool Tombstone::collision(Entity& entity)
{
	return false;
}
