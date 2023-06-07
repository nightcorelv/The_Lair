#pragma once
#include"GameState.h"

class Menu :public GameState
{
private:

	int selected = 1;
	///xxxx///
	static DirectX::SpriteBatch* spriteBatch;
	GUISprite* backGround = nullptr;
	GUISprite* buttonStart = nullptr;		//Selected 1
	GUISprite* buttonExit = nullptr;		//Selected 2
	GUISprite* buttonRecords = nullptr;		//Selected 3
	GUISprite* buttonHowToPlay = nullptr;	//Selected 4
	GUISprite* howToPlay = nullptr;
	
	//GUIText* text;

	static ID3D11RenderTargetView* spriteView;
	static ID3D11RasterizerState* rss;
	static ID3D11BlendState* blendState;
	static ID3D11DepthStencilState* dss;
	static ID3D11SamplerState* sampler;

	///xxx///

	 
	static UINT g_width;
	static UINT g_height;
	bool howToPlaySpriteActive = false;

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void SetState();

public:

	Menu(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height);
	virtual ~Menu();


	// Inherited via GameState
	virtual void update() override;

	virtual void render() override;

	virtual void handleEvents() override;

	virtual void Init(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height) override;

	virtual void SpritePass() override;

	///*virtual void Release() override;*/ //using destructor to realese stuff

	///xxx///

};