#pragma once
#include "Movable.h"

class Door : public Movable
{
private:
	bool isRaised;
	bool raising;
	bool isLowering;
	float startHeight;
public:
	Door(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Door(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	void raiseDoor(float tileHeight, float raisedHeight);
	void lowerDoor(float tileHeight);
	bool isDoorRaised();
	bool isDoorRaising();
	bool isDoorLowering();
	void reset();
};