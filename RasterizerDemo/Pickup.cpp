#include "Pickup.h"

Pickup::Pickup(ID3D11Device* device, string filename, XMFLOAT3 Position, float* whd, XMFLOAT3 Rotation)
	:Entity(device, filename, Position, Rotation, 0, whd)
{
	itemAudio.load(L"../Sounds/itemAudio.wav");
	picked = false;
}

Pickup::Pickup(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, float* whd, XMFLOAT3 Rotation)
	: Entity(device, mesh, Position, Rotation, 0, whd)
{
	itemAudio.load(L"../Sounds/itemAudio.wav");
	picked = false;
}

Pickup::Pickup()
	:Entity()
{
	itemAudio.load(L"../Sounds/itemAudio.wav");
}

void Pickup::pickUp()
{
	picked = true;
}

bool Pickup::isPicked()
{
	return picked;
}

bool Pickup::playerOverlap(Entity* player)
{
	bool overlapping = false;

	float point[5][3];
	point[0][0] = player->getPosition().x + player->getMesh()->getOffsetX();
	point[0][1] = player->getPosition().y + (player->getMesh()->getOffsetY() / 2);
	point[0][2] = player->getPosition().z + player->getMesh()->getOffsetZ();
	point[1][0] = player->getPosition().x + player->getMesh()->getOffsetX() + player->getMesh()->getWidth();
	point[1][1] = player->getPosition().y + (player->getMesh()->getOffsetY() / 2);
	point[1][2] = player->getPosition().z + player->getMesh()->getOffsetZ() + player->getMesh()->getDepth();
	point[2][0] = player->getPosition().x + player->getMesh()->getOffsetX() - player->getMesh()->getWidth();
	point[2][1] = player->getPosition().y + (player->getMesh()->getOffsetY() / 2);
	point[2][2] = player->getPosition().z + player->getMesh()->getOffsetZ() + player->getMesh()->getDepth();
	point[3][0] = player->getPosition().x + player->getMesh()->getOffsetX() + player->getMesh()->getWidth();
	point[3][1] = player->getPosition().y + (player->getMesh()->getOffsetY() / 2);
	point[3][2] = player->getPosition().z + player->getMesh()->getOffsetZ() - player->getMesh()->getDepth();
	point[4][0] = player->getPosition().x + player->getMesh()->getOffsetX() - player->getMesh()->getWidth();
	point[4][1] = player->getPosition().y + (player->getMesh()->getOffsetY() / 2);
	point[4][2] = player->getPosition().z + player->getMesh()->getOffsetZ() - player->getMesh()->getDepth();

	for (int i = 0; i < 5; i++)
	{
		if (getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth() >= point[i][0] && getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth() <= point[i][0] &&
			getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight() >= point[i][1] && getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() <= point[i][1] &&
			getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth() >= point[i][2] && getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth() <= point[i][2])
		{
			overlapping = true;
		}
	}
	return overlapping;
}

void Pickup::rotateItem()
{
	increaseRotation(0,0.007,0);
}

void Pickup::playAudio()
{
	itemAudio.play();
}
