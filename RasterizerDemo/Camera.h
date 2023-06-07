#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
using namespace DirectX;
using namespace std;
using namespace Microsoft;
using namespace WRL;

class Camera
{
private:
	ComPtr<ID3D11Buffer> viewMatrixBuffer;			//Holds View Matrix Buffer
	ComPtr<ID3D11Buffer> CameraPosBuffer;			//Holds Camera Position Buffer
	XMFLOAT3 lookAtPos;								//Look at Position
	XMFLOAT3 cameraPos;								//Camera Position
	XMFLOAT3 upDirection;							//Up direction
	XMFLOAT3 rotation;								//Rotation
	float nearPlane = 0.1f;							//Min render distance
	float farPlane = 200.0f;						//Max render distance
	unsigned int width;
	unsigned int height;

public:
	Camera(ID3D11Device* device, XMFLOAT3 pos, XMFLOAT3 lookAt, XMFLOAT3 upDir, unsigned int width, unsigned int height);
	void updateCameraBuffers(ID3D11DeviceContext* immediateContext);					//Updates buffers with data from member variables. Therefore run set on variable you wish to update and then update to apply.
	void setPos(XMFLOAT3 pos);															//Set member variables
	void setLookAt(XMFLOAT3 lookAt);
	void setUpDir(XMFLOAT3 upDir);
	void setRotation(XMFLOAT3 rot);
	void SetViewMX(XMFLOAT3 pos, XMFLOAT3 lookAt, XMFLOAT3 upDir);
	XMFLOAT3 getPosition();																//Get member variables
	XMFLOAT3 getLookAt();
	XMFLOAT3 getUpDir();
	ID3D11Buffer* getCameraMTXBuffer();
	ID3D11Buffer* getCameraPosBuffer();
};