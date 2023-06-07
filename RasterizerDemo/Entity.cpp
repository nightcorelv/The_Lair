#include "Entity.h"

Entity::Entity(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:id(ID::unassigned)
{
	position = Position;
	rotation = Rotation;
	this->mesh = Mesh(device, filename, false, 0, Position, Rotation, raycastType, whd);
}

Entity::~Entity()
{
	
}

ID Entity::getId() const

{
    return id;
}

XMFLOAT3 Entity::getPosition()
{
    return position;
}

XMFLOAT3 Entity::getRotation()
{
	return rotation;
}

void Entity::draw(ID3D11DeviceContext * immediateContext, ID3D11Buffer * CameraMTXBuffer)
{
	this->mesh.bindAndDrawIndexed(immediateContext, CameraMTXBuffer);
}

void Entity::update(ID3D11DeviceContext * immediateContext)
{
	this->mesh.updateMatrix(immediateContext);
}

Mesh* Entity::getMesh()
{
    return &mesh;
}


Entity::Entity(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:id(ID::unassigned), position(Position), rotation(Rotation),
	mesh(mesh, device, Position, Rotation, raycastType, whd)
{

}

//Entity::Entity(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
//	:id(ID::unassigned), position(Position), rotation(Rotation),
//	mesh(mesh, skelMesh, device, Position, Rotation, raycastType, whd)
//{
//
//}

Entity::Entity(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:id(ID::unassigned), position(Position), rotation(Rotation)
{
	this->mesh = Mesh(mesh, device, Position, Rotation, raycastType, whd);
	this->mesh.setTextureKey(mesh.material.texureFileName);
}

//Entity::Entity(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
//	:id(ID::unassigned), position(Position), rotation(Rotation)
//{
//	this->mesh = Mesh(mesh, device, Position, Rotation, raycastType, whd);
//	this->mesh.setTextureKey(mesh.material.texureFileName);
//}

void Entity::updateMesh(ID3D11DeviceContext* immediateContext)
{
    this->mesh.updateMatrix(immediateContext);
}

Entity::Entity()
{
}

void Entity::setPosition(XMFLOAT3 newPosition)
{
	position = newPosition;
    mesh.setPos(newPosition);
}

void Entity::setRotation(XMFLOAT3 newRotation)
{
	rotation = newRotation;
	this->mesh.setRot(newRotation);
}

void Entity::setID(ID id)
{
    this->id = id;
}

void Entity::increasePosition(float x, float y, float z)
{
	position = XMFLOAT3(position.x + x, position.y + y, position.z + z);
	mesh.setPos(position);
}

void Entity::increaseRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(rotation.x + x, rotation.y + y, rotation.z + z);
	mesh.setRot(rotation);
}

int Entity::getTile()
{
	return tile;
}

void Entity::setTile(float x, float y, int xTiles, int tileWidth)
{
	int tileNr = (int)((x / tileWidth) + 0.5);
	int temp = xTiles * (int)((y + 1.5) / tileWidth);
	tileNr += temp;
	tile = tileNr;
}

void Entity::makeVirtual()
{
}

bool Entity::getTeleported()
{
	return justTeleported;
}

void Entity::setTelepored(bool teleported)
{
	justTeleported = teleported;
}

void Entity::setIndex(int i)
{
	index = i;
}

int Entity::getIndex()
{
	return index;
}

//void Entity::animate(ID3D11DeviceContext* immediateContext, bool animated)
//{
//	if (this->mesh.getAnimState() == -1)
//	{
//		this->mesh.Animate(immediateContext, 0, false);
//	}
//	else
//	{
//		this->mesh.Animate(immediateContext, this->mesh.getAnimState(), animated);
//	}
//}

//Probably unnecessary
//void Entity::animate(ID3D11DeviceContext* immediateContext, int animState)
//{
//	if (setAnimState(animState))
//	{
//		this->mesh.Animate(immediateContext, getAnimState());
//	}
//}

//bool Entity::getUseAnim()
//{
//	return this->mesh.isAnimated();
//}

void Entity::setMesh(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation)
{
	this->mesh = Mesh(mesh, device, Position, Rotation);
	this->mesh.setTextureKey(mesh.getTextureKey());
}

void Entity::setMove(bool m)
{
	shouldMove = m;
}

bool Entity::getMove()
{
	return shouldMove;
}

void Entity::move(Entity* bellamy, Entity** cull, int size)
{
}