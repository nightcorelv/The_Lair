#pragma once
#include"GameState.h"
#include <vector>

//struct Records
//{
//	// add to score,r
//	static bool Add(float score);
//
//	//get score
//	float Get();
//
//	//save score array to a file
//	static void Save();
//	//load score array from file
//	static void Load();
//
//private:
//
//	static float data;
//};

class Records : public GameState
{
private:


	static DirectX::SpriteBatch* spriteBatch;
	GUISprite* backGround;

	GUIText* text;
	GUISprite* exitButton;

	static ID3D11RenderTargetView* spriteView;
	static ID3D11RasterizerState* rss;
	static ID3D11BlendState* blendState;
	static ID3D11DepthStencilState* dss;
	static ID3D11SamplerState* sampler;

	static UINT g_width;
	static UINT g_height;

public:

	
	static float data;

	Records(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height);
	virtual ~Records();

	// add to score,r
	static bool Add(float score);

	//get score
	float Get();

	//save score array to a file
	static void Save();
	//load score array from file
	static void Load();

	// Inherited via GameState
	virtual void update() override;

	virtual void render() override;

	virtual void handleEvents() override;

	//Sprite&Text
	virtual void Init(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height) override;

	virtual void SpritePass() override;

	/*virtual void Release() override;*/ //using destructor to release stuff


};
