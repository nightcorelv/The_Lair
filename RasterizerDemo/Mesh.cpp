#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <DirectXMath.h>
#include <unordered_map>

std::unordered_map<std::string, ID3D11Texture2D*> textureMap;
std::unordered_map<std::string, ID3D11ShaderResourceView*> textureSRV;
Mesh* currentMesh = nullptr;
//---------------------------------------------------------------Functions for initilization-----------------------------------------------------------------//

UINT padBuffer(UINT currSize)
{
	UINT add = 0;
	while ((currSize + add) % 16 != 0)
	{
		add++;
	}
	return currSize + add;
}

void splitStrAtChar(string & str, char toFind, vector<string> & strList, bool keepPrefix)
{
	//variables
	vector<int> charIndex;
	string tempStr;

	//read string and find char index (portion of string before first found char)
	for (int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == toFind)
		{
			charIndex.push_back(i);
		}
	}

	//option to keep prefix in str list
	if (keepPrefix)
	{
		tempStr.assign(str, 0, charIndex.at(0));
		strList.push_back(tempStr);
	}

	//split string into smaller strings at char index
	for (int i = 0; i < charIndex.size(); i++)
	{
		bool doubleSpace = false;
		int startIndex = charIndex.at(i) + 1;

		//check if double space, if so skip
		if (i < charIndex.size() - 1)
		{
			if (charIndex.at(i) + 1 == charIndex.at(i + 1))
			{
				doubleSpace = true;
			}
		}

		if (!doubleSpace)
		{
			if (keepPrefix)
			{
				startIndex = charIndex.at(i) + 2;
			}
			if (i == charIndex.size() - 1)
			{
				int endIndex = str.length() - 1;
				int strLength = endIndex + 1 - startIndex;
				tempStr.assign(str, (startIndex - 1), strLength + 1);
			}
			else
			{
				int endIndex = charIndex.at(i + 1) - 1;
				int strLength = endIndex + 1 - startIndex;
				tempStr.assign(str, (startIndex - 1), strLength + 1);
			}
			strList.push_back(tempStr);
		}
	}
};

XMFLOAT4 makeArr(string str)//converts string to float 4
{
	//temporary variables
	vector<string> tempStr;
	float temp[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

	//Split string
	splitStrAtChar(str, ' ', tempStr, false);

	//assign to float
	for (int i = 0; i < tempStr.size(); i++)
	{
		temp[i] = atof(tempStr.at(i).c_str());
	}
	return(XMFLOAT4{ temp });
}

void assembleList(string strList[], double * vptrList[], double * vnptrList[], double * vtptrList[], vector<SimpleVertex>& VectorData, vector<int>& indexBuff, vector<string> & usedIndexList)
{
	//Assemble final format (SimpleVertex) //f v1/vt1/vn1
	for (int i = 0; i < 3; i++)//3
	{
		//Temp variables
		float tempArr[3]{ 0,0,0 };
		float tempArr1[3]{ 0,0,0 };
		float tempArr2[2]{ 0,0 };
		vector<string> f_IndexList;
		int repeatIndex;

		//spara undan lista och kolla om upprepad (för index buffer) om unik kör push back till index buffer , om inte strunta i det...
		//assumes tris
		bool isRepeat = false;
		for (int j = 0; j < usedIndexList.size(); j++)
		{
			if (strList[i] == usedIndexList.at(j))
			{
				isRepeat = true;
				repeatIndex = j;
			}
		}
		if (!isRepeat)
		{
			//Is Uniqe Index
			usedIndexList.push_back(strList[i]);

			//get indicies from f_strList as array. v1/vt6/vn4 -> [1][6][4]
			splitStrAtChar(strList[i], '/', f_IndexList, true);			//Str arr


			//get information from corresponding list using indicies
			for (int j = 0; j < 3; j++)
			{
				//cout << "F strList Index: " + to_string(j) + " cont: " + f_IndexList.at(j) << endl;
				tempArr[j] = vptrList[atoi(f_IndexList.at(0).c_str()) - 1][j]; //V Index 
				tempArr1[j] = vnptrList[atoi(f_IndexList.at(2).c_str()) - 1][j]; //VN Index 
			}
			for (int j = 0; j < 2; j++)
			{
				tempArr2[j] = vtptrList[atoi(f_IndexList.at(1).c_str()) - 1][j]; //VT Index 
			}

			//Create and push vertex data
			SimpleVertex triangle
			{
				{tempArr[0],tempArr[1],tempArr[2]}, {tempArr1[0],tempArr1[1],tempArr1[2]}, {tempArr2[0], tempArr2[1]}
			};
			VectorData.push_back(triangle);

			indexBuff.push_back(VectorData.size() - 1);
		}
		else if (isRepeat)
		{
			indexBuff.push_back(repeatIndex);
		}
	}
}

vector<SimpleVertex> loadObj(string filename, vector<int> & submeshStart, vector<int> & indexBufferData, vector<XMFLOAT3> & NormalperFace, bool saveFaceNorm, XMFLOAT3& center, float& w, float& h, float& d, float* temp)
{
	int nrOfV = 0;
	int nrOfVT = 0;
	int nrOfVN = 0;
	ifstream in;


	//Check nr of v, vn, vt
	in.open(filename +".obj");
	if (in.is_open())
	{
		in.ignore();
		std::string line;
		while (std::getline(in, line))
		{
			if (!line.empty())
			{
				if (line.at(0) == *"v")
				{
					if (line.at(1) == *"n")
					{
						nrOfVN++;
					}
					else if (line.at(1) == *"t")
					{
						nrOfVT++;
					}
					else
					{
						nrOfV++;
					}
				}
			}
		}
		in.close();
	}
	else
	{
		//load ERROR mesh
	}


	vector<SimpleVertex> VectorData;						//Holds data in format [[v][vn][vt]] 
	double** vptrList = new double*[nrOfV] {nullptr};		//Holds all Vtx Pos data
	double** vnptrList = new double*[nrOfVN] {nullptr};		//Holds all Vtx Norm data
	double** vtptrList = new double*[nrOfVT] {nullptr};		//Holds all UV Pos data
	vector<string> usedIndexList;							//Holds all uniqe vert indexes v/vn/vt

	//pekararray för v, vn, vt och för index array
	in.open(filename + ".obj");
	if (in.is_open())
	{
		int vIndex = 0;
		int vnIndex = 0;
		int vtIndex = 0;
		char lastChar = ' ';

		in.ignore();
		std::string line;
		while (std::getline(in, line))
		{

			//Temp arrays
			vector<string> v_strList;
			vector<string> vt_strList;
			vector<string> vn_strList;
			vector<string> f_strList;


			//Check if v, vn, vt
			if (!line.empty())
			{
				//Variable to keep track of where new submesh data begins  
				char currentChar = line.at(0);

				//assign v/vt/vn data to tep array
				if (line.at(0) == *"v")
				{
					if (line.at(1) == *"n")
					{
						splitStrAtChar(line, ' ', vn_strList, false);
						//assign info on current line to larger array of same type
						vnptrList[vnIndex] = new double[3]{ atof(vn_strList.at(0).c_str()), atof(vn_strList.at(1).c_str()),  atof(vn_strList.at(2).c_str()) };
						vnIndex++;
					}
					else if (line.at(1) == *"t")
					{
						splitStrAtChar(line, ' ', vt_strList, false);
						vtptrList[vtIndex] = new double[3]{  atof(vt_strList.at(0).c_str()), 1 - atof(vt_strList.at(1).c_str()) };
						vtIndex++;
					}
					else
					{
						splitStrAtChar(line, ' ', v_strList, false);
						vptrList[vIndex] = new double[3]{ atof(v_strList.at(0).c_str()), atof(v_strList.at(1).c_str()),  atof(v_strList.at(2).c_str()) };
						vIndex++;
					}
				}


				//if new submesh save index start for new mesh
				if (lastChar == 'f' && currentChar != lastChar && currentChar != 's')
				{
					submeshStart.push_back(indexBufferData.size());
				}


				if (line.at(0) == *"f")
				{
					splitStrAtChar(line, ' ', f_strList, false);

					//spara undan lista och kolla om upprepad (för index buffer) om unik kör push back till index buffer , om inte strunta i det...

					if (f_strList.size() == 4)//Quad triangulation	//skapar problem med normaler??? //kanske inte kan hantera mer än xx längd på indexen???!!! 
					{
						string testA;
						string testB;
						vector<string> testList;
						vector<string> testList2;

						//extract [][][this][&this] string
						splitStrAtChar(f_strList.at(2), '/', testList, false);
						splitStrAtChar(f_strList.at(3), '/', testList2, false);

						//Create new strings with corrected data for two tris (swap vn index for 3rd and 4th element)
						testA.assign(f_strList.at(3).begin(), f_strList.at(3).end() - 2);
						testA.append(testList.at(1));
						testB.assign(f_strList.at(2).begin(), f_strList.at(2).end() - 2);
						testB.append(testList2.at(1));

						//Create corrected array and run assembleList function for each tris.
						string correctedArr[2][3]{ {f_strList.at(0),f_strList.at(1),testA},{testA,f_strList.at(1),testB} };
						for (int i = 0; i < 2; i++)
						{
							assembleList(correctedArr[i], vptrList, vnptrList, vtptrList, VectorData, indexBufferData, usedIndexList);
						}
					}
					else //will fail if not tris
					{
						assembleList(f_strList.data(), vptrList, vnptrList, vtptrList, VectorData, indexBufferData, usedIndexList);
					}
				}
				lastChar = line.at(0);
			}
		}
		in.close();
	}
	submeshStart.push_back(indexBufferData.size());


	//Save face normals
	vector<XMFLOAT3> usedNormals;
	for (int i = 0; i < nrOfVN; i++)
	{
		XMFLOAT3 tempFloat;
		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				tempFloat.x = vnptrList[i][j];
			}
			else if (j == 1)
			{
				tempFloat.y = vnptrList[i][j];
			}
			else if (j == 2)
			{
				tempFloat.z = vnptrList[i][j];
			}
		}
		//Check if repeat
		bool isRepeat = false;
		for (int k = 0; k < usedNormals.size(); k++)
		{
			if (tempFloat.x == usedNormals.at(k).x && tempFloat.y == usedNormals.at(k).y && tempFloat.z == usedNormals.at(k).z)
			{
				isRepeat = true;
			}
		}
		if (!isRepeat)
		{
			NormalperFace.push_back(tempFloat);
		}
		usedNormals.push_back(tempFloat);
	}

	if (temp == nullptr)
	{
		//get furthest vertexes
		float highestX = 0;
		float lowestX = 0;
		float highestY = 0;
		float lowestY = 0;
		float highestZ = 0;
		float lowestZ = 0;
		for (int i = 0; i < nrOfV; i++)
		{
			if (vptrList[i][0] > highestX)
			{
				highestX = vptrList[i][0];
			}
			if (vptrList[i][0] < lowestX)
			{
				lowestX = vptrList[i][0];
			}
			if (vptrList[i][1] > highestY)
			{
				highestY = vptrList[i][1];
			}
			if (vptrList[i][1] < lowestY)
			{
				lowestY = vptrList[i][1];
			}
			if (vptrList[i][2] > highestZ)
			{
				highestZ = vptrList[i][2];
			}
			if (vptrList[i][2] < lowestZ)
			{
				lowestZ = vptrList[i][2];
			}
		}
		if ((highestX - lowestX) / 2 > (highestZ - lowestZ) / 2)
		{
			w = (highestX - lowestX) / 2;
			d = (highestX - lowestX) / 2;
		}
		else
		{
			w = (highestZ - lowestZ) / 2;
			d = (highestZ - lowestZ) / 2;
		}
		h = (highestY - lowestY) / 2;

		center = XMFLOAT3(lowestX + ((highestX - lowestX) / 2), lowestY + ((highestY - lowestY) / 2), lowestZ + ((highestZ - lowestZ) / 2));
	}
	else
	{
		w = temp[0];
		h = temp[1];
		d = temp[2];
		center = XMFLOAT3(temp[3], temp[4], temp[5]);

		delete[]temp;
	}
	

	for (int i = 0; i < nrOfV; i++)
	{
		delete vptrList[i];
	}
	delete[] vptrList;

	for (int i = 0; i < nrOfVN; i++)
	{
		delete vnptrList[i];
	}
	delete[] vnptrList;

	for (int i = 0; i < nrOfVT; i++)
	{
		delete vtptrList[i];
	}
	delete[] vtptrList;

	return VectorData;
}

vector<XMFLOAT4> loadMtl(string filename, vector<string> & filenamesKA,	vector<string> & filenamesKD, vector<string> & filenamesKS )
{
	vector<XMFLOAT4> matArr;		//format [ka][kd][ks+Ns]
	ifstream in;
	bool noKs = true;

	in.open(filename +".mtl");//kolla så filenames i slutet inte inehåller spaces
	if (in.is_open())
	{
		int ksIndex = 0;

		in.ignore();
		std::string line;
		while (std::getline(in, line))
		{
			vector<string> tempStrList;

			if (line.at(0) == *"K") //Colors
			{
				if (line.at(1) == *"d") //Diffuse color
				{
					matArr.push_back(makeArr(line));
				}
				else if (line.at(1) == *"a") //Ambient color
				{
					matArr.push_back(makeArr(line));
				}
				else if (line.at(1) == *"s") //Specular color
				{
					matArr.push_back(makeArr(line));
					ksIndex = matArr.size() - 1;
					noKs = false;
				}
			}
			else if (line.at(0) == *"N") //Exponents
			{

				splitStrAtChar(line, ' ', tempStrList, false);

				if (line.at(1) == *"i") //Optical density
				{
					//Do something?
				}
				else if (line.at(1) == *"s") //Specular Exponent
				{
					//Get number as float //assign to W in diffuse color float 4
					matArr.at(ksIndex).w = atof(tempStrList.at(0).c_str());
				}
			}
			else if (line.at(0) == *"m") //maps
			{
				if (line.at(4) == *"K")
				{
					splitStrAtChar(line, ' ', tempStrList, false);
					//Remove any remaining spaces
					tempStrList.at(0).erase(remove_if(tempStrList.at(0).begin(), tempStrList.at(0).end(), isspace), tempStrList.at(0).end());
					//Filter out .IFF filenames
					vector<string> temp;
					splitStrAtChar(tempStrList.at(0), '.', temp, false);
					if (!(temp.at(0) == ".iff"))
					{
						if (line.at(5) == *"a") //Ambient texture
						{
							filenamesKA.push_back(tempStrList.at(0));
							cout << "MkA: " + tempStrList.at(0) << endl;
						}
						else if (line.at(5) == *"d") //Diffuse texture
						{
							filenamesKD.push_back(tempStrList.at(0));
							cout << "MkD: " + tempStrList.at(0) << endl;
						}
						else if (line.at(5) == *"s") //Specular color texture
						{
							filenamesKS.push_back(tempStrList.at(0));
							cout << "MkS: " + tempStrList.at(0) << endl;
						}
					}
				}
			}
		}
		in.close();
	}
	if (noKs)
	{
		matArr.push_back(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	return(matArr);
}

bool CreateTx(ID3D11Device * device, string filename, vector<ComPtr<ID3D11Texture2D>> & loadedTexture, vector<ComPtr<ID3D11ShaderResourceView>> & textureView)
{
	//ID3D11Texture2D* tempTexture;
	//ID3D11ShaderResourceView* tempTextureView;

	if (textureMap.find(filename) == textureMap.end())
	{
		if (currentMesh != nullptr)
		{
			ID3D11Texture2D* tempTexture;
			ID3D11ShaderResourceView* tempTextureView;

			currentMesh->setTextureKey(filename);
			currentMesh->orgTextureKey = filename;
			currentMesh = nullptr;

			int width = 0;
			int heigth = 0;
			int channels = 0;
			unsigned char* img = stbi_load(("../Textures/"+filename).c_str(), &width, &heigth, &channels, STBI_rgb_alpha);
			if (img == NULL)
			{
				std::cerr << "error loading img!" << std::endl;
			}

			//skapa textur
			D3D11_TEXTURE2D_DESC textureDesc;
			textureDesc.Width = width;
			textureDesc.Height = heigth;
			textureDesc.MipLevels = 1;								//olika nivåer av kompression
			textureDesc.ArraySize = 1;								//arraystorlek
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//24 bits unsigned normaliserad int som djupet? till stencil 8 bitars unsigned int.
			textureDesc.SampleDesc.Count = 1;						// defaultvärden
			textureDesc.SampleDesc.Quality = 0;						//Sample quality
			textureDesc.Usage = D3D11_USAGE_DEFAULT;				//säger hur den ska användas när vi kommer åt den ()
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;		//binda till shader resource??
			textureDesc.CPUAccessFlags = 0;							// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
			textureDesc.MiscFlags = 0;								//misc flagga
			D3D11_SUBRESOURCE_DATA texture = { 0 };
			texture.pSysMem = img;									//pointer to img data
			texture.SysMemPitch = width * 4;						// number of bytes that a row contains in the image.

			//crate texture, assign it to loadedTexture
			HRESULT hr = device->CreateTexture2D(&textureDesc, &texture, &tempTexture);
			//loadedTexture.push_back(tempTexture);

			//Create texture View, then add to array.
			if (FAILED(device->CreateShaderResourceView(tempTexture, nullptr, &tempTextureView)))
			{
				std::cerr << "error creating TextureView!" << std::endl;
			}
			textureMap.insert(std::make_pair(filename, tempTexture));
			textureSRV.insert(std::make_pair(filename, tempTextureView));

			//textureView.push_back(tempTextureView);

			//free img data & release temp variables
			stbi_image_free(img);
		}

	}
	else
	{
		if (currentMesh != nullptr)
		{
			currentMesh->setTextureKey(filename);
			currentMesh->orgTextureKey = filename;
			currentMesh = nullptr;
		}

	}

	//tempTexture->Release();
	//tempTextureView->Release();
	return true;
}

bool CreateVtxBuffer(ID3D11Device * device, string filename, ComPtr<ID3D11Buffer> & vertexBuffer, vector<int> & submeshIndex, vector<int> & indexBufferData, vector<XMFLOAT3> & NormalperFace, bool saveFaceNorm, XMFLOAT3& center, float& w, float& h, float& d, float* ref)
{
	//--------------------------------------------------OBJ-----------------------------------------------------------//

	//call obj loader
	vector<SimpleVertex>VectorData = loadObj(filename, submeshIndex, indexBufferData, NormalperFace, saveFaceNorm, center, w, h, d, ref);

	//create Vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = VectorData.size() * sizeof(SimpleVertex);	//size of buffer
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;							//sets interaction with gpu and cpu, currently set to gpu read only
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;					//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = 0;										//Specifies cpu acess
	bufferDesc.MiscFlags = 0;											//Misc flags
	bufferDesc.StructureByteStride = 0;									//Size of each element in structure


	//holds geometry data
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = VectorData.data();									//pointer to geometry data
	data.SysMemPitch = 0;												//distance in bytes from the beginning of one line of a texture to the next
	data.SysMemSlicePitch = 0;											//for 3d texture


	//crate Buffer, assign it to vertexBuffer
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, vertexBuffer.GetAddressOf());
	return !FAILED(hr);
}

bool CreateVtxBuffer(ID3D11Device* device, LspScene::Mesh& mesh, ComPtr<ID3D11Buffer>& vertexBuffer, vector<int>& submeshIndex, vector<int>& indexBufferData, vector<XMFLOAT3>& NormalperFace, XMFLOAT3& center, float& w, float& h, float& d, float* whd, bool multiTex = false)
{
	int texTile = rand() % 4;
	vector<SimpleVertex> vertices;
	float highestX = 0;
	float lowestX = 0;
	float highestY = 0;
	float lowestY = 0;
	float highestZ = 0;
	float lowestZ = 0;

	for (LspScene::Mesh::Vertex &vertex : mesh.vertices)
	{
		bool unique = true;
		int index = vertices.size();
		for (int i = 0; i < vertices.size(); i++)
		{
			if (vertex.position[0] == vertices[i].pos[0] && vertex.position[1] == vertices[i].pos[1] && vertex.position[2] == vertices[i].pos[2] && vertex.normal[0] == vertices[i].norm[0] &&
				vertex.normal[1] == vertices[i].norm[1] && vertex.normal[2] == vertices[i].norm[2] && vertex.uv[0] == vertices[i].uv[0] && 1-vertex.uv[1] == vertices[i].uv[1])
			{
				unique = false;
				index = i;
			}
		}
		if (unique)
		{
			if (multiTex)
			{
				float u = vertex.uv[0];
				float v = 1 - vertex.uv[1];
				u *= 0.5;
				v *= 0.5;
				if (texTile%2 == 1)
				{
					v += 0.5;
				}
				if (texTile > 1)
				{
					u += 0.5;
				}

				vertices.push_back(SimpleVertex({ vertex.position[0], vertex.position[1], vertex.position[2] }, { vertex.normal[0], vertex.normal[1], vertex.normal[2] }, { u, v }));
			}
			else
			{
				vertices.push_back(SimpleVertex({ vertex.position[0], vertex.position[1], vertex.position[2] }, { vertex.normal[0], vertex.normal[1], vertex.normal[2] }, { vertex.uv[0], 1 - vertex.uv[1] }));
			}
			
		}
		indexBufferData.push_back(index);


		if (whd == nullptr)
		{
			if (vertex.position[0] > highestX)
			{
				highestX = vertex.position[0];
			}
			if (vertex.position[0] < lowestX)
			{
				lowestX = vertex.position[0];
			}
			if (vertex.position[1] > highestY)
			{
				highestY = vertex.position[1];
			}
			if (vertex.position[1] < lowestY)
			{
				lowestY = vertex.position[1];
			}
			if (vertex.position[2] > highestZ)
			{
				highestZ = vertex.position[2];
			}
			if (vertex.position[2] < lowestZ)
			{
				lowestZ = vertex.position[2];
			}
		}
	}

	submeshIndex.push_back(indexBufferData.size());

	if (whd == nullptr)
	{
		w = (highestX - lowestX) / 2;
		h = (highestY - lowestY) / 2;
		d = (highestZ - lowestZ) / 2;

		center = XMFLOAT3(lowestX + ((highestX - lowestX) / 2), lowestY + ((highestY - lowestY) / 2), lowestZ + ((highestZ - lowestZ) / 2));
	}
	else
	{
		w = whd[0];
		h = whd[1];
		d = whd[2];
		center = XMFLOAT3(whd[3], whd[4], whd[5]);

		delete[]whd;
	}

	//create Vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = vertices.size() * sizeof(SimpleVertex);	//size of buffer
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;							//sets interaction with gpu and cpu, currently set to gpu read only
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;					//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = 0;										//Specifies cpu acess
	bufferDesc.MiscFlags = 0;											//Misc flags
	bufferDesc.StructureByteStride = 0;									//Size of each element in structure


	//holds geometry data
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices.data();									//pointer to geometry data
	data.SysMemPitch = 0;												//distance in bytes from the beginning of one line of a texture to the next
	data.SysMemSlicePitch = 0;											//for 3d texture


	//create Buffer, assign it to vertexBuffer
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, vertexBuffer.GetAddressOf());
	return !FAILED(hr);
}

// SkinVtxBuffer for meshes without skeleton. Bones set to -1
//bool CreateSkinVtxBuffer(ID3D11Device* device, LspMesh& mesh, ComPtr<ID3D11Buffer>& skinVtxBuffer)
//{
//	vector<SkinnedVtx> vertices;
//	for (int i = 0; i < (int)mesh.mesh.vertices.size(); i++)
//	{
//		SkinnedVtx vtx;
//		vtx.id = -1;
//		vtx.conBones = { -1, -1, -1, -1 };
//		vtx.weights = { 0, 0, 0, 0 };
//		vertices.push_back(vtx);
//	}
//
//	//create Vertex buffer
//	D3D11_BUFFER_DESC bufferDesc;
//	bufferDesc.ByteWidth = padBuffer((sizeof(SkinnedVtx) * vertices.size()));			//size of buffer
//	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;							//sets interaction with gpu and cpu, currently set to gpu read only
//	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					//Specifies the type of buffer
//	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					//Specifies cpu acess
//	bufferDesc.MiscFlags = 0;											//Misc flags
//	bufferDesc.StructureByteStride = 0;									//Size of each element in structure
//
//
//	//holds geometry data
//	D3D11_SUBRESOURCE_DATA data;
//	data.pSysMem = vertices.data();										//pointer to geometry data
//	data.SysMemPitch = 0;												//distance in bytes from the beginning of one line of a texture to the next
//	data.SysMemSlicePitch = 0;											//for 3d texture
//
//
//	//create Buffer, assign it to vertexBuffer
//	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, skinVtxBuffer.GetAddressOf());
//	return !FAILED(hr);
//}
//
// SkinVtxBuffer for meshes with skeleton.
//bool CreateSkinVtxBuffer(ID3D11Device* device, LspSkeletalMesh* skelMesh, ComPtr<ID3D11Buffer>& skinVtxBuffer)
//{
//	if (skelMesh != nullptr)
//	{
//		vector<SkinnedVtx> vertices;
//		// For each vertex
//		for (int i = 0; i < (int)skelMesh->mesh.vertices.size(); i++)
//		{
//			bool gotOne = false;
//			SkinnedVtx vtx;
//			int bones[4]{ -1, -1, -1, -1 };
//			int boneIndex = 0;
//			float weights[4]{ 0, 0, 0, 0 };
//			int weightIndex = 0;
//
//			// Find a matching weight
//			for (int j = 0; j < skelMesh->mesh.weights.size(); j++)
//			{
//				if (skelMesh->mesh.weights.at(j).vertexIndex == i)
//				{
//					//Find the bone index in our bone vector
//					for (int k = 0; k < (int)skelMesh->bones.size(); k++)
//					{
//						if (skelMesh->mesh.weights.at(j).joint->name == skelMesh->skeleton.at(k).name)
//						{
//							bones[boneIndex] = k;
//							boneIndex++;
//							weights[weightIndex] = skelMesh->mesh.weights.at(j).value;
//							weightIndex++;
//							gotOne = true;
//							break;
//						}
//					}
//					
//				}
//				
//			}
//			if (gotOne && weights[0]>0 && weights[0]<1 && weights[1]>0 && weights[1] < 1 && weights[2]>0 && weights[2] < 1 && weights[3]>0 && weights[3] < 1)
//			{
//				float totalWeights = weights[0] + weights[1] + weights[2] + weights[3];
//				vtx.id = 1;
//				vtx.conBones.x = bones[0];
//				vtx.conBones.y = bones[1];
//				vtx.conBones.z = bones[2];
//				vtx.conBones.w = bones[3];
//
//				vtx.weights.x = weights[0] / totalWeights;
//				vtx.weights.y = weights[1] / totalWeights;
//				vtx.weights.z = weights[2] / totalWeights;
//				vtx.weights.w = weights[3] / totalWeights;
//
//				float total = vtx.weights.x + vtx.weights.y + vtx.weights.z + vtx.weights.w;
//
//				vertices.push_back(vtx);
//			}
//			
//		}
//		//create Vertex buffer
//		D3D11_BUFFER_DESC bufferDesc;
//		bufferDesc.ByteWidth = padBuffer((sizeof(SkinnedVtx) * vertices.size()));					//size of buffer
//		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;							//sets interaction with gpu and cpu, currently set to gpu read only
//		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					//Specifies the type of buffer
//		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					//Specifies cpu acess
//		bufferDesc.MiscFlags = 0;											//Misc flags
//		bufferDesc.StructureByteStride = 0;									//Size of each element in structure
//
//
//		//holds geometry data
//		D3D11_SUBRESOURCE_DATA data;
//		data.pSysMem = vertices.data();											//pointer to geometry data
//		data.SysMemPitch = 0;												//distance in bytes from the beginning of one line of a texture to the next
//		data.SysMemSlicePitch = 0;											//for 3d texture
//
//
//		//create Buffer, assign it to vertexBuffer
//		HRESULT hr = device->CreateBuffer(&bufferDesc, &data, skinVtxBuffer.GetAddressOf());
//		return !FAILED(hr);
//	}
//	return false;
//}

bool CreateActorTypeBuffer(ID3D11Device * device, int actorType, ComPtr<ID3D11Buffer>& actorTypeBuffer)
{
	int temp = actorType;

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = 16;													//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;													//Misc flags
	cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = &temp;														//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &actorTypeBuffer);
	return !FAILED(hr);
}

bool createIndexBuffer(ID3D11Device * device, vector<int> & indexBufferData, ComPtr<ID3D11Buffer>& indexBuffer)
{
	//create Vertex buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = indexBufferData.size() * sizeof(int);		//size of buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;								//sets interaction with gpu and cpu, currently set to gpu read only
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;						//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = 0;										//Specifies cpu acess
	bufferDesc.MiscFlags = 0;											//Misc flags
	bufferDesc.StructureByteStride = sizeof(int);						//Size of each element in structure


	//holds geometry data
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indexBufferData.data();									//pointer to geometry data
	data.SysMemPitch = 0;												//distance in bytes from the beginning of one line of a texture to the next
	data.SysMemSlicePitch = 0;											//for 3d texture

	//crate Buffer, assign it to indexBuffer
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, indexBuffer.GetAddressOf());
	return !FAILED(hr);
}

bool CreateMaterialBuffer(ID3D11Device * device, string filename, vector <ComPtr<ID3D11Buffer>> & materialBuffer, vector<string> & filenamesKA, vector<string> & filenamesKD, vector<string> & filenamesKS)
{
	//--------------------------------------------------MTL-----------------------------------------------------------//

	
	//call mtl loader, get Mtl data in float 4 arr for buffer & filenames of any attatched files
	vector<XMFLOAT4>materialData = loadMtl(filename, filenamesKA, filenamesKD, filenamesKS);

	bool suceeded = false;
	
	//Create and add buffers to vector
	int nrOfMaterials = (materialData.size() / 3);
	for (int i = 0; i < nrOfMaterials; i++)
	{
		//temp variable
		ComPtr<ID3D11Buffer> tempBuffer;
		vector<XMFLOAT4> tempMatArr;

		tempMatArr.push_back(materialData.at(0 + (i * 3)));
		tempMatArr.push_back(materialData.at(1 + (i * 3)));
		tempMatArr.push_back(materialData.at(2 + (i * 3)));


		//Create material Buffer
		D3D11_BUFFER_DESC cBuffDesc;
		cBuffDesc.ByteWidth = tempMatArr.size() * sizeof(XMFLOAT4);				//size of buffer
		cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
		cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
		cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
		cBuffDesc.MiscFlags = 0;													//Misc flags
		cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
		D3D11_SUBRESOURCE_DATA cBufData = { 0 };
		cBufData.pSysMem = tempMatArr.data();										//pointer to data


		//crate Buffer, assign it to materialBuffer
		//format 3 XMFLOAT4 per material in order [KA][KD][KS] note [ks] has exponent added into 4th element (.w).
		HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &tempBuffer);

		materialBuffer.push_back(tempBuffer);
		

		suceeded = !FAILED(hr);
	}

	return suceeded;
};

bool CreateMaterialBuffer(ID3D11Device* device, LspScene::Mesh& mesh, vector <ComPtr<ID3D11Buffer>>& materialBuffer)
{
	vector<XMFLOAT4>materialData;
	//ads

	materialData.push_back(XMFLOAT4(mesh.material->ambient[0], mesh.material->ambient[1], mesh.material->ambient[2], 1));
	materialData.push_back(XMFLOAT4(mesh.material->diffuse[0], mesh.material->diffuse[1], mesh.material->diffuse[2], 1));
	materialData.push_back(XMFLOAT4(mesh.material->specular[0], mesh.material->specular[1], mesh.material->specular[2], mesh.material->specularPower));

	bool suceeded = false;

	//Create and add buffers to vector
	int nrOfMaterials = (materialData.size() / 3);
	for (int i = 0; i < nrOfMaterials; i++)
	{
		//temp variable
		ComPtr<ID3D11Buffer> tempBuffer;
		vector<XMFLOAT4> tempMatArr;

		tempMatArr.push_back(materialData.at(0 + (i * 3)));
		tempMatArr.push_back(materialData.at(1 + (i * 3)));
		tempMatArr.push_back(materialData.at(2 + (i * 3)));


		//Create material Buffer
		D3D11_BUFFER_DESC cBuffDesc;
		cBuffDesc.ByteWidth = tempMatArr.size() * sizeof(XMFLOAT4);				//size of buffer
		cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
		cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
		cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
		cBuffDesc.MiscFlags = 0;													//Misc flags
		cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
		D3D11_SUBRESOURCE_DATA cBufData = { 0 };
		cBufData.pSysMem = tempMatArr.data();										//pointer to data


		//crate Buffer, assign it to materialBuffer
		//format 3 XMFLOAT4 per material in order [KA][KD][KS] note [ks] has exponent added into 4th element (.w).
		HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, &tempBuffer);

		materialBuffer.push_back(tempBuffer);


		suceeded = !FAILED(hr);
	}

	return suceeded;
};

bool CreateMxBuffer(ID3D11Device * device, ComPtr<ID3D11Buffer>& matrixBuffer, XMMATRIX matrix)
{
	XMMATRIX world = matrix;

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(XMMATRIX) *2;				//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData;						//holds matrix data
	cBufData.pSysMem = &world;								//pointer to mat data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, matrixBuffer.GetAddressOf());
	return !FAILED(hr);
}

//bool CreateMxBuffer(ID3D11Device* device, ComPtr<ID3D11Buffer>& matrixBuffer, boneMx info)
//{
//	//Is this how you make the matrix buffer for the bones' finalMx to the vertex shader
//	D3D11_BUFFER_DESC cBuffDesc;
//	cBuffDesc.ByteWidth = sizeof(boneMx);	//size of buffer
//	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
//	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
//	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
//	cBuffDesc.MiscFlags = 0;								//Misc flags
//	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
//	D3D11_SUBRESOURCE_DATA cBufData;						//holds matrix data
//	cBufData.pSysMem = &info;								//pointer to mat data
//
//
//	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, matrixBuffer.GetAddressOf());
//	return !FAILED(hr);
//}

void calculateTile(float tileSize, float x, float y)
{

}

//------------------------------------------------------------------------Constructor------------------------------------------------------------------------//

Mesh::Mesh(ID3D11Device * device, string filename, bool saveFaceNorm, int actorType, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd) //whd is a float[6] with width-height-depth-centerOffset data for the bounding box, this will override the automatic bounding box
	:position(Position), rotation(Rotation), rays(raycastType)
{
	//set matrix
	this->matrix = { XMMatrixTranslation(Position.x, Position.y, Position.z) * (XMMatrixRotationZ(Rotation.z) * XMMatrixRotationY(Rotation.y) * XMMatrixRotationX(Rotation.x) * XMMatrixScaling(1.0f, 1.0f, 1.0f))};

	//Filename strings, may contain more than one if obj includes submesh
	vector<string> filenamesKA;		
	vector<string> filenamesKD;
	vector<string> filenamesKS;
	vector<int> indexBufferData;

	currentMesh = this;
	//Create buffer containing vertex data
	if (!CreateVtxBuffer(device, filename, this->vertexBuffer, this->submeshIndex, indexBufferData, this->normalPerFace, saveFaceNorm, this->center, this->width, this->height, this->depth, whd))
	{
		std::cerr << "error creating vBuffer for: " + filename << std::endl;
	}

	if (!createIndexBuffer(device, indexBufferData, this->indexBuffer))
	{
		std::cerr << "error creating IndexBuffer for: " + filename << std::endl;
	}

	//Create buffer containing Matrix data
	if (!CreateMxBuffer(device, this->matrixBuffer, this-> matrix))
	{
		std::cerr << "error creating matrixBuffer for: " + filename << std::endl;
	}

	//Create buffer containing material data & fill filename  vectors
	if (!CreateMaterialBuffer(device, filename, this->materialBuffer, filenamesKA, filenamesKD, filenamesKS))
	{
		std::cerr << "error creating MaterialBuffer for: " + filename << std::endl;
	}


	//Load and create new textures & texture views //koll så inte = iff
	for (int i = 0; i < filenamesKA.size(); i++)
	{
		if (!CreateTx(device, filenamesKA.at(i), this->ambient_Texture, this->ambient_View))
		{
			std::cerr << "error creating texture!" << std::endl;
		}
	}
	for (int i = 0; i < filenamesKD.size(); i++)
	{
		if (!CreateTx(device, filenamesKD.at(i), this->diffuse_Texture, this->diffuse_View))
		{
			std::cerr << "error creating texture!" << std::endl;
		}
	}
	if (filenamesKD.size() == 0)//If no texture apply default texture
	{
		if (!CreateTx(device, "DefaultTexture.png", this->diffuse_Texture, this->diffuse_View))
		{
			std::cerr << "error creating texture!" << std::endl;
		}
	}

	if (!CreateActorTypeBuffer(device, actorType, this->actorTypeBuffer))
	{
		std::cerr << "error creating matrixBuffer for: " + filename << std::endl;
	}

	for (int i = 0; i < filenamesKS.size(); i++)
	{
		if (!CreateTx(device, filenamesKS.at(i), this->specular_Texture, this->specular_View))
		{
			std::cerr << "error creating texture!" << std::endl;
		}
	}
	if (width >= 2.5 || depth >= 2.5)
	{
		big = true;
	}
}

Mesh::Mesh(const Mesh& copy, ID3D11Device* device, XMFLOAT3 pos, XMFLOAT3 rot, int raycastType, float* whd)
{
	this->ambient_Texture = copy.ambient_Texture;
	this->diffuse_Texture = copy.diffuse_Texture;
	this->specular_Texture = copy.specular_Texture;
	this->ambient_View = copy.ambient_View;
	this->diffuse_View = copy.diffuse_View;
	this->specular_View = copy.specular_View;
	this->materialBuffer = copy.materialBuffer;
	this->vertexBuffer = copy.vertexBuffer;
	this->indexBuffer = copy.indexBuffer;
	this->actorTypeBuffer = copy.actorTypeBuffer;
	//this->skinVtxBuffer = copy.skinVtxBuffer;
	//this->boneMxBuffer = copy.boneMxBuffer;
	this->submeshIndex = copy.submeshIndex;
	this->normalPerFace = copy.normalPerFace;
	this->position = pos;
	this->rotation = rot;
	this->matrix = { XMMatrixTranslation(this->position.x, this->position.y, this->position.z) * (XMMatrixRotationZ(this->rotation.z) * XMMatrixRotationY(this->rotation.y) * XMMatrixRotationX(this->rotation.x) * XMMatrixScaling(1.0f, 1.0f, 1.0f)) };
	this->big = copy.big;
	this->textureKey = copy.textureKey;
	this->orgTextureKey = copy.orgTextureKey;

	//Create buffer containing Matrix data
	if (!CreateMxBuffer(device, this->matrixBuffer, this->matrix))
	{
		std::cerr << "error creating matrixBuffer" << std::endl;
	}
	if (raycastType != 0)
	{
		this->rays = raycastType;
	}
	else
	{
		this->rays = copy.rays;
	}
	if (whd == nullptr)
	{
		this->width = copy.width;
		this->height = copy.height;
		this->depth = copy.depth;
		this->center = copy.center;
	}
	else
	{
		this->width = whd[0];
		this->height = whd[1];
		this->depth = whd[2];
		this->center = XMFLOAT3(whd[3], whd[4], whd[5]);

		delete[]whd;
	}
	this->move = false;
	if (width >= 2.5 || depth >= 2.5)
	{
		big = true;
	}
}

//Mesh::Mesh(const Mesh& copy, LspSkeletalMesh* skelMesh, ID3D11Device* device, XMFLOAT3 pos, XMFLOAT3 rot, int raycastType, float* whd)
//{
//	this->ambient_Texture = copy.ambient_Texture;
//	this->diffuse_Texture = copy.diffuse_Texture;
//	this->specular_Texture = copy.specular_Texture;
//	this->ambient_View = copy.ambient_View;
//	this->diffuse_View = copy.diffuse_View;
//	this->specular_View = copy.specular_View;
//	this->materialBuffer = copy.materialBuffer;
//	this->vertexBuffer = copy.vertexBuffer;
//	this->indexBuffer = copy.indexBuffer;
//	this->actorTypeBuffer = copy.actorTypeBuffer;
//	this->skinVtxBuffer = copy.skinVtxBuffer;
//	this->boneMxBuffer = copy.boneMxBuffer;
//	this->submeshIndex = copy.submeshIndex;
//	this->normalPerFace = copy.normalPerFace;
//	this->position = pos;
//	this->rotation = rot;
//	this->matrix = { XMMatrixTranslation(this->position.x, this->position.y, this->position.z) * (XMMatrixRotationZ(this->rotation.z) * XMMatrixRotationY(this->rotation.y) * XMMatrixRotationX(this->rotation.x) * XMMatrixScaling(1.0f, 1.0f, 1.0f)) };
//	this->big = copy.big;
//	this->useAnims = copy.useAnims;
//	this->animState = copy.animState;
//	this->maxStates = copy.maxStates;
//	this->lastState = copy.lastState;
//	this->walkAnim = copy.walkAnim;
//	this->shootAnim = copy.shootAnim;
//	this->attackAnim = copy.attackAnim;
//	this->idleAnim = copy.idleAnim;
//	this->hurtAnim = copy.hurtAnim;
//	this->textureKey = copy.textureKey;
//	this->orgTextureKey = copy.orgTextureKey;
//
//	if (copy.lspSkelMesh != nullptr)
//	{
//		this->lspSkelMesh = skelMesh;
//	}
//
//	//Create buffer containing Matrix data
//	if (!CreateMxBuffer(device, this->matrixBuffer, this->matrix))
//	{
//		std::cerr << "error creating matrixBuffer" << std::endl;
//	}
//	if (raycastType != 0)
//	{
//		this->rays = raycastType;
//	}
//	else
//	{
//		this->rays = copy.rays;
//	}
//	if (whd == nullptr)
//	{
//		this->width = copy.width;
//		this->height = copy.height;
//		this->depth = copy.depth;
//		this->center = copy.center;
//	}
//	else
//	{
//		this->width = whd[0];
//		this->height = whd[1];
//		this->depth = whd[2];
//		this->center = XMFLOAT3(whd[3], whd[4], whd[5]);
//
//		delete[]whd;
//	}
//	this->move = false;
//	if (width >= 2.5 || depth >= 2.5)
//	{
//		big = true;
//	}
//}

Mesh::Mesh(LspMesh& input, ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd, bool multiTex)
{
	vector<int> indexBufferData;
	currentMesh = this;

	if (!CreateVtxBuffer(device, input.mesh, this->vertexBuffer, this->submeshIndex, indexBufferData, this->normalPerFace, this->center, this->width, this->height, this->depth, whd, multiTex))
	{
		std::cerr << "error creating vBuffer for: " + input.mesh.name << std::endl;
	}

	if (!createIndexBuffer(device, indexBufferData, this->indexBuffer))
	{
		std::cerr << "error creating IndexBuffer for: " + input.mesh.name << std::endl;
	}

	//if (!CreateSkinVtxBuffer(device, input, skinVtxBuffer)) //edit
	//{
	//	std::cerr << "error creating SkinVtxBuffer for: " + input.mesh.name << std::endl;
	//}

	//Create buffer containing material data
	if (!CreateMaterialBuffer(device, input.mesh, this->materialBuffer))
	{
		std::cerr << "error creating MaterialBuffer for: " + input.mesh.name << std::endl;
	}

	if (!CreateTx(device, /*"../Textures/" + */input.mesh.material->texureFileName, this->ambient_Texture, this->ambient_View))
	{
		std::cerr << "error creating texture" << std::endl;
	}
	if (!CreateTx(device, /*"../Textures/" + */input.mesh.material->texureFileName, this->diffuse_Texture, this->diffuse_View))
	{
		std::cerr << "error creating texture!" << std::endl;
	}
	if (!CreateTx(device, /*"../Textures/" + */input.mesh.material->texureFileName, this->specular_Texture, this->specular_View))
	{
		std::cerr << "error creating texture!" << std::endl;
	}

	if (!CreateActorTypeBuffer(device, 0, this->actorTypeBuffer))
	{
		std::cerr << "error creating matrixBuffer for: " + input.mesh.name << std::endl;
	}

	this->position = Position;
	this->rotation = Rotation;
	this->matrix = { XMMatrixTranslation(this->position.x, this->position.y, this->position.z) * (XMMatrixRotationZ(this->rotation.z) * XMMatrixRotationY(this->rotation.y) * XMMatrixRotationX(this->rotation.x) * XMMatrixScaling(1.0f, 1.0f, 1.0f)) };

	//Create buffer containing Matrix data
	if (!CreateMxBuffer(device, this->matrixBuffer, this->matrix))
	{
		std::cerr << "error creating matrixBuffer" << std::endl;
	}

	//boneMx matrices = {};
	//if (!CreateMxBuffer(device, this->boneMxBuffer, matrices))
	//{
	//	std::cerr << "error creating boneMatrixBuffer" << std::endl;
	//}
	
	this->rays = raycastType;
	this->move = false;
	if (width >= 2.5 || depth >= 2.5)
	{
		big = true;
	}
}

//Mesh::Mesh(LspSkeletalMesh& input, ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int raycastType, float* whd)
//{
//	vector<int> indexBufferData;
//	currentMesh = this;
//	lspSkelMesh = &input;
//
//	if (!CreateVtxBuffer(device, lspSkelMesh->mesh, this->vertexBuffer, this->submeshIndex, indexBufferData, this->normalPerFace, this->center, this->width, this->height, this->depth, whd))
//	{
//		std::cerr << "error creating vBuffer for: " + lspSkelMesh->mesh.name << std::endl;
//	}
//
//	if (!createIndexBuffer(device, indexBufferData, this->indexBuffer))
//	{
//		std::cerr << "error creating IndexBuffer for: " + lspSkelMesh->mesh.name << std::endl;
//	}
//
//	if (!CreateSkinVtxBuffer(device, this->lspSkelMesh, skinVtxBuffer))
//	{
//		std::cerr << "error creating SkinVtxBuffer for: " + input.mesh.name << std::endl;
//	}
//
//	//Create buffer containing material data
//	if (!CreateMaterialBuffer(device, lspSkelMesh->mesh, this->materialBuffer))
//	{
//		std::cerr << "error creating MaterialBuffer for: " + lspSkelMesh->mesh.name << std::endl;
//	}
//
//	if (!CreateTx(device,/* "../Textures/" + */lspSkelMesh->mesh.material->texureFileName, this->ambient_Texture, this->ambient_View))
//	{
//		std::cerr << "error creating texture" << std::endl;
//	}
//	if (!CreateTx(device,/* "../Textures/" + */lspSkelMesh->mesh.material->texureFileName, this->diffuse_Texture, this->diffuse_View))
//	{
//		std::cerr << "error creating texture!" << std::endl;
//	}
//	if (!CreateTx(device,/* "../Textures/" + */lspSkelMesh->mesh.material->texureFileName, this->specular_Texture, this->specular_View))
//	{
//		std::cerr << "error creating texture!" << std::endl;
//	}
//
//	//Set to 2 for "animation mesh"
//	if (!CreateActorTypeBuffer(device, 0, this->actorTypeBuffer))
//	{
//		std::cerr << "error creating matrixBuffer for: " + lspSkelMesh->mesh.name << std::endl;
//	}
//
//	this->position = Position;
//	this->rotation = Rotation;
//	this->matrix = { XMMatrixTranslation(this->position.x, this->position.y, this->position.z) * (XMMatrixRotationZ(this->rotation.z) * XMMatrixRotationY(this->rotation.y) * XMMatrixRotationX(this->rotation.x) * XMMatrixScaling(1.0f, 1.0f, 1.0f)) };
//	//Create buffer containing Matrix data
//	if (!CreateMxBuffer(device, this->matrixBuffer, matrix))
//	{
//		std::cerr << "error creating matrixBuffer" << std::endl;
//	}
//
//	boneMx matrices = {};
//	//Calculate transformation matrix for each bone.
//	for (int i = 0; i < lspSkelMesh->bones.size(); i++)
//	{
//		CalcMxPerFrame(lspSkelMesh->bones.at(i), *lspSkelMesh);
//		XMFLOAT4X4 tmp;
//		XMStoreFloat4x4(&tmp, lspSkelMesh->bones.at(i).finalMx);
//		matrices.mx[i] = tmp;
//	}
//
//	if (!CreateMxBuffer(device, this->boneMxBuffer, matrices))
//	{
//		std::cerr << "error creating matrixBuffer" << std::endl;
//	}
//
//	this->rays = raycastType;
//	this->move = false;
//	if (width >= 2.5 || depth >= 2.5)
//	{
//		big = true;
//	}
//}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	//if (lspSkelMesh != nullptr && dynamic)
	//{
	//	delete lspSkelMesh;
	//}
}

//---------------------------------------------------------------------Public Functions----------------------------------------------------------------------//

bool Mesh::updateMatrix(ID3D11DeviceContext* immediateContext)
{
	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

	
	//set matrix
	this->matrix = { ((XMMatrixRotationZ(this->rotation.z * XM_PI) * XMMatrixRotationY(this->rotation.y * XM_PI) * XMMatrixRotationX(this->rotation.x * XM_PI)) * XMMatrixTranslation(this->position.x, this->position.y, this->position.z) * XMMatrixScaling(1.0f, 1.0f, 1.0f)) };

	//Update matrix buffer
	XMMATRIX baseMat = { matrix };


	HRESULT hr = immediateContext->Map(this->matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &baseMat, sizeof(baseMat));

	//UnMap
	immediateContext->Unmap(this->matrixBuffer.Get(), 0);
	return !FAILED(hr);
}

void Mesh::bindAndDrawIndexed(ID3D11DeviceContext * immediateContext, ID3D11Buffer*& viewProjBuffer)
{
	UINT stride = sizeof(SimpleVertex);													
	UINT offset = 0;
	vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(this->matrixBuffer.Get());																//This Matrix
	tempBuff.push_back(viewProjBuffer);																			//Main Camera buffer

	immediateContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset);			//Set Vbuffer
	immediateContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);						//Set Index Buffer
	immediateContext->VSSetConstantBuffers(0, 2, tempBuff.data());												//Buffer info to send to Vshader (world matrix, view/Proj)
	//immediateContext->VSSetConstantBuffers(2, 1, this->skinVtxBuffer.GetAddressOf());
	//immediateContext->VSSetConstantBuffers(3, 1, this->boneMxBuffer.GetAddressOf());

	int startIndex = 0;
	for (int i = 0; i < submeshIndex.size(); i++)//för antalet meshes
	{
		vector<ID3D11Buffer*> tempConstBuff;																	//Temporary bufferArray fpr PS. Holds: [Material], [ActorType] buffers
		tempConstBuff.push_back(materialBuffer.at(i).Get());
		tempConstBuff.push_back(this->actorTypeBuffer.Get());
		tempConstBuff.push_back(nullptr);
		tempConstBuff.push_back(nullptr);

		//vector<ID3D11ShaderResourceView*> tempSrv;															//For sending multiple kinds of textures to ps (unimplimented)
		//tempViewBuffer.push_back(ambient_View.at(i).Get());
		//tempViewBuffer.push_back(diffuse_View.at(i).Get());
		//tempViewBuffer.push_back(specular_View.at(i).Get());

		//Update nrOfVtx
		int nrOfVtx = submeshIndex.at(i) - startIndex;
		immediateContext->PSSetConstantBuffers(0, 2, tempConstBuff.data());										//Set material properties for this mesh
		//immediateContext->PSSetShaderResources(0, 1, diffuse_View.at(i).GetAddressOf());						//Set shader resources for this submesh
		immediateContext->PSSetShaderResources(0, 1, &textureSRV[textureKey]);						//Set shader resources for this submesh
		immediateContext->DrawIndexed(nrOfVtx, startIndex, 0);													//draw mesh

		//Update starting position
		startIndex += nrOfVtx;
	}
}

void Mesh::BindAndDrawCubeMap(ID3D11DeviceContext * immediateContext, ID3D11Buffer *& viewProjBuffer,  ID3D11ShaderResourceView * srv, ID3D11Buffer *& cameraPosBuffer)
{
	ID3D11ShaderResourceView* nullSrv[2]{ nullptr , nullptr};
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(this->matrixBuffer.Get());																//This Matrix
	tempBuff.push_back(viewProjBuffer);																			//Main Camera buffer


	immediateContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset);			//Set Vbuffer
	immediateContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);						//Set Index Buffer
	immediateContext->VSSetConstantBuffers(0, 2, tempBuff.data());												//Buffer info to send to Vshader (world matrix, view/Proj)

	int startIndex = 0;
	for (int i = 0; i < submeshIndex.size(); i++)//för antalet meshes
	{
		vector<ID3D11Buffer*> tempConstBuff;																	//Temporary bufferArray fpr PS. Holds: [Material], [ActorType], [CamPos] buffers
		tempConstBuff.push_back(materialBuffer.at(0).Get());													//Set at 0, specific for cube map
		tempConstBuff.push_back(this->actorTypeBuffer.Get());
		tempConstBuff.push_back(cameraPosBuffer);
		tempConstBuff.push_back(viewProjBuffer);

		vector<ID3D11ShaderResourceView*> tempSrv;																//Temp buffer for Srv's. Holds: [Texture], [TextureCube] views
		tempSrv.push_back(nullptr);														//First slot goes unused in shadder but is needed since slot 1 cant take textureCube
		tempSrv.push_back(srv);

		//Update nrOfVtx
		int nrOfVtx = submeshIndex.at(i) - startIndex;
		immediateContext->PSSetConstantBuffers(0, 3, tempConstBuff.data());										//Set material properties for this mesh
		immediateContext->PSSetShaderResources(0, 2, tempSrv.data());											//Set shader resources for this submesh
		//skicka in buffer med submesh index???
		immediateContext->DrawIndexed(nrOfVtx, startIndex, 0);													//draw mesh

		//Update starting position
		startIndex += nrOfVtx;
	}
	immediateContext->PSSetShaderResources(0, 2, nullSrv);
}

XMMATRIX Mesh::getMatrix()
{
	return this->matrix;
}

void Mesh::setPos(XMFLOAT3 pos)
{
	this->position = pos;
}

void Mesh::setRot(XMFLOAT3 rot)
{
	this->rotation = rot;
}

vector<XMFLOAT3>  Mesh::getFaceNormals()//in WS
{
	//Variables
	vector<XMFLOAT3> tempVector;

	//Multiply normal with Object world matrix 
	for (int i = 0; i < normalPerFace.size(); i++)
	{
		XMFLOAT3 finalNormal;
		XMVECTOR tempXmVector = XMVectorSet(normalPerFace.at(i).x, normalPerFace.at(i).y, normalPerFace.at(i).z, 0);
		XMVECTOR transformedVector = XMVector3Transform(tempXmVector, this->matrix);
		XMStoreFloat3(&finalNormal, transformedVector);
		tempVector.push_back(finalNormal);
	}

	return tempVector;
}

XMFLOAT3 Mesh::getPos()
{
	return this->position;
}

XMFLOAT3 Mesh::getRot()
{
	return this->rotation;
}

bool Mesh::operator==(const Mesh& v) const
{
		bool posEquals = false;
		bool normEquals = false;
		bool uvEquals = false;
		bool allEqual = false;

		if (this->vertexBuffer == v.vertexBuffer)
		{
			posEquals = true;
		}
		if (this->indexBuffer == v.indexBuffer)
		{
			normEquals = true;
		}
		if (this->submeshIndex == v.submeshIndex)
		{
			uvEquals = true;
		}
		if (posEquals && normEquals && uvEquals)
		{
			allEqual = true;
		}
		return allEqual;
}

//bool Mesh::isAnimated() const
//{
//	return useAnims;
//}
//
//int Mesh::getMaxStates()
//{
//	return maxStates;
//}
//
//int Mesh::getLastState()
//{
//	return lastState;
//}
//
//int Mesh::addState(string filename, AnimationType type)
//{
//
//	if (lspSkelMesh != nullptr)
//	{
//		if (LoadAnimation(*lspSkelMesh, filename))
//		{
//			if (!useAnims)
//			{
//				useAnims = true;
//			}
//			maxStates++;
//
//			switch (type)
//			{
//			case AnimationType::walk:
//				walkAnim = maxStates - 1;
//				break;
//			case AnimationType::attack:
//				attackAnim = maxStates - 1;
//				break;
//			case AnimationType::shoot:
//				shootAnim = maxStates - 1;
//				break;
//			case AnimationType::idle:
//				idleAnim = maxStates - 1;
//				break;
//			case AnimationType::hurt:
//				hurtAnim = maxStates - 1;
//				break;
//			default:
//				break;
//			}
//
//			return ++animState;
//		}
//	}
//
//	return animState;
//}
//
//bool Mesh::setAnimState(int state)
//{
//	if (state < maxStates)
//	{
//		lastState = animState;
//		animState = state;
//		return true;
//	}
//	return false;
//}
//
//int Mesh::getAnimState() const
//{
//	if (useAnims)
//	{
//		return animState;
//	}
//
//	return 0;
//}
//
//int Mesh::getWalkAnim() const
//{
//	return walkAnim;
//}
//
//int Mesh::getAttackAnim() const
//{
//	return attackAnim;
//}
//
//int Mesh::getShootAnim() const
//{
//	return shootAnim;
//}
//
//int Mesh::getIdleAnim() const
//{
//	return idleAnim;
//}
//
//bool Mesh::Animate(ID3D11DeviceContext* immediateContext, int animation, bool animate)
//{
//	//char text[100];
//	//sprintf_s(text, "Hi\n"/*, lspSkelMesh->skeleton.at(i).animations.at(animIndex).keys.at(animFrame).key*/);
//	//OutputDebugStringA(text);
//	bool succeeded = false;
//	if (lspSkelMesh != nullptr && animate)
//	{
//		// Check if we are already playing this animation
//		if (animIndex == animation)
//		{
//			// Yes --> check if there are frames left
//			if (animFrame < 100)
//			{
//				// Yes --> move to the next frame on this animation
//				animFrame++;
//			}
//			else
//			{
//				// No --> Restart from first frame
//				animFrame = 0;
//			}
//			
//		}
//		else
//		{
//			// No --> start on frame 0 of the new animation
//			animFrame = 0;
//			animIndex = animation;
//		}
//
//		// For each bone
//		for (int iBone = 0; iBone < (int)lspSkelMesh->bones.size(); iBone++)
//		{
//			// Check if we have an animation on that index
//			if ((int)lspSkelMesh->skeleton.at(iBone).animations.size() > animIndex)
//			{
//				// For each key this animation
//				for (int iKey = 0; iKey < (int)lspSkelMesh->skeleton.at(iBone).animations.at(animIndex).keys.size(); iKey++)
//				{
//					// Check if we have a key matching this frame
//					if (lspSkelMesh->skeleton.at(iBone).animations.at(animIndex).keys.at(iKey).key == animFrame)
//					{
//						//reset bone position
//						XMVECTOR pos;
//						XMVECTOR rot;
//						XMVECTOR scale;
//						
//						XMMATRIX identityMx = XMMatrixIdentity();
//						
//						if (XMMatrixDecompose(&scale, &rot, &pos, identityMx))
//						{
//							// Reset bone rotation
//							lspSkelMesh->skeleton.at(iBone).transform.rotation[0] = XMVectorGetX(rot);
//							lspSkelMesh->skeleton.at(iBone).transform.rotation[1] = XMVectorGetY(rot);
//							lspSkelMesh->skeleton.at(iBone).transform.rotation[2] = XMVectorGetZ(rot);
//						}
//						// Change bone rotation
//						lspSkelMesh->skeleton.at(iBone).transform.rotation[0] = lspSkelMesh->skeleton.at(iBone).animations.at(animIndex).keys.at(iKey).data.rotation[0];
//						lspSkelMesh->skeleton.at(iBone).transform.rotation[1] = lspSkelMesh->skeleton.at(iBone).animations.at(animIndex).keys.at(iKey).data.rotation[1];
//						lspSkelMesh->skeleton.at(iBone).transform.rotation[2] = lspSkelMesh->skeleton.at(iBone).animations.at(animIndex).keys.at(iKey).data.rotation[2];
//					}
//					//else
					//{
					//	//interpolate...
					//}
//				}
//			}
//		}
//		// Update bone matrix buffer
//
//		// Map
//		D3D11_MAPPED_SUBRESOURCE map;
//		ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//
//		// Update matrix buffer
//		boneMx matrices = {};
//		for (int i = 0; i < (int)lspSkelMesh->bones.size(); i++)
//		{
//			// Calculate new matrices
//			CalcMxPerFrame(lspSkelMesh->bones.at(i), *lspSkelMesh);
//			XMFLOAT4X4 tmp;
//			XMStoreFloat4x4(&tmp, lspSkelMesh->bones.at(i).finalMx);
//			matrices.mx[i] = tmp;
//		}
//
//		HRESULT hr = immediateContext->Map(this->boneMxBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
//		memcpy(map.pData, &matrices, sizeof(boneMx));
//
//		// UnMap
//		immediateContext->Unmap(this->boneMxBuffer.Get(), 0);
//		succeeded = !FAILED(hr);
//	}
//	else if (lspSkelMesh != nullptr && !animate)
//	{
//		//Fill buffer with identity matrices for no movement
//
//		animFrame = 0;
//
//		// Map
//		D3D11_MAPPED_SUBRESOURCE map;
//		ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//		// Update matrix buffer
//		boneMx matrices = {};
//		for (int i = 0; i < (int)lspSkelMesh->bones.size(); i++)
//		{
//			XMFLOAT4X4 tmp;
//			XMStoreFloat4x4(&tmp, XMMATRIX(XMMatrixIdentity()));
//			matrices.mx[i] = tmp;
//		}
//
//		HRESULT hr = immediateContext->Map(this->boneMxBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
//		memcpy(map.pData, &matrices, sizeof(boneMx));
//
//		// UnMap
//		immediateContext->Unmap(this->boneMxBuffer.Get(), 0);
//		succeeded = !FAILED(hr);
//	}
//
//	return succeeded;
//}
//
//LspSkeletalMesh* Mesh::getSkelMesh() const
//{
//	return lspSkelMesh;
//}


//--------------------------------------------------------------------- Collision ----------------------------------------------------------------------//

XMFLOAT3* Mesh::getRays(int& nr, XMFLOAT3& direction, float rot)
{
	float x = 0;
	float y = -height;
	float z = 0;
	float root = sqrt(0.5);
	XMFLOAT3* rays = nullptr;
	if (rot == 0)
	{
		direction = XMFLOAT3(0, 0, 1);
		z = depth;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[3] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			nr = 9;
		}
	}
	else if (rot == 0.25)
	{
		direction = XMFLOAT3(root, 0, root);
		x = width;
		z = depth;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			nr = 9;
		}
	}
	else if (rot == 0.5)
	{
		direction = XMFLOAT3(1, 0, 0);
		x = width;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			nr = 9;
		}
	}
	else if (rot == 0.75)
	{
		direction = XMFLOAT3(root, 0, -root);
		x = width;
		z = -depth;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x - (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			nr = 9;
		}
	}
	else if (rot == 1)
	{
		direction = XMFLOAT3(0, 0, -1);
		z = -depth;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[3] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y + 0.001, position.z + center.z + z);
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x - width, position.y + center.y + y, position.z + center.z + z);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x + width, position.y + center.y + y, position.z + center.z + z);
			nr = 9;
		}
	}
	else if (rot == 1.25)
	{
		direction = XMFLOAT3(-root, 0, -root);
		x = -width;
		z = -depth;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + (2 * depth));
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + (2 * depth));
			nr = 9;
		}
	}
	else if (rot == 1.5)
	{
		direction = XMFLOAT3(-1, 0, 0);
		x = -width;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - depth);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z + depth);
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - depth);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z + depth);
			nr = 9;
		}
	}
	else if (rot == 1.75)
	{
		direction = XMFLOAT3(-root, 0, root);
		x = -width;
		z = depth;
		if (this->rays == 0)
		{
			rays = new XMFLOAT3[1];
			rays[0] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			nr = 1;
		}
		else if (this->rays == 1)
		{
			rays = new XMFLOAT3[3];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			nr = 3;
		}
		else if (this->rays == 2)
		{
			rays = new XMFLOAT3[4];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			y = height;
			rays[2] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[3] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			nr = 4;
		}
		else if (this->rays == 3)
		{
			rays = new XMFLOAT3[6];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			y = height;
			rays[3] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			nr = 6;
		}
		else if (this->rays == 4)
		{
			rays = new XMFLOAT3[9];
			rays[0] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[1] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z);
			rays[2] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y + 0.001, position.z + center.z + z - (2 * depth));
			y = 0;
			rays[3] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[4] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[5] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			y = height;
			rays[6] = XMFLOAT3(position.x + center.x + x + (2 * width), position.y + center.y + y, position.z + center.z + z);
			rays[7] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z);
			rays[8] = XMFLOAT3(position.x + center.x + x, position.y + center.y + y, position.z + center.z + z - (2 * depth));
			nr = 9;
		}
	}
	else
	{
		direction = XMFLOAT3(0, 1, 0);
		rays = new XMFLOAT3[1];
		rays[0] = XMFLOAT3(position.x + center.x, position.y + center.y, position.z + center.z);
		nr = 1;
	}
	return rays;
}

float Mesh::collision(XMFLOAT3* rayOrigin, int nr, XMFLOAT3 direction)
{
	float minT = -1;
	for (int i = 0; i < nr; i++)
	{
		bool xTest = false;
		bool yTest = false;
		bool zTest = false;
		float txMin = -1;
		float txMax = -1;
		float tyMin = -1;
		float tyMax = -1;
		float tzMin = -1;
		float tzMax = -1;

		//test x
		if (direction.x == 0)
		{
			if ((position.x + center.x - width) < rayOrigin[i].x && (position.x + center.x + width) > rayOrigin[i].x)
			{
				xTest = true;
			}
		}
		else
		{
			float k = position.x + center.x - width - rayOrigin[i].x;
			txMin = k / direction.x;
			k = position.x + center.x + width - rayOrigin[i].x;
			txMax = k / direction.x;
			if (txMin > txMax)
			{
				float temp = txMin;
				txMin = txMax;
				txMax = temp;
			}
		}

		//test y
		if (direction.y == 0)
		{
			if ((position.y + center.y - height) <= rayOrigin[i].y && (position.y + center.y + height) >= rayOrigin[i].y)
			{
				yTest = true;
			}
		}
		else
		{
			float k = position.y + center.y - height - rayOrigin[i].y;
			tyMin = k / direction.y;
			k = position.y + center.y + height - rayOrigin[i].y;
			tyMax = k / direction.y;
			if (tyMin > tyMax)
			{
				float temp = tyMin;
				tyMin = tyMax;
				tyMax = temp;
			}
		}

		//test z
		if (direction.z == 0)
		{
			if ((position.z + center.z - depth) < rayOrigin[i].z && (position.z + center.z + depth) > rayOrigin[i].z)
			{
				zTest = true;
			}
		}
		else
		{
			float k = position.z + center.z - depth - rayOrigin[i].z;
			tzMin = k / direction.z;
			k = position.z + center.z + depth - rayOrigin[i].z;
			tzMax = k / direction.z;
			if (tzMin > tzMax)
			{
				float temp = tzMin;
				tzMin = tzMax;
				tzMax = temp;
			}
		}

		//compare
		if (xTest && yTest || xTest && zTest || yTest && zTest || xTest && tyMin <= tzMax && tzMin <= tyMax || yTest && txMin <= tzMax && tzMin <= txMax ||
			zTest && tyMin <= txMax && txMin <= tyMax || txMin <= tyMax && txMin <= tzMax && tyMin <= txMax && tyMin <= tzMax && tzMin <= txMax && tzMin <= tyMax)
		{
			//find lowest t
			float temp = -1;
			float arr[3] = { txMin, tyMin, tzMin };
			for (int i = 0; i < 3; i++)
			{
				if (arr[i] > temp)
				{
					temp = arr[i];
				}
			}
			if (temp >= 0)
			{
				if (minT < 0 || temp < minT)
				{
					minT = temp;
				}
			}
		}
	}
	return minT;
}

XMFLOAT3* Mesh::getGravityRays()
{
	XMFLOAT3* rays = new XMFLOAT3[9];
	rays[0] = XMFLOAT3(position.x + center.x, position.y + center.y - height, position.z + center.z);
	rays[1] = XMFLOAT3(position.x + center.x + width, position.y + center.y - height, position.z + center.z + depth);
	rays[2] = XMFLOAT3(position.x + center.x + width, position.y + center.y - height, position.z + center.z - depth);
	rays[3] = XMFLOAT3(position.x + center.x - width, position.y + center.y - height, position.z + center.z - depth);
	rays[4] = XMFLOAT3(position.x + center.x - width, position.y + center.y - height, position.z + center.z + depth);
	rays[5] = XMFLOAT3(position.x + center.x + width, position.y + center.y - height, position.z + center.z);
	rays[6] = XMFLOAT3(position.x + center.x - width, position.y + center.y - height, position.z + center.z);
	rays[7] = XMFLOAT3(position.x + center.x, position.y + center.y - height, position.z + center.z + depth);
	rays[8] = XMFLOAT3(position.x + center.x, position.y + center.y - height, position.z + center.z - depth);
	return rays;
}

float Mesh::gravityCheck(XMFLOAT3* rayOrigin)
{
	float minT = -1;
	XMFLOAT3 direction = XMFLOAT3(0, -1, 0);
	//check middle
	bool xTest = false;
	bool zTest = false;
	float tyMin = -1;
	float tyMax = -1;
	if ((position.x + center.x - width) < rayOrigin[0].x && (position.x + center.x + width) > rayOrigin[0].x)
	{
		xTest = true;
	}
	if ((position.z + center.z - height) < rayOrigin[0].z && (position.z + center.z + height) > rayOrigin[0].z)
	{
		zTest = true;
	}
	float k = position.y + center.y - height - rayOrigin[0].y;
	tyMin = k / direction.y;
	k = position.y + center.y + height - rayOrigin[0].y;
	tyMax = k / direction.y;
	if (tyMin > tyMax)
	{
		float temp = tyMin;
		tyMin = tyMax;
		tyMax = temp;
	}
	if (xTest && zTest && tyMin >= 0)
	{
		minT = tyMin;
	}

	//check rest
	for (int i = 1; i < 9; i++)
	{
		xTest = false;
		zTest = false;
		tyMin = -1;
		tyMax = -1;
		if ((position.x + center.x - width) < rayOrigin[i].x && (position.x + center.x + width) > rayOrigin[i].x)
		{
			xTest = true;
		}
		if ((position.z + center.z - height) < rayOrigin[i].z && (position.z + center.z + height) > rayOrigin[i].z)
		{
			zTest = true;
		}
		k = position.y + center.y - height - rayOrigin[i].y;
		tyMin = k / direction.y;
		k = position.y + center.y + height - rayOrigin[i].y;
		tyMax = k / direction.y;
		if (tyMin > tyMax)
		{
			float temp = tyMin;
			tyMin = tyMax;
			tyMax = temp;
		}
		if (xTest && zTest && tyMin >= 0 && tyMin < minT || xTest && zTest && tyMin >= 0 && minT < 0)
		{
			minT = tyMin;

		}
	}
	
	return minT;
}

float Mesh::getOffsetX()
{
	return center.x;
}

float Mesh::getOffsetY()
{
	return center.y;
}

float Mesh::getOffsetZ()
{
	return center.z;
}

float Mesh::getWidth()
{
	return width;
}

float Mesh::getHeight()
{
	return height;
}

float Mesh::getDepth()
{
	return depth;
}

std::string Mesh::getTextureKey()
{
	return textureKey;
}

void Mesh::setTextureKey(std::string key)
{
	textureKey = key;
}

void Mesh::Release()
{
	for (std::pair<std::string, ID3D11Texture2D*> element : textureMap)
	{
		if(element.second != nullptr)
			element.second->Release();
	}
	for (std::pair<std::string, ID3D11ShaderResourceView*> element : textureSRV)
	{
		if (element.second != nullptr)
			element.second->Release();
	}
}

void Mesh::updateHitbox(float* whd)
{
	if (whd != nullptr)
	{
		width = whd[0];
		height = whd[1];
		depth = whd[2];
		center = XMFLOAT3(whd[3], whd[4], whd[5]);
		delete[]whd;
	}
}

void Mesh::rotateHitBox()
{
	float temp = width;
	width = depth;
	depth = temp;
}

bool Mesh::getBig()
{
	return big;
}

void TextureCollection::Load(ID3D11Device* device,std::string path)
{
	if (textureMap.find(path) == textureMap.end())
	{
		ID3D11Texture2D* tempTexture;
		ID3D11ShaderResourceView* tempTextureView;

		int width = 0;
		int heigth = 0;
		int channels = 0;
		unsigned char* img = stbi_load(("../Textures/" + path).c_str(), &width, &heigth, &channels, STBI_rgb_alpha);
		if (img == NULL)
		{
			std::cerr << "error loading img!" << std::endl;
		}

		//skapa textur
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = heigth;
		textureDesc.MipLevels = 1;								//olika nivåer av kompression
		textureDesc.ArraySize = 1;								//arraystorlek
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//24 bits unsigned normaliserad int som djupet? till stencil 8 bitars unsigned int.
		textureDesc.SampleDesc.Count = 1;						// defaultvärden
		textureDesc.SampleDesc.Quality = 0;						//Sample quality
		textureDesc.Usage = D3D11_USAGE_DEFAULT;				//säger hur den ska användas när vi kommer åt den ()
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;		//binda till shader resource??
		textureDesc.CPUAccessFlags = 0;							// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
		textureDesc.MiscFlags = 0;								//misc flagga
		D3D11_SUBRESOURCE_DATA texture = { 0 };
		texture.pSysMem = img;									//pointer to img data
		texture.SysMemPitch = width * 4;						// number of bytes that a row contains in the image.

		//crate texture, assign it to loadedTexture
		HRESULT hr = device->CreateTexture2D(&textureDesc, &texture, &tempTexture);
		//loadedTexture.push_back(tempTexture);

		//Create texture View, then add to array.
		if (FAILED(device->CreateShaderResourceView(tempTexture, nullptr, &tempTextureView)))
		{
			std::cerr << "error creating TextureView!" << std::endl;
		}
		textureMap.insert(std::make_pair(path, tempTexture));
		textureSRV.insert(std::make_pair(path, tempTextureView));

		//textureView.push_back(tempTextureView);

		//free img data & release temp variables
		stbi_image_free(img);

	}
}
