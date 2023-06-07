#pragma once
#include "PointLight.h"


struct LightStruct
{
	XMFLOAT4 pos;
	XMFLOAT4 color;
	XMFLOAT4 ambi;
	XMFLOAT4 diff;
	XMFLOAT4 spec;
	XMFLOAT4X4 viewMatrix;
	LightStruct(XMFLOAT4 position, XMFLOAT4 color, XMFLOAT4X4 matrix, XMFLOAT4 ambientComp, XMFLOAT4 diffuseComp, XMFLOAT4 specularComp)
	{
		this->pos = position;
		this->color = color;
		this->viewMatrix = matrix;
		this->ambi = ambientComp;
		this->diff = diffuseComp;
		this->spec = specularComp;
	}
};

class LightHandler
{
private:

	ComPtr<ID3D11ShaderResourceView> lightBuffView;				
	ComPtr<ID3D11ShaderResourceView> shaderView;				//ska hålla i data från x antal depthTextures
	ComPtr<ID3D11Texture2D> depthTextures;						//behöver en shader resource view
	vector<ComPtr<ID3D11DepthStencilView>> depthViews;
	vector<ComPtr<ID3D11Buffer>> viewBuffers;					//View/proj matrix for each light
	vector<PointLight> Pointlights;
	ComPtr<ID3D11Buffer> lightBuffer;							//int nr of lights // varje slot lightArray[2]; - Color, Pos
	ComPtr<ID3D11Buffer> numLightBuffer;
	UINT shadowHeight;
	UINT shadowWidth;
	vector<Mesh> boundingSphere;
	XMFLOAT4 ambComp = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.0f);
	XMFLOAT4 difComp = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	XMFLOAT4 speComp = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

public:
	
	LightHandler(UINT shadowHeight = 1024, UINT shadowWidth = 1024);
	void addPointlight(ID3D11Device * device, XMFLOAT4 position, XMFLOAT4 color, XMFLOAT4 FocusPos, XMFLOAT4 UpDir);
	void finalizeLights(ID3D11Device * device, ID3D11DeviceContext* immediateContext);

	bool updateLightBuffer(ID3D11DeviceContext* immediateContext, int ptLightIndex, XMFLOAT4 position, XMFLOAT4 color, float lightIntensity); //Add update mesh pos
	bool updateMatrixAndViewBuffer(ID3D11DeviceContext * immediateContext, int ptLightIndex, XMFLOAT4 position, XMFLOAT4 lookAt, XMFLOAT4 upDirection);

	ID3D11Buffer* getLightViewBuffer(int ptLightIndex);
	int getNrOfLights();

	void bindLightBuffer(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& cameraPosBuffer, ID3D11ShaderResourceView** dfSrvArr);
	void bindDepthStencil(ID3D11DeviceContext * immediateContext, int ptLightIndex);
	void bindAndDrawIndexedMesh(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer);
};