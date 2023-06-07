#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include <d3d11.h>
#include "RoomHandler.h"

class Player;
class GuiHandler
{

private:

	static DirectX::SpriteBatch* spriteBatch;
	static GUISprite* spriteTimeBone;
	static GUISprite* spriteObjectBox;
	static GUISprite* spriteWeaponBox;
	static GUISprite* spriteHealthBack;
	static GUISprite* spriteHealthMiddle;
	static GUISprite* spriteHealthTop;

	static GUISprite* spriteWeaponBoneBoost;
	static GUISprite* spriteObjectShield;
	static GUISprite* spriteObjectShieldBoost;
	

	static GUIText* timeText;
	static GUIText* HPText;
	static GUIText* DmText;

	static vector<GUISprite*> mapObjects;
	static GUISprite* mapBackground;

	static GUISprite* spriteGameOver;
	static GUISprite* spriteWin;
	static GUISprite* exitButton;
	static GUISprite* startButton;

	static GUIText* recordText;
	static GUIText* playerText;

	static ID3D11RenderTargetView* spriteView;
	static ID3D11RasterizerState* rss;
	static ID3D11BlendState* blendState;
	static ID3D11DepthStencilState* dss;

	static void SpritePass();
	static void TextPass();
	
public:
	static bool once;
	static bool exit;
	static void Init(ID3D11Device * &device, ID3D11DeviceContext * &immediateContext, ID3D11SamplerState * shadowSampler, const unsigned int width, const unsigned int height, Player * player, RoomHandler & roomHandler); 
	static void Update();
	static void Release();
};

