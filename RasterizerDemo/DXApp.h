#pragma once
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "DXUtil.h"


class DXApp
{
	bool play = false;
	bool exit = false;

public:
	

	DXApp(HINSTANCE hInstance);
	virtual~DXApp(void);
	float color[4]{ 0,0,0,0 };

	//Main appication loop
	bool Run();

	//Framework metods
	virtual bool Init();
	//virtual void Update(float dt) = 0;
	//virtual void Render(float dt) = 0;
	virtual LRESULT MsgProc(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam);
	UINT		m_ClientWidth;
	UINT		m_ClientHeight;

protected:
	WINDOWINFO info = {};
	//win attributes
	HWND		m_hAppWnd;
	HINSTANCE	m_hAppInstance;

	//LPCWSTR
	std::wstring     m_AppTitel;  //std::string makes chineesecaracters??
	DWORD		m_WndStyle;

	// DirectX Attributes
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	IDXGISwapChain*		    m_pSwapChain;
	ID3D11RenderTargetView* m_pRTV;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D11_VIEWPORT			m_Viewport;

protected:
	//initialize window 
	bool InitWindow();

	//initialize Direct3D
	bool InitDirect3D();

};
