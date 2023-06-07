#include "Pile.h"

float testCollision(Entity**& culling, int used, XMFLOAT3* rays, int nr, XMFLOAT3 direction, int& obj, Pile* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;

	for (int i = 1; i < used; i++)
	{
		if (culling[i] != ref && culling[i] != self && culling[i]->getId() != ID::skull && culling[i]->getId() != ID::pile && culling[i]->getId() != ID::wraith)
		{
			float temp = culling[i]->getMesh()->collision(rays, nr, direction);
			if (temp >= 0 && temp < distance || distance < 0 && temp >= 0)
			{
				obj = i;
				distance = temp;
			}
		}
		else if (culling[i]->getId() == ID::skull )
		{
			
			Character* temp = dynamic_cast<Character*>(culling[i]);
		
			if (temp != NULL && !temp->getAbsorbed()&& temp->isDead() && !self->getAbsorbed() )
			{
				if (self->overlap(temp))
				{
					temp->setAbsorbed(true);
					self->absorb(temp);
					
				}
			}
		}
	}
	return distance;
}

float hypn(float x, float y)
{
	float h2 = pow(x, 2) + pow(y, 2);
	return sqrt(h2);
}

Pile::Pile(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime, int requiredToGrow)
	:Enemy(device, "../Meshes/Pile_20220506", Position, Rotation, raycastType, whd, health, damage, 0.05f, 20)
{
	bonesRequired = requiredToGrow;
	setID(ID::pile);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");

}

Pile::Pile(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime, int requiredToGrow)
	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, 0.05f, 20)
{
	bonesRequired = requiredToGrow;
	setID(ID::pile);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

void Pile::absorb(Entity* skull)
{

	assimilatedSkuls.push_back(skull);
	if (assimilatedSkuls.size() >= bonesRequired)
	{
		increaseDamage(5);
		bonesRequired += 2;

	}
}


void Pile::move(Entity* bellamy, Entity** cull, int size)
{
	Player* PL = dynamic_cast<Player*>(bellamy);
	if (!isDead())
	{
		moveThowards(PL, cull, size);
	}
	else
	{
		
		for (size_t i = 0; i < assimilatedSkuls.size(); i++)
		{
			Character* temp = dynamic_cast<Character*>(assimilatedSkuls.at(i));
			temp->setAbsorbed(false);
			
		}
		assimilatedSkuls.clear();
	}
}

bool Pile::overlap(Entity* player)
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

void Pile::moveThowards(Player*& charakter, Entity** cull, int size)
{
	
	if (!canAttack())
	{
		atkCountdown();
	}
	//if player is overlaping pile
	if (overlap(charakter))
	{
		if (canAttack())
		{
			charakter->takeDmg(getDmg());
			charakter->setCantakeDamage(false);
			charakter->PlaydamageAudio();
			charakter->getMesh()->textureKey = "Damage.png";


			atkCountdown();
		}
		
	}

	XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };
	//calculate direction normalitisation

	float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
	hypotenusan = std::sqrt(hypotenusan);
	direction.x /= hypotenusan;
	direction.y /= hypotenusan;
	direction.z /= hypotenusan;

	XMFLOAT3 rotation = { 0,0,0 };
	float intervall = 0.03f;
	if (direction.z > intervall)
	{
		rotation.y = 0;
		if (direction.x < intervall * -1)
		{
			rotation.y = 1.75;

		}
		else if (direction.x > intervall)
		{
			rotation.y = 0.25;
		}

	}
	else if (direction.z < intervall * -1)
	{
		rotation.y = 1;
		if (direction.x < intervall * -1)
		{
			rotation.y = 1.25;

		}
		else if (direction.x > intervall)
		{
			rotation.y = 0.75;
		}

	}
	else if (direction.x < 0)
	{
		rotation.y = 1.5;
	}
	else if (direction.x > 0)
	{
		rotation.y = 0.5;
	}
	this->setRotation(rotation);

	direction.x *= getSpeed();
	direction.y *= getSpeed();
	direction.z *= getSpeed();

	int nrr;
	int close = -1;
	XMFLOAT3 dir;
	XMFLOAT3* boxRays = getMesh()->getRays(nrr, dir, getRotation().y);

	float dis = testCollision(cull, size, boxRays, nrr, dir, close, this, charakter);
	
	if (dis > 0.5 || dis == -1)
	{
		while (hypn(direction.x, direction.z) > 0.5)
		{
			direction.x *= 0.98f;
			direction.z *= 0.98f;
		}
		//Free to move
	}
	else
	{
		direction = XMFLOAT3(0, 0, 0);
		jump();
	}
	if (hypotenusan <= 0.1)
	{
		direction = XMFLOAT3(0, 0, 0);
	}
	handleJump();
	increasePosition(direction.x, 0, direction.z);
	moveSkulls();
	delete[]boxRays;
	if (getHealth() <= 0)
	{
		SetDead(true);
	}
}

bool Pile::collision(Entity& entity)
{
	return false;
}

void Pile::moveSkulls()
{


	for (size_t i = 0; i < assimilatedSkuls.size(); i++)
	{
		float ofsetDegrees = 360.0f / (float)assimilatedSkuls.size() * i;
		float newDegree = degrees + ofsetDegrees;

		float x = this->getPosition().x;
		float z = this->getPosition().z;

	        bool test =	this->getAbsorbed();
		float radians = newDegree * 3.14159265359f / 180;

		float newx = cos(radians) * 3 + getPosition().x;
		float newz = sin(radians) * 3 + getPosition().z;


		XMFLOAT3 newpos(newx, getPosition().y +1, newz);
		assimilatedSkuls.at(i)->setPosition(newpos);
	}
	degrees += 5;
	if (degrees >= 360)
	{
		degrees = 0;
	}
}

float Pile::hitDetection(Player * &player, XMFLOAT3 * &dir)
{
	float dis = -1;
	if (overlap(player))
	{
		dis = 0;
	}
	else
	{
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
					if (dis < 0 || temp < dis)
					{
						dis = temp;
					}
				}
			}
		}
		delete[]pos;
		delete[]dir;
	}

	return dis;
}

void Pile::clearAssim()
{
	if (assimilatedSkuls.size()>0)
	{
		assimilatedSkuls.clear();
	}
	
}

