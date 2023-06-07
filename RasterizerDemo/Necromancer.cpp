#include "Necromancer.h"

//Necromancer::Necromancer(int damage, float attackTime, int health, Mesh& mesh)
//{
//	setupCharacter(health, damage, 0, attackTime);
//	setMesh(mesh);
//}

Necromancer::Necromancer(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Enemy(device, "../Meshes/zNecroManWithStaff", Position, Rotation, raycastType, whd, health, damage, speed)
{
	LspMesh projectile;	projectile.sceneFileName = "../LspMeshes/Orb";	LoadMesh(projectile);	PH = new projectileHandeler(device, projectile, "../Sounds/necroshot.wav");
	setID(ID::necromancer);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

Necromancer::Necromancer(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed)
{
	LspMesh projectile;	projectile.sceneFileName = "../LspMeshes/Orb";	LoadMesh(projectile);	PH = new projectileHandeler(device, projectile, "../Sounds/necroshot.wav");
	setID(ID::necromancer);
	setAtkAudio("../Sounds/bellamyhit.wav");
	setDamageAud("../Sounds/monsterhit.wav");
}

//Necromancer::Necromancer(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
//	:Enemy(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed)
//{
//	LspMesh projectile;	projectile.sceneFileName = "../LspMeshes/Orb";	LoadMesh(projectile);	PH = new projectileHandeler(device, projectile, "../Sounds/necroshot.wav");
//	setID(ID::necromancer);
//}

float necrocollision(Entity**& culling, int used, XMFLOAT3* rays, int nr, XMFLOAT3 direction, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;
	for (int i = 0; i < used; i++)
	{
		ID test = culling[i]->getId();
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

float hypnecro(float x, float y)
{
	float h2 = pow(x, 2) + pow(y, 2);
	return sqrt(h2);
}


void Necromancer::moveThowards(Player*& charakter, Entity** cull, int size)
{
	// direction towards player

	XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };
	//calculate direction normalitisation

	float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
	hypotenusan = std::sqrt(hypotenusan);
	direction.x /= hypotenusan;
	direction.y /= hypotenusan;
	direction.z /= hypotenusan;

	direction.x *= getSpeed();
	direction.y *= getSpeed();
	direction.z *= getSpeed();

	


	/// collision calculation
	int nrr;
	int close = -1;
	XMFLOAT3 dir;
	XMFLOAT3* boxRays = getMesh()->getRays(nrr, dir, getRotation().y);

	float dis = necrocollision(cull, size, boxRays, nrr, dir, close, this);

	if (!canAttack())
	{
		atkCountdown();
	}
	if (dis > 0.5 || dis == -1)
	{
		while (hypnecro(direction.x, direction.z) > 0.5)
		{
			direction.x *= 0.98f;
			direction.z *= 0.98f;
		}
		//Free to move
		//this->getMesh()->setAnimState(this->getMesh()->getWalkAnim());
	}
	else
	{
        direction = XMFLOAT3(0, 0, 0);
		if (cull[close]->getId() == ID::player && canAttack() && charakter->canBeAttacked())
		{
			if (getHealth() < (getMaxHealth() / 2))
			{
				charakter->takeDmg(getDmg());
				charakter->setCantakeDamage(false);
				charakter->PlaydamageAudio();
				atkCountdown();
				charakter->getMesh()->textureKey = "Damage.png";
			}
		}
		else if (cull[close]->getId() == ID::player)
		{

		}
		
		else
		{
			jump();
		}
		//this->getMesh()->setAnimState(this->getMesh()->getIdleAnim());
	}
	
	handleJump();
	increasePosition(direction.x, 0, direction.z);
	delete[]boxRays;





}

void Necromancer::shoot(Player*& charakter)
{

	this->PH->getShots();
	if (this->PH->getShots() < this->PH->getProjectileArray().size())
	{
		XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };
		//calculate direction normalitisation
		float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
		hypotenusan = std::sqrt(hypotenusan);
		direction.x /= hypotenusan;
		direction.y /= hypotenusan;
		direction.z /= hypotenusan;
		XMFLOAT3 aim = direction;
		direction.x *= 2;
		direction.y *= 2;
		direction.z *= 2;
		//spawn projectile 
		XMFLOAT3 spawnpos;
	   spawnpos.x = getPosition().x + direction.x;
	   spawnpos.y = getPosition().y + 1, 5;
	   spawnpos.z = getPosition().z + direction.z;
		


	   this->PH->projectiles.at(this->PH->getShots())->setspeed(0.8f);
		this->PH->projectiles.at(this->PH->getShots())->setPosition(spawnpos);
		this->PH->projectiles.at(this->PH->getShots())->setInitialposition(spawnpos);
		this->PH->projectiles.at(this->PH->getShots())->setactive(true);
		this->PH->getProjectileArray().at(this->PH->getShots())->setInitialDirection(aim);
		this->PH->fireshot();

		//this->getMesh()->setAnimState(this->getMesh()->getShootAnim()); //Needs a timer!!
	}


}



void Necromancer::heal(int hp)
{
	increaseHealth(hp);
}



bool Necromancer::collision(Entity& entity)
{
	return false;
}

void Necromancer::behaviour(Player*& charakter, Entity** cull, int size)
{
	PH->handleprojectiles(charakter,cull,size);
	float internaltime = GetTimer();
	XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };
	//calculate direction normalitisation

	float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
	hypotenusan = std::sqrt(hypotenusan);
	direction.x /= hypotenusan;
	direction.y /= hypotenusan;
	direction.z /= hypotenusan;

	direction.x *= getSpeed();
	direction.y *= getSpeed();
	direction.z *= getSpeed();



	XMFLOAT3 rotation = { 0,0,0 };

	float intervall = 0.01;
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



	if (getHealth() <= 0)
	{
		SetDead(true);
	}


  	if (internaltime <= 0)
	{
		setBehaviour(rand() %3);
	}
	addTime(1);
	
	if (GetTimer() >= 120)
	{
		if (getHealth() < (getMaxHealth() / 2))
		{

			if (getBehaviour() == 1)
			{
				moveThowards(charakter, cull, size);
				if (GetTimer() >= 240)
				{

					setTimer(0);

				}

			}
			else if (getBehaviour() == 2)
			{

				spawning = true;
				setTimer(0);

			}
			else
			{
				moveThowards(charakter, cull, size);
				if (GetTimer() >= 240)
				{

					setTimer(0);

				}

			}


		}
		else
		{

			if (getBehaviour() == 1)
			{
				shoot(charakter);
				setTimer(0);

			}
			else if (getBehaviour() == 2)
			{


				spawning = true;
				setTimer(0);

			}
			else
			{
				moveThowards(charakter, cull, size);
				if (GetTimer() >= 240)
				{

					setTimer(0);

				}

			}
		}
	}


}



float Necromancer::GetTimer()
{
	return internalTimer;
}

void Necromancer::setTimer(float time)
{
	internalTimer = time;
}

void Necromancer::addTime(float time)
{
	internalTimer += time;
}

void Necromancer::setBehaviour(int i)
{
	behaviourint = i;
}

int Necromancer::getBehaviour()
{
	return behaviourint;
}

bool Necromancer::Spawning()
{
	return spawning;
}

void Necromancer::hasspawned()
{
	spawning = false;
}

ID Necromancer::GetenemyID()
{
	ID temp;
	int randomenemy = rand() % 3;

	switch (randomenemy)
	{
	case 0:
		temp = ID::skull;
		break;
	case 1:
		temp = ID::pile;
		break;
	case 2:
 		temp = ID::wraith;
		break;

	}

	
	return temp ;
}

std::vector<Projectile*> Necromancer::getProjectileArray()
{
	return PH->getProjectileArray();
}

projectileHandeler* Necromancer::getProjectileHandeler()
{
	return this->PH;
}

Necromancer::~Necromancer()
{
	delete this->PH;
}

void Necromancer::move(Entity* bellamy, Entity** cull, int size)
{
	Player* PL = dynamic_cast<Player*>(bellamy);
	behaviour(PL, cull, size);

}
