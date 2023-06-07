#pragma once
#include "Mesh.h"

class Tile
{
private:
	
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	int activeIndex;
	int nrOfMeshes = 3;
	vector<Mesh> meshArr;
	string filenames[3]{ "../Meshes/tileCube", "../Meshes/g", "../Meshes/ramp" }; //kolla på att fixa filsökvägen...

public:
	Tile(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int tileType = 0);
	void bindAndDrawIndexed(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer);
	void updateTile(ID3D11DeviceContext * immediateContext);
	void setTile(int tileType);
	void setPos(XMFLOAT3 pos);
	void setRotation(XMFLOAT3 rot);
	XMMATRIX getMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	Mesh* getMesh();
	int getType();
};