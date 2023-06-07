#include "GuiHandler.h"
#include "Time.h"
#include "player.h"
#include "RoomHandler.h"
#include "Records.h"

DirectX::SpriteBatch* GuiHandler::spriteBatch = nullptr;
GUISprite* GuiHandler::spriteTimeBone = nullptr;
GUISprite* GuiHandler::spriteObjectBox = nullptr;

GUISprite* GuiHandler::spriteWeaponBox = nullptr;
GUISprite* GuiHandler::spriteHealthBack = nullptr;
GUISprite* GuiHandler::spriteHealthMiddle = nullptr;
GUISprite* GuiHandler::spriteHealthTop = nullptr;

GUISprite* GuiHandler::spriteWeaponBoneBoost = nullptr;
GUISprite* GuiHandler::spriteObjectShield = nullptr;
GUISprite* GuiHandler::spriteObjectShieldBoost = nullptr;

GUIText* GuiHandler::timeText = nullptr;
GUIText* GuiHandler::HPText = nullptr;
GUIText* GuiHandler::DmText = nullptr;

GUISprite* GuiHandler::mapBackground = nullptr;
vector<GUISprite*> GuiHandler::mapObjects;

GUISprite* GuiHandler::spriteGameOver = nullptr;
GUISprite* GuiHandler::spriteWin = nullptr;
GUISprite* GuiHandler::exitButton = nullptr;
GUISprite* GuiHandler::startButton = nullptr;

GUIText* GuiHandler::recordText = nullptr;
GUIText* GuiHandler::playerText = nullptr;

ID3D11RenderTargetView* GuiHandler::spriteView = nullptr;
ID3D11RasterizerState* GuiHandler::rss = nullptr;
ID3D11BlendState* GuiHandler::blendState;
ID3D11DepthStencilState* GuiHandler::dss = nullptr;

ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_immediateContext = nullptr;
ID3D11SamplerState* g_shadowSampler = nullptr;
Player* g_player = nullptr;
RoomHandler* g_roomHandler = nullptr;
bool GuiHandler::exit = false;
bool GuiHandler::once = true;

int g_sec = 0;
int g_min = 0;
int g_hor = 0;

void GuiHandler::SpritePass()
{
	spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState, g_shadowSampler, dss, rss);
	

	if (!g_player->isDead() && !g_player->isNecroManDead)
	{

		///Draw Sprites Fonts etc.///


		spriteHealthMiddle->SetScale({ g_player->getHPforHealthBar() + 0.02f, 0.5f });	///health scales from 0.0 till 0.3 player.getHPforHealthBar()

		if (g_player->hasDmgBoost())
		{
			spriteWeaponBoneBoost->Draw(spriteBatch);
		}
		else
		{
			spriteWeaponBox->Draw(spriteBatch);
		}

		if (g_player->hasShield())
		{
			spriteObjectShield->Draw(spriteBatch);
		}
		else
		{
			spriteObjectBox->Draw(spriteBatch);
		}

		//Map
		int currentRoomIndex = g_roomHandler->getCurrentRoomIndex();
		mapBackground->Draw(spriteBatch);
		vector<XMFLOAT3> map = g_roomHandler->getMap();
		int objectIndex = 0;
		for (int i = 0; i < map.size(); i++)
		{
			if (map.at(i).z > 0)
			{
				float screenCoordinateX = 1070 + (map.at(i).x * 140);
				float screenCoordinateY = 50 + ((map.at(i).y * -1) * 140);

				if (objectIndex < 15)	//Array size 15, aka max 15 rooms can be rendered
				{
					if (map.at(i).z == currentRoomIndex)
					{
						mapObjects.at(objectIndex)->SetTint(DirectX::Colors::Green.v);
					}
					else
					{
						mapObjects.at(objectIndex)->SetTint(DirectX::Colors::DarkGray.v);
					}
					mapObjects.at(objectIndex)->SetPosition(XMFLOAT2(screenCoordinateX, screenCoordinateY));
					objectIndex++;
				}
			}
		}


		//Draw Sprite

		spriteTimeBone->Draw(spriteBatch); //smartpointer

		spriteHealthBack->Draw(spriteBatch);
		spriteHealthMiddle->Draw(spriteBatch);
		spriteHealthTop->Draw(spriteBatch);

		for (int i = 0; i < 15; i++)
		{
			mapObjects.at(i)->Draw(spriteBatch);
		}

	}



	///xxxx/// 

	spriteBatch->End();
	g_immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);
}

void GuiHandler::TextPass()
{


	spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState, g_shadowSampler, dss, rss);

	if (!g_player->isDead() && !g_player->isNecroManDead)
	{
		int sec = (int)Time::CurrentTime() % 60;
		int min = (int)Time::CurrentTime() / 60;
		int hor = (int)Time::CurrentTime() / 3600;

		std::wstring time = std::to_wstring(hor) + L" : " + std::to_wstring(min) + L" : " + std::to_wstring(sec);
		///Draw Sprites Fonts etc.///
		timeText->setText(time.c_str()/*(int)Time::CurrentTime()).c_str()*/);

		//Draw Text
		timeText->Draw(spriteBatch);// , std::to_wstring(Time::CurrentTime()).c_str(), DirectX::SimpleMath::Vector2(300, 100)); //(.get returns the pointer from the uniqepointer ,text to display, position)

		std::wstring HpX = L"x " + std::to_wstring(g_player->getHpIncrease());
		std::wstring DmgX = L"x " + std::to_wstring(g_player->getDmgIncrease());


		if (g_player->getHpIncrease() >= 1)
		{
			HPText->setText(HpX.c_str());
			HPText->Draw(spriteBatch);
		}

		if (g_player->getDmgIncrease() >= 1)
		{
			DmText->setText(DmgX.c_str());
			DmText->Draw(spriteBatch);
		}


		//if (Input::GetKeyPressed(Input::Key::Up_Arrow))
		//{
		//	g_player->isNecroManDead = true;
		//}

	}
	else
	{

		if (g_player->isDead())
		{

			spriteGameOver->Draw(spriteBatch);

		}
		else if (g_player->isNecroManDead)
		{

			if (once)
			{
				Records::Load();
				if (Records::Add((int)Time::CurrentTime())) //if player time is a record, save the time to file
				{
					Records::Save();
				};
				g_sec = (int)Time::CurrentTime() % 60;
				g_min = (int)Time::CurrentTime() / 60;
				g_hor = (int)Time::CurrentTime() / 3600;

				once = false;
			}

			
			int theRecord = Records::data;  //Get The RecordTime from file

			int recSec = theRecord % 60;
			int recMin = theRecord / 60;
			int recHor = theRecord / 3600;

			std::wstring recordTime = L"The Record Score: " + std::to_wstring(recHor) + L" : " + std::to_wstring(recMin) + L" : " + std::to_wstring(recSec);
			std::wstring playerTime = L"Your Time: " + std::to_wstring(g_hor) + L" : " + std::to_wstring(g_min) + L" : " + std::to_wstring(g_sec);

			recordText->setText(recordTime.c_str());
			playerText->setText(playerTime.c_str());

			spriteWin->Draw(spriteBatch);
			recordText->Draw(spriteBatch);
			playerText->Draw(spriteBatch);

			
		}

		exitButton->SetPosition(DirectX::SimpleMath::Vector2(150, startButton->GetPosition().y));

		if (Input::GetKeyPressed(Input::Key::Right_Arrow) || Input::GetKeyPressed(Input::Key::D))
		{
			if (exitButton->GetTint() == DirectX::Colors::GreenYellow.v)
			{
				startButton->SetTint(DirectX::Colors::GreenYellow.v);

				exitButton->SetTint(DirectX::SimpleMath::Color(1, 1, 1));
			}

		}
		if (Input::GetKeyPressed(Input::Key::Left_Arrow) || Input::GetKeyPressed(Input::Key::A))
		{
			if (startButton->GetTint() == DirectX::Colors::GreenYellow.v)
			{
				exitButton->SetTint(DirectX::Colors::GreenYellow.v);
				startButton->SetTint(DirectX::SimpleMath::Color(1, 1, 1));

				
			}

		}

		if (Input::GetKeyPressed(Input::Key::Enter)) //GetKeyPressed(Input::Key::Enter));
		{

			if (startButton->GetTint() == DirectX::Colors::GreenYellow.v)
			{
				g_roomHandler->setReset(true);
				/*g_player->isNecroManDead = false;*/
				once = true;
			}
			else
			{
				exit = true;

			}

		}


		exitButton->Draw(spriteBatch);
		startButton->Draw(spriteBatch);



	}

	///xxxx/// 

	spriteBatch->End();
	g_immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);
}

void GuiHandler::Init(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, ID3D11SamplerState* shadowSampler, const unsigned int width, const unsigned int height, Player* player, RoomHandler& roomHandler)
{
	g_device = device;
	g_immediateContext = immediateContext;
	g_shadowSampler = shadowSampler; 
	g_player = player;
	g_roomHandler = &roomHandler;

	spriteBatch = new DirectX::SpriteBatch(immediateContext);
	float scale = 0.5f;
	spriteTimeBone = new GUISprite(DirectX::SimpleMath::Vector2(width / 2, 100), 0.0f);
	spriteTimeBone->Load(device, L"../Textures/Sprites/TimeBone.dds");
	spriteTimeBone->SetScale({ 0.35f, 0.35f });
	
	//Textures= "WeaponBoxEmty.dds", "WeaponBoxBone.dds", "WeaponBoxBoneBoost.dds"
	spriteWeaponBox = new GUISprite(DirectX::SimpleMath::Vector2(200 - 70, 90 + 120), 0.0f);
	spriteWeaponBox->Load(device, L"../Textures/Sprites/WeaponBoxBone.dds");
	spriteWeaponBox->SetScale({ scale,scale });

	spriteWeaponBoneBoost = new GUISprite(DirectX::SimpleMath::Vector2(200 - 70, 90 + 120), 0.0f);
	spriteWeaponBoneBoost->Load(device, L"../Textures/Sprites/WeaponBoxBoneBoost.dds");
	spriteWeaponBoneBoost->SetScale({ scale,scale });

	//Textures= "ObjectBoxEmty.dds", "ObjectBoxShield.dds", "ObjectBoxShieldBoost.dds"
	spriteObjectBox = new GUISprite(DirectX::SimpleMath::Vector2(200 + 60, 90 + 120), 0.0f);
	spriteObjectBox->Load(device, L"../Textures/Sprites/ObjectBoxEmty.dds"); 
	spriteObjectBox->SetScale({ scale,scale });

	spriteObjectShield = new GUISprite(DirectX::SimpleMath::Vector2(200 + 60, 90 + 120), 0.0f);
	spriteObjectShield->Load(device, L"../Textures/Sprites/ObjectBoxShield.dds");
	spriteObjectShield->SetScale({ scale,scale });

	spriteObjectShieldBoost = new GUISprite(DirectX::SimpleMath::Vector2(200 + 60, 90 + 120), 0.0f);
	spriteObjectShieldBoost->Load(device, L"../Textures/Sprites/ObjectBoxShieldBoost.dds");
	spriteObjectShieldBoost->SetScale({ scale,scale });


	//DONT TOUTCH ANYTHING!!!!! exept SetScale on X-value
	spriteHealthBack = new GUISprite(DirectX::SimpleMath::Vector2(200, 80), 0.0f);
	spriteHealthBack->Load(device, L"../Textures/Sprites/HealthBack.dds");
	spriteHealthBack->SetScale({ scale,scale });

	spriteHealthMiddle = new GUISprite(DirectX::SimpleMath::Vector2(131, 93), 0.0f);
	spriteHealthMiddle->Load(device, L"../Textures/Sprites/HealthBar.dds");
	spriteHealthMiddle->SetScale({ scale , scale});	///health scales from 0.0 till 0.3
	spriteHealthMiddle->SetOrigin(DirectX::SimpleMath::Vector2(150));

	spriteHealthTop = new GUISprite(DirectX::SimpleMath::Vector2(200, 80), 0.0f);
	spriteHealthTop->Load(device, L"../Textures/Sprites/HealthTop.dds");
	spriteHealthTop->SetScale({ scale,scale });

	timeText = new GUIText(DirectX::SimpleMath::Vector2(width / 2 - 70, 100 - 30), 0.0f);
	timeText->Load(device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	timeText->setText(std::to_wstring((int)Time::CurrentTime()).c_str()/*L"Time"*/);
	timeText->SetTint(DirectX::Colors::SaddleBrown.v);

	HPText = new GUIText(DirectX::SimpleMath::Vector2(spriteHealthTop->GetPosition().x - (spriteHealthTop->GetWidth() /1.45), 95), 0.0f);
	HPText->Load(device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	HPText->setText(L"X");
	HPText->SetTint(DirectX::Colors::Beige.v);

	DmText = new GUIText(DirectX::SimpleMath::Vector2(spriteHealthTop->GetPosition().x - (spriteHealthTop->GetWidth() / 1.45), 230), 0.0f);
	DmText->Load(device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	DmText->setText(L"X");
	DmText->SetTint(DirectX::Colors::Beige.v);


	//GameOver/xxx/GameWin
	spriteGameOver = new GUISprite(DirectX::SimpleMath::Vector2(width/2, height/2), 0.0f);
	spriteGameOver->Load(device, L"../Textures/Sprites/GameOver.dds");
	spriteGameOver->SetScale({ 0.68f, 0.68f });

	spriteWin = new GUISprite(DirectX::SimpleMath::Vector2(width / 2, height / 2), 0.0f);
	spriteWin->Load(device, L"../Textures/Sprites/RecordsBackGround.dds");
	spriteWin->SetScale({ 0.68f, 0.68f });

	exitButton = new GUISprite(DirectX::SimpleMath::Vector2(width - 150, height - 100), 0.0f);
	exitButton->Load(device, L"../Textures/Sprites/Exit.dds");
	exitButton->SetScale({ 0.7f, 0.7f });

	startButton = new GUISprite(DirectX::SimpleMath::Vector2(width - 150, height - 100), 0.0f);
	startButton->Load(device, L"../Textures/Sprites/Start.dds");
	startButton->SetScale({ 0.5f, 0.5f });
	startButton->SetTint(DirectX::Colors::GreenYellow.v);

	recordText = new GUIText(DirectX::SimpleMath::Vector2(width / 2 - 500, height / 2 - 100), 0.0f);
	recordText->Load(device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	recordText->setText(L"The Record Score: ");
	recordText->SetTint(DirectX::Colors::Aquamarine.v);
	recordText->SetScale({ 1.8f, 1.8f });

	playerText = new GUIText(DirectX::SimpleMath::Vector2(width / 2 - 500, height/2 + 40), 0.0f);
	playerText->Load(device, L"../Textures/Sprites/GothenbergTextura.spritefont");
	playerText->setText(L"Your Time: ");
	playerText->SetTint(DirectX::Colors::Aquamarine.v);
	playerText->SetScale({ 1.8f, 1.8f });

	//Map
	for (int i = 0; i < 15; i++)
	{
		mapObjects.push_back(new  GUISprite(DirectX::SimpleMath::Vector2(1500, 90), 0.0f));
		mapObjects.at(i)->Load(device, L"../Textures/Sprites/WeaponBoxEmty.dds");
		mapObjects.at(i)->SetScale({ 0.1,0.1 });
	}
	//Map Background
	mapBackground = new GUISprite(DirectX::SimpleMath::Vector2(1150, 125), 0.0f);
	mapBackground->Load(device, L"../Textures/Sprites/testMapBackground.dds");
	mapBackground->SetScale({ 0.5f,0.5f });



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

void GuiHandler::Update()
{
	SpritePass();
	TextPass();

}

void GuiHandler::Release()
{
	if(spriteView != nullptr)
		spriteView->Release();

	if (rss != nullptr)
		rss->Release();

	if (blendState != nullptr)
		blendState->Release();

	if (dss != nullptr)
		dss->Release();

	if (spriteBatch != nullptr)
		delete spriteBatch;

	if (spriteTimeBone != nullptr)
		delete spriteTimeBone;

	if (spriteObjectBox != nullptr)
		delete spriteObjectBox;

	if (spriteWeaponBox != nullptr)
		delete spriteWeaponBox;

	if (spriteHealthBack != nullptr)
		delete spriteHealthBack;

	if (spriteHealthMiddle != nullptr)
		delete spriteHealthMiddle;

	if (spriteHealthTop != nullptr)
		delete spriteHealthTop;

	if (spriteWeaponBoneBoost != nullptr)
		delete spriteWeaponBoneBoost;

	if (spriteObjectShield != nullptr)
		delete spriteObjectShield;

	if (spriteObjectShieldBoost != nullptr)
		delete spriteObjectShieldBoost;

	for (int i = 0; i < 15; i++)
	{
		if (mapObjects.at(i) != nullptr)
		{
			delete mapObjects.at(i);
		}
	}

	if (mapBackground != nullptr)
		delete mapBackground;

	if (timeText != nullptr)
		delete timeText;

	if (HPText != nullptr)
		delete HPText;

	if (DmText != nullptr)
		delete DmText;

	if (spriteGameOver != nullptr)
		delete spriteGameOver;

	if (spriteWin != nullptr)
		delete spriteWin;

	if (exitButton != nullptr)
		delete exitButton;

	if (startButton != nullptr)
		delete startButton;

	if (recordText != nullptr)
		delete recordText;

	if (playerText != nullptr)
		delete playerText;
}
