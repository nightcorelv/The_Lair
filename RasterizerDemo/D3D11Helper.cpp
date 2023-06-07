#include "D3D11Helper.h"


bool CreateInterface(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immidiateContext, IDXGISwapChain*& swapChain)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

	swapChainDesc.BufferDesc.Width = width;												//Width
	swapChainDesc.BufferDesc.Height = height;											//Height
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;									//refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;			//format: A four-component, 32-bit unsigned-normalized-integer format that supports 8 bits per channel R, G, B, Alpha. 0 to 1
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//scanline behaviour (drawing on screen)
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// Flags indicating how an image is scaled to fit a given monitor's resolution.
	swapChainDesc.SampleDesc.Count = 1;													//Anti aliasing sample count
	swapChainDesc.SampleDesc.Quality = 0;												//Anti aliasing sample quality
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;						//the target for our render pipeline
	swapChainDesc.BufferCount = 2;														//no of back buffers
	swapChainDesc.OutputWindow = window;												//window we want to output to
	swapChainDesc.Windowed = true;														//vi vill köra i windowed mode
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;								//metoden den använder vid byte av front och back end buffer (default)
	swapChainDesc.Flags = 0;

	//if in debug give value to flags
	UINT flags = 0;

#ifdef _DEBUG

	if (_DEBUG)
	{
		flags = D3D11_CREATE_DEVICE_DEBUG;
	}

#endif


	//Describes the set of features targeted by a Direct3D device.
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	//Create swapchain, assign it to swapChain, device, and immidiateContext
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &immidiateContext);

	//device->QueryInterface(__uuidof(ID3D11Debug), (void**)&dbgPtr);

	//Send COM Code back if not failed
	return !FAILED(hr);
}

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv)
{
	//create container for texture
	ID3D11Texture2D* backBuffer = nullptr;

	//index of buffer we want, uuid of interface we want to recive from sub object, pointer to hold what we recived.
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "failed to get buffer" << std::endl;
		return false;
	}

	//Create render target view, assign it to rtv
	HRESULT hr = device->CreateRenderTargetView(backBuffer, NULL, &rtv);

	//relese referance to buffer
	backBuffer->Release();

	return !FAILED(hr);
}

bool CreateUnorderedAccessView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11UnorderedAccessView*& backBufferUav)
{
	//create container for texture
	ID3D11Texture2D* backBuffer = nullptr;

	//index of buffer we want, uuid of interface we want to recive from sub object, pointer to hold what we recived.
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "failed to get buffer" << std::endl;
		return false;
	}

	//Create render target view, assign it to rtv
	HRESULT hr = device->CreateUnorderedAccessView(backBuffer, NULL, &backBufferUav);

	//relese referance to buffer
	backBuffer->Release();






	return !FAILED(hr);
}


bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView)
{
	//skapa svartvit textur som representerar djup i en scen
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;								//olika nivååer av kompression
	textureDesc.ArraySize = 1;								//en buffer
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		//24 bits unsigned normaliserad int som djupet? till stencil 8 bitars unsigned int.
	textureDesc.SampleDesc.Count = 1;						// defaultvärden
	textureDesc.SampleDesc.Quality = 0;						//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;				//säger hur den ska användas när vi kommer åt den ()
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;							// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
	textureDesc.MiscFlags = 0;								//misc flagga


	//if we failed to create depth texture
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "failed to create depth stencil texture" << std::endl;
		return false;
	}

	//crate depth stencil, assign it to dsView
	HRESULT hr = device->CreateDepthStencilView(dsTexture, nullptr, &dsView);
	return !FAILED(hr);
}

void setViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0;								//moves where we start rendering from
	viewport.TopLeftY = 0;								//-----//-----
	viewport.Width = static_cast<float>(width);			//Changes viewport width
	viewport.Height = static_cast<float>(height);		//Changes viewport height
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool setUpDefferedRendering(UINT width, UINT height, ID3D11Device*& device, ID3D11Texture2D** dfTexArr, ID3D11ShaderResourceView** dfSrvArr,
	ID3D11RenderTargetView** dfRtvArr)
{
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	D3D11_TEXTURE2D_DESC textureDesc2{};
	textureDesc2.Width = width;
	textureDesc2.Height = height;
	textureDesc2.MipLevels = 1;
	textureDesc2.ArraySize = 1;
	textureDesc2.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc2.SampleDesc.Count = 1;
	textureDesc2.Usage = D3D11_USAGE_DEFAULT;
	textureDesc2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;


	//skapa texturerna...
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 || i == 6)
		{
			if (FAILED(device->CreateTexture2D(&textureDesc2, nullptr, &dfTexArr[i])))
			{
				std::cerr << "failed to create df texture" << std::endl;
				return false;
			}
		}
		else
		{
			if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dfTexArr[i])))
			{
				std::cerr << "failed to create df texture" << std::endl;
				return false;
			}
		}

	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc2{};
	renderTargetViewDesc2.Format = textureDesc2.Format;
	renderTargetViewDesc2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	//skapa varje rtv...
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 || i == 6)
		{
			if (FAILED(device->CreateRenderTargetView(dfTexArr[i], &renderTargetViewDesc2, &dfRtvArr[i])))
			{
				std::cerr << "failed to create RenderTargetView" << std::endl;
				return false;
			}
		}
		else
		{
			if (FAILED(device->CreateRenderTargetView(dfTexArr[i], &renderTargetViewDesc, &dfRtvArr[i])))
			{
				std::cerr << "failed to create RenderTargetView" << std::endl;
				return false;
			}
		}

	}


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc2{};
	shaderResourceViewDesc2.Format = textureDesc2.Format;
	shaderResourceViewDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc2.Texture2D.MipLevels = 1;

	//skapa varje resource view...
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 || i == 6)
		{
			if (FAILED(device->CreateShaderResourceView(dfTexArr[i], &shaderResourceViewDesc2, &dfSrvArr[i])))
			{
				std::cerr << "failed to create ShaderResourceView" << std::endl;
				return false;
			}
		}
		else
		{
			if (FAILED(device->CreateShaderResourceView(dfTexArr[i], &shaderResourceViewDesc, &dfSrvArr[i])))
			{
				std::cerr << "failed to create ShaderResourceView" << std::endl;
				return false;
			}
		}

	}

	return true;
};

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immidiateContext, IDXGISwapChain*& swapChain, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, ID3D11UnorderedAccessView*& backBufferUav, ID3D11Texture2D** dfTexArr, ID3D11ShaderResourceView** dfSrvArr,
	ID3D11RenderTargetView** dfRtvArr, ID3D11RenderTargetView*& rtv, ID3D11Texture2D** cTexArr, ID3D11ShaderResourceView** cSrvArr, ID3D11RenderTargetView** cRtvArr)
{
	if (!CreateInterface(width, height, window, device, immidiateContext, swapChain))
	{
		std::cerr << "error creating interface" << std::endl;
		return false;
	}

	if (!CreateRenderTargetView(device, swapChain, rtv))
	{
		std::cerr << "error creating UnorderedAccessView" << std::endl;
		return false;
	}

	if (!CreateUnorderedAccessView(device, swapChain, backBufferUav))
	{
		std::cerr << "error creating UnorderedAccessView" << std::endl;
		return false;
	}

	if (!CreateDepthStencil(device, width, height, dsTexture, dsView))
	{
		std::cerr << "error creating interface" << std::endl;
		return false;
	}

	if (!setUpDefferedRendering(width, height, device, dfTexArr, dfSrvArr, dfRtvArr))
	{
		std::cerr << "error creating DF interface" << std::endl;
		return false;
	}

	//width and height should be the same as used in cubemap
	if (!setUpDefferedRendering(width, height, device, cTexArr, cSrvArr, cRtvArr))
	{
		std::cerr << "error creating DF interface" << std::endl;
		return false;
	}

	//set the viewport
	setViewport(viewport, width, height);

	return true;
}
