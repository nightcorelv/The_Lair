#include "Temporary.h"

Temporary::Temporary(ID3D11Device * device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation)
	:Pickup(device, filename, Position)
{
}

Temporary::Temporary(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation)
	: Pickup(device, mesh, Position)
{
}

void Temporary::countdown(float timePassed)
{
	timer -= timePassed;
}

float Temporary::getTimeLeft()
{
	return timer;
}

void Temporary::resetTimer()
{
	timer = 20.0f;
}
