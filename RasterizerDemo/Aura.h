#pragma once
#include "Pickup.h"

class Player;

class Aura : public  Pickup
{
private:

	int dmg;											//The amount of damage that the aura makes.
	float radius;										//Used to calculate if a creature is within the aura if it is attached to the player.

	float damagetime = 10;
	float damageTimer;
	bool candamage;

#pragma region Particle

	static Aura* currentAura;
	static Player* s_player;

	struct PlayerPosConstantStruct
	{
		float pos[3];
		float radios;
	};
	static PlayerPosConstantStruct playerS;
	static ID3D11Buffer* constantBuffer;
	static D3D11_MAPPED_SUBRESOURCE mappedResource;

	static std::string textureKey;
	static ID3D11BlendState* blendState;
	static ID3D11SamplerState* sampler;
	static std::unordered_map<std::string, ID3D11Texture2D*> textureMap;
	static std::unordered_map<std::string, ID3D11ShaderResourceView*> textureSRV;

#pragma endregion

public:
	//Aura(int dmg, float radius, Mesh* mesh);
	Aura(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int dmg = 1, float radius = 1.0f);
	Aura(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int dmg = 1, float radius = 1.0f);
	int getDmg() const;									//The amount of damage that the aura makes.
	float getRadius() const;							//Used to calculate if a creature is within the aura if it is attached to the player.
	void combineAuras(Aura* other);						//If the player already has an aura the new damage is added
	bool collision(Entity& entity);						//Don't know if this is required for anything in particular. I needed something to make the base class abstract.
	void makeVirtual();
	void damagecountdown();
	void setCanDamage(bool state);
	bool canDamage();
	void setdamageTimer0();
	

	// particle functions
	static void Reset();
	static void Init(ID3D11Device* device);
	static void SetPlayer(Player*& p);
	static void Bind(ID3D11DeviceContext* immediateContext);
	static void UnBind(ID3D11DeviceContext* immediateContext);
	static void Release();

protected:

};
