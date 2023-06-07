#include "Door.h"
#include "Time.h"

Door::Door(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:Movable(device, "../Meshes/testDoor", Position, Rotation, 0, whd), startHeight(Position.y), isRaised(false), raising(false), isLowering(false)
{
	setID(ID::door);
}

Door::Door(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
	:Movable(device, mesh, Position, Rotation, 0, whd), startHeight(Position.y), isRaised(false), raising(false), isLowering(false)
{
	setID(ID::door);
}

void Door::raiseDoor(float tileHeight, float raisedHeight)
{
	//Variables
	XMFLOAT3 currentPos = this->getPosition();


	if (currentPos.y >= tileHeight + 5 + raisedHeight)
	{
		this->isRaised = true;
		raising = false;
	}
	else
	{
		raising = true;
		this->setPosition(XMFLOAT3(currentPos.x, currentPos.y + 0.25f, currentPos.z));
	}
}

void Door::lowerDoor(float tileHeight)
{
	//Variables
	XMFLOAT3 currentPos = this->getPosition();


	if (currentPos.y <= tileHeight)
	{
		this->isRaised = false;
		isLowering = false;
	}
	else
	{
		isLowering = true;
		this->setPosition(XMFLOAT3(currentPos.x, currentPos.y - 0.25f, currentPos.z));
	}
}

bool Door::isDoorRaised()
{
	return this->isRaised;
}

bool Door::isDoorRaising()
{
	return this->raising;
}

bool Door::isDoorLowering()
{
	return this->isLowering;
}

void Door::reset()
{
	this->isRaised = false;
	this->raising = false;
	this->isLowering = false;
}
