#include "WindowHelper.h"
#include <iostream>
#include <string>
#include "Input.h"
#include "GUIHandler.h"



//hanterar events eller medelanden fönstret tar imot
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{



	if (message == WM_KEYDOWN)
	{
		Input::SetKeyDown(wParam);

		//unsigned char keycode = static_cast<unsigned char>(wParam);

		//switch (wParam)
		//{
		//case VK_LEFT:
		//	break;
		//case VK_RIGHT:
		//	break;
		//case VK_UP:
		//	break;
		//case VK_DOWN:
		//	break;
		//}
	}
	if (message == WM_KEYUP)
	{
		Input::SetKeyUp(wParam);
	}

	if (message == WM_CHAR)
	{
		unsigned char letter = static_cast<unsigned char>(wParam);

		if (letter == 'w')
		{

		}
		return letter;
	}

	if (message == WM_LBUTTONDOWN)
	{
		Input::SetKeyDown(0x01);
	}
	if (message == WM_RBUTTONDOWN)
	{
		Input::SetKeyDown(0x02);
	}
	if (message == WM_LBUTTONUP)
	{
		Input::SetKeyUp(0x01);
	}
	if (message == WM_RBUTTONUP)
	{
		Input::SetKeyUp(0x02);
	}

	//if we close the window message is set to destroy
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	//if (GuiHandler::exit)
	//{
	//	PostQuitMessage(0);
	//	
	//}
	//default returns 
	return DefWindowProc(hwnd, message, wParam, lParam);
}


bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND& window)
{
	//variables for window class
	const wchar_t CLASS_NAME[] = L"DWindow";	//Window CLASS name

	//create window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);						//size of structure
	wc.style = CS_OWNDC;						//Determines proterties of window, several can be combined
	wc.lpfnWndProc = WindowProc;				//pointer to window procedure, An function that processes messages sent to the window
	wc.cbClsExtra = 0;							//extra bytes allocated per class instance
	wc.cbWndExtra = 0;							//extra bytes allocated per window
	wc.hInstance = instance;					//assign pointer
	wc.hIcon = nullptr;							//specific icon for window
	wc.hCursor = LoadCursorW(nullptr,IDC_CROSS /*IDC_ARROW*/);						//specific cursor
	/*wc.hbrBackground = nullptr;	*/	            //specific background brush
	wc.hbrBackground = CreatePatternBrush((HBITMAP)LoadImage(0, L"../Textures/Sprites/Loading.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE));
	/*wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0)));*/
	wc.lpszMenuName = nullptr;					//menu name (we dont have a menu)
	wc.lpszClassName = CLASS_NAME;				//Name of window
	wc.hIconSm = nullptr;						//handle to a small icon associated with the window class.
	RegisterClassEx(&wc);						// register windowclas, stakes const ptr to windowsclass

	//Adjusts the window to specified size without top title-Bar = GameWindow
	RECT r = { 0, 0, width, height };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	//CenterWindow to middle of Screen
	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	// create window instance (windowhandle window)
	window = CreateWindowExW(0, CLASS_NAME, L"The Lair", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, x/*CW_USEDEFAULT*/, y, width, height, nullptr, nullptr, instance, nullptr);


	//test if failed
	if (window == nullptr)
	{
		std::cerr << "failed: " << GetLastError() << std::endl;
		return false;
	}

	//show window
	ShowWindow(window, nCmdShow);

	return true;
}