#include "Menu.h"
#include <iostream>
#include "Input.h"


DirectX::SpriteBatch* Menu::spriteBatch = nullptr;

ID3D11RenderTargetView* Menu::spriteView = nullptr;
ID3D11RasterizerState* Menu::rss = nullptr;
ID3D11BlendState* Menu::blendState;
ID3D11DepthStencilState* Menu::dss = nullptr;
ID3D11SamplerState* Menu::sampler = nullptr;

//not to Realese here, done in main.
ID3D11Device* gm_device = nullptr;
ID3D11DeviceContext* gm_immediateContext = nullptr;

///
UINT Menu::g_width;
UINT Menu::g_height;


Menu::Menu(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height)
	:GameState("Menu")
{

	Init(device, immediateContext, width, height);
	
	float scale = 0.5f;
	backGround = new GUISprite(DirectX::SimpleMath::Vector2(g_width / 2, g_height/2), 0.0f);
	backGround->Load(gm_device, L"../Textures/Sprites/BackGroundTest.dds");
	backGround->SetScale({ scale, scale });

	buttonStart = new GUISprite(DirectX::SimpleMath::Vector2(g_width - 200, g_height - 100), 0.0f);
	buttonStart->Load(gm_device, L"../Textures/Sprites/Start.dds");
	buttonStart->SetScale({ scale, scale });
	buttonStart->SetPosition(DirectX::SimpleMath::Vector2(g_width - 200, g_height - 100));
	buttonStart->SetTint(DirectX::Colors::GreenYellow.v);

	buttonExit = new GUISprite(DirectX::SimpleMath::Vector2(200, g_height - 80), 0.0f);
	buttonExit->Load(gm_device, L"../Textures/Sprites/Exit.dds");
	buttonExit->SetScale({ scale, scale });
	buttonExit->SetPosition(DirectX::SimpleMath::Vector2(buttonExit->GetWidth() / 2 + 50, g_height - 80));

	buttonRecords = new GUISprite(DirectX::SimpleMath::Vector2(200, buttonExit->GetPosition().y - 110 ), 0.0f);
	buttonRecords->Load(gm_device, L"../Textures/Sprites/Records.dds");
	buttonRecords->SetScale({ scale, scale });
	buttonRecords->SetPosition(DirectX::SimpleMath::Vector2(buttonRecords->GetWidth()/2 + 50, buttonExit->GetPosition().y - 110));

	buttonHowToPlay = new GUISprite(DirectX::SimpleMath::Vector2(200, buttonRecords->GetPosition().y - 110), 0.0f);
	buttonHowToPlay->Load(gm_device, L"../Textures/Sprites/HowToPlay.dds");
	buttonHowToPlay->SetScale({ scale, scale });
	buttonHowToPlay->SetPosition(DirectX::SimpleMath::Vector2(buttonHowToPlay->GetWidth()/2 + 50, buttonRecords->GetPosition().y - 110));

	howToPlay = new GUISprite(DirectX::SimpleMath::Vector2(g_width / 2, g_height / 2), 0.0f);
	howToPlay->Load(gm_device, L"../Textures/Sprites/Tutorial.dds");
	howToPlay->SetScale({ 0.66f, 0.66f });

	//text = new GUIText(DirectX::SimpleMath::Vector2(g_width / 2 - 70, 110 - 30), 0.0f);
	//text->Load(g_device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	//text->setText(L"Player"/*std::to_wstring((int)Time::CurrentTime()).c_str()*/L"Time");
	//text->SetTint(DirectX::Colors::SaddleBrown.v);

	activeState = EnumState::NO_CHANGE;
}

Menu::~Menu()
{
	if (spriteView != nullptr)
		spriteView->Release();

	if (rss != nullptr)
		rss->Release();

	if (blendState != nullptr)
		blendState->Release();

	if (dss != nullptr)
		dss->Release();

	if (sampler != nullptr)
		sampler->Release();

	if (spriteBatch != nullptr)
		delete spriteBatch;

	if (backGround != nullptr)
		delete backGround;

	if (buttonStart != nullptr)
		delete buttonStart;

	if (buttonExit != nullptr)
		delete buttonExit;

	if (buttonRecords != nullptr)
		delete buttonRecords;

	if (buttonHowToPlay != nullptr)
		delete buttonHowToPlay;

	if (howToPlay != nullptr)
		delete howToPlay;

	//if (text != nullptr)
	//	delete text;

}



void Menu::Init(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext,const unsigned int width, const unsigned int height)
{
	g_width = width;
	g_height = height;
	gm_device = device;
	gm_immediateContext = immediateContext;
	
	spriteBatch = new DirectX::SpriteBatch(immediateContext);

	D3D11_SAMPLER_DESC samplerDesc = {};
	//filter Nearst Neighbhor or Bilinear for 2D textures, linear get a smoother then zoom in to the texture
	//min is minification, mag is bigger, mip is mipmapping
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	gm_device->CreateSamplerState(&samplerDesc, &sampler);

	//


	D3D11_RASTERIZER_DESC desc = {};
	desc.AntialiasedLineEnable = false;

	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&desc, &rss);

	D3D11_DEPTH_STENCIL_DESC dssdesc = {};
	dssdesc.DepthEnable = true;
	dssdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dssdesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; //D3D11_COMPARISON_LESS //D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;//D3D11_COMPARISON_LESS_EQUAL
	device->CreateDepthStencilState(&dssdesc, &dss);


	//SpriteBlendShape
	D3D11_BLEND_DESC blendDesc{};
	D3D11_RENDER_TARGET_BLEND_DESC& brt = blendDesc.RenderTarget[0];

	brt.BlendEnable = true;
	brt.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	brt.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	brt.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	brt.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	brt.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	brt.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL; //write all channels

	device->CreateBlendState(&blendDesc, &blendState);
}

void Menu::SpritePass()
{
	spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState, sampler, dss, rss);


	/////Draw Sprites Fonts etc.///
	// 
	//text->setText(std::to_wstring(Time::CurrentTime()).c_str());
	////Draw Text
	//text->Draw(spriteBatch);


	//Draw Sprite

	if(howToPlaySpriteActive)
	{
		howToPlay->Draw(spriteBatch);
	}
	else
	{
		backGround->Draw(spriteBatch); //smartpointer
		buttonStart->Draw(spriteBatch);
		buttonExit->Draw(spriteBatch);
		buttonRecords->Draw(spriteBatch);
		buttonHowToPlay->Draw(spriteBatch);

	}
	
	///xxxx/// 

	spriteBatch->End();
	gm_immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);

}

void Menu::moveUp()
{

	if (Input::GetKeyPressed(Input::Key::Up_Arrow) || Input::GetKeyPressed(Input::Key::W))
	{
		if (buttonHowToPlay->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonExit->SetTint(DirectX::Colors::GreenYellow.v);
			
			buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 2;
		}
		else if (buttonStart->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonHowToPlay->SetTint(DirectX::Colors::GreenYellow.v);

			buttonExit->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 4;
		}
		else if (buttonRecords->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonHowToPlay->SetTint(DirectX::Colors::GreenYellow.v);

			buttonExit->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 4;
		}
		else if (buttonExit->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonRecords->SetTint(DirectX::Colors::GreenYellow.v);

			buttonExit->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 3;
		}
	}
}

void Menu::moveDown()
{

	if (Input::GetKeyPressed(Input::Key::Down_Arrow) || Input::GetKeyPressed(Input::Key::S)) //detta sker redan n�r funktionen kallas???
	{
	    if (buttonExit->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonHowToPlay->SetTint(DirectX::Colors::GreenYellow.v);

			buttonExit->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 4;
		}
		else if (buttonStart->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonExit->SetTint(DirectX::Colors::GreenYellow.v);

			buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 2;
		}
		else if (buttonRecords->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonExit->SetTint(DirectX::Colors::GreenYellow.v);

			buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 2;
		}
		else if (buttonHowToPlay->GetTint() == DirectX::Colors::GreenYellow.v)
		{
			buttonRecords->SetTint(DirectX::Colors::GreenYellow.v);

			buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonExit->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			selected = 3;
		}
	}
}

void Menu::moveRight()
{

	if (Input::GetKeyPressed(Input::Key::Right_Arrow) || Input::GetKeyPressed(Input::Key::D))
	{
		buttonStart->SetTint(DirectX::Colors::GreenYellow.v);

		buttonExit->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
		buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
		buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
		selected = 1;
	}
}

void Menu::moveLeft()
{
	if (Input::GetKeyPressed(Input::Key::Left_Arrow) || Input::GetKeyPressed(Input::Key::A))
	{
		buttonExit->SetTint(DirectX::Colors::GreenYellow.v);

		buttonStart->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
		buttonRecords->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
		buttonHowToPlay->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
		selected = 2;
	}
}

void Menu::SetState()
{
	if (selected == 2)/*(buttonExit->GetTint() == DirectX::Colors::GreenYellow.v)*/
	{
		activeState = EnumState::EXIT;

	}
	else if (selected == 1)/*(buttonStart->GetTint() == DirectX::Colors::GreenYellow.v)*/
	{
		activeState = EnumState::PLAY;
	}
	else if (selected == 3)/*(buttonRecords->GetTint() == DirectX::Colors::GreenYellow.v)*/
	{
		activeState = EnumState::RECORDS;

	}
	else if (selected == 4)/*(buttonHowToPlay->GetTint() == DirectX::Colors::GreenYellow.v)*/
	{
		howToPlaySpriteActive = true;
	}

}

void Menu::update()
{

	if (!howToPlaySpriteActive)
	{
		if (Input::GetKeyPressed(Input::Key::Enter)) //GetKeyPressed(Input::Key::Enter));
		{
			SetState();

		}

	}
	else if (howToPlaySpriteActive && Input::GetKeyPressed(Input::Key::Enter))
	{
		howToPlaySpriteActive = false;
	}
	
}

void Menu::render()
{
	SpritePass();
	

}

void Menu::handleEvents()
{

	moveUp();
	moveDown();
	moveRight();
	moveLeft();

}

//void Menu::Release()
//{
//	if (spriteView != nullptr)
//		spriteView->Release();
//
//	if (rss != nullptr)
//		rss->Release();
//
//	if (blendState != nullptr)
//		blendState->Release();
//
//	if (dss != nullptr)
//		dss->Release();
//
//	if (spriteBatch != nullptr)
//		delete spriteBatch;
//
//	if (backGround != nullptr)
//		delete backGround;
//
//	if (buttonStart != nullptr)
//		delete buttonStart;
//
//	if (buttonExit != nullptr)
//		delete buttonExit;
//
//	if (buttonRecords != nullptr)
//		delete buttonRecords;
//
//	if (buttonHowToPlay != nullptr)
//		delete buttonHowToPlay;
//
//	if (howToPlay != nullptr)
//		delete howToPlay;
//
//	//if (text != nullptr)
//	//	delete text;
//
//}

