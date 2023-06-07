#include "Player.h"

XMFLOAT3 normalizedVector(float x, float y, float z)
{
	float hyp = pow(x, 2) + pow(y, 2) + pow(z, 2);
	hyp = sqrt(hyp);
	return XMFLOAT3(x / hyp, y / hyp, z / hyp);
}

Player::Player(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Character(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, attackTime)

{
	setAtkAudio("../Sounds/swing.wav");
	setDamageAud("../Sounds/bellamyhit.wav");
	setInvulnTime(1000);
	setID(ID::player);
	startingHp = health;
	startingDmg = damage;
}

//Player::Player(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
//	:Character(device, mesh, skelMesh, Position, Rotation, raycastType, whd, health, damage, speed, attackTime)
//
//{
//	setAtkAudio("../Sounds/swing.wav");
//	setDamageAud("../Sounds/bellamyhit.wav");
//	setInvulnTime(1000);
//	setID(ID::player);
//	startingHp = health;
//	startingDmg = damage;
//}

Player::Player(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Character(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, attackTime)

{
	setAtkAudio("../Sounds/swing.wav");
	setDamageAud("../Sounds/bellamyhit.wav");
	setInvulnTime(1000);
	setID(ID::player);
	startingHp = health;
	startingDmg = damage;
}

//Player::Player(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
//	:Character(device, mesh, Position, Rotation, raycastType, whd, health, damage, speed, attackTime)
//
//{
//	setAtkAudio("../Sounds/swing.wav");
//	setDamageAud("../Sounds/bellamyhit.wav");
//	setInvulnTime(1000);
//	setID(ID::player);
//	startingHp = health;
//	startingDmg = damage;
//}

Player::Player(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, int health, int damage, float speed, float attackTime)
	:Character(device, "../Meshes/BellardBoneHit", Position, Rotation, raycastType, whd, health, damage, speed, attackTime)
{
	
	setAtkAudio("../Sounds/swing.wav");
	setDamageAud("../Sounds/bellamyhit.wav");

	setID(ID::player);
	startingHp = health;
	startingDmg = damage;
	setInvulnTime(30);
}


void Player::updateJump()
{
	if (Input::GetKeyDown(Input::Key::Space) && jumpSpeed <= 0 && canJump)
	{
		jumpSpeed = jumpStrength;
		canJump = false;
	}
	if (jumpSpeed > 0)
	{
		jumpSpeed -= gravity;
	}
	if (jumpSpeed < 0)
	{
		jumpSpeed = 0;
	}
	float spd = jumpSpeed;
	if (grabbedBox != nullptr)
	{
		spd *= 0.95f;
		grabbedBox->increasePosition(0, spd, 0);
	}
	increasePosition(0, spd, 0);
}

void Player::resetJump()
{
	canJump = true;
}

void Player::disableJump()
{
	canJump = false;
}

//Returns how much health has increased since starting the game.
int Player::getHpIncrease()
{
	return getMaxHealth() - startingHp;
}

//Returns how much damage has increased since starting the game.
int Player::getDmgIncrease()
{
	return getDmg() - startingDmg;
}

//Enemies who are too close take damage, if the player has an aura activated
void Player::handleAura(vector<Entity*>& cull)
{
	if (hasAura())
	{

		for (int i = 0; i < cull.size(); i++)
		{
			if (cull[i]->getId() != ID::puzzlebox)
			{
				if (auraOverlap(cull[i]))
				{
					if (myAura->canDamage())
					{
						Character* temp = dynamic_cast<Character*>(cull[i]);
						temp->takeDmg(myAura->getDmg());
						temp->getMesh()->textureKey = "Damage.png";
						temp->PlaydamageAudio();
						myAura->setdamageTimer0();
						myAura->setCanDamage(false);
					
					}
					
					 
					
				}
			}
			
		}
		if (!myAura->canDamage())
		{
			myAura->damagecountdown();
		}
		
		auratimer++;
	}
	if (auratimer >= auraTime)
	{
		myAura->Reset();
		myAura = nullptr;
		auratimer = 0;

	}
}

XMFLOAT3* Player::getAtkRays(XMFLOAT3*& dir)
{
	if (getRotation().y == 0)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth()/2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(-2, 0, 5);
		dir[1] = normalizedVector(0, 0, 5);
		dir[2] = normalizedVector(2, 0, 5);

		return pos;
	}
	else if (getRotation().y == 0.25)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(3, 0, 5);
		dir[1] = normalizedVector(5, 0, 5);
		dir[2] = normalizedVector(5, 0, 3);

		return pos;
	}
	else if (getRotation().y == 0.5)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		
		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(5, 0, 2);
		dir[1] = normalizedVector(5, 0, 0);
		dir[2] = normalizedVector(5, 0, -2);

		return pos;
	}
	else if (getRotation().y == 0.75)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(5, 0, -3);
		dir[1] = normalizedVector(5, 0, -5);
		dir[2] = normalizedVector(3, 0, -5);

		return pos;
	}
	else if (getRotation().y == 1)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth()/2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() + (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(2, 0, -5);
		dir[1] = normalizedVector(0, 0, -5);
		dir[2] = normalizedVector(-2, 0, -5);

		return pos;
	}
	else if (getRotation().y == 1.25)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth()/2));
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(-3, 0, -5);
		dir[1] = normalizedVector(-5, 0, -5);
		dir[2] = normalizedVector(-5, 0, -3);

		return pos;
	}
	else if (getRotation().y == 1.5)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth()/2));
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() - (getMesh()->getDepth() / 2));
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(-5, 0, -2);
		dir[1] = normalizedVector(-5, 0, 0);
		dir[2] = normalizedVector(-5, 0, 2);

		return pos;
	}
	else if (getRotation().y == 1.75)
	{
		XMFLOAT3* pos = new XMFLOAT3[9];
		pos[0] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[1] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[2] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth()/2), getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() + 0.001f, getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[3] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[4] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[5] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[6] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + (getMesh()->getDepth() / 2));
		pos[7] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth(), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());
		pos[8] = XMFLOAT3(getPosition().x + getMesh()->getOffsetX() - (getMesh()->getWidth() / 2), getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight(), getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth());

		dir = new XMFLOAT3[3];
		dir[0] = normalizedVector(-5, 0, 3);
		dir[1] = normalizedVector(-5, 0, 5);
		dir[2] = normalizedVector(-3, 0, 5);

		return pos;
	}
	XMFLOAT3* pos = new XMFLOAT3[9];
	for (int i = 0; i < 9; i++)
	{
		pos[i] = XMFLOAT3(0, 0, 0);
	}
	return pos;
}

void Player::resetplayer()
{
	setMaxHealt(startingHp);
	setHpToMax();
	setDamage(startingDmg);
	myShield = nullptr;
	damageBoost = nullptr;
	myAura = nullptr;
	grabbedBox = nullptr;
	this->SetDead(false);

}

bool Player::auraOverlap(Entity* test)
{
	bool overlapping = false;

	if (myAura != nullptr)
	{
		float point[5][3];
		point[0][0] = test->getPosition().x + test->getMesh()->getOffsetX();
		point[0][1] = test->getPosition().y + (test->getMesh()->getOffsetY() / 2);
		point[0][2] = test->getPosition().z + test->getMesh()->getOffsetZ();
		point[1][0] = test->getPosition().x + test->getMesh()->getOffsetX() + test->getMesh()->getWidth();
		point[1][1] = test->getPosition().y + (test->getMesh()->getOffsetY() / 2);
		point[1][2] = test->getPosition().z + test->getMesh()->getOffsetZ() + test->getMesh()->getDepth();
		point[2][0] = test->getPosition().x + test->getMesh()->getOffsetX() - test->getMesh()->getWidth();
		point[2][1] = test->getPosition().y + (test->getMesh()->getOffsetY() / 2);
		point[2][2] = test->getPosition().z + test->getMesh()->getOffsetZ() + test->getMesh()->getDepth();
		point[3][0] = test->getPosition().x + test->getMesh()->getOffsetX() + test->getMesh()->getWidth();
		point[3][1] = test->getPosition().y + (test->getMesh()->getOffsetY() / 2);
		point[3][2] = test->getPosition().z + test->getMesh()->getOffsetZ() - test->getMesh()->getDepth();
		point[4][0] = test->getPosition().x + test->getMesh()->getOffsetX() - test->getMesh()->getWidth();
		point[4][1] = test->getPosition().y + (test->getMesh()->getOffsetY() / 2);
		point[4][2] = test->getPosition().z + test->getMesh()->getOffsetZ() - test->getMesh()->getDepth();

		for (int i = 0; i < 5; i++)
		{
			if (getPosition().x + getMesh()->getOffsetX() + getMesh()->getWidth() + myAura->getRadius() >= point[i][0] && getPosition().x + getMesh()->getOffsetX() - getMesh()->getWidth() <= point[i][0] &&
				getPosition().y + getMesh()->getOffsetY() + getMesh()->getHeight() >= point[i][1] && getPosition().y + getMesh()->getOffsetY() - getMesh()->getHeight() <= point[i][1] &&
				getPosition().z + getMesh()->getOffsetZ() + getMesh()->getDepth() + myAura->getRadius() >= point[i][2] && getPosition().z + getMesh()->getOffsetZ() - getMesh()->getDepth() <= point[i][2])
			{
				overlapping = true;
			}
		}
	}

	return overlapping;
}

//Increases health
void Player::heal(int hp)
{
	increaseHealth(hp);
}

//Increases max health
void Player::buffHealth(int value)
{
	increaseMaxHealth(value);
}

//Increases damage
void Player::addDmg(int value)
{
	increaseDamage(value);
}

//Player movement based on key inputs. Also moves the camera the same way.
void Player::move(Camera & camera, float &x, float &z)
{
	//Variables
	bool walking = false;
	x = 0;
	z = 0;
	float div = 1.7f * (1 - Time::DeltaTimeInSeconds());
	float rotationConstant = 0;
	float increase = 1.0f / div;
	XMFLOAT3 charPos = getPosition();
	XMFLOAT3 camPos{ charPos.x, charPos.y + 25.0f, charPos.z - 25.0f };

	//Determine rotation
	if (Input::GetKeyDown(Input::Key::W))			//w
	{
		this->setRotation(XMFLOAT3(0, 0 + rotationConstant, 0));
		if (Input::GetKeyDown(Input::Key::A))		//a
		{
			this->setRotation(XMFLOAT3(0, 1.75f + rotationConstant, 0));
			increase = (float)sqrt(0.5) / div;
			x -= increase;
		}
		else if (Input::GetKeyDown(Input::Key::D))	//d
		{
			this->setRotation(XMFLOAT3(0, 0.25f + rotationConstant, 0));
			increase = (float)sqrt(0.5) / div;
			x += increase;
		}
		z += increase;
		walking = true;
	}
	else if (Input::GetKeyDown(Input::Key::S))		//s
	{
		this->setRotation(XMFLOAT3(0, 1 + rotationConstant, 0));
		if (Input::GetKeyDown(Input::Key::A))		//a
		{
			this->setRotation(XMFLOAT3(0, 1.25f + rotationConstant, 0));
			increase = (float)sqrt(0.5) / div;
			x -= increase;
		}
		else if (Input::GetKeyDown(Input::Key::D))	//d
		{
			this->setRotation(XMFLOAT3(0, 0.75f + rotationConstant, 0));
			increase = (float)sqrt(0.5) / div;
			x += increase;
		}
		z -= increase;
		walking = true;
	}
	else if (Input::GetKeyDown(Input::Key::A))		//a
	{
		this->setRotation(XMFLOAT3(0, 1.5f + rotationConstant, 0));
		x -= increase;
		walking = true;
	}
	else if (Input::GetKeyDown(Input::Key::D))		//d
	{
		this->setRotation(XMFLOAT3(0, 0.5f + rotationConstant, 0));
		x += increase;
		walking = true;
	}

	//If walking and not attacking. Might want to look at blending of animations for attacking and walking at the same time...
	if (walking && canAttack())
	{
		//this->getMesh()->setAnimState(this->getMesh()->getWalkAnim());
	}
	else if (canAttack())
	{
		//this->getMesh()->setAnimState(this->getMesh()->getIdleAnim());
	}
	else if (!canAttack())
	{
		//this->getMesh()->setAnimState(this->getMesh()->getAttackAnim());
	}
	//Set camera
	camera.setLookAt(charPos);
	camera.setPos(camPos);
}

//Check if touching a box
bool Player::touchingBox(Puzzlebox& box)
{
	bool touching = false;

	float playerposX = getPosition().x;
	float playerposZ = getPosition().z;
	float boxposX = box.getPosition().x;
	float boxposZ = box.getPosition().z;

	if (playerposX < 0)
	{
		playerposX *= -1;
	}
	if (playerposZ < 0)
	{
		playerposZ *= -1;
	}
	if (boxposX < 0)
	{
		boxposX *= -1;
	}
	if (boxposZ < 0)
	{
		boxposZ *= 1;
	}

	if ((playerposX - boxposX <= 2.5 && playerposX - boxposX >= -2.5) &&
		(boxposZ - playerposZ <= 2.5 && boxposZ - playerposZ >= -2.5))
	{
		touching = true;
	}
	return touching;
}

//Add a pickup, either to a pointer or simply add the buff connected
bool Player::addPickup(Pickup* pickup)
{
	bool result = false;
	
	if (!hasShield() && pickup->getId() == ID::shield)
	{
		pickup->pickUp();
		Shield* newShield = dynamic_cast<Shield*>(pickup);
		myShield = newShield;
		result = true;
	}
	if (pickup->getId() == ID::healthpotion)
	{
		pickup->pickUp();
		heal(dynamic_cast<HealthPotion*>(pickup)->getHealth());
		result = true;
	}
	if (!hasDmgBoost() && pickup->getId() == ID::damageboost)
	{
		pickup->pickUp();
		DamageBoost* newDmgBoost = dynamic_cast<DamageBoost*>(pickup);
		newDmgBoost->setBoost(getDmg());
		addDmg(newDmgBoost->getBoost());
		damageBoost = newDmgBoost;
		result = true;
	}
	if (pickup->getId() == ID::damagebuff)
	{
		pickup->pickUp();
		addDmg(dynamic_cast<DamageBuff*>(pickup)->getBoost());
		result = true;
	}
	if (pickup->getId() == ID::healthbuff)
	{
		pickup->pickUp();
		HealthBuff* hpBuff = dynamic_cast<HealthBuff*>(pickup);
		buffHealth(hpBuff->getHealth());
		heal(hpBuff->getHealth());
		result = true;
	}
	if (pickup->getId() == ID::aura)
	{
		if (!hasAura())
		{
			pickup->pickUp();
			myAura = dynamic_cast<Aura*>(pickup);
			result = true;
		}
		else
		{
			pickup->pickUp();
			myAura->combineAuras(dynamic_cast<Aura*>(pickup));
			result = true;
		}
	}
	return result;
}

//Returns a value between 0-0.3 that matches how much health is left compared to max health.
float Player::getHPforHealthBar()
{
	float hp = (float)getHealth();
	float maxHp = (float)getMaxHealth();
	float proc = hp / maxHp;
	float setValue = proc * 0.5f;

	return setValue;
}

//Check if myShield is equipped
bool Player::hasShield() const
{
	return myShield != nullptr;
}

//Check if myAura is equipped
bool Player::hasAura() const
{
	return myAura != nullptr;
}

//Check if damageBoost is equipped
bool Player::hasDmgBoost() const
{
	return damageBoost != nullptr;
}

//Decrease incoming damage if a shield is equipped
int Player::useShield(int incomingDmg)
{
	int decreased = incomingDmg;
	if (hasShield())
	{
		decreased = myShield->decreaseDmg(decreased);
	}
	return decreased;
}

//Returns aura's damage if an aura is equipped
int Player::getAuraDmg() const
{
	int auraDmg = 0;
	if (hasAura())
	{
		auraDmg = myAura->getDmg();
	}
	return auraDmg;
}

//Countdown for pickup timers
void Player::countdownPickups(float timePassed)
{
	if (hasShield())
	{
		myShield->countdown(timePassed);
	}

	if (hasDmgBoost())
	{
		damageBoost->countdown(timePassed);
	}
}

//Remove pickups that are used up or have no time left
void Player::removePickup(PickupHandler* handler)
{
	if (hasShield() && (myShield->usedUp() || myShield->getTimeLeft() <= 0))
	{
		myShield->resetTimer();
		handler->removePickup(myShield->getId());
		myShield = nullptr;
	}

	if (hasDmgBoost() && damageBoost->getTimeLeft() <= 0)
	{
		damageBoost->resetTimer();
		addDmg(-damageBoost->getBoost());
		handler->removePickup(damageBoost->getId());
		damageBoost = nullptr;
	}
}

bool Player::collision(Entity& entity)
{
	return false;
}

//Grab puzzlebox
void Player::setBox(Entity* box)
{
	grabbedBox = box;
}

//Return the grabbed puzzlebox
Entity* Player::getBox()
{
	return grabbedBox;
}

//Release puzzlebox
void Player::releaseBox()
{
	grabbedBox = nullptr;
}

//Check if standing in a pile
bool Player::getPile()
{
	return inPile;
}

//Set if standing in a pile
void Player::setPile(bool pile)
{
	inPile = pile;
}

