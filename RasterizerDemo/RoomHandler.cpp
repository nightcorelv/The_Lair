#include "RoomHandler.h"

//---------------------------------------------------------------Internal Functions-----------------------------------------------------------------//
void splStrAtChar(string& str, char toFind, vector<string>& strList, bool keepPrefix)
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

void createPathX(int differanceX, int startX, int originY, int** indexesOfRoomPtrArr)
{
	for (int i = 0; i < abs(differanceX); i++)
	{
		if (differanceX > -1)//om positiv
		{
			indexesOfRoomPtrArr[(startX + 1) + i][originY] = i + 2;
		}
		else                 //om negativt
		{
			indexesOfRoomPtrArr[(startX - 1) - i][originY] = i + 2;
		}
	}
}

void createPathY(int differanceY, int startY, int originX, int** indexesOfRoomPtrArr)
{
	for (int i = 0; i < abs(differanceY); i++)
	{

		if (differanceY > -1)//om positiv
		{
			indexesOfRoomPtrArr[originX][(startY + 1) + i] = i + 2;
		}
		else                 //om negativt
		{
			indexesOfRoomPtrArr[originX][(startY - 1) - i] = i + 2;
		}
	}
}

int randInrange(int start, int end)
{
	//Finds random index between start-end
	int foundIndex = 0;
	if (!(start == 0 && end == 0 || start == end))//Skip if both 0 or equal
	{
		if (end > start)
		{
			if (start > 0)
			{
				foundIndex = rand() % (end - start) + start;
			}
			else
			{
				foundIndex = rand() % end + start;
			}
		}
		else
		{
			if (end > 0)
			{
				foundIndex = rand() % (start - end) + end;
			}
			else
			{
				foundIndex = rand() % start + end;
			}
		}
	}
	return foundIndex;
}

XMFLOAT4 checkAdjacentTiles(int X, int Y, int** roomIndexArr, int roomExponent)
{
	//Checks doors to Adjacent Rooms
	XMFLOAT4 leftRightUpDown(0, 0, 0, 0);

	if (Y > 0)				//check behind X
	{
		leftRightUpDown.x = roomIndexArr[X][Y - 1];
		cout << "Cont: " + to_string(roomIndexArr[X][Y - 1]) + " X/Y: " + to_string(X) + "/" + to_string(Y - 1) << endl;
	}
	if (Y < roomExponent - 1)//check in front of X
	{
		leftRightUpDown.y = roomIndexArr[X][Y + 1];
		cout << "Cont: " + to_string(roomIndexArr[X][Y + 1]) + " X/Y: " + to_string(X) + "/" + to_string(Y + 1) << endl;
	}
	if (X > 0)				//check above Y
	{
		leftRightUpDown.z = roomIndexArr[X - 1][Y];
		cout << "Cont: " + to_string(roomIndexArr[X - 1][Y]) + " X/Y: " + to_string(X - 1) + "/" + to_string(Y) << endl;
	}
	if (X < roomExponent - 1)//check below Y
	{
		leftRightUpDown.w = roomIndexArr[X + 1][Y];
		cout << "Cont: " + to_string(roomIndexArr[X + 1][Y]) + " X/Y: " + to_string(X + 1) + "/" + to_string(Y) << endl;
	}
	return leftRightUpDown;
}

bool foundInArr(vector<int> arr, int toFind)
{
	bool found = false;
	for (int k = 0; k < arr.size(); k++)
	{
		if (arr.at(k) == toFind)
		{
			found = true;
			//randomRoom = roomIndexes.at(rand() % roomIndexes.size());
		}
	}
	return found;
}

//--------------------------------------------------------------Constructor/Destructor----------------------------------------------------------------//

RoomHandler::RoomHandler(ID3D11Device* device)
	:currentRoom(0), deviceReference(device)
{
	//Variables
	int incNrs = 0;	//For room
	int incrNr = 0;	//For tile
	int tileType = 0;
	int zpos = 0;
	int xpos = 0;

	//Create Room Array
	for (int i = 0; i < roomExponent; i++)//X
	{
		roomIndexArr[i] = new int[roomExponent];
		for (int j = 0; j < roomExponent; j++)//Y
		{
			roomIndexArr[i][j] = 0;
			incNrs++;
		}
	}
	//create custom hitboxes
	float* whd1 = new float[6]{ tileWidth / 2, 100, tileWidth / 2, 0, 0, 0 };
	float* whd2 = new float[6]{3, 1.5, 1.5, 0, 0, 0};
	float* whd3 = new float[6]{ 3, 1.5, 1.5, 0, 0, 0 };
	float* whd4 = new float[6]{ 3, 1.5, 1.5, 0, 0, 0 };
	float* whd5 = new float[6]{ 3, 1.5, 1.5, 0, 0, 0 };
	float* whd6 = new float[6]{ tileWidth / 2, 600, tileWidth / 2, 0, 0, 0 };

	//Fill meshArray
	LspMesh Tile1;				Tile1.sceneFileName = "../LspMeshes/TileV1Tall";				LoadMesh(Tile1);				meshes.push_back(Mesh(Tile1, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0));			//14 DONE
	LspMesh Tile2;				Tile2.sceneFileName = "../LspMeshes/TileV2Tall";				LoadMesh(Tile2);				meshes.push_back(Mesh(Tile2, device, XMFLOAT3(0, 0, 0)));								//15 DONE
	LspMesh Tile3;				Tile3.sceneFileName = "../LspMeshes/TileV3Tall";				LoadMesh(Tile3);				meshes.push_back(Mesh(Tile3, device, XMFLOAT3(0, 0, 0)));								//16 DONE
	LspMesh Wall1;				Wall1.sceneFileName = "../LspMeshes/wall";						LoadMesh(Wall1);				meshes.push_back(Mesh(Wall1, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, whd6));								//17 DONE
																																meshes.push_back(Mesh(Tile1, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, whd1));	//18 DONE
	LspMesh Temp1;				Temp1.sceneFileName = "../LspMeshes/TileV1Tall";				LoadMesh(Temp1);				meshes.push_back(Mesh(Temp1, device, XMFLOAT3(0, 0, 0)));								//19 - was g
	LspMesh Arch;				Arch.sceneFileName = "../LspMeshes/Arch";						LoadMesh(Arch);					meshes.push_back(Mesh(Arch, device, XMFLOAT3(0, 0, 0)));								//20 DONE
	LspMesh DoorFrame;			DoorFrame.sceneFileName = "../LspMeshes/DoorFrameTall";			LoadMesh(DoorFrame);			meshes.push_back(Mesh(DoorFrame, device, XMFLOAT3(0, 0, 0)));							//21 DONE
	LspMesh Temp2;				Temp2.sceneFileName = "../LspMeshes/TileV1Tall";				LoadMesh(Temp2);				meshes.push_back(Mesh(Temp2, device, XMFLOAT3(0, 0, 0)));								//22 - was tiny cube
	LspMesh Catacomb1;			Catacomb1.sceneFileName = "../LspMeshes/CatacombV1";			LoadMesh(Catacomb1);			meshes.push_back(Mesh(Catacomb1, device, XMFLOAT3(0, 0, 0)));							//23 DONE
	LspMesh Catacomb2;			Catacomb2.sceneFileName = "../LspMeshes/CatacombV2";			LoadMesh(Catacomb2);			meshes.push_back(Mesh(Catacomb2, device, XMFLOAT3(0, 0, 0)));							//24 DONE - face problem?
	LspMesh WallEndPillar;		WallEndPillar.sceneFileName = "../LspMeshes/wallEndPillar";		LoadMesh(WallEndPillar);		meshes.push_back(Mesh(WallEndPillar, device, XMFLOAT3(0, 0, 0)));						//25 DONE - konstigt material?
	LspMesh Pillar1;			Pillar1.sceneFileName = "../LspMeshes/Pillar";					LoadMesh(Pillar1);				meshes.push_back(Mesh(Pillar1, device, XMFLOAT3(0, 0, 0)));								//26 DONE - face problem?
	LspMesh Temp3;				Temp3.sceneFileName = "../LspMeshes/TileV3Tall";				LoadMesh(Temp3);				meshes.push_back(Mesh(Temp3, device, XMFLOAT3(0, 0, 0)));								//27 - was pillar v2 stone
	LspMesh Pillar2BlueTorch;	Pillar2BlueTorch.sceneFileName = "../LspMeshes/Pillarv2BT";		LoadMesh(Pillar2BlueTorch);		meshes.push_back(Mesh(Pillar2BlueTorch, device, XMFLOAT3(0, 0, 0)));					//28 DONE
	LspMesh Pillar2GreenTorch;	Pillar2GreenTorch.sceneFileName = "../LspMeshes/Pillatv2GT";	LoadMesh(Pillar2GreenTorch);	meshes.push_back(Mesh(Pillar2GreenTorch, device, XMFLOAT3(0, 0, 0)));					//29 DONE
	LspMesh GraveSkeleton;		GraveSkeleton.sceneFileName = "../LspMeshes/GraveSkeleton";		LoadMesh(GraveSkeleton);		meshes.push_back(Mesh(GraveSkeleton, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, whd2));//30 DONE - kräver custom hitbox
	LspMesh Grave2;				Grave2.sceneFileName = "../LspMeshes/GraveV2";					LoadMesh(Grave2);				meshes.push_back(Mesh(Grave2, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, whd3));	//31 DONE - kräver custom hitbox
	LspMesh Grave2Lid1;			Grave2Lid1.sceneFileName = "../LspMeshes/GraveV2Lid";			LoadMesh(Grave2Lid1);			meshes.push_back(Mesh(Grave2Lid1, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, whd4));//32 DONE
	LspMesh Grave2Lid2;			Grave2Lid2.sceneFileName = "../LspMeshes/grave2Lid2";			LoadMesh(Grave2Lid2);			meshes.push_back(Mesh(Grave2Lid2, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, whd5));//33 DONE - face problem?
	LspMesh Grave2Lid3;			Grave2Lid3.sceneFileName = "../LspMeshes/GraveV2LidV3";			LoadMesh(Grave2Lid3);			meshes.push_back(Mesh(Grave2Lid3, device, XMFLOAT3(0, 0, 0)));							//34 DONE
	LspMesh Grave1;				Grave1.sceneFileName = "../LspMeshes/Grave1";					LoadMesh(Grave1);				meshes.push_back(Mesh(Grave1, device, XMFLOAT3(0, 0, 0)));								//35 DONE - kräver custom hitbox
	LspMesh TombStone1;			TombStone1.sceneFileName = "../LspMeshes/TombStone1";			LoadMesh(TombStone1);			meshes.push_back(Mesh(TombStone1, device, XMFLOAT3(0, 0, 0)));							//36 DONE- bör göras tjockare, custom hitbox?
	LspMesh TombStone2;			TombStone2.sceneFileName = "../LspMeshes/TombStone2";			LoadMesh(TombStone2);			meshes.push_back(Mesh(TombStone2, device, XMFLOAT3(0, 0, 0)));							//37 DONE - bör göras tjockare, custom hitbox?
	LspMesh TombStone3;			TombStone3.sceneFileName = "../LspMeshes/TombStone3";			LoadMesh(TombStone3);			meshes.push_back(Mesh(TombStone3, device, XMFLOAT3(0, 0, 0)));							//38 DONE - bör göras tjockare, custom hitbox?
	LspMesh Torch;				Torch.sceneFileName = "../LspMeshes/Torch";						LoadMesh(Torch);				meshes.push_back(Mesh(Torch, device, XMFLOAT3(0, 0, 0)));								//39 DONE
	LspMesh Skeleton;			Skeleton.sceneFileName = "../LspMeshes/Skeleton";				LoadMesh(Skeleton);				meshes.push_back(Mesh(Skeleton, device, XMFLOAT3(0, 0, 0)));							//40 DONE - kräver custom hitbox
	LspMesh Skull;				Skull.sceneFileName = "../LspMeshes/Skull";						LoadMesh(Skull);				meshes.push_back(Mesh(Skull, device, XMFLOAT3(0, 0, 0)));								//41 DONE
	LspMesh BlueTorchWall;		BlueTorchWall.sceneFileName = "../LspMeshes/BlueTorchWall";		LoadMesh(BlueTorchWall);		meshes.push_back(Mesh(BlueTorchWall, device, XMFLOAT3(0, 0, 0)));						//42 DONE - face problem?
	LspMesh GreenTorchWall;		GreenTorchWall.sceneFileName = "../LspMeshes/GreenTorchWall";	LoadMesh(GreenTorchWall);		meshes.push_back(Mesh(GreenTorchWall, device, XMFLOAT3(0, 0, 0)));						//43 DONE - face problem?
	LspMesh Catacomb3;			Catacomb3.sceneFileName = "../LspMeshes/CatacombV3";			LoadMesh(Catacomb3);			meshes.push_back(Mesh(Catacomb3, device, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0, nullptr));							//44 DONE - kräver custom hitbox
	LspMesh Catacomb4;			Catacomb4.sceneFileName = "../LspMeshes/CatacombV4";			LoadMesh(Catacomb4);			meshes.push_back(Mesh(Catacomb4, device, XMFLOAT3(0, 0, 0)));							//45 DONE - kräver custom hitbox


	//Create Tile Array
	for (int i = 0; i < nrOfTiles; i++)
	{
		if (i % xDiv == 0 && i > 0)
		{
			incrNr++;
			zpos = incrNr;
			xpos += -xDiv;
		}
		//Place tile
		float finX = tileWidth * xpos;
		float finZ = tileWidth * zpos;
		XMFLOAT3 tilePos((finX), yPoint, (finZ));
		tiles.push_back(Entity(device, meshes[rand()%3], tilePos));
		tiles.back().setTile(tiles.back().getPosition().x, tiles.back().getPosition().z, getxDiv(), getTileWidth());
		this->activeIndex.push_back(i);
		xpos++;
	}

	//Create Static Actor Array
	for (int i = 0; i < nrOfstatic; i++)//tile är placholder
	{
		special.push_back(Entity(device, meshes[0], XMFLOAT3(0, 80, 0)));
	}

	//Set midpoint
	this->midPoint = XMFLOAT3(((tileWidth * xDiv / 2) - (tileWidth / 2)), yPoint, ((tileWidth * zDiv / 2) - (tileWidth / 2)));

	//Randomize World
	this->randomizeWorldLayout(device);
}

RoomHandler::~RoomHandler()
{
	for (int i = 0; i < roomExponent; i++)
	{
		delete[] roomIndexArr[i];
		roomIndexArr[i] = nullptr;
	}
	delete[] roomIndexArr;
}

//---------------------------------------------------------------  Functions  -----------------------------------------------------------------//

void RoomHandler::setRoom(int roomNrX, int roomNrY, ID3D11DeviceContext* immediateContext, ID3D11Device* device)
{
	//Variables
	int xWidth = 0;
	int zWidth = 0;
	int zpos = 0;
	int realxWidth = 0;
	vector<int> Index;
	ifstream in;

	//Clear special tileData
	specialTilesInfo.clear();
	for (int i = 0; i < activeStatic; i++)
	{
		this->special.at(i).setPosition(XMFLOAT3(0, 80, 0));
	}
	this->activeStatic = 0;
	this->dorwayTileIndex = XMFLOAT4(0, 0, 0, 0);

	//Prevent exit of world array
	if (roomNrX >= roomExponent || roomNrX < 0)
	{
		roomNrX = roomExponent - 1;
	}
	if (roomNrX < 0)
	{
		roomNrX = 0;
	}
	if (roomNrY >= roomExponent)
	{
		roomNrY = roomExponent - 1;
	}
	if (roomNrY < 0)
	{
		roomNrY = 0;
	}

	//Set Current room
	int indexofroomtoload = roomIndexArr[roomNrY][roomNrX];
	this->currentRoom = indexofroomtoload;


	//Check room connections
	XMFLOAT4 leftRightUpDown = checkAdjacentTiles(roomNrY, roomNrX, roomIndexArr, roomExponent);

	//Check room Height & width
	in.open("../DataFiles/room" + std::to_string(indexofroomtoload) + ".txt");
	if (in.is_open())
	{
		std::string line;
		while (std::getline(in, line))
		{
			if (!line.empty())
			{
				if (line.at(0) == *"r") //Room layout
				{
					vector<string> temp;
					splStrAtChar(line, ' ', temp, false);
					realxWidth = temp.size();
					zWidth++;
				}
			}
		}
		in.close();
	}
	int leftoverX = (xDiv - xWidth);
	int leftoverY = (zDiv - zWidth);

	//forDorway Generation
	bool firstDorwayTileRight = true;
	bool firstDorwayTileLeft = true;
	bool firstDorwayTileUP = true;
	bool firstDorwayTileDOWN = true;

	//Load room tiles
	in.open("../DataFiles/room" + std::to_string(indexofroomtoload) + ".txt");
	if (in.is_open())
	{
		std::string line;
		while (std::getline(in, line))
		{
			if (!line.empty())
			{
				if (line.at(0) == *"r") //Room layout
				{
					vector<string> temp;
					splStrAtChar(line, ' ', temp, false);
					for (int i = 0; i < temp.size(); i++)	//set tile type & position
					{
						int rotation = rand() % 4;
						if (rotation == 0)
						{

						}
						else
						{
							rotation = 1;
						}
						int currentIndex = i + (zpos * xDiv);													//Index
						int tileType = stoi(temp.at(i));														//TileType
						Index.push_back(currentIndex);															//Add to index list
						XMFLOAT3 currentPos = this->tiles.at(currentIndex).getPosition();						//Get tile Pos
						XMFLOAT3 targetPos(currentPos.x, (float)(yPoint + (tileType * tileWidth / 4) ), currentPos.z);	//create target Pos
						this->tiles.at(currentIndex).setMesh(device, meshes[rotation], targetPos, XMFLOAT3(0, rand() % 4, 0));			//Set tile Pos
						this->tiles.at(currentIndex).setPosition(targetPos);

						//-------------------// Walls //-------------------//


						//or last row UP wall
						if (zpos == zWidth - 1)
						{
							//Room has a door UP
							if (leftRightUpDown.z > 0)
							{
								//Mask to avoid covering dorway (UP)
								if (i > 0 && i % (realxWidth / 2) == 0 || i > 0 && i % ((realxWidth / 2) - 1) == 0 && i < (realxWidth / 2) + 1)//||&& i < (realxWidth / 2)+1
								{
									if (firstDorwayTileUP)
									{
										dorwayTileIndex.z = currentIndex;
									}
									firstDorwayTileUP = false;
								}
								else
								{
									this->tiles.at(currentIndex).setMesh(device, meshes.at(3), targetPos);
								}
							}
							else
							{
								this->tiles.at(currentIndex).setMesh(device, meshes.at(3), targetPos);
							}
						}


						//if on first DOWN wall
						if (zpos == 0)
						{
							XMFLOAT3 downWallPos(currentPos.x, yPoint +tileWidth, currentPos.z);	//create target Pos

							//Room has a door DOWN
							if (leftRightUpDown.w > 0)
							{
								//Mask to avoid covering dorway (Down)
								if (i > 0 && i % (realxWidth / 2) == 0 || i > 0 && i % ((realxWidth / 2) - 1) == 0 && i < (realxWidth / 2) + 1)//||&& i < (realxWidth / 2)+1
								{
									if (firstDorwayTileDOWN)
									{
										dorwayTileIndex.w = currentIndex;
									}
									firstDorwayTileDOWN = false;
								}
								else
								{
									this->tiles.at(currentIndex).setMesh(device, meshes.at(4), downWallPos);
								}
							}
							else
							{
								this->tiles.at(currentIndex).setMesh(device, meshes.at(4), downWallPos);
							}
						}

						//First collumn (left wall)
						if (i == 0)
						{
							//Room has a door to the left
							if (leftRightUpDown.x > 0)
							{
								//Mask to avoid covering dorway (left)//FEL???
								if (zpos % (zWidth / 2) == 0 && zpos < (zWidth / 2) + 1 && zpos >(zWidth / 2) - 2 || zpos % ((zWidth / 2) - 1) == 0 && zpos < (zWidth / 2) + 1 && zpos >(zWidth / 2) - 2)
								{
									if (firstDorwayTileLeft)
									{
										dorwayTileIndex.x = currentIndex;
									}
									firstDorwayTileLeft = false;
								}
								else
								{
									this->tiles.at(currentIndex).setMesh(device, meshes.at(3), targetPos);
								}
							}
							else
							{
								this->tiles.at(currentIndex).setMesh(device, meshes.at(3), targetPos);
							}
						}


						//Last collumn (right wall)
						if (i == (realxWidth - 1))
						{
							//Room has a door to the right
							if (leftRightUpDown.y > 0)
							{
								//Mask to avoid covering dorway (right)//fel!!
								if (zpos % (zWidth / 2) == 0 && zpos < (zWidth / 2) + 1 && zpos >(zWidth / 2) - 2 || zpos % ((zWidth / 2) - 1) == 0 && zpos < (zWidth / 2) + 1 && zpos >(zWidth / 2) - 2)
								{
									if (firstDorwayTileRight)
									{
										dorwayTileIndex.y = currentIndex;
									}
									firstDorwayTileRight = false;
								}
								else
								{
									this->tiles.at(currentIndex).setMesh(device, meshes.at(3), targetPos);
								}
							}
							else
							{
								this->tiles.at(currentIndex).setMesh(device, meshes.at(3), targetPos);
							}
						}

						//Special indexes in room array, for spawning some tile types (walls, holes, etc) does not 
						if (tileType == 99) //Wall
						{
							this->tiles.at(currentIndex).setMesh(device, meshes.at(3), currentPos);
						}
						else if (tileType == 98) //Invisible Wall
						{
							this->tiles.at(currentIndex).setMesh(device, meshes.at(4), currentPos);
						}
						else if (tileType == 97) //Empty Tile
						{
							this->tiles.at(currentIndex).setMesh(device, meshes.at(8), currentPos);
						}

					}
					xWidth = temp.size();
					leftoverX = (xDiv - xWidth);
					zpos++;
				}
				if (line.at(0) == *"s") //Special Tile
				{
					vector<string> temp;
					vector<string> temp1;
					splStrAtChar(line, ' ', temp, true);
					splStrAtChar(temp.at(1), '/', temp1, true);
					int maxTiles = (xDiv * zDiv) - 1;


					if (stoi(temp1.at(1)) > 13)//set special tile type & position
					{

						//int invertedRL = 215 - spTile.at(i).z;
						//float modulus = (int)(spTile.at(i).z) % 18;
						//int realIndex = invertedRL + (modulus - 1);


						int inverted = maxTiles - stoi(temp1.at(2));
						float modulus = (int)(stoi(temp1.at(2))) % 18;
						float modulus2 = (int)(stoi(temp1.at(2))) % 12;
						int realIndex = inverted + (modulus - 1);

						//Tile pos
						XMFLOAT3 tilePos = tiles.at(stoi(temp1.at(2))).getPosition();



						float actorHeight = (tilePos.y + (tileWidth / 2)) + (stoi(temp1.at(0)) * (tileWidth / 4));
						XMFLOAT3 specPos = XMFLOAT3(tilePos.x, actorHeight, tilePos.z);	//Target pos for Special tile
						XMFLOAT3 specRot = XMFLOAT3(0, (float)stoi(temp1.at(3)) / 4.0f, 0);

						this->special.at(activeStatic).setMesh(device, meshes.at(stoi(temp1.at(1)) - 13), specPos, specRot);
						if (specRot.y == 0.5 || specRot.y == 1.5)
						{
							this->special.at(activeStatic).getMesh()->rotateHitBox();
						}
						this->special.at(activeStatic).setPosition(specPos);
						activeStatic++;
					}
					else
					{

						int inverted = maxTiles - stoi(temp1.at(2));
						float modulus = (int)(stoi(temp1.at(2))) % 18;
						int realIndex = inverted + modulus;

						//Tile pos
						XMFLOAT3 tilePos = tiles.at(stoi(temp1.at(2))).getPosition();
						float actorHeight = (tilePos.y + (tileWidth/2)) + (stoi(temp1.at(0)) * (tileWidth/4));

						//Add to actorDataArray (get in main for spawn data)
						XMFLOAT4 data(actorHeight, stoi(temp1.at(1)), stoi(temp1.at(2)), (float)stoi(temp1.at(3)) / 4.0f);
						this->specialTilesInfo.push_back(data);
					}
				}
			}
		}
		in.close();
	}
	/*
s 0/20/9/4
s 0/20/108/6
s 0/20/107/2
s 0/20/207/0
	*/

	//Set dorways
	if (dorwayTileIndex.x > 0)//Left
	{
		XMFLOAT3 tilePos = this->tiles.at(dorwayTileIndex.x+xDiv).getPosition();
		this->special.at(activeStatic).setMesh(device, meshes.at(7), tilePos, XMFLOAT3(0, 1.5, 0));
		this->special.at(activeStatic).setRotation(XMFLOAT3(0, 1.5, 0));
		activeStatic++;
	}
	if (dorwayTileIndex.y > 0)//Right
	{
		XMFLOAT3 tilePos = this->tiles.at(dorwayTileIndex.y ).getPosition();
		this->special.at(activeStatic).setMesh(device, meshes.at(7), tilePos, XMFLOAT3(0, 0.5, 0));
		this->special.at(activeStatic).setRotation(XMFLOAT3(0, 0.5, 0));
		activeStatic++;
	}
	if (dorwayTileIndex.z > 0)//DOWN
	{
		XMFLOAT3 tilePos = this->tiles.at(dorwayTileIndex.z + 1).getPosition();
		this->special.at(activeStatic).setMesh(device, meshes.at(7), tilePos, XMFLOAT3(0, 1, 0));
		this->special.at(activeStatic).setRotation(XMFLOAT3(0, 0, 0));
		activeStatic++;
	}
	if (dorwayTileIndex.w > 0)//UP
	{
		XMFLOAT3 tilePos = this->tiles.at(dorwayTileIndex.w).getPosition();
		this->special.at(activeStatic).setMesh(device, meshes.at(6), tilePos, XMFLOAT3(0, 0, 0));
		this->special.at(activeStatic).setRotation(XMFLOAT3(0, 1, 0));
		activeStatic++;
	}



	//Set new active tiles
	if (Index.size() > 1)
	{
		this->activeIndex.clear();
		activeIndex = Index;

		//Set new Midpoint
		this->midPoint = XMFLOAT3(((tileWidth * xWidth / 2) - (tileWidth / 2)), yPoint, ((tileWidth * zpos / 2) - (tileWidth / 2)));

		//Set new Height/Width
		this->roomWidth = xWidth * tileWidth;
		this->roomHeight = zpos * tileWidth;

		//Update tiles
		this->updateTiles(immediateContext);
	}

	currentX = xWidth;
	currentZ = zpos;
}

void RoomHandler::randomizeWorldLayout(ID3D11Device* device)
{
	//Variables
	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;
	int differanceX = 0;
	int differanceY = 0;
	int differance = 0;
	int randNr = rand() % 2;
	int randNr1 = rand() % 2;


	//Reset world matrix values
	for (int i = 0; i < roomExponent; i++)//X
	{
		for (int j = 0; j < roomExponent; j++)//Y
		{
			roomIndexArr[i][j] = 0;
		}
	}
	mapCoordinates.clear();


	//Randomize start & end points
	while (differance < roomRange.x || differance == 0)
	{
		startX = rand() % roomExponent;
		startY = rand() % roomExponent;
		endX = rand() % roomExponent;
		endY = rand() % roomExponent;

		//Difference
		differanceX = endX - startX;
		differanceY = endY - startY;
		differance = abs(differanceX) + (abs(differanceY) - 1);

		//Re-calculate if path too short or long
		if (differance < roomRange.x || differance > roomRange.y)
		{
			differance = 0;
		}
	}

	//Randomize mid point
	int midX = randInrange(startX, endX);
	int midY = randInrange(startY, endY);
	int midDiffX = midX - startX;
	int midDiffY = midY - startY;
	int midDiffXE = endX - midX;
	int midDiffYE = endY - midY;


	//CreatePaths  
	if (randNr == 0)	//to mid	//(if sats ändrar ordningen x-y eller y-x)
	{
		createPathX(midDiffX, startX, startY, roomIndexArr);
		createPathY(midDiffY, startY, midX, roomIndexArr);
	}
	else
	{
		createPathX(midDiffX, startX, midY, roomIndexArr);
		createPathY(midDiffY, startY, startX, roomIndexArr);
	}
	if (randNr1 == 0)	//to end
	{
		createPathX(midDiffXE, midX, midY, roomIndexArr);
		createPathY(midDiffYE, midY, endX, roomIndexArr);
	}
	else
	{
		createPathX(midDiffXE, midX, endY, roomIndexArr);
		createPathY(midDiffYE, midY, midX, roomIndexArr);
	}

	//fungerande rum: 9,  .....//behöver uppdateras
	// 
	//Mark start, mid, end point
	roomIndexArr[startX][startY] = 9;
	roomIndexArr[midX][midY] = 21; //10
	roomIndexArr[endX][endY] = 25;//25
	this->startIndexXY = XMFLOAT2(startX, startY);
	vector<int> usedIndex;
	usedIndex.push_back(roomIndexArr[startX][startY]);
	usedIndex.push_back(roomIndexArr[midX][midY]);
	usedIndex.push_back(roomIndexArr[endX][endY]);
	int found = 0;//temp variable can remove

	//find non empty indexes
	for (int i = 0; i < roomExponent; i++)//X
	{
		for (int j = 0; j < roomExponent; j++)//Y
		{
			int roomIndex = roomIndexArr[i][j];
			//Check if room
			if (roomIndex != 0)
			{
				//Set random room index (that has not been used before)
				if (roomIndex != roomIndexArr[startX][startY] && roomIndex != roomIndexArr[midX][midY] && roomIndex != roomIndexArr[endX][endY])
				{
					bool repetedRoom = true;
					int randomRoom = roomIndexes.at(rand() % roomIndexes.size());
					while (repetedRoom)
					{
						randomRoom = roomIndexes.at(rand() % roomIndexes.size());
						if (randomRoom != 0)
						{
							repetedRoom = foundInArr(usedIndex, randomRoom);
						}
					}
					roomIndexArr[i][j] = randomRoom;
					usedIndex.push_back(roomIndexArr[i][j]);
				}
				//Ceck room connections	//Ladda motsvarande rum
				XMFLOAT4 leftRightUpDown = checkAdjacentTiles(i, j, roomIndexArr, roomExponent);
				if (leftRightUpDown.x > 0 && leftRightUpDown.y > 0 && leftRightUpDown.z > 0 && leftRightUpDown.w > 0)
				{	//korsning

				}
				else if (leftRightUpDown.x > 0 && leftRightUpDown.y > 0 && leftRightUpDown.z > 0 && leftRightUpDown.w == 0)//ur//ul //dl //dr saknas...
				{	//L R U

				}
				else if (leftRightUpDown.x > 0 && leftRightUpDown.y > 0 && leftRightUpDown.z == 0 && leftRightUpDown.w > 0)
				{	//L R D

				}
				else if (leftRightUpDown.x > 0 && leftRightUpDown.y > 0 && leftRightUpDown.z == 0 && leftRightUpDown.w == 0)
				{	//L R
					
					//
					//spawn side room (up or down)
					if (i + 1 < roomExponent && i - 1 >= 0)
					{
						//Variables for checking room connections
						int aboveOrBelow = rand() % 2;
						XMFLOAT4 behind_LRUD = checkAdjacentTiles(i, j - 1, roomIndexArr, roomExponent);
						XMFLOAT4 front_LRUD = checkAdjacentTiles(i, j + 1, roomIndexArr, roomExponent);

						//If room behind and in front only have side connections (none up & down)
						if (behind_LRUD.x > 0 && behind_LRUD.y > 0 && behind_LRUD.z == 0 && behind_LRUD.w == 0 || behind_LRUD.x > 0 && behind_LRUD.y == 0 && behind_LRUD.z == 0 && behind_LRUD.w == 0)
						{
							if (front_LRUD.x > 0 && front_LRUD.y > 0 && front_LRUD.z == 0 && front_LRUD.w == 0 || front_LRUD.x == 0 && front_LRUD.y > 0 && front_LRUD.z == 0 && front_LRUD.w == 0)
							{
								//Spawn side room to random side of current room
								if (aboveOrBelow == 1)	//Right
								{
									roomIndexArr[i + 1][j] = 23;//10
								}
								else					//Left
								{
									roomIndexArr[i - 1][j] = 23;//10
								}
							}
						}
					}

				}
				else if (leftRightUpDown.x == 0 && leftRightUpDown.y == 0 && leftRightUpDown.z > 0 && leftRightUpDown.w > 0)
				{	//UD
					
					//spawn side room (right or left)
					if (j + 1 < roomExponent && j - 1 >= 0)
					{
						//Variables for checking room connections
						int rightOrleft = rand() % 2;
						XMFLOAT4 up_LRUD = checkAdjacentTiles(i + 1, j, roomIndexArr, roomExponent);
						XMFLOAT4 down_LRUD = checkAdjacentTiles(i - 1, j, roomIndexArr, roomExponent);

						//If room above or below only have up & down connections (none to sides)
						if (up_LRUD.x == 0 && up_LRUD.y == 0 && up_LRUD.z > 0 && up_LRUD.w > 0 || up_LRUD.x == 0 && up_LRUD.y == 0 && up_LRUD.z == 0 && up_LRUD.w > 0)
						{
							if (down_LRUD.x == 0 && down_LRUD.y == 0 && down_LRUD.z > 0 && down_LRUD.w > 0 || down_LRUD.x == 0 && down_LRUD.y == 0 && down_LRUD.z > 0 && down_LRUD.w == 0)
							{
								//Spawn side room above or below current room
								if (rightOrleft == 1)		//Down
								{
									roomIndexArr[i][j + 1] = 30;
								}
								else                        //UP
								{
									roomIndexArr[i][j - 1] = 30;
								}
							}
						}
					}
				}
				else if (leftRightUpDown.x > 0 && leftRightUpDown.y == 0 && leftRightUpDown.z == 0 && leftRightUpDown.w == 0)
				{	//L

				}
				else if (leftRightUpDown.x == 0 && leftRightUpDown.y > 0 && leftRightUpDown.z == 0 && leftRightUpDown.w == 0)
				{	//R

				}
				else if (leftRightUpDown.x == 0 && leftRightUpDown.y == 0 && leftRightUpDown.z > 0 && leftRightUpDown.w == 0)
				{	//U

				}
				else if (leftRightUpDown.x == 0 && leftRightUpDown.y == 0 && leftRightUpDown.z == 0 && leftRightUpDown.w > 0)
				{	//D

				}
				found++;
			}
		}
	}
	for (int i = 0; i < roomExponent; i++)//X
	{
		for (int j = 0; j < roomExponent; j++)//Y
		{
			if (roomIndexArr[i][j] != 0)
			{
				//Save room Coordinates for map
				float finX = -tileWidth * i;
				float finZ = tileWidth * j;
				mapCoordinates.push_back(XMFLOAT3(finZ / 30, finX / 30, roomIndexArr[i][j]));
			}
		}
	}

}

void RoomHandler::bindAndDrawTiles(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer)
{
	for (int i = 0; i < activeIndex.size(); i++)
	{
		this->tiles.at(activeIndex.at(i)).draw(immediateContext, viewProjBuffer);
	}
}

int RoomHandler::getCurrentRoomIndex()
{
	return this->currentRoom;
}

XMFLOAT3 RoomHandler::getMidPoint()
{
	return this->midPoint;
}

XMFLOAT2 RoomHandler::getRoomDimensions()
{
	return XMFLOAT2((float)this->roomWidth, (float)this->roomHeight);
}

XMFLOAT2 RoomHandler::getCurrentRoomDivXY()
{
	return XMFLOAT2(roomWidth / tileWidth, roomHeight / tileWidth);
}

XMFLOAT2 RoomHandler::getMaxRoomDivXY()
{
	return XMFLOAT2(this->xDiv, this->zDiv);
}

XMFLOAT3 RoomHandler::getTilePos(int tileIndex)
{
	return this->tiles.at(tileIndex).getPosition();
}

void RoomHandler::setReset(bool value)
{
	this->reset = value;
}

bool RoomHandler::checkReset()
{
	return reset;
}

int RoomHandler::getClosestTile(XMFLOAT3 playerPos)
{
	int shortestVector = 4;
	int tileIndex = -1;
	for (int i = 0; i < tiles.size(); i++)//check shortest vector
	{
		XMFLOAT3 tilepos = tiles.at(i).getPosition();
		float x = playerPos.x - tilepos.x;
		float y = playerPos.y - tilepos.y;
		float z = playerPos.z - tilepos.z;

		int vectorLenght = sqrt((x * x) + (y * y) + (z * z));

		if (vectorLenght < shortestVector && shortestVector < 5)
		{
			shortestVector = vectorLenght;
			tileIndex = i;
		}
	}

	return tileIndex;
}

XMFLOAT2 RoomHandler::getStartingRoom()
{
	return this->startIndexXY;
}

vector<XMFLOAT4> RoomHandler::getRoomSpecialTileInfo()
{
	return this->specialTilesInfo;
}

bool RoomHandler::isTileActive(int tileIndex)
{
	//check if active
	bool isActive = false;
	for (int i = 0; i < activeIndex.size(); i++)
	{
		if (activeIndex.at(i) == tileIndex)
		{
			isActive = true;
			i = activeIndex.size();
		}
	}
	return isActive;
}


void RoomHandler::raiseTile(int tileIndex)
{
	XMFLOAT3 tilepos = tiles.at(tileIndex).getPosition();
	if (*tiles.at(tileIndex).getMesh() == meshes.at(8))
	{
		tiles.at(tileIndex).setMesh(this->deviceReference, meshes.at(1), XMFLOAT3(tilepos.x, tilepos.y - 1, tilepos.z));
	}

	//if (!isTileActive(tileIndex))//set active if not
	//{
	//	activeIndex.push_back(tileIndex);
	//	tiles.at(tileIndex).setPosition(XMFLOAT3(tilepos.x, tilepos.y - 1, tilepos.z));
	//}

	tilepos = tiles.at(tileIndex).getPosition();
	if (tilepos.y < yPoint)
	{
		tiles.at(tileIndex).setPosition(XMFLOAT3(tilepos.x, tilepos.y + 0.5f, tilepos.z));
	}
	else if (tilepos.y > yPoint)
	{
		tiles.at(tileIndex).setPosition(XMFLOAT3(tilepos.x, yPoint, tilepos.z));
	}

}

void RoomHandler::updateTiles(ID3D11DeviceContext* immediateContext)
{
	for (int i = 0; i < activeIndex.size(); i++)
	{
		tiles.at(activeIndex.at(i)).updateMesh(immediateContext);
	}

	for (int i = 0; i < special.size(); i++)
	{
		this->special.at(i).updateMesh(immediateContext);
	}
}

void RoomHandler::updateTile(ID3D11DeviceContext* immediateContext, int tileIndex)
{
	tiles.at(tileIndex).updateMesh(immediateContext);
}

Entity* RoomHandler::getTile(int index)
{
	return &tiles[index];
}

XMFLOAT4 RoomHandler::getDorwayTileIndex()
{
	return this->dorwayTileIndex;
}

int RoomHandler::getxDiv()
{
	return xDiv;
}

int* RoomHandler::getRoomInfo()
{
	int* info = new int[4];
	info[0] = xDiv;
	info[1] = zDiv;
	info[2] = currentX;
	info[3] = currentZ;
	return info;
}

int RoomHandler::getTileWidth()
{
	return this->tileWidth;
}

int RoomHandler::getNrOfStaticActors()
{
	return this->activeStatic;
}

Entity* RoomHandler::getStaticActors(int index)
{
	return &this->special.at(index);
}

vector<XMFLOAT3> RoomHandler::getMap()
{
	return this->mapCoordinates;
}
