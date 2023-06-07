#pragma once
#include "Entity.h"

class Movable : public Entity
{
public:
	Movable(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	//Movable(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Movable(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	//Movable(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Movable(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	void move();
};
