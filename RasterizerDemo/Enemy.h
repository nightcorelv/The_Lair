#pragma once
#include "Character.h"
#include "Player.h"

class Enemy : public Character
{
private:
	float attackDistance = 0.0f;
	float gravity = 0.1f;											//strength of gravity
	bool canJump = false;											//internal variable for wether the mesh can jump yet
	float jumpSpeed = 0;											//current upward acceleration
	float jumpStrength = 2;											//strength of jump
	float walkJumpStr = 1.5;										//strength of walking jump
	float lockJump = false;

public:
	Enemy(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float atkTimer = 20, float jumpHeight = 2);
	Enemy(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float atkTimer = 20, float jumpHeight = 2);
	//Enemy(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, int health = 1, int damage = 1, float speed = 1, float atkTimer = 20, float jumpHeight = 2);
	void move();
	void followCharacter(Character& character);								//Enemies should follow the player. Should it have a certain reach?
	void handleJump();
	void setJump(bool jump);
	bool getJump();
	void stopJump();
	virtual float hitDetection(Player*& player, XMFLOAT3*& dir);

protected:
	void setAttackDistance(float value);
	void jump();
	void walkJump();
};