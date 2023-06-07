#include "projectileHandeler.h"
#include"Necromancer.h"

projectileHandeler::projectileHandeler(ID3D11Device* device, LspMesh& mesh, string audio)
{
	wstring newA(audio.begin(), audio.end());
	shotAud.load(newA);
	Projectile* temp[5]{
		new Projectile(device, mesh, XMFLOAT3(0, 0, -3), XMFLOAT3(0, 0, 0), 1, 0.2f, 1),
		new Projectile(device, mesh, XMFLOAT3(0, 0, -3), XMFLOAT3(0, 0, 0), 1, 0.2f, 1),
		new Projectile(device, mesh, XMFLOAT3(0, 0, -3), XMFLOAT3(0, 0, 0), 1, 0.2f, 1),
		new Projectile(device, mesh, XMFLOAT3(0, 0, -3), XMFLOAT3(0, 0, 0), 1, 0.2f, 1),
		new Projectile(device, mesh, XMFLOAT3(0, 0, -3), XMFLOAT3(0, 0, 0), 1, 0.2f, 1) };

	for (size_t i = 0; i < 5; i++)
	{
		projectiles.push_back(temp[i]);
	}
}

projectileHandeler::~projectileHandeler()
{
	for (size_t i = 0; i < projectiles.size(); i++)
	{
		delete projectiles.at(i);
	}
}

float ProjectileCollision(Entity**& culling, int used, XMFLOAT3* rays, int nr, XMFLOAT3 direction, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;
	for (int i = 1; i < used; i++)
	{
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

float hypProj(float x, float y)
{
	float h2 = (float)pow(x, 2) + (float)pow(y, 2);
	return sqrt(h2);
}


void projectileHandeler::destroyProjectile(std::vector<Projectile*> bulletArray, int index)
{

	//inactivate the bulet att id
	bulletArray.at(index)->setactive(false);
	bulletArray.at(index)->resetTime();

	//pushing back used bullet;
	Projectile* temp = bulletArray.at(index);
	bulletArray.at(index)->setPosition(XMFLOAT3{0,0,0});
	std::vector<Projectile*> temparr;
	for (size_t i = 0; i < index; i++)
	{
		temparr.push_back(bulletArray.at(i));
	}
	for (size_t i = index+1; i < bulletArray.size(); i++)
	{
		temparr.push_back(bulletArray.at(i));
	}
	temparr.push_back(temp);
	projectiles = temparr;
	//remove one bullet from necromancers active bullets
	removeshot();



}


std::vector<Projectile*> projectileHandeler::getProjectileArray()
{
	return projectiles ;
}

void projectileHandeler::handleprojectiles( Player*& charakter, Entity** cull, int size)
{
	for (size_t i = 0; i <usedProjectiles; i++)
	{
		if (projectiles.at(i)->lifespawn())
		{
			destroyProjectile(projectiles, (int)i);
		}
		else
		{
			if (projectiles.at(i)->getActive())
			{
				if (projectiles.at(i)->move(charakter, cull, size))
				{
					destroyProjectile(projectiles, (int)i);
				}
			}
		}
	}
}

void projectileHandeler::fireshot()
{
	usedProjectiles++;
	shotAud.stop();
	shotAud.play();
}

void projectileHandeler::removeshot()
{
	usedProjectiles--;
}

void projectileHandeler::BindAndDrawProjectiles(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer)
{
	for (size_t i = 0; i < usedProjectiles; i++)
	{
		projectiles.at(i)->getMesh()->bindAndDrawIndexed(immediateContext, viewProjBuffer);
	}
}

void projectileHandeler::updateProjectileMesh(ID3D11DeviceContext* immediateContext)
{
	
	for (size_t i = 0; i < this->getShots(); i++)
	{
		this->getProjectileArray().at(i)->update(immediateContext);
	}
}

int projectileHandeler::getShots()
{
	return usedProjectiles;
}

float projectileHandeler::returnDistanve(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return   sqrt(pow(v1.x - v2.x, 2) + pow(v1.z - v2.z, 2));
}

Projectile* projectileHandeler::getprojectile(int index)
{
	return projectiles[index];
}



