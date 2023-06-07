#include "Wraith.h"

float testWraithCollision(Entity**& culling, int used, XMFLOAT3* rays, int nr, XMFLOAT3 direction, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
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

void Wraith::charge()
{
}

Wraith::Wraith(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime, float chargeTime, float maxTime)
	:Enemy(device, "../Meshes/Wraith", Position, Rotation, raycastType, whd, health, damage, speed), chargeTime(chargeTime), maxChargeTime(maxTime)
{
	LspMesh projectile;	projectile.sceneFileName = "../LspMeshes/Laser";	LoadMesh(projectile);	PH = new projectileHandeler(device, projectile, "../Sounds/wraithlaser.wav");
	setID(ID::wraith);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

Wraith::Wraith(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime, float chargeTime, float maxTime)
	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed), chargeTime(chargeTime), maxChargeTime(maxTime)
{
	LspMesh projectile;	projectile.sceneFileName = "../LspMeshes/Laser";	LoadMesh(projectile);	PH = new projectileHandeler(device, projectile, "../Sounds/wraithlaser.wav");
	setID(ID::wraith);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

//Wraith::Wraith(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime, float chargeTime, float maxTime)
//	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed), chargeTime(chargeTime), maxChargeTime(maxTime)
//{
//	LspMesh projectile;	projectile.sceneFileName = "../LspMeshes/Laser";	LoadMesh(projectile);	PH = new projectileHandeler(device, projectile, "../Sounds/wraithlaser.wav");
//	setID(ID::wraith);
//	setAtkAudio("../Sounds/bellamyhit.wav");
//	setDamageAud("../Sounds/monsterhit.wav");
//}

Wraith::~Wraith()
{
	delete this->PH;
}

projectileHandeler* Wraith::getProjectileHandeler()
{
	return  this->PH;
}


void Wraith::move(Entity* bellamy, Entity** cull, int size, Entity** cull2, int size2)
{
	Player* PL = dynamic_cast<Player*>(bellamy);
	behaviour(PL, cull, size, cull2, size2);
}

void Wraith::shoot(Player*& charakter)
{
	 int test =this->PH->getShots();
	 indexProjectile = this->PH->getShots();
	if (this->PH->getShots() < this->PH->getProjectileArray().size())
	{
		XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x, 0, charakter->getPosition().z - getPosition().z };

		//calculate direction normalisation
		float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
		hypotenusan = std::sqrt(hypotenusan);
		direction.x /= hypotenusan;
		direction.y /= hypotenusan;
		direction.z /= hypotenusan;
		this->PH->projectiles.at(this->PH->getShots())->setPosition(getPosition());
		XMFLOAT3 aim = direction;
		direction.x *= 2;
		direction.y *= 2;
		direction.z *= 2;
		//spawn projectile 
		XMFLOAT3 spawnpos;
		spawnpos.x = getPosition().x + direction.x;
		spawnpos.y = getPosition().y +1,5;
		spawnpos.z = getPosition().z + direction.z;
		XMFLOAT3* nP = new XMFLOAT3(getPosition().x, getPosition().y, getPosition().z);
		float radians = getRotation().y * (2 * (float)acos(0.0));
		//setRotation(newrotation);


		float z = cos(radians);
		float x = sin(radians);
		XMFLOAT3 v = { x,0,z };
		int obj = - 1;

		delete nP;
		this->PH->projectiles.at(this->PH->getShots())->setlaser();
		this->PH->projectiles.at(this->PH->getShots())->setspeed(5);
		this->PH->projectiles.at(this->PH->getShots())->setPosition(spawnpos);
		this->PH->projectiles.at(this->PH->getShots())->setInitialposition(spawnpos);
		this->PH->projectiles.at(this->PH->getShots())->setRotation(getRotation());
		this->PH->projectiles.at(this->PH->getShots())->setactive(true);
	
		this->PH->getProjectileArray().at(this->PH->getShots())->setInitialDirection(v);
		this->PH->fireshot();

		//this->getMesh()->setAnimState(this->getMesh()->getShootAnim());
	}


}

bool Wraith::collision(Entity& entity)
{
	return false;
}

void Wraith::behaviour(Player*& charakter, Entity** cull, int size, Entity** cull2, int size2)
{
	PH->handleprojectiles(charakter, cull2, size2);
	XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };

	float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
	hypotenusan = std::sqrt(hypotenusan);
	direction.x /= hypotenusan;
	direction.y /= hypotenusan;
	direction.z /= hypotenusan;

	float dotR = (direction.x * 0) + (direction.z * 1);
	float detR = (direction.x * 1) - (direction.z * 0);

	float rotation2 = atan2(detR, dotR);
	float worldrotation = rotation2 / (2 * (float)acos(0.0));


	XMFLOAT3 newrotation{ 0, worldrotation, 0 };
	float yrot = getRotation().y;
	float radians = getRotation().y * (2 * (float)acos(0.0));
	setRotation(newrotation);
	

	if (internalTimer == 90)
	{
		shoot(charakter);
	}
	else if (internalTimer >= 310)
	{
		teleport(charakter, cull, size);
		internalTimer = 0;
		setTelepored(true);
		//this->getMesh()->setAnimState(this->getMesh()->getIdleAnim());  //Might need an extra timer check to set as idle
	}
	else
	{
		setTelepored(false);
	}
	internalTimer ++;
}

void Wraith::teleport(Player*& charakter, Entity** cull, int size)
{
	vector<XMFLOAT3> directions;
	for (int i = 0; i < 4; i++)
	{
		int nrr;
		int close = -1;
		XMFLOAT3 dir;
		XMFLOAT3* boxRays = charakter->getMesh()->getRays(nrr, dir, i*0.5f);

		float dis = testWraithCollision(cull, size, boxRays, nrr, dir, close);
		if (dis > 6 || dis == -1)
		{
			bool sameDir = false;
			if (charakter->getRotation().y == 0 && dir.z == 1)
			{
				sameDir = true;
			}
			else if (charakter->getRotation().y == 0.5 && dir.x == 1)
			{
				sameDir = true;
			}
			else if (charakter->getRotation().y == 1 && dir.z == -1)
			{
				sameDir = true;
			}
			else if (charakter->getRotation().y == 1.5 && dir.x == -1)
			{
				sameDir = true;
			}
			if (!sameDir)
			{
				directions.push_back(dir);
			}
		}
		delete[]boxRays;
	}

	if (directions.size() > 0)
	{
		int i = rand() % directions.size();
		XMFLOAT3 newPos = XMFLOAT3(charakter->getPosition().x + (directions[i].x * 3), charakter->getPosition().y + 3, charakter->getPosition().z + (directions[i].z * 3));
		setPosition(newPos);
	}
	else
	{
		XMFLOAT3 newPos = XMFLOAT3(getPosition().x, getPosition().y + 1, getPosition().z);
		setPosition(newPos);
	}
	
	
	
}

int Wraith::returnProjectileIndex()
{
	return indexProjectile;
}


