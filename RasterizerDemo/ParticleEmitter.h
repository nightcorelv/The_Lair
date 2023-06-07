#pragma once
#include <iostream>
#include <d3d11.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <array>
#include <wrl/client.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace DirectX;
using namespace std;
using namespace Microsoft;
using namespace WRL;

class ParticleEmitter
{
private:
	ComPtr <ID3D11UnorderedAccessView> PT_UAV;		//UAV for vertex shader
	ComPtr<ID3D11Buffer> PT_vertexBuffer;			//Vertex buffer
	ComPtr<ID3D11Buffer> PT_posBuffer;				//Position buffer
	vector<XMFLOAT3> PtData;						//
	XMFLOAT3 Position;								//Position
	int nrOfParticles;								//Nr of points in buffer
	int maxHeight = 10;								//Respawn height (gör på compute shader?)
	bool active;

public:
	ParticleEmitter(ID3D11Device * device, XMFLOAT3 Pos, int nrOfPT, int randRange = 10);
	ID3D11Buffer* getVTXBuffer();
	ID3D11Buffer* getPosBuffer();
	void BindAndDraw(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& timeBuffer, ID3D11ComputeShader*& particleUpdateShader);
	void Unbind(ID3D11DeviceContext* immediateContext);
	ID3D11UnorderedAccessView* getUAV();
	int getNrOfParticles();
	void updateBuffer(ID3D11DeviceContext* immediateContext);
	XMFLOAT3 getPosition();
	void setPosition(XMFLOAT3 Pos);
	void setActive(bool onOrOff);
	bool isActive();
};