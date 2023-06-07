#include "Character.h"

void Character::setCanAttack(bool tf)
{
	this->ableToAtk = tf;
}

Character::Character(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Movable(device, mesh, Position, Rotation, raycastType, whd), maxHealth(health), health(health), damage(damage), speed(speed), atkTime(attackTime)
{
	this->dead = false;
	this->ableToAtk = false;
}

//Character::Character(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
//	:Movable(device, mesh, skelMesh, Position, Rotation, raycastType, whd), maxHealth(health), health(health), damage(damage), speed(speed), atkTime(attackTime)
//{
//	this->dead = false;
//	this->ableToAtk = false;
//}

Character::Character(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Movable(device, mesh, Position, Rotation, raycastType, whd), maxHealth(health), health(health), damage(damage), speed(speed), atkTime(attackTime)
{
	this->dead = false;
	this->ableToAtk = false;
}

//Character::Character(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
//	:Movable(device, mesh, Position, Rotation, raycastType, whd), maxHealth(health), health(health), damage(damage), speed(speed), atkTime(attackTime)
//{
//	this->dead = false;
//	this->ableToAtk = false;
//}

Character::Character(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Movable(device, filename, Position, Rotation, raycastType, whd), maxHealth(health), health(health), damage(damage), speed(speed), atkTime(attackTime)
{
	this->dead = false;
	this->ableToAtk = false;
}

void Character::takeDmg(int value)
{
	health -= value;
	if (health <= 0)
	{
		dead = true;
	}
}

void Character::attack()
{
	setCanAttack(false);
	resetAttackTime();
	//this->getMesh()->setAnimState(this->getMesh()->getAttackAnim());
}

int Character::getDmg() const
{
	return damage;
}

bool Character::isDead() const
{
	return dead;
}

void Character::atkCountdown()
{
	if (attackTimer >= atkTime)
	{
		setCanAttack(true);
		attackTimer = 0;
		//this->getMesh()->setAnimState(this->getMesh()->getIdleAnim());
	}
	else
	{
		setCanAttack(false);
		attackTimer++;
		//this->getMesh()->setAnimState(this->getMesh()->getAttackAnim());
	}
}

void Character::invulCountdown()
{
	if (!canBeAttacked())
	{
		if (invulnerabilitytimer >= invulnerabilitytime)
		{
			setCantakeDamage(true);
			invulnerabilitytimer = 0;
		}
		else
		{
			setCantakeDamage(false);
			invulnerabilitytimer++;

		}
	}
	
}

void Character::setCantakeDamage(bool state)
{
	cantakeDamage = state;
}

bool Character::canBeAttacked()
{
	return cantakeDamage;
}

void Character::setInvulnTime(float time)
{
	invulnerabilitytime = time;
}

bool Character::canAttack() const
{
	return ableToAtk;
}

bool Character::withinReach(Character& other)
{
	bool canReach = false;

	float myX = this->getPosition().x;
	float myZ = this->getPosition().z;
	float otherX = other.getPosition().x;
	float otherZ = other.getPosition().z;

	if (myX < 0)
	{
		myX *= -1;
	}
	if (myZ < 0)
	{
		myZ *= -1;
	}
	if (otherX < 0)
	{
		otherX *= -1;
	}
	if (otherZ < 0)
	{
		otherZ *= -1;
	}

	if ((myX - otherX <= attackDist && myX - otherX >= -attackDist) &&
		(otherZ - myZ <= attackDist && otherZ - myZ >= -attackDist))
	{
		canReach = true;
	}

	return canReach;
}

void Character::setAttackDist(float value)
{
	attackDist = value;
}

void Character::setSpeed(float s)
{
	speed = s;
}

void Character::setMaxHealt(float value)
{
	maxHealth = value;
}

int Character::getMaxHealth()
{
	return maxHealth;
}



int Character::getHealth() const
{
	return health;
}

void Character::SetDead(bool state)
{
	dead = state;
	//if (dead)
	//{
	//	this->getMesh()->setAnimState(-1);
	//}
}

bool Character::getAbsorbed()
{
	return absorbed;
}

void Character::setAbsorbed(bool state)
{
	absorbed = state;
}

void Character::setHpToMax()
{
	health = maxHealth;
}

void Character::setAtkAudio(string file)
{
	wstring newaud(file.begin(), file.end());
	atkAud.load(newaud);
}

void Character::setDamageAud(string file)
{
	wstring newaud(file.begin(), file.end());
	
	damageAud.load(newaud);
}

void Character::PlaydamageAudio()
{
	damageAud.stop();
	damageAud.play();
}

void Character::playPunchaudio()
{
	atkAud.stop();
	atkAud.play();
}

void Character::setDamage(float value)
{
	damage = value;
}

float Character::getSpeed() const
{
	return speed;
}

void Character::increaseHealth(int value)
{
	health += value;
	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

void Character::increaseMaxHealth(int value)
{
	maxHealth += value;
}

void Character::increaseDamage(int value)
{
	damage += value;
}

void Character::setupCharacter(int health, int damage, float speed, float atkTime, float atkDist)
{
	this->maxHealth = health;
	this->health = health;
	this->damage = damage;
	this->speed = speed;
	this->atkTime = atkTime;
	this->attackTimer = atkTime;
	this->attackDist = atkDist;
}

void Character::resetAttackTime()
{
	attackTimer = 0;
}
