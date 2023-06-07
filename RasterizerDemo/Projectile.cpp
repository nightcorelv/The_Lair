#include "Projectile.h"

float testProjectileCollision(Entity**& culling, int used, XMFLOAT3* ray, XMFLOAT3 direction, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;
	for (int i = 0; i < used; i++)
	{
		if (culling[i] != ref && culling[i] != self)
		{
			float temp = culling[i]->getMesh()->collision(ray, 1, direction);
			if (temp >= 0 && temp < distance || distance < 0 && temp >= 0)
			{
				obj = i;
				distance = temp;
			}
		}
	}
	return distance;
}

float vectorLength(float x, float y, float z)
{
	float temp = pow(x, 2) + pow(y, 2) + pow(z, 2);
	return sqrt(temp);
}

Projectile::Projectile(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation, int dmg, float speed, float dir)
	:Entity(device, filename, Position, Rotation), damage(dmg), speed(speed), direction(dir)
{
	this->getMesh()->setTextureKey(getMesh()->getTextureKey());
	setID(ID::projectile);
}

Projectile::Projectile(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int dmg, float speed, float dir)
	:Entity(device, mesh, Position, Rotation), damage(dmg), speed(speed), direction(dir)
{
	this->getMesh()->setTextureKey(getMesh()->getTextureKey());
	setID(ID::projectile);
}

bool Projectile::hit(Player& player)
{
	return false;
}

bool Projectile::move(Player*& charakter , Entity** cull, int size)
{
	bool hej = false;
	if (getActive())
	{
		XMFLOAT3 direction = { charakter->getPosition().x - getPosition().x,0, charakter->getPosition().z - getPosition().z };
		//calculate direction normalitisation

		float hypotenusan = std::pow(direction.x, 2) + std::pow(direction.y, 2) + std::pow(direction.z, 2);
		hypotenusan = std::sqrt(hypotenusan);
		direction.x /= hypotenusan;
		direction.y /= hypotenusan;
		direction.z /= hypotenusan;

		// testing angle calk

		float dot2 = (initialdirection.x * direction.x) + (initialdirection.z * direction.z);
		float det2 = (initialdirection.x * direction.z) - (initialdirection.z * direction.x);

		float angle360 = atan2(det2, dot2);
		float yRotation = angle360 / (2 * (float)acos(0.0));
		angle360 = (angle360 * 180) / (2 * (float)acos(0.0));
		if (!laser)
		{

			int stop = 0;
			if (angle360 > 12)
			{
				float newx = ((float)cos(0.03) * initialdirection.x) - ((float)sin(0.03) * initialdirection.z);
				float newz = ((float)sin(0.03) * initialdirection.x) + ((float)cos(0.03) * initialdirection.z);
				initialdirection.x = newx;
				initialdirection.z = newz;
			}
			else if (angle360 < -12)


			{
				float newx = ((float)cos(-0.03) * initialdirection.x) - ((float)sin(-0.03) * initialdirection.z);
				float newz = ((float)sin(-0.03) * initialdirection.x) + ((float)cos(-0.03) * initialdirection.z);
				initialdirection.x = newx;
				initialdirection.z = newz;
			}

		

		}
		float x = initialdirection.x * speed;
		float y = initialdirection.y * speed;
		float z = initialdirection.z * speed;

		float dotR = (initialdirection.x * 0) + (initialdirection.z * 1);
		float detR = (initialdirection.x * 1) - (initialdirection.z * 0);

		float rotation2 = atan2(detR, dotR);
		float worldrotation = rotation2 / (2 * (float)acos(0.0));

		int close = -1;
		XMFLOAT3* tempPos = new XMFLOAT3(getPosition());
		float dis = testProjectileCollision(cull, size, tempPos, direction, close);
		delete tempPos;
		if (dis == -1)
		{
			//Free to move
		}
		else
		{
			if (cull[close]->getId() == ID::player)
			{
				charakter->getMesh()->textureKey = "Damage.png";
				Player* temp = dynamic_cast<Player*>(cull[close]);
				if (temp->hasShield())
				{
					temp->takeDmg(temp->useShield(getDmg()));
					charakter->setCantakeDamage(false);
					charakter->PlaydamageAudio();
					
				}
				else
				{
					temp->takeDmg(getDmg());
					charakter->setCantakeDamage(false);
					charakter->PlaydamageAudio();
					
				}
			}
			hej = true;
		}
		increasePosition(x, 0, z);

		XMFLOAT3 newrotation{ 0, worldrotation, 0 };
		setRotation(newrotation);
		
	}
	return hej;
}

int Projectile::getDmg() const
{
	return damage;
}

void Projectile::setDirection(float radians)
{
	direction = radians;
}

bool Projectile::collision(Entity& entity)
{
	return false;
}

bool Projectile::lifespawn()
{
	bool old = false;

	internalTimer += 0.25;
	if (internalTimer > 30)
	{
		old = true;
		internalTimer = 0;
	}
	return old;
}

void Projectile::setactive(bool activate)
{
	active = activate;
}

bool Projectile::getActive()
{
	return active;
}

void Projectile::setInitialDirection(XMFLOAT3 direction)
{
	initialdirection = direction;
}

void Projectile::setInitialposition(XMFLOAT3 position)
{
	initialposition = position;
}

void Projectile::setspeed(float spe)
{
	speed = spe;
}

bool Projectile::Laser()
{
	return laser;
}

void Projectile::setlaser()
{
	laser = true;
}

void Projectile::resetTime()
{
	internalTimer = 0;
}


