#include "PipelineHelper.h"
#include "Time.h"


bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderGeo, ID3D11PixelShader*& pShader, ID3D11PixelShader*& PXShaderGeo, ID3D11PixelShader*& PT_PixelShader2, ID3D11ComputeShader*& cubeMapShader, ID3D11ComputeShader*& cShader, ID3D11GeometryShader*& gShader,
	ID3D11ComputeShader*& particleUpdateShader, ID3D11ComputeShader*& PT_UpdateShader_Rising, string& vShaderBytecode, string& vgShaderBytecode)
{
	//variables for reading data from file
	std::string shaderdata;
	std::ifstream reader;

	//---------------------- Vertex shader ------------------------//

	//Read from file that contains vertex shader
	reader.open("../Shaders/vertexShader.cso", std::ios::binary | std::ios::ate);
	
	//check if found file
	if (!reader.is_open())
	{
		std::cerr << "didnt find vertex shader file!" << std::endl;
		return false;
	}

	// go to end of file, allocate data depending on how many bytes left over, go back to beginning
	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);


	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderdata.c_str(), shaderdata.length(), nullptr, &vShader)))
	{
		std::cerr << "failed to create vertex shader!" << std::endl;
		return false;
	}
	vShaderBytecode = shaderdata;
	shaderdata.clear();
	reader.close();


	//---------------------- VtxGeo shader ------------------------//

	reader.open("../Shaders/ParticleVertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "didnt find Particle vertex shader file!" << std::endl;
		return false;
	}
	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
	if (FAILED(device->CreateVertexShader(shaderdata.c_str(), shaderdata.length(), nullptr, &vShaderGeo)))
	{
		std::cerr << "failed to create Particle vertex shader!" << std::endl;
		return false;
	}
	vgShaderBytecode = shaderdata;
	shaderdata.clear();
	reader.close();


	//---------------------- Pixel shader ------------------------//

	//Read from file that contains pixel shader
	reader.open("../Shaders/PixelShader.cso", std::ios::binary | std::ios::ate);  //specifiera path (pixelshader)
	if (!reader.is_open())
	{
		std::cerr << "didnt find pixel shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);// gå till slutet
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));//reservera plats baserat på hur många bytes 
	reader.seekg(0, std::ios::beg);

	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());// assignar från strömmen till string

	//försöker skapa pixelshader
	if (FAILED(device->CreatePixelShader(shaderdata.c_str(), shaderdata.length(), nullptr, &pShader)))
	{
		std::cerr << "failed to create Pixel shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();


	//---------------------- Particle Pixel shader ------------------------//

	//Read from file that contains pixel shader
	reader.open("../Shaders/ParticlePixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "didnt find Particle pixel shader file!" << std::endl;
		return false;
	}
	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	//försöker skapa pixelshader
	if (FAILED(device->CreatePixelShader(shaderdata.c_str(), shaderdata.length(), nullptr, &PXShaderGeo)))
	{
		std::cerr << "failed to create Pixel shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();

	//---------------------- Particle Pixel shader ------------------------//

//Read from file that contains pixel shader
	reader.open("../Shaders/PT_PixelShader2.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "didnt find Particle pixel shader file!" << std::endl;
		return false;
	}
	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	//försöker skapa pixelshader
	if (FAILED(device->CreatePixelShader(shaderdata.c_str(), shaderdata.length(), nullptr, &PT_PixelShader2)))
	{
		std::cerr << "failed to create Pixel shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();

	//---------------------- Cube shader ------------------------//

	reader.open("../Shaders/CubeShader1.cso", std::ios::binary | std::ios::ate);  //specifiera path (pixelshader)
	if (!reader.is_open())
	{
		std::cerr << "didnt find cube shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);// gå till slutet
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));//reservera plats baserat på hur många bytes 
	reader.seekg(0, std::ios::beg);

	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());// assignar från strömmen till string

	//försöker skapa pixelshader
	if (FAILED(device->CreateComputeShader(shaderdata.c_str(), shaderdata.length(), nullptr, &cubeMapShader)))
	{
		std::cerr << "failed to create Cube shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();


	//---------------------- Geo shader ------------------------//

	//Read from file that contains geo shader
	reader.open("../Shaders/geometryShader.cso", std::ios::binary | std::ios::ate);  //specifiera path
	if (!reader.is_open())
	{
		std::cerr << "didnt find Geo shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	//Create shader
	if (FAILED(device->CreateGeometryShader(shaderdata.c_str(), shaderdata.length(), nullptr, &gShader)))
	{
		std::cerr << "failed to create Geo shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();


	//---------------------- Compute shader ------------------------//

	//Read from file that contains compute shader
	reader.open("../Shaders/ComputeShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "didnt find compute shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	//försöker skapa computeshader
	if (FAILED(device->CreateComputeShader(shaderdata.c_str(), shaderdata.length(), nullptr, &cShader)))
	{
		std::cerr << "failed to create Compute shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();

	//---------------------- Paricle Update shader ------------------------//

	reader.open("../Shaders/particleUpdateShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "didnt find particle Update shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateComputeShader(shaderdata.c_str(), shaderdata.length(), nullptr, &particleUpdateShader)))
	{
		std::cerr << "failed to create particle Update shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();

	//---------------------- Rising Paricle Update shader ------------------------//

	reader.open("../Shaders/PTUpdateRisingPT.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "didnt find particle Update shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderdata.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderdata.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateComputeShader(shaderdata.c_str(), shaderdata.length(), nullptr, &PT_UpdateShader_Rising)))
	{
		std::cerr << "failed to create particle Update shader!" << std::endl;
		return false;
	}
	shaderdata.clear();
	reader.close();

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, std::string& vShaderBytecode)
{
	//description of what information each vertex contains
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},								//v
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },   //vn
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }			//vt
	};

	//Create assign inputlayout 
	HRESULT hr = device->CreateInputLayout(inputDesc, 3, vShaderBytecode.c_str(), vShaderBytecode.length(), &inputLayout);
	return !FAILED(hr);
}

bool CreateAltInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayoutGeo, std::string& vgShaderBytecode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 1, vgShaderBytecode.c_str(), vgShaderBytecode.length(), &inputLayoutGeo);
	return !FAILED(hr);
}

bool CreateCameraMTXBuffer(ID3D11Device* device, ID3D11Buffer*& constBuffer)
{

	XMVECTOR cameraPos = XMVectorSet(0, 0, -12, 0);//hämta till pixelshader
	XMVECTOR focusPos = XMVectorSet(0, 0, 10, 0);
	XMVECTOR upDir = XMVectorSet(0, 1, 0, 0);

	float angle = 0;
	//tar in i radianer

	//XMMATRIX world = {  XMMatrixRotationZ(0) * XMMatrixRotationY(angle) * XMMatrixRotationX(0) *XMMatrixScaling(2.2f / 4.0f, 1.0f, 1.0f) * XMMatrixTranslation(0,0,0) };
	XMMATRIX view = { XMMatrixLookAtLH(cameraPos,focusPos, upDir) * XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 0.75f, 0.1f, 100.0f) };
	//XMMATRIX matArray[2] = { world,view };

	//
	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMMATRIX);					//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure

	//holds matrix data
	D3D11_SUBRESOURCE_DATA cBufData;
	cBufData.pSysMem = &view;							//pointer to geometry data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &constBuffer);


	return !FAILED(hr);
}

bool CreateSampler(ID3D11Device* device, ID3D11SamplerState*& samplerState)
{
	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//crate sampler, assign it to samplerState.
	HRESULT hr = device->CreateSamplerState(&sampleDesc, &samplerState);

	return !FAILED(hr);
}

bool CreateShadowSampler(ID3D11Device* device, ID3D11SamplerState*& samplerState)
{
	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.BorderColor[0] = 1.0f;
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	//crate sampler, assign it to samplerState.
	HRESULT hr = device->CreateSamplerState(&sampleDesc, &samplerState);

	return !FAILED(hr);
}

bool CreateCameraBuffer(ID3D11Device* device, ID3D11Buffer*& cameraBuffer)
{
	XMFLOAT4 cameraPosition(0, 0, -12, 1);//constant exponent


	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMFLOAT4);					//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure

	//holds matrix data
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };
	cBufData.pSysMem = &cameraPosition;						//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &cameraBuffer);
	return !FAILED(hr);
}

bool CreatePtBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11UnorderedAccessView*& particleUav)
{
	XMFLOAT3 PtData[6]{ {-7, 5, -7}, {7, 5, 7}, {0, 5, 3}, {5, 5, 6}, {0,0,0}, {1,1,5} };

	D3D11_BUFFER_DESC bufferDesc;										//create Vertex buffer
	bufferDesc.ByteWidth = 6 * sizeof(XMFLOAT3);						//size of buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;								//sets interaction with gpu and cpu, currently set to gpu read only
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;					//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = 0;										//Specifies cpu acess
	bufferDesc.MiscFlags = 0;											//Misc flags
	bufferDesc.StructureByteStride = 0;									//Size of each element in structure
	D3D11_SUBRESOURCE_DATA data;										//holds geometry data
	data.pSysMem = &PtData;												//pointer to geometry data
	data.SysMemPitch = 0;												//distance in bytes from the beginning of one line of a texture to the next
	data.SysMemSlicePitch = 0;											//for 3d texture


	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = size(PtData) * 3;
	uavDesc.Buffer.Flags = 0;

	if (FAILED(device->CreateUnorderedAccessView(vertexBuffer, &uavDesc, &particleUav)))
	{
		std::cerr << "failed to create UAV" << std::endl;
		return false;
	}
	return !FAILED(hr);
}

bool CreateMatrixBuffer(ID3D11Device* device, ID3D11Buffer*& matrixBuffer)//create world matrix for pointlist
{
	XMMATRIX world = { XMMatrixTranslation(0,0,0) * (XMMatrixRotationZ(0) * XMMatrixRotationY(0) * XMMatrixRotationX(0) * XMMatrixScaling(1.0f, 1.0f, 1.0f)) };

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMMATRIX);					//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData;						//holds matrix data
	cBufData.pSysMem = &world;								//pointer to mat data

	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &matrixBuffer);
	return !FAILED(hr);
}

bool createTimeBuffer(ID3D11Device* device, ID3D11Buffer*& timeBuffer)
{
	float timer = Time::CurrentTime();

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = 16;								//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };
	cBufData.pSysMem = &timer;								//pointer to data

	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &timeBuffer);
	return !FAILED(hr);
}


bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11VertexShader*& vShaderGeo, ID3D11PixelShader*& pShader, ID3D11PixelShader*& PXShaderGeo, ID3D11PixelShader*& PT_PixelShader2, ID3D11ComputeShader*& cubeMapShader,
	ID3D11GeometryShader*& gShader, ID3D11ComputeShader*& particleUpdateShader, ID3D11ComputeShader*& PT_UpdateShader_Rising, ID3D11InputLayout*& inputLayout, ID3D11InputLayout*& inputLayoutGeo, ID3D11Buffer*& CameraMTXBuffer, ID3D11Buffer*& PtListBuffer,
	ID3D11Buffer*& PtworldMatrix, ID3D11SamplerState*& samplerState, ID3D11SamplerState*& shadowSampler, ID3D11Buffer*& CameraPosBuffer, ID3D11ComputeShader*& cShader, ID3D11UnorderedAccessView*& particleUav,
	ID3D11Buffer*& timeBuffer)
{
	//String to hold vShaderByteCode
	std::string vShaderByteCode;
	std::string vgShaderByteCode;

	//load Vertex shader and Pixel shader
	if (!LoadShaders(device, vShader, vShaderGeo, pShader, PXShaderGeo, PT_PixelShader2, cubeMapShader, cShader, gShader, particleUpdateShader, PT_UpdateShader_Rising, vShaderByteCode, vgShaderByteCode))
	{
		std::cerr << "error loading shaders!" << std::endl;
		return false;
	}

	//Define what information each vertex holds
	if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
	{
		std::cerr << "error creating inputlayout!" << std::endl;
		return false;
	}

	//Create InputLayout for Geometry pass
	if (!CreateAltInputLayout(device, inputLayoutGeo, vgShaderByteCode))
	{
		std::cerr << "error creating Alt inputlayout!" << std::endl;
		return false;
	}

	//Create Sampler
	if (!CreateSampler(device, samplerState))
	{
		std::cerr << "error creating sampler!" << std::endl;
		return false;
	}

	//Create Shadow Sampler
	if (!CreateShadowSampler(device, shadowSampler))
	{
		std::cerr << "error creating Shadow sampler!" << std::endl;
		return false;
	}

	//Create buffer containing World, projection & view Matrix data
	if (!CreateCameraMTXBuffer(device, CameraMTXBuffer))
	{
		std::cerr << "error creating constBuffer!" << std::endl;
		return false;
	}

	//Create buffer containing camera data
	if (!CreateCameraBuffer(device, CameraPosBuffer))
	{
		std::cerr << "error creating cameraBuffer!" << std::endl;
		return false;
	}

	//Create buffer containing Ptlist data
	if (!CreatePtBuffer(device, PtListBuffer, particleUav))
	{
		std::cerr << "error creating PtBuffer!" << std::endl;
		return false;
	}

	//Unused remove
	if (!CreateMatrixBuffer(device, PtworldMatrix))
	{
		std::cerr << "error creating PtlistMatrix!" << std::endl;
		return false;
	}


	//Create buffer containing camera data
	if (!createTimeBuffer(device, timeBuffer))
	{
		std::cerr << "error creating timeBuffer!" << std::endl;
		return false;
	}


	return true;
}