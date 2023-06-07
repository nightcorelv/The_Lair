#pragma once
#include "Pickup.h"

class Temporary : public Pickup
{
private:
	float timer = 20.0f;//Keeps track on how much time is left until the pickup is no longer usable.

public:
	Temporary(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	Temporary(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	void countdown(float timePassed);//Counts down based on the time passed since the last frame.
	float getTimeLeft();//Returns the time left of the countdown.
	void resetTimer();
};