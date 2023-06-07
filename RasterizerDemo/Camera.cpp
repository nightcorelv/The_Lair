#include "Camera.h"

//---------------------------------------------------------------Functions for initilization-----------------------------------------------------------------//

bool CreateMXBuffer(ID3D11Device* device, ComPtr<ID3D11Buffer>& constBuffer, XMFLOAT3 pos, XMFLOAT3 lookAt, XMFLOAT3 upDir, unsigned int width, unsigned int height)
{
	XMVECTOR cameraPos = XMVectorSet(pos.x, pos.y, pos.z, 0);
	XMVECTOR focusPos = XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 0);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	XMMATRIX view = { XMMatrixLookAtLH(cameraPos,focusPos, upDirection) * XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, (float)width / (float)height, 0.1f, 200.0f) };


	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMMATRIX);					//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData;						//holds data
	cBufData.pSysMem = &view;								//pointer to geometry data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, constBuffer.GetAddressOf());
	return !FAILED(hr);
}

bool CreatePosBuffer(ID3D11Device* device, ComPtr<ID3D11Buffer>& cameraBuffer, XMFLOAT3 pos)
{
	XMFLOAT4 cameraPosition(pos.x, pos.y, pos.z, 0);

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMFLOAT4);					//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };				//holds matrix data
	cBufData.pSysMem = &cameraPosition;						//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &cameraBuffer);
	return !FAILED(hr);
}

//------------------------------------------------------------------------Constructor------------------------------------------------------------------------//

Camera::Camera(ID3D11Device* device, XMFLOAT3 pos, XMFLOAT3 lookAt, XMFLOAT3 upDir, unsigned int width, unsigned int height)
	:lookAtPos(lookAt), cameraPos(pos), upDirection(upDir), width(width), height(height)
{
	//Create camera MX buffer
	if (!CreateMXBuffer(device, this->viewMatrixBuffer, pos, lookAt, upDir, width, height))
	{
		std::cerr << "error creating Camera MX Buffer" << std::endl;
	}

	//Create camera Pos buffer
	if (!CreatePosBuffer(device, this->CameraPosBuffer, pos))
	{
		std::cerr << "error creating Camera Pos Buffer" << std::endl;
	}
}

//---------------------------------------------------------------------Public Functions----------------------------------------------------------------------//

void Camera::updateCameraBuffers(ID3D11DeviceContext* immediateContext)
{
		//Camera MTX buffer
		D3D11_MAPPED_SUBRESOURCE map;
		ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//Create matrix
		XMVECTOR cameraPos = XMVectorSet(this->cameraPos.x, this->cameraPos.y, this->cameraPos.z, 0);
		XMVECTOR focusPos = XMVectorSet(this->lookAtPos.x, this->lookAtPos.y, this->lookAtPos.z, 0);
		XMVECTOR upDir = XMVectorSet(this->upDirection.x, this->upDirection.y, this->upDirection.z, 0);
		XMMATRIX view = { (XMMatrixRotationX(this->rotation.x) * XMMatrixRotationY(this->rotation.y) * XMMatrixRotationZ(this->rotation.z)) * XMMatrixLookAtLH(cameraPos,focusPos, upDir) * 
			XMMatrixPerspectiveFovLH(45.0f * DirectX::XM_PI / 180, (float)width / (float)height, this->nearPlane, this->farPlane) };
		
		//Unmap
		immediateContext->Map(this->viewMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, &view, sizeof(XMMATRIX));
		immediateContext->Unmap(this->viewMatrixBuffer.Get(), 0);

		//CameraPosBuffer
		XMFLOAT4 camPos(this->cameraPos.x, this->cameraPos.y, this->cameraPos.z, 0);
		ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
		immediateContext->Map(this->CameraPosBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, &camPos, sizeof(XMFLOAT4));
		immediateContext->Unmap(this->CameraPosBuffer.Get(), 0);
}

void Camera::setPos(XMFLOAT3 pos)
{
	this->cameraPos = pos;
}

void Camera::setLookAt(XMFLOAT3 lookAt)
{
	this->lookAtPos = lookAt;
}

void Camera::setUpDir(XMFLOAT3 upDir)
{
	this->upDirection = upDir;
}

void Camera::setRotation(XMFLOAT3 rot)
{
	this->rotation = rot;
}

void Camera::SetViewMX(XMFLOAT3 pos, XMFLOAT3 lookAt, XMFLOAT3 upDir)
{
	this->cameraPos = pos;
	this->lookAtPos = lookAt;
	this->upDirection = upDir;
}

XMFLOAT3 Camera::getPosition()
{
	return this->cameraPos;
}

XMFLOAT3 Camera::getLookAt()
{
	return this->lookAtPos;
}

XMFLOAT3 Camera::getUpDir()
{
	return this->upDirection;
}

ID3D11Buffer* Camera::getCameraMTXBuffer()
{
	return this->viewMatrixBuffer.Get();
}

ID3D11Buffer* Camera::getCameraPosBuffer()
{
	return this->CameraPosBuffer.Get();
}
