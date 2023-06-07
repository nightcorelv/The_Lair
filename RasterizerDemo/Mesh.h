#pragma once
#include <iostream>
#include <d3d11.h>
#include<vector>
#include<sstream>
#include <string>
#include <DirectXMath.h>
#include <array>
#include "PipelineHelper.h"
#include <wrl/client.h>
#include "Input.h"
#include "AnimationType.h"
#include <LspAPI.h>
using namespace DirectX;
using namespace std;
using namespace Microsoft;
using namespace WRL;

struct TextureCollection
{
	static void Load(ID3D11Device* device, std::string path);
};

class Mesh
{
private:		
	vector<ComPtr<ID3D11Texture2D>> ambient_Texture;				//Texture array
	vector<ComPtr<ID3D11Texture2D>> diffuse_Texture;
	vector<ComPtr<ID3D11Texture2D>> specular_Texture;
	vector<ComPtr<ID3D11ShaderResourceView>> ambient_View;			//Texture view array
	vector<ComPtr<ID3D11ShaderResourceView>> diffuse_View;
	vector<ComPtr<ID3D11ShaderResourceView>> specular_View;
	vector<ComPtr<ID3D11Buffer>> materialBuffer;					//Buffers


	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> actorTypeBuffer;							//determines special properties for Actor (only used for cubeMap currently)
	vector<int> submeshIndex;										//records index where new submesh starts
	vector<XMFLOAT3> normalPerFace;									//Vector of normals per face, only filled if SaveFaceNormals
	
	//flytta till egen klass??
	ComPtr<ID3D11Buffer> matrixBuffer;								//Holds current Mat Data
	XMMATRIX matrix;												//Holds current Mat Data
	XMFLOAT3 position;
	XMFLOAT3 rotation;

	//ComPtr<ID3D11Buffer> boneMxBuffer;								//Holds transformation matrices for each bone.
	//vector<XMMATRIX> bonesFinalMx;
	//
	//Animation states, did not finish
	//bool useAnims = false;
	//int animState = -1;
	//int maxStates = 0;
	//int lastState = -1;
	//int walkAnim = -1;
	//int shootAnim = -1;
	//int attackAnim = -1;
	//int idleAnim = -1;
	//int hurtAnim = -1;
	//
	//LspSkeletalMesh* lspSkelMesh = nullptr;
	//int animFrame = 0;
	//int animIndex = -1;
	//
	//ComPtr<ID3D11Buffer> skinVtxBuffer;

	bool big = false;
	float width;													//Bounding-Box width (x) (from center to edge)
	float height;													//Bounding-Box height (y)
	float depth;													//Bounding-Box depth (z)
	XMFLOAT3 center;												//Bounding-Box offset from object position
	int rays;														//type of formation for rays during collision-checks
																	//0 = 1, bottom mid
																	//1 = 3, along the bottom
																	//2 = 4, one per corner
																	//3 = 6, one per corner and one bottom/top mid 
																	//4 = 9, grid of 3x3 rays
	bool move = false;

public:

	Mesh(ID3D11Device * device, string filename, bool SaveFaceNormals, int actorType, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0,0,0), int raycastType = 0, float* whd = nullptr);
	Mesh(const Mesh& copy, ID3D11Device* device, XMFLOAT3 pos, XMFLOAT3 rot, int raycastType = 0, float* whd = nullptr);
	//Mesh(const Mesh& copy, LspSkeletalMesh* skelMesh, ID3D11Device* device, XMFLOAT3 pos, XMFLOAT3 rot, int raycastType = 0, float* whd = nullptr);
	Mesh(LspMesh& input, ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr, bool multiTex = false);
	//Mesh(LspSkeletalMesh& input, ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), int raycastType = 0, float* whd = nullptr);
	Mesh();
	~Mesh();
	bool updateMatrix(ID3D11DeviceContext* immediateContext);
	void bindAndDrawIndexed(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer);
	void BindAndDrawCubeMap(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer, ID3D11ShaderResourceView* srv, ID3D11Buffer *& cameraPosBuffer);
	vector<XMFLOAT3> getFaceNormals();
	void setPos(XMFLOAT3 pos);
	void setRot(XMFLOAT3 rot);
	XMMATRIX getMatrix();
	XMFLOAT3 getPos();
	XMFLOAT3 getRot();
	bool operator ==(const Mesh& v) const;

	//bool isAnimated() const;
	//int getMaxStates();
	//int getLastState();
	//int addState(string filename, AnimationType type);
	//bool setAnimState(int state);
	//int getAnimState() const;
	//int getWalkAnim() const;
	//int getAttackAnim() const;
	//int getShootAnim() const;
	//int getIdleAnim() const;
	//bool Animate(ID3D11DeviceContext* immediateContext, int animation = 0, bool animate = true);
	//LspSkeletalMesh* getSkelMesh() const;

	XMFLOAT3* getRays(int& nr, XMFLOAT3& direction, float rot);
	float collision(XMFLOAT3* rayOrigin, int nr, XMFLOAT3 direction);
	XMFLOAT3* getGravityRays();
	float gravityCheck(XMFLOAT3* rayOrigin);

	float getOffsetX();
	float getOffsetY();
	float getOffsetZ();
	float getWidth();
	float getHeight();
	float getDepth();

	std::string textureKey;
	std::string orgTextureKey;
	std::string getTextureKey();
	void setTextureKey(std::string key);
	static void Release();
	void updateHitbox(float* whd);
	void rotateHitBox();
	bool getBig();

};
