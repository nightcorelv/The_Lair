#pragma once
#include "Projectile.h"
#include "Necromancer.h"
#include <vector>
#include"Audio.h"

class projectileHandeler
{

public:
	std::vector<Projectile*> projectiles;
	int usedProjectiles = 0;
	Audio shotAud;

	projectileHandeler(ID3D11Device* device, LspMesh& mesh, string audio);
	~projectileHandeler();
	void destroyProjectile(std::vector<Projectile*> bulletArray, int index);
	std::vector<Projectile*> getProjectileArray();
	void  handleprojectiles(Player*& charakter, Entity** cull, int size);
	void fireshot();
	void removeshot();
	void BindAndDrawProjectiles(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer);
	void updateProjectileMesh(ID3D11DeviceContext* immediateContext);
	int getShots();
	float returnDistanve(XMFLOAT3 v1, XMFLOAT3 v2);
	Projectile* getprojectile(int index);
};