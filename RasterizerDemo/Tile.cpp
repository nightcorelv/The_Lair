#include "Tile.h"

Tile::Tile(ID3D11Device * device, XMFLOAT3 Position, XMFLOAT3 Rotation, int tileType)
	:position(Position), rotation(Rotation), activeIndex(tileType)
{
	for (int i = 0; i < nrOfMeshes; i++)
	{
		this->meshArr.push_back(Mesh(device, this->filenames[i], false, 0, Position, Rotation, 0));
	}
}

void Tile::bindAndDrawIndexed(ID3D11DeviceContext * immediateContext, ID3D11Buffer *& viewProjBuffer)
{
	this->meshArr.at(this->activeIndex).bindAndDrawIndexed(immediateContext, viewProjBuffer);
}

void Tile::updateTile(ID3D11DeviceContext * immediateContext)
{
	this->meshArr.at(this->activeIndex).setRotation(this->rotation);
	this->meshArr.at(this->activeIndex).setPos(this->position);
	this->meshArr.at(this->activeIndex).updateMatrix(immediateContext);
}

void Tile::setTile(int tileType)
{
	this->activeIndex = tileType;
}

void Tile::setPos(XMFLOAT3 pos)
{
	this->position = pos;
	for (int i = 0; i < this->nrOfMeshes; i++)
	{
		this->meshArr.at(i).setPos(pos);
	}
	
}

void Tile::setRotation(XMFLOAT3 rot)
{
	this->rotation = rot;
	this->meshArr.at(this->activeIndex).setRotation(rot);
}

XMMATRIX Tile::getMatrix()
{
	return this->meshArr.at(this->activeIndex).getMatrix();
}

XMFLOAT3 Tile::getPosition()
{
	return this->position;
}

XMFLOAT3 Tile::getRotation()
{
	return this->rotation;
}

Mesh* Tile::getMesh()
{
	return &meshArr.at(activeIndex);
}

int Tile::getType()
{
	return this->activeIndex;
}
