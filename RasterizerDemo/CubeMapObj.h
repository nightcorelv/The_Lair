#pragma once
#include "Mesh.h"

class CubeMapHandler
{
private:

	vector<ComPtr<ID3D11DepthStencilView>> depthViews;			//Depth stencil arr, one for each camera
	ComPtr<ID3D11ShaderResourceView> shaderView;				//Shader resource view looking at depth Map array
	ComPtr<ID3D11Texture2D> depthTextures;						//TextureArray for depth data
	vector <ComPtr<ID3D11UnorderedAccessView>> unorderedViews;  //Points to specific index in texture array
	ComPtr<ID3D11ShaderResourceView> renderedTextureView;		//Srv looking at renderedTexture arr
	ComPtr<ID3D11Texture2D> renderTextures;						//TextureArray for Render data
	vector<ComPtr<ID3D11Buffer>> viewBuffers;					//View/proj matrix for each camera
	ComPtr<ID3D11Buffer> posBuffer;

	XMFLOAT4 position;											//Position
	UINT texHeight = 273;/*1024;*/										//Texture Height
	UINT texWidth = 273;/*1024;	*/									//Texture Width
	Mesh cubeMesh;												//obj behöver ha 6 submesh!!! () //kolla cube texture
	
public:
	CubeMapHandler(ID3D11Device * device, ID3D11DeviceContext* immediateContext, XMFLOAT4 pos);
	ID3D11Buffer* getViewMatrix(int cameraIndex);
	ID3D11DepthStencilView* getDepthStencil(int cameraIndex);
	ID3D11ShaderResourceView* getRenderedTextureView(int cameraIndex);
	ID3D11UnorderedAccessView* getUav(int cameraIndex);
	ID3D11Buffer* getPosBuffer();
	//void updateBuffers();

	void bindDepthStencil(ID3D11DeviceContext * immediateContext, int cameraIndex);
	void bindAndDrawIndexedMesh(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer, ID3D11Buffer *& cameraPosBuffer);
};



