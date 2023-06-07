#include "CubeMapObj.h"
#include "stb_image.h"


bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ComPtr<ID3D11Texture2D> & dsTexture, vector <ComPtr<ID3D11DepthStencilView>> & dsViews, ComPtr<ID3D11ShaderResourceView> & shaderView )
{
	int nrOfCameras = 6;//skriv om

	D3D11_TEXTURE2D_DESC textureDesc = {};					//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;								//olika nivååer av kompression
	textureDesc.ArraySize = nrOfCameras;						//en buffer
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;		// MapUsageTypless(usage);
	textureDesc.SampleDesc.Count = 1;						// defaultvärden
	textureDesc.SampleDesc.Quality = 0;						//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;				//säger hur den ska användas när vi kommer åt den ()
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;		//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;							// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
	textureDesc.MiscFlags = 0;								//misc flagga


	//depth texture
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, dsTexture.GetAddressOf())))
	{
		std::cerr << "failed to create depth stencil texture" << std::endl;
		return false;
	}

	//DepthStencil views
	for (int i = 0; i < nrOfCameras; i++)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
		descView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descView.Flags = 0;
		descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		descView.Texture2DArray.MipSlice = 0;
		descView.Texture2DArray.FirstArraySlice = i;
		descView.Texture2DArray.ArraySize = 1;


		ComPtr<ID3D11DepthStencilView> tempDsView;
		//crate depth stencil, assign it to dsView
		if (FAILED(device->CreateDepthStencilView(dsTexture.Get(), &descView, tempDsView.GetAddressOf())))
		{
			std::cerr << "failed to create depth stencil texture" << std::endl;
			return false;
		}

		//push 
		dsViews.push_back(tempDsView);
	}


	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;//DXGI_FORMAT_R24G8_TYPELESS
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;

	//create shader resource view 
	HRESULT hr = device->CreateShaderResourceView(dsTexture.Get(), &shaderResourceViewDesc, shaderView.GetAddressOf());
	return !FAILED(hr);
}


bool CreateRenderTargets(ID3D11Device* device, UINT width, UINT height, ComPtr<ID3D11Texture2D> & Texture, vector <ComPtr<ID3D11UnorderedAccessView>> & renderViews, ComPtr<ID3D11ShaderResourceView> & renderedTextureView)
{
	//variables
	vector<string> filenames{ "Left.png", "Right.png", "Bottom.png", "Top.png", "Front.png", "Back.png" };// "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg" 
	vector<unsigned char*> images;
	int wth = 0;
	int hth = 0;
	int channels = 0;

	for (int i = 0; i < 6; i++)
	{
		unsigned char* img = stbi_load(("../Textures/" + filenames.at(i)).c_str(), &wth, &hth, &channels, STBI_rgb_alpha);
		if (img == NULL)
		{
			return 0;
		}
		else
		{
			images.push_back(img);
		}
		
	}





	int nrOfCameras = 6;

	D3D11_TEXTURE2D_DESC textureDesc = {};												//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = wth;
	textureDesc.Height = hth;
	textureDesc.MipLevels = 1u;															//olika nivååer av kompression
	textureDesc.ArraySize = nrOfCameras;												//en buffer
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;									//MapUsageTypless(usage);
	textureDesc.SampleDesc.Count = 1;													//Defaultvärden
	textureDesc.SampleDesc.Quality = 0;													//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;											//säger hur den ska användas när vi kommer åt den ()
	textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;	//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;														// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;							//misc flagga


	/*	D3D11_SUBRESOURCE_DATA data = {0};
	data.pSysMem = images.data();
	data.SysMemPitch = wth * 4;
	data.SysMemSlicePitch = 0;
	*/

	D3D11_SUBRESOURCE_DATA data[6];
	for (int i = 0; i < 6; i++)
	{
		data[i].pSysMem = images[i];
		data[i].SysMemPitch = wth * 4;
		data[i].SysMemSlicePitch = 0;
	}

	
	//create texture
	if (FAILED(device->CreateTexture2D(&textureDesc, data, Texture.GetAddressOf())))
	{
		std::cerr << "failed to create texture" << std::endl;
		return false;
	}


	////Unordered views
	//for (int i = 0; i < nrOfCameras; i++)
	//{
	//	D3D11_UNORDERED_ACCESS_VIEW_DESC descView = {};
	//	descView.Format = textureDesc.Format;
	//	descView.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	//	descView.Texture2DArray.MipSlice = 0;
	//	descView.Texture2DArray.FirstArraySlice = (UINT)i;
	//	descView.Texture2DArray.ArraySize = 1;
	//	ComPtr<ID3D11UnorderedAccessView> tempView;
	//	//crate Uav, assign it to temp
	//	if (FAILED(device->CreateUnorderedAccessView(Texture.Get(), &descView, tempView.GetAddressOf())))
	//	{
	//		std::cerr << "failed to create depth stencil texture" << std::endl;
	//		return false;
	//	}
	//	//push 
	//	renderViews.push_back(tempView);
	//}




	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;

	//create shader resource view 
	HRESULT hr = device->CreateShaderResourceView(Texture.Get(), &shaderResourceViewDesc, renderedTextureView.GetAddressOf());

	for (int i = 0; i < 6; i++)
	{
		stbi_image_free(images[i]);
	}

	return !FAILED(hr);
}

bool CreateCameraPosBuffer(ID3D11Device * device, ComPtr<ID3D11Buffer> cameraBuffer, XMFLOAT4 position)
{
	XMFLOAT4 cameraPosition = position;

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(cameraPosition);			//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };				//holds matrix data
	cBufData.pSysMem = &cameraPosition;						//pointer to data

	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &cameraBuffer);
	return !FAILED(hr);
}

bool CreateViewBuffer(ID3D11Device * device, vector<ComPtr<ID3D11Buffer>>& viewBuffer, XMFLOAT4 position, Mesh & cubeMesh)
{
	//Variables
	XMVECTOR cameraPos;
	XMVECTOR focusPos;
	XMVECTOR upDir;
	XMMATRIX view;
	vector<XMFLOAT3> faceNormal = cubeMesh.getFaceNormals();

	
	for (int i = 0; i < 6; i++)
	{
		//hämta normal, det blir look at
		//get normal funktion för mesh klass??
		//behöver multipliceras med worldmatris för att sättas på rätt plats i scen (normalisering kanske)

		cameraPos = XMVectorSet(position.x, position.y, position.z, 0);
		focusPos = XMVectorSet(position.x + faceNormal.at(i).x, position.y + faceNormal.at(i).y, position.z + faceNormal.at(i).z, 0);
		upDir = XMVectorSet(0, 1, 0, 0);
		view = { XMMatrixLookAtLH(cameraPos,focusPos, upDir) * XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 1.0f, 0.05f, 100.0f) };


		//Description
		D3D11_BUFFER_DESC cBuffDesc;
		cBuffDesc.ByteWidth = sizeof(XMMATRIX);					//size of buffer
		cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
		cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
		cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
		cBuffDesc.MiscFlags = 0;								//Misc flags
		cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
		D3D11_SUBRESOURCE_DATA cBufData;						//holds matrix data
		cBufData.pSysMem = &view;								//pointer to geometry data

		//Create Buffer
		ComPtr < ID3D11Buffer > tempBuff;
		device->CreateBuffer(&cBuffDesc, &cBufData, &tempBuff);
		viewBuffer.push_back(tempBuff);
	}
	return true;
}



CubeMapHandler::CubeMapHandler(ID3D11Device * device, ID3D11DeviceContext* immediateContext, XMFLOAT4 pos)
	:cubeMesh(device, "../Meshes/InvertedCube", true, 1, XMFLOAT3(pos.x, pos.y, pos.z)), position(pos)
{	
	//Create depth stencil, Depth-texture, depthViewArr & shader resource view
	if (!CreateDepthStencil(device, this->texWidth, this->texHeight, this->depthTextures, this->depthViews, this->shaderView))
	{
		std::cerr << "error creating Dstencil/Srv!" << std::endl;
	}

	//Create SRV's & UAV's
	if (!CreateRenderTargets(device, this->texWidth, this->texHeight, this->renderTextures, this->unorderedViews, this->renderedTextureView))
	{
		std::cerr << "error creating RenderTargets!" << std::endl;
	}

	//Create ViewMX buffers
	if (!CreateViewBuffer(device, this->viewBuffers, this->position, this->cubeMesh))
	{
		std::cerr << "error creating ViewMX Buffer" << std::endl;
	}

	//Create cameraPosBuffer
	if (!CreateCameraPosBuffer(device, this->posBuffer, this->position))
	{
		std::cerr << "error creating PosBuffer!" << std::endl;
	}

	//set position of cube
	this->cubeMesh.setPos(XMFLOAT3(pos.x, pos.y, pos.z));
	this->cubeMesh.updateMatrix(immediateContext);
}

ID3D11Buffer * CubeMapHandler::getViewMatrix(int cameraIndex)
{
	return this->viewBuffers.at(cameraIndex).Get();
}

ID3D11DepthStencilView * CubeMapHandler::getDepthStencil(int cameraIndex)
{
	return this->depthViews.at(cameraIndex).Get();
}

ID3D11ShaderResourceView * CubeMapHandler::getRenderedTextureView(int cameraIndex)
{
	return this->renderedTextureView.Get();
}

ID3D11UnorderedAccessView * CubeMapHandler::getUav(int cameraIndex)
{
	return unorderedViews.at(cameraIndex).Get();
}

ID3D11Buffer * CubeMapHandler::getPosBuffer()
{
	return this->posBuffer.Get();
}

void CubeMapHandler::bindDepthStencil(ID3D11DeviceContext * immediateContext, int cameraIndex)
{
	//Variables
	ID3D11RenderTargetView* nullRtv{ nullptr };

	//Clear Depth Stencil
	immediateContext->ClearDepthStencilView(this->depthViews.at(cameraIndex).Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//Set render targets
	immediateContext->OMSetRenderTargets(1, &nullRtv, this->depthViews.at(cameraIndex).Get());
}

void CubeMapHandler::bindAndDrawIndexedMesh(ID3D11DeviceContext * immediateContext, ID3D11Buffer *& viewProjBuffer, ID3D11Buffer *& cameraPosBuffer)
{
	this->cubeMesh.BindAndDrawCubeMap(immediateContext, viewProjBuffer, this->renderedTextureView.Get(), cameraPosBuffer);
}
