#include "PointLight.h"

PointLight::PointLight(ID3D11Device * device, XMFLOAT4 lightColor, XMFLOAT4 lightPos, XMFLOAT4 FocusPos, XMFLOAT4 UpDir, XMFLOAT4 ambComp, XMFLOAT4 difComp, XMFLOAT4 speComp)
	:position(lightPos), color(lightColor), upDirection(UpDir), focusPosition(FocusPos), ambientComponent(ambComp), diffuseComponent(difComp), specularComponent(speComp)
{
	XMVECTOR pos = XMVectorSet(this->position.x, this->position.y, this->position.z, this->position.w);
	XMVECTOR focusPos = XMVectorSet(FocusPos.x, FocusPos.y, FocusPos.z, FocusPos.w);
	XMVECTOR upPos = XMVectorSet(UpDir.x, UpDir.y, UpDir.z, UpDir.w);
	this->view = { XMMatrixLookAtLH(pos,focusPos, upPos) * XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 0.75f, 0.1f, 400.0f) };
}

XMFLOAT4 PointLight::getPosition()
{
	return this->position;
}

XMFLOAT4 PointLight::getColor()
{
	return this->color;
}

XMFLOAT4 PointLight::getUpDir()
{
	return this->upDirection;
}

XMFLOAT4 PointLight::getFocusPos()
{
	return this->focusPosition;
}

XMMATRIX PointLight::getViewMatrix()
{
	return this->view;
}

void PointLight::updatePosition(XMFLOAT4 pos)
{
	this->updateMatrix(pos, this->focusPosition, this->upDirection);
}

void PointLight::updateColor(XMFLOAT4 color)
{
	this->color = color;
}

XMFLOAT4 PointLight::getAmbientComponent()
{
	return this->ambientComponent;
}

XMFLOAT4 PointLight::getDiffuseComponent()
{
	return this->diffuseComponent;
}

XMFLOAT4 PointLight::getSpecularComponent()
{
	return this->specularComponent;
}

void PointLight::updateMatrix(XMFLOAT4 lightPos, XMFLOAT4 FocusPos, XMFLOAT4 UpDir)
{
	XMVECTOR pos = XMVectorSet(lightPos.x, lightPos.y, lightPos.z, lightPos.w);
	XMVECTOR focusPos = XMVectorSet(FocusPos.x, FocusPos.y, FocusPos.z, FocusPos.w);
	XMVECTOR upPos = XMVectorSet(UpDir.x, UpDir.y, UpDir.z, UpDir.w);

	this->position = lightPos;
	this->view = { XMMatrixLookAtLH(pos,focusPos, upPos) * XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 0.75f, 0.1f, 100.0f) };
}