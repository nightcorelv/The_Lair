#pragma once
#include "Entity.h"
#include "Audio.h"

class Pickup : public Entity
{
private:
	bool picked;			//Keeps track of if the pickup is on the ground or taken by the player.
	Audio itemAudio;

public:

	Pickup(ID3D11Device* device, string filename, XMFLOAT3 Position, float* whd = new float[6] {2, 2, 2, 0, 0, 0}, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	Pickup(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, float* whd = new float[6] {2, 2, 2, 0, 0, 0}, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	Pickup();
	virtual void makeVirtual() = 0;
	void pickUp();
	bool isPicked();
	bool playerOverlap(Entity* player);
	void rotateItem();
	void playAudio();

protected:

};