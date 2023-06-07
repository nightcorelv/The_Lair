#include "Skull.h"

float testSkullCollision(Entity**& culling, int used, XMFLOAT3* rays, int nr, XMFLOAT3 direction, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;
	for (int i = 1; i < used; i++)
	{
		if (culling[i] != ref && culling[i] != self)
		{
			float temp = culling[i]->getMesh()->collision(rays, nr, direction);
			if (temp >= 0 && temp < distance || distance < 0 && temp >= 0)
			{
				obj = i;
				distance = temp;
			}
		}
	}
	return distance;
}

float hyp(float x, float y)
{
	float h2 = (float)(pow(x, 2) + pow(y, 2));
	return sqrt(h2);
}

Skull::Skull(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float atkTimer)
	:Enemy(device, "../Meshes/Skull_20220504", Position, Rotation, raycastType, whd, health, damage, speed, atkTimer)
{
	//setupCharacter(health, damage, 0, attackTime);
	setID(ID::skull);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

Skull::Skull(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float atkTimer)
	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, atkTimer)
{
	//setupCharacter(health, damage, 0, attackTime);
	setID(ID::skull);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

//Skull::Skull(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float atkTimer)
//	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, atkTimer)
//{
//	//setupCharacter(health, damage, 0, attackTime);
//	setID(ID::skull);
//}

void Skull::move(Entity* bellamy, Entity** cull, int size)
{
	Player* PL = dynamic_cast<Player*>(bellamy);
	if (!isDead())
	{
		moveThowards(PL, cull, size);
	}
}

void Skull::moveThowards(Player* charakter, Entity** cull, int size)
{
	// direction towards player

	XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };
	//calculate direction normalitisation

	float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
	hypotenusan = std::sqrt(hypotenusan);
	direction.x /= hypotenusan;
	direction.y = hypotenusan;
	direction.z /= hypotenusan;

	XMFLOAT3 rotation = { 0,0,0 };
	float intervall = 0.2f;
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
	 /// collision calculation
	int nrr;
	int close = -1;
	XMFLOAT3 dir;
	XMFLOAT3* boxRays = getMesh()->getRays(nrr, dir, getRotation().y);
	
	float dis = testSkullCollision(cull, size, boxRays, nrr, dir, close);
	
	if (!canAttack())
	{
		atkCountdown();
	}




	if (dis > 0.5 || dis == -1)
	{
		while (hyp(direction.x, direction.z) > 0.5)
		{
			direction.x *= 0.98f;
			direction.z *= 0.98f;
		}
		//Free to move
	}
	else
	{
		direction = XMFLOAT3(0, 0, 0);
		if (cull[close]->getId() == ID::player && canAttack())
		{
			charakter->getMesh()->textureKey = "Damage.png";
			if (charakter->hasShield())
			{
				charakter->takeDmg(charakter->useShield(getDmg()));
				charakter->setCantakeDamage(false);
				charakter->PlaydamageAudio();
				atkCountdown();
			}
			else
			{
				charakter->takeDmg(getDmg());
				charakter->setCantakeDamage(false);
				charakter->PlaydamageAudio();
				atkCountdown();
			}
		}
		else if (cull[close]->getId() == ID::player)
		{
			
		}
		else
		{
			jump();
		}
	}
	if (hypotenusan <= 0.1)
	{
		direction = XMFLOAT3(0, 0, 0);
	}
	walkJump();
	handleJump();
	increasePosition(direction.x, 0, direction.z);
	delete[]boxRays;
	if (getHealth() <= 0)
	{
		SetDead(true);
	}
}

bool Skull::collision(Entity& entity)
{
	return false;
}
