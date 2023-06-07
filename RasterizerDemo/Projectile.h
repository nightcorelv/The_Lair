#pragma once
#include "Player.h"

class Projectile : public Entity
{
private:
	int damage;											//Damage output.
	float speed;										//Movement speed.
	float direction;									//Direction in which the projectile is fired.
	bool active = false;
	float internalTimer = 0;
	XMFLOAT3 initialdirection;
	XMFLOAT3 initialposition;
	bool laser = false;

public:

	//Projectile(int dmg, float speed, float dir, Mesh mesh);
	Projectile(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int dmg = 1, float speed = 5.0f, float dir = 1);
	Projectile(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int dmg = 1, float speed = 5.0f, float dir = 1);
	bool hit(Player& player);
	bool move(Player*& charakter, Entity** cull, int size);
	int getDmg() const;
	void setDirection(float radians);
	bool collision(Entity& entity);
	bool lifespawn();
	void setactive(bool active);
	bool getActive();
	void setInitialDirection(XMFLOAT3 direction);
	void setInitialposition(XMFLOAT3 position);
	void setspeed(float spe);
	bool Laser();
	void setlaser();
	void resetTime();

};