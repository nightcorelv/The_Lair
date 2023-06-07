#include "Records.h"
#include <fstream>

DirectX::SpriteBatch* Records::spriteBatch = nullptr;

ID3D11RenderTargetView* Records::spriteView = nullptr;
ID3D11RasterizerState* Records::rss = nullptr;
ID3D11BlendState* Records::blendState;
ID3D11DepthStencilState* Records::dss = nullptr;
ID3D11SamplerState* Records::sampler = nullptr;

//not to Realese here, done in main.
ID3D11Device* gr_device = nullptr;
ID3D11DeviceContext* gr_immediateContext = nullptr;

///
UINT Records::g_width;
UINT Records::g_height;

float Records::data;
int recSec; 
int recMin; 
int recHor;


std::wstring recordTime = L" ";


Records::Records(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height)
	:GameState("Records")
{
	Init(device, immediateContext, width, height);
	backGround = new GUISprite(DirectX::SimpleMath::Vector2(g_width / 2, g_height/2), 0.0f);
	backGround->Load(gr_device, L"../Textures/Sprites/RecordsBackGround.dds");
	backGround->SetScale({ 0.60f,0.60f });


	text = new GUIText(DirectX::SimpleMath::Vector2(g_width / 2 - 400 , g_height / 2 - 30), 0.0f);
	text->Load(gr_device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	text->setText(L"Player"/*std::to_wstring((int)Time::CurrentTime()).c_str()*/L"Time");
	text->SetScale({ 1.5f, 1.5f });
	text->SetTint(DirectX::Colors::Aquamarine.v);

	
	exitButton = new GUISprite(DirectX::SimpleMath::Vector2(g_width - 150, g_height - 100 ), 0.0f);
	exitButton->Load(gr_device, L"../Textures/Sprites/Exit.dds");
	exitButton->SetScale({ 0.50f, 0.50f });
	exitButton->SetTint(DirectX::Colors::GreenYellow.v);

	Load();
	recSec = (int)data % 60;
	recMin = (int)data / 60;
	recHor = (int)data / 3600;
	recordTime = L"The Record Score: " + std::to_wstring(recHor) + L" : " + std::to_wstring(recMin) + L" : " + std::to_wstring(recSec);

	activeState = EnumState::NO_CHANGE;


}

Records::~Records()
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

	if (text != nullptr)
		delete text;

	if (exitButton != nullptr)
		delete exitButton;

}

void Records::SpritePass()
{
	spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState, sampler, dss, rss);

	//update this

	//Example to set text ......std::wstring DmgX = L"x " + std::to_wstring(g_player->getDmgIncrease());
							  //HPText->setText(DmgX.c_str());

	text->setText(recordTime.c_str());


	/////Draw Sprites Fonts etc.///

	backGround->Draw(spriteBatch); //smartpointer

	text->Draw(spriteBatch);

	exitButton->Draw(spriteBatch);
	///xxxx/// 

	spriteBatch->End();
	gr_immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);

}

void Records::Init(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, const unsigned int width, const unsigned int height)
{
	g_width = width;
	g_height = height;
	gr_device = device;
	gr_immediateContext = immediateContext;
	

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

	gr_device->CreateSamplerState(&samplerDesc, &sampler);

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

bool Records::Add(float score)
{
	bool ranked = false;
	if (score < data || data == 0)
	{
		data = score;
		ranked = true;
	}
	return ranked;
}

float Records::Get()
{
	return data;
}

void Records::Save()
{
	std::ofstream out("../data/score.txt");
	if (out.is_open())
	{
		out << data;
		out.close();
	}

}

void Records::Load()
{
	std::ifstream in("../data/score.txt");
	if (in.is_open())
	{
		in >> data;
		in.close();
	}
}

void Records::update()
{
	
}

void Records::render()
{

	SpritePass();
}

void Records::handleEvents()
{
	if (Input::GetKeyPressed(Input::Key::Enter)) //GetKeyPressed(Input::Key::Enter));
	{
		activeState = EnumState::MENU;

	}
}


//void Records::Release()
//{
//
//}

