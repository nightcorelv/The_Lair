#pragma once
#include "Movable.h"
#include "Audio.h"

class Character : public Movable
{
private:
	int maxHealth;										//Limits the possible amount of healing.
	int health;											//Keeps track of health points.
	int damage;											//Keeps track of damage output.
	float speed;										//Keeps track of movement speed.
	bool dead = false;									//Is the character dead?
	float attackTimer;									//Keeps track of attack speed.

	float atkTime;										//Used for resetting attack timer.
	bool ableToAtk = true;								//Is the character ready to attack?
	float attackDist;
	bool absorbed = false;
	float invulnerabilitytime;
	float invulnerabilitytimer;
	bool cantakeDamage = true;

	 Audio damageAud;
	 Audio atkAud;


	void setCanAttack(bool tf);							//This should be false after attacking and then set to true when the countdown is finished.

public:
	Character(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 15);
	//Character(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 15);
	Character(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 15);
	//Character(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 15);
	Character(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float attackTime = 15);
	void takeDmg(int value);							//Used when this character is being hit.
	void attack();										//Resets the attack timer.
	int getDmg() const;									//Returns this character's damage.
	bool isDead() const;								//Is the character dead?
	void atkCountdown();								//Reduces the attack timer.
	void invulCountdown();
	void setCantakeDamage(bool state);
	void setInvulnTime(float time);
	bool canBeAttacked();
	bool canAttack() const;								//Can this character attack?
	bool withinReach(Character& other);
	void setSpeed(float s);
	void setMaxHealt(float value);
	int getMaxHealth();
	int getHealth() const;								//Returns health.
	void resetAttackTime();								//Resets attack timer.
	void SetDead(bool state);
	bool getAbsorbed();
	void setAbsorbed(bool state);
	void setHpToMax();
	void setAtkAudio(string file);
	void setDamageAud(string file);
	void PlaydamageAudio();
	void playPunchaudio();
	void setDamage(float value);
	

protected:
	void setAttackDist(float value);
	float getSpeed() const;								//Returns movement speed.
	void increaseHealth(int value);						//Used for healing.
	void increaseMaxHealth(int value);					//Used for buffing max health.
	void increaseDamage(int value);						//Used for buffing damage.
	void setupCharacter(int health, int damage, float speed, float atkTime, float atkDist);	//Used when creating subclass objects.
};