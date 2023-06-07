#pragma once
#include "Mesh.h"

class PointLight
{
private:

	XMMATRIX view;
	XMFLOAT4 upDirection;
	XMFLOAT4 focusPosition;
	XMFLOAT4 position;
	XMFLOAT4 ambientComponent;
	XMFLOAT4 diffuseComponent;
	XMFLOAT4 specularComponent;
	XMFLOAT4 color;

public:

	PointLight(ID3D11Device * device, XMFLOAT4 lightColor, XMFLOAT4 lightPos, XMFLOAT4 FocusPos, XMFLOAT4 UpDir, XMFLOAT4 ambComp, XMFLOAT4 difComp, XMFLOAT4 speComp);
	XMFLOAT4 getPosition();
	XMFLOAT4 getColor();
	XMFLOAT4 getUpDir();
	XMFLOAT4 getFocusPos();
	XMMATRIX getViewMatrix();
	XMFLOAT4 getAmbientComponent();
	XMFLOAT4 getDiffuseComponent();
	XMFLOAT4 getSpecularComponent();
	void updatePosition(XMFLOAT4 pos);
	void updateColor(XMFLOAT4 color);
	void updateMatrix(XMFLOAT4 lightPos, XMFLOAT4 FocusPos, XMFLOAT4 UpDir);

};