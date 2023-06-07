#pragma once
#include "Entity.h"
#include "Character.h"
#include "Shield.h"
#include "HealthPotion.h"
#include "DamageBoost.h"
#include "DamageBuff.h"
#include "HealthBuff.h"
#include "Aura.h"
#include "Puzzlebox.h"
#include "Camera.h"
#include "Time.h"
#include "PickupHandler.h"

class Player : public Character
{
private:
	Shield* myShield = nullptr;									//Decreases the incoming damage.
	DamageBoost* damageBoost = nullptr;							//Increases damage output.
	Aura* myAura = nullptr;										//Harms enemies within a certain radius.
	Entity* grabbedBox = nullptr;
	void heal(int hp);											//Increases current health.
	void buffHealth(int value);									//Used if the player picks up a health buff.
	void addDmg(int value);										//Used if the player picks up a damage buff.
	bool inPile = false;
	float gravity = 0.1f;											//strength of gravity
	bool canJump = false;											//internal variable for wether the mesh can jump yet
	float jumpSpeed = 0;											//current upward acceleration
	float jumpStrength = 2;											//strength of jump
	int startingHp;
	int startingDmg;
	int atkCooldown = 0;
	bool auraOverlap(Entity* test);
	float auratimer = 0;
	float auraTime = 300;
	

 

public:
	bool isNecroManDead = false; //Have we won? ..behöver sättas true !!!!!
	Player(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 15.0f);
	//Player(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 15.0f);
	Player(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 15.0f);
	//Player(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 15.0f);
	Player(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0,0,0), int raycastType = 0, float* whd = nullptr, int health = 10, int damage = 5, float speed = 5.0f, float attackTime = 15.0f);
	void move(Camera & camera, float &x, float &z);
	bool touchingBox(Puzzlebox& box);
	bool addPickup(Pickup* pickup);								//Used when the player takes a pickup of any kind.
	float getHPforHealthBar(); 
	bool hasShield() const;										//Has the player picked up a shield?
	bool hasAura() const;										//Has the player picked up an aura yet?
	bool hasDmgBoost() const;									//Has the player picked up a temporary damage boost?
	int useShield(int incomingDmg);								//If the player is holding a shield, the incoming damage should be reduced.
	int getAuraDmg() const;										//To damage enemies with the aura.
	void countdownPickups(float timePassed);					//Some pickups are temporary and only last a certain amount of time.
	void removePickup(PickupHandler* handler);										//^
	bool collision(Entity& entity);
	void setBox(Entity* box);
	Entity* getBox();
	void releaseBox();
	bool getPile();
	void setPile(bool pile);
	void updateJump();
	void resetJump();
	void disableJump();
	int getHpIncrease();
	int getDmgIncrease();
	void handleAura(vector<Entity*>& cull);
	XMFLOAT3* getAtkRays(XMFLOAT3*& dir);
	void resetplayer();

protected:

};