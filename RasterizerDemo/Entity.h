#pragma once
#include "Mesh.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "ID.h"
#include "PolyMorph.h"

using namespace DirectX;

class Entity : public PolyMorph
{
private:
	Mesh mesh;														//The entity's mesh.
	ID id;
	int tile;														//current tile
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	bool shouldMove = false;
	bool justTeleported = false;
	int index = 0;
	int type;														//type of object
																	//0 = standard object
																	//-add types here- 

public:
	Entity(ID3D11Device* device, string filename, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Entity(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	//Entity(ID3D11Device* device, Mesh& mesh, LspSkeletalMesh* skelMesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Entity(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	//Entity(ID3D11Device* device, LspSkeletalMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Entity();
	~Entity();
	void draw(ID3D11DeviceContext* immediateContext, ID3D11Buffer* CameraMTXBuffer);

	ID getId() const;
	Mesh* getMesh();

	void setPosition(XMFLOAT3 pos);
	void increasePosition(float x, float y, float z);
	void setRotation(XMFLOAT3 rot);
	void increaseRotation(float x, float y, float z);
	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	void update(ID3D11DeviceContext* immediateContext);
	void updateMesh(ID3D11DeviceContext* immediateContext);
	int getTile();
	void setTile(float x, float y, int xTiles, int tileWidth);
	void setMesh(ID3D11Device* device, Mesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	void setMove(bool m);
	bool getMove();
	virtual void move(Entity* bellamy, Entity** cull, int size);
	void setID(ID id);
	void makeVirtual();
	bool getTeleported();
	void setTelepored(bool teleported);
	void setIndex(int i);
	int getIndex();
	void animate(ID3D11DeviceContext* immediateContext, bool animated = true);
	//void animate(ID3D11DeviceContext* immediateContext, int animState); //Probably unnecessary
	bool getUseAnim();
	
};