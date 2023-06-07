#include "DXApp.h"
#include "GameState.h"
#include "Menu.h"
#include "Records.h"
#include "Input.h"

namespace 
{
	//used to forward msg to user-defined proc funktion
	DXApp* g_pApp = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if(g_pApp)
	//	return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	//else 
	//	return DefWindowProc(hwnd, msg, wParam, lParam);

	if(g_pApp != nullptr)
		return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	else
	{
		return 0;
	}

}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_QUIT:
	{ 
		if (exit)
		{
			PostQuitMessage(0);
		}
		/*if (PostQuitMessage(0))
		{

		}*/
		/*PostQuitMessage(0);*/
		break;
	}
	case WM_KEYDOWN:
		Input::SetKeyDown(wParam);
		break;

	case WM_KEYUP:
		Input::SetKeyUp(wParam);
		break;

	case WM_LBUTTONDOWN:
		Input::SetKeyDown(0x01);
		break;

	case WM_RBUTTONDOWN:
		Input::SetKeyDown(0x02);
		break;

	case WM_LBUTTONUP:
		Input::SetKeyUp(0x01);
		break;

	case WM_RBUTTONUP:
		Input::SetKeyUp(0x02);
		break;

	case WM_DESTROY:
		if(!play)
			PostQuitMessage(0);
		/*PostQuitMessage(0);*/
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	//return LRESULT();
}


DXApp::DXApp(HINSTANCE hInstance)
{
	//window initialization

	m_hAppWnd = NULL;					//NULL no Window yet
	m_hAppInstance = hInstance;
	m_ClientWidth = 1280;
	m_ClientHeight = 720;
	m_AppTitel = L" The Lair ";
	m_WndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;                     //initzialize as g_pApp

	//direct3d initialization

	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pRTV = nullptr;
	m_pSwapChain = nullptr;
	

}

DXApp::~DXApp(void)
{
	//CleanUp Direct3D
	//if (m_pImmediateContext)
	//{
	//	if(m_pImmediateContext)
	//	m_pImmediateContext->ClearState();
	//}
	if (m_pRTV != nullptr)
	{
		Memory::SafeRelease(m_pRTV);
	}
	if (m_pSwapChain != nullptr)
	{
		Memory::SafeRelease(m_pSwapChain);
	}
	if (m_pImmediateContext != nullptr)
	{
		Memory::SafeRelease(m_pImmediateContext);
	}	
	if (m_pDevice != nullptr)
	{
		Memory::SafeRelease(m_pDevice);
	}

	UnregisterClassW(L"DXAPPWNDCLASS", m_hAppInstance);
	DestroyWindow(m_hAppWnd);
	
	
}


bool DXApp::Run()//main //fixa bool för krysset //fixa popa upp i bakgrunden?loading
{
	Init();
	GameState* current = nullptr;
	
	current = new Menu(m_pDevice, m_pImmediateContext, m_ClientWidth, m_ClientHeight);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		//if (!play)
		//{

		current->handleEvents();
		current->update();
		m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);
		current->render();
		//render()
		m_pSwapChain->Present(1, 0);

		//}
		//else
		//{
		//	break;
		//}
			
		if (GameState::activeState != EnumState::NO_CHANGE)
		{

			switch (GameState::activeState)
			{
			case EnumState::MENU:
				delete current;
				m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);

				current = new Menu(m_pDevice, m_pImmediateContext, m_ClientWidth, m_ClientHeight);
				break;
			case EnumState::PLAY:
				delete current;
				
				play = true;

				/*current = nullptr;*/
				//CloseWindow(m_hAppWnd);
				//UnregisterClassW(L"DXAPPWNDCLASS", m_hAppInstance);
				//DestroyWindow(m_hAppWnd);
				return true;

				/*current = nullptr;*/
				break;
			case EnumState::RECORDS:
				delete current;

				m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);

				current = new Records(m_pDevice, m_pImmediateContext, m_ClientWidth, m_ClientHeight);
				break;
			case EnumState::EXIT: //minnesLäcka när man kör exit // trycket man på krysset stänger den inte ner programet antagligen för att main fortfarande körs.
				delete current;

				/*current = nullptr;*/
				/*CloseWindow(m_hAppWnd);*/
				/*msg.message = WM_QUIT;*/
				exit = true;
				return false; 
				break;
			}
		}
	}

	if(current != nullptr)
		delete current;



	/*CloseWindow(m_hAppWnd);*/
	return false; /* play;*/ //there our exitCode is
}


bool DXApp::Init()
{
	if (!InitWindow())
	{
		return false;

	}

	if (!InitDirect3D())
	{
		return false;
	}

	return true;
}

bool DXApp::InitWindow()
{
	//windowclass fror properties of window
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0; //extra bytes
	wcex.cbWndExtra = 0; //extra bytes
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;					// draw horizontal and vertigal ,DirectX overrides this
	wcex.hInstance = m_hAppInstance;						//set instance
	wcex.lpfnWndProc = MainWndProc;							//no funktion as callback
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);			//can be used to set custum icon -import resoursfile- make entry source id 
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);				//Cursore style
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); //first cleared to color first run or re-draw
	wcex.lpszMenuName = NULL;								//no windows menu Name
	wcex.lpszClassName = L"DXAPPWNDCLASS";					//Defines the name of the class IMPORTANT
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			//smaller icon

	if (!RegisterClassEx(&wcex)) //regrister window
	{
		OutputDebugString(L"\nFAILED TO CREATE WINDOW-CLASS!\n");
		return false;
	}

	//Adjusts the window to specified size without top title-Bar = GameWindow
	RECT r = { 0, 0, m_ClientWidth, m_ClientHeight }; 
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;

	//CenterWindow to middle of Screen
	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	//Creates window

	m_hAppWnd = CreateWindowExW(0,L"DXAPPWNDCLASS", m_AppTitel.c_str() , m_WndStyle, x, y, width, height, NULL, NULL, m_hAppInstance, NULL);
	
	if (!m_hAppWnd)
	{
		OutputDebugString(L"\nFAILED TO CREATE WINDOW!\n");
		return false;

	}

	if (::GetWindowInfo(m_hAppWnd, &info))
	{
		RECT rect;
		rect = info.rcClient;
		m_ClientWidth = rect.right - rect.left;
		m_ClientHeight = rect.bottom - rect.top;
	}

	ShowWindow(m_hAppWnd, SW_SHOW); 
	return true;
}

bool DXApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif//DeBug

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,   //use this 
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = //can target older features of dirextX
	{
		
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3

	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapdesc;
	ZeroMemory(&swapdesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapdesc.BufferCount = 1; //dubbel buffer
	swapdesc.BufferDesc.Width = m_ClientWidth;
	swapdesc.BufferDesc.Height = m_ClientHeight;
	swapdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapdesc.BufferDesc.RefreshRate.Numerator = 60;
	swapdesc.BufferDesc.RefreshRate.Denominator = 1;
	swapdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapdesc.OutputWindow = m_hAppWnd;
	swapdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapdesc.Windowed = true;
	swapdesc.SampleDesc.Count = 1;
	swapdesc.SampleDesc.Quality = 0;
	swapdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //alt-Enter FUllScreen ,wont resize buffer yet

	HRESULT result;
	for (int i=0;i<numDriverTypes;i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, NULL, featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
			&swapdesc, &m_pSwapChain, &m_pDevice, &m_FeatureLevel, &m_pImmediateContext);
		if (SUCCEEDED(result))
		{
			m_DriverType = driverTypes[i];
			break;
		}

	}
	if (FAILED(result))
	{
		OutputDebugString(L"FAILED TO CREATE DEVICE AND SWAPCHAIN!");
		return false;
	}

	//D3D11_RENDER_TARGET_VIEW_DESC desc = {};
	//desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//desc.Texture2D.MipSlice = 0;

	ID3D11Texture2D* m_pBackBufferTexture;
	m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTexture)); //store buffer to backbuffer texture
	//m_pDevice->CreateShaderResourceView(texture2D.data, &description, &shaderdata);
	m_pDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pRTV);
	Memory::SafeRelease(m_pBackBufferTexture);

	//Bind RenderTargetView
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, nullptr); //(numberOfViewstoSEt, target, depthstencil)

	//create viewPort
	m_Viewport.Width = static_cast<float>(m_ClientWidth);
	m_Viewport.Height = static_cast<float>(m_ClientHeight);
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	//BindViewPort
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	return true;
}

//void DXApp::Update(float dt)
//{
//}




