#include "LightHandler.h"

bool CreateLtBuffer(ID3D11Device * device, ComPtr<ID3D11Buffer>& lightBuffer, vector<PointLight> & Pointlights, ComPtr<ID3D11ShaderResourceView> & lightBuffView)
{
	vector<LightStruct> structVector;
	for (int i = 0; i < Pointlights.size(); i++)
	{
		XMFLOAT4 ambComp = Pointlights.at(i).getAmbientComponent(); //skicka in
		XMFLOAT4 difComp = Pointlights.at(i).getDiffuseComponent();
		XMFLOAT4 speComp = Pointlights.at(i).getSpecularComponent();

		XMFLOAT4X4 matrix;
		XMFLOAT4 pos = Pointlights.at(i).getPosition();
		XMFLOAT4 col = Pointlights.at(i).getColor();
		XMMATRIX tempMatrix = XMMatrixTranspose(Pointlights.at(i).getViewMatrix());
		XMStoreFloat4x4(&matrix, tempMatrix);

		//ändra till float4
		LightStruct lightArray(pos, col, matrix, ambComp, difComp, speComp);
		structVector.push_back(lightArray);
	}

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = sizeof(LightStruct) * structVector.size();			//size of buffer //*nr of elements
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;				//Misc flags
	cBuffDesc.StructureByteStride = sizeof(LightStruct);						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = structVector.data();										//pointer to data


	//Create light buffer
	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, lightBuffer.GetAddressOf());


	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.FirstElement = 0;
	shaderResourceViewDesc.Buffer.NumElements = structVector.size();

	//create shader resource view 
	device->CreateShaderResourceView(lightBuffer.Get(), &shaderResourceViewDesc, lightBuffView.GetAddressOf());
	return !FAILED(hr);
}


bool CreateNumLTBuffer(ID3D11Device * device, ComPtr<ID3D11Buffer>& lightBuffer, int nrOfLights)
{
	int buff = nrOfLights;

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = 16;													//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;													//Misc flags
	cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = &buff;													//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, lightBuffer.GetAddressOf());
	return !FAILED(hr);
}


bool CreateViewBuffer(ID3D11Device * device, ComPtr<ID3D11Buffer>& viewBuffer, PointLight & ptLight)
{
	XMMATRIX view = { ptLight.getViewMatrix() };

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMMATRIX);					//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData;						//holds matrix data
	cBufData.pSysMem = &view;								//pointer to geometry data

	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, viewBuffer.GetAddressOf());
	return !FAILED(hr);
}


bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ComPtr<ID3D11Texture2D> & dsTexture, vector <ComPtr<ID3D11DepthStencilView>> & dsViews, ComPtr<ID3D11ShaderResourceView> & shaderView, int nrOfLights)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};					//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;								//olika nivååer av kompression
	textureDesc.ArraySize = nrOfLights;						//en buffer
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;		// MapUsageTypless(usage);
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
	for (int i = 0; i < nrOfLights; i++)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
		descView.Format = DXGI_FORMAT_D32_FLOAT;
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
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;//DXGI_FORMAT_R24G8_TYPELESS
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;

	//create shader resource view 
	HRESULT hr = device->CreateShaderResourceView(dsTexture.Get(), &shaderResourceViewDesc, shaderView.GetAddressOf());
	return !FAILED(hr);
}


LightHandler::LightHandler(UINT shadowWidth, UINT shadowHeight)
	:shadowHeight(shadowHeight), shadowWidth(shadowWidth)
{

}


void LightHandler::addPointlight(ID3D11Device * device, XMFLOAT4 position, XMFLOAT4 color, XMFLOAT4 FocusPos, XMFLOAT4 UpDir)
{
	//Skapa ljus
	PointLight templight(device, color, position, FocusPos, UpDir, this->ambComp, this->difComp, this->speComp);
	this->Pointlights.push_back(templight);
}


void LightHandler::finalizeLights(ID3D11Device * device, ID3D11DeviceContext* immediateContext)
{
	//lägg till info från light i buffers 
	int nrOfLights = this->Pointlights.size();

	//Create depth stencil, textureArr, depthViewArr & shader resource view
	if (!CreateDepthStencil(device, this->shadowWidth, this->shadowHeight, this->depthTextures, this->depthViews, this->shaderView, nrOfLights))
	{
		std::cerr << "error creating Dstencil/Srv!" << std::endl;
	}

	//Create buffer containing projection & view Matrix data
	for (int i = 0; i < nrOfLights; i++)
	{
		ComPtr <ID3D11Buffer> tempBuffer;
		if (!CreateViewBuffer(device, tempBuffer, this->Pointlights.at(i)))
		{
			std::cerr << "error creating viewBuffer!" << std::endl;
		}
		this->viewBuffers.push_back(tempBuffer);
	}

	//Create single buffer containing all light data
	if (!CreateLtBuffer(device, this->lightBuffer, this->Pointlights, this->lightBuffView))
	{
		std::cerr << "error creating lightBuffer!" << std::endl;
	}

	//Create buffer with int for nrof lights.
	if (!CreateNumLTBuffer(device, this->numLightBuffer, nrOfLights))
	{
		std::cerr << "error creating numlightBuffer!" << std::endl;
	}

	//Create Meshes
	for (int i = 0; i < nrOfLights; i++)
	{
		XMFLOAT4 pos = this->Pointlights.at(i).getPosition();
		XMMATRIX mat = XMMatrixTranslation(pos.x, pos.y, pos.z);
		Mesh tempMesh(device, "../Meshes/g", false, 0, XMFLOAT3(pos.x, pos.y, pos.z));
		//tempMesh.updateMatrix(immediateContext, mat);
		this->boundingSphere.push_back(tempMesh);
	}
}


//Update functions not tested, may not be functional!


bool LightHandler::updateLightBuffer(ID3D11DeviceContext * immediateContext, int ptLightIndex,XMFLOAT4 position, XMFLOAT4 color, float lightIntensity)
{
	////uppdatera ljus & matris
	//this->Pointlights.at(ptLightIndex).updateColor(color);
	//this->updateMatrixAndViewBuffer(immediateContext, ptLightIndex, position, Pointlights.at(ptLightIndex).getFocusPos(), Pointlights.at(ptLightIndex).getUpDir());


	////Map
	//D3D11_MAPPED_SUBRESOURCE map;
	//ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

	////Hämta och uppdatara data för buffer.
	//vector<LightStruct> structVector;
	//for (int i = 0; i < Pointlights.size(); i++)
	//{
	//	XMFLOAT4X4 matrix;
	//	XMFLOAT4 pos = Pointlights.at(i).getPosition();
	//	XMFLOAT4 col = Pointlights.at(i).getColor();
	//	XMMATRIX tempMatrix = XMMatrixTranspose(Pointlights.at(i).getViewMatrix());
	//	XMStoreFloat4x4(&matrix, tempMatrix);

	//	//ändra till float4
	//	LightStruct lightArray(col, pos, matrix);
	//	structVector.push_back(lightArray);
	//}

	//HRESULT hr = immediateContext->Map(this->lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	//memcpy(map.pData, structVector.data(), sizeof(LightStruct)); //sizeof(structVector));		//kanske ger problem!!!

	////UnMap
	//immediateContext->Unmap(this->lightBuffer.Get(), 0);
	return true;//!FAILED(hr);
}


bool LightHandler::updateMatrixAndViewBuffer(ID3D11DeviceContext * immediateContext, int ptLightIndex, XMFLOAT4 position, XMFLOAT4 lookAt, XMFLOAT4 upDirection)
{
	//Update lightMat data
	this->Pointlights.at(ptLightIndex).updatePosition(position);
	this->Pointlights.at(ptLightIndex).updateMatrix(position, lookAt, upDirection);
	
	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//Get updated matrix
	XMMATRIX view = { this->Pointlights.at(ptLightIndex).getViewMatrix() };

	HRESULT hr = immediateContext->Map(this->viewBuffers.at(ptLightIndex).Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &view, sizeof(XMMATRIX));

	//UnMap
	immediateContext->Unmap(this->viewBuffers.at(ptLightIndex).Get(), 0);
	return !FAILED(hr);
}


int LightHandler::getNrOfLights()
{
	return this->Pointlights.size();
}


void LightHandler::bindLightBuffer(ID3D11DeviceContext * immediateContext, ID3D11Buffer *& cameraPosBuffer, ID3D11ShaderResourceView** dfSrvArr)
{
	//Add Srv & Buffer data from lights to temp vectors
	vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(cameraPosBuffer);								//Camera matrix
	tempBuff.push_back(this->numLightBuffer.Get());					//Buffer for nr Lights

	vector<ID3D11ShaderResourceView*> tempSrv;
	for (int i = 0; i < 8; i++)
	{
		tempSrv.push_back(dfSrvArr[i]);
	}
	tempSrv.push_back(this->shaderView.Get());						//ShadowMap(s)
	tempSrv.push_back(this->lightBuffView.Get());					//Srv for light structuredBuffer content (pos, color, lightViewMatrix)
	

	immediateContext->CSSetShaderResources(0, 10, tempSrv.data());	//Bind Srv's
	immediateContext->CSSetConstantBuffers(0, 2, tempBuff.data());	//Bind Buffers's
}


void LightHandler::bindDepthStencil(ID3D11DeviceContext * immediateContext, int ptLightIndex)
{
	//Variables
	ID3D11RenderTargetView* nullRtv{ nullptr };

	//Clear Depth Stencil
	immediateContext->ClearDepthStencilView(this->depthViews.at(ptLightIndex).Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//Set render targets
	immediateContext->OMSetRenderTargets(1, &nullRtv, this->depthViews.at(ptLightIndex).Get());
}


void LightHandler::bindAndDrawIndexedMesh(ID3D11DeviceContext * immediateContext, ID3D11Buffer *& viewProjBuffer)
{
	//Nr of lights
	int nrOfLights = this->Pointlights.size();

	//Draw all meshes
	for (int i = 0; i < nrOfLights; i++)
	{
		this->boundingSphere.at(i).bindAndDrawIndexed(immediateContext, viewProjBuffer);
	}	
}


ID3D11Buffer* LightHandler::getLightViewBuffer(int ptLightIndex)
{
	return this->viewBuffers.at(ptLightIndex).Get();
}