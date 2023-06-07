#include "Enemy.h"

Enemy::Enemy(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float atkTimer, float jumpHeight)
	:Character(device, filename, Position, Rotation, raycastType, whd, health, damage, speed, atkTimer), attackDistance(5.0f)
{
	jumpStrength = jumpHeight;
}

Enemy::Enemy(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float atkTimer, float jumpHeight)
	: Character(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, atkTimer), attackDistance(5.0f)
{
	jumpStrength = jumpHeight;
}

//Enemy::Enemy(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float atkTimer, float jumpHeight)
//	: Character(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, atkTimer), attackDistance(5.0f)
//{
//	jumpStrength = jumpHeight;
//}

void Enemy::move()
{
}

//Move toward character's position (mainly used for player)
void Enemy::followCharacter(Character& character)
{
	//Move toward a character's position (mostly used for player)
}

//Calculates the speed/arch when jumping
void Enemy::handleJump()
{
	if (jumpSpeed > 0 && !lockJump)
	{
		increasePosition(0, jumpSpeed, 0);
		jumpSpeed -= gravity;
	}
	else
	{
		lockJump = false;
	}
}

//Set if can jump
void Enemy::setJump(bool jump)
{
	canJump = jump;
}

//Check if can jump
bool Enemy::getJump()
{
	return canJump;
}

//Stop jumping
void Enemy::stopJump()
{
	jumpSpeed = 0;
	canJump = false;
	lockJump = true;
}

//Check distance of ray hit
float Enemy::hitDetection(Player*& player, XMFLOAT3*& dir)
{
	float minT = -1;
	XMFLOAT3* pos = player->getAtkRays(dir);
	for (int i = 0; i < 9; i++)
	{
		bool xTest = false;
		bool yTest = false;
		bool zTest = false;
		float txMin = -1;
		float txMax = -1;
		float tyMin = -1;
		float tyMax = -1;
		float tzMin = -1;
		float tzMax = -1;

		//test x
		if (dir[i % 3].x == 0)
		{
			if ((getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth()) <= pos[i].x && (getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth()) >= pos[i].x)
			{
				xTest = true;
			}
		}
		else
		{
			float k = getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth() - pos[i].x;
			txMin = k / dir[i % 3].x;
			k = getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth() - pos[i].x;
			txMax = k / dir[i % 3].x;
			if (txMin > txMax)
			{
				float temp = txMin;
				txMin = txMax;
				txMax = temp;
			}
		}

		//test y
		if (dir[i % 3].y == 0)
		{
			if ((getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight()) <= pos[i].y && (getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight()) >= pos[i].y)
			{
				yTest = true;
			}
		}
		else
		{
			float k = getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() - pos[i].y;
			tyMin = k / dir[i % 3].y;
			k = getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight() - pos[i].y;
			tyMax = k / dir[i % 3].y;
			if (tyMin > tyMax)
			{
				float temp = tyMin;
				tyMin = tyMax;
				tyMax = temp;
			}
		}

		//test z
		if (dir[i % 3].z == 0)
		{
			if ((getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth()) <= pos[i].z && (getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth()) >= pos[i].z)
			{
				zTest = true;
			}
		}
		else
		{
			float k = getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth() - pos[i].z;
			tzMin = k / dir[i % 3].z;
			k = getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth() - pos[i].z;
			tzMax = k / dir[i % 3].z;
			if (tzMin > tzMax)
			{
				float temp = tzMin;
				tzMin = tzMax;
				tzMax = temp;
			}
		}

		//compare
		if (xTest && yTest || xTest && zTest || yTest && zTest || xTest && tyMin <= tzMax && tzMin <= tyMax || yTest && txMin <= tzMax && tzMin <= txMax ||
			zTest && tyMin <= txMax && txMin <= tyMax || txMin <= tyMax && txMin <= tzMax && tyMin <= txMax && tyMin <= tzMax && tzMin <= txMax && tzMin <= tyMax)
		{
			//find lowest t
			float temp = -1;
			float arr[3] = { txMin, tyMin, tzMin };
			for (int i = 0; i < 3; i++)
			{
				if (arr[i] > temp)
				{
					temp = arr[i];
				}
			}
			if (temp >= 0)
			{
				if (minT < 0 || temp < minT)
				{
					minT = temp;
				}
			}
		}
	}
	delete[]pos;
	delete[]dir;

	return minT;
}

//Set attack distance
void Enemy::setAttackDistance(float value)
{
	attackDistance = value;
}

//
void Enemy::jump()
{
	if (jumpSpeed > 0)
	{
		canJump = false;
	}
	if (canJump)
	{
		jumpSpeed = jumpStrength;
		canJump = false;
	}
}

//
void Enemy::walkJump()
{
	if (jumpSpeed > 0)
	{
		canJump = false;
	}
	if (canJump)
	{
		jumpSpeed = walkJumpStr;
		canJump = false;
	}
}
