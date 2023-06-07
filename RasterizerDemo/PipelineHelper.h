#pragma once
#include <Windows.h>
#include <array>
#include <d3d11.h>
#include <string>
#include <fstream>
#include <iostream>
#include <DirectXMath.h>
#include<string>
#include<vector>
#include<sstream>
#include <array>

using namespace DirectX;
using namespace std;

struct SimpleVertex
{
	float pos[3];
	float norm[3];
	float uv[2];
	//int id;

	SimpleVertex(const std::array<float, 3>& position, const std::array<float, 3> color, const std::array<float, 2> uv/*, const int id*/)
	{
		for (int i = 0; i < 3; i++)
		{
			pos[i] = position[i];
			norm[i] = color[i];
		}
		this->uv[0] = uv[0];
		this->uv[1] = uv[1];
		//this->id = id;
	}
};

struct SkinnedVtx
{
	XMINT4 conBones;	//int4 bones;
	XMFLOAT4 weights;	//float4 weights;
	int id;				//-1 means no bones
};

struct boneMx
{
	XMFLOAT4X4 mx[50];
};

bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderGeo, ID3D11PixelShader*& pShader, ID3D11PixelShader*& PXShaderGeo, ID3D11PixelShader*& PT_PixelShader2, ID3D11ComputeShader*& cubeMapShader,
	ID3D11GeometryShader*& gShader, ID3D11ComputeShader*& particleUpdateShader, ID3D11ComputeShader*& PT_UpdateShader_Rising, ID3D11InputLayout*& inputLayout, ID3D11InputLayout*& inputLayoutGeo, ID3D11Buffer*& constBuffer, ID3D11Buffer*& PtListBuffer,
	ID3D11Buffer*& PtworldMatrix, ID3D11SamplerState*& samplerState, ID3D11SamplerState*& shadowSampler, ID3D11Buffer*& CameraPosBuffer, ID3D11ComputeShader*& cShader, ID3D11UnorderedAccessView*& particleUav,
	ID3D11Buffer*& timeBuffer);