#include "Movable.h"

Movable::Movable(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:Entity(device, mesh, Position, Rotation, raycastType, whd)
{
}

//Movable::Movable(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
//	:Entity(device, mesh, skelMesh, Position, Rotation, raycastType, whd)
//{
//}

Movable::Movable(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	: Entity(device, mesh, Position, Rotation, raycastType, whd)
{
}

//Movable::Movable(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
//	: Entity(device, mesh, Position, Rotation, raycastType, whd)
//{
//}

Movable::Movable(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:Entity(device, filename, Position, Rotation, raycastType, whd)
{
}

void Movable::move()
{
}
