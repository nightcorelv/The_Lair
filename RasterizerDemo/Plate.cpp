#include "Plate.h"

Plate::Plate(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, float* whd)
	:Movable(device, "../Meshes/PuzzelBoxPlatePurple", Position, Rotation, 0, whd)
{
	setID(ID::plate);
}

Plate::Plate(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, float* whd)
	: Movable(device, mesh, Position, Rotation, 0, whd)
{
	setID(ID::plate);
}

void Plate::overlapCheck(Entity** cull, int used)
{
	bool overlapping = false;

	for (int i = 0; i < used; i++)
	{
		float point[5][3];
		point[0][0] = cull[i]->getPosition().x + cull[i]->getMesh()->getOffsetX();
		point[0][1] = cull[i]->getPosition().y + cull[i]->getMesh()->getOffsetY();
		point[0][2] = cull[i]->getPosition().z + cull[i]->getMesh()->getOffsetZ();
		point[1][0] = cull[i]->getPosition().x + cull[i]->getMesh()->getOffsetX() + cull[i]->getMesh()->getWidth();
		point[1][1] = cull[i]->getPosition().y + cull[i]->getMesh()->getOffsetY();
		point[1][2] = cull[i]->getPosition().z + cull[i]->getMesh()->getOffsetZ() + cull[i]->getMesh()->getDepth();
		point[2][0] = cull[i]->getPosition().x + cull[i]->getMesh()->getOffsetX() - cull[i]->getMesh()->getWidth();
		point[2][1] = cull[i]->getPosition().y + cull[i]->getMesh()->getOffsetY();
		point[2][2] = cull[i]->getPosition().z + cull[i]->getMesh()->getOffsetZ() + cull[i]->getMesh()->getDepth();
		point[3][0] = cull[i]->getPosition().x + cull[i]->getMesh()->getOffsetX() + cull[i]->getMesh()->getWidth();
		point[3][1] = cull[i]->getPosition().y + cull[i]->getMesh()->getOffsetY();
		point[3][2] = cull[i]->getPosition().z + cull[i]->getMesh()->getOffsetZ() - cull[i]->getMesh()->getDepth();
		point[4][0] = cull[i]->getPosition().x + cull[i]->getMesh()->getOffsetX() - cull[i]->getMesh()->getWidth();
		point[4][1] = cull[i]->getPosition().y + cull[i]->getMesh()->getOffsetY();
		point[4][2] = cull[i]->getPosition().z + cull[i]->getMesh()->getOffsetZ() - cull[i]->getMesh()->getDepth();

		for (int i = 0; i < 5; i++)
		{
			if (getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth() >= point[i][0] && getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth() <= point[i][0] &&
				getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight() + 2 >= point[i][1] && getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() <= point[i][1] &&
				getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth() >= point[i][2] && getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth() <= point[i][2])
			{
				overlapping = true;
			}
		}
	}
	
	isActivated = overlapping;
}

bool Plate::getOverlap()
{
	return isActivated;
}
