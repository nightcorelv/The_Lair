#pragma once
#include"Entity.h"
#include <time.h> 
#include <fstream>

using namespace std;

class RoomHandler
{
private:
	//World
	int roomExponent = 7;											//creates array of roomExponent * roomExponent
	int nrOfRooms = roomExponent * roomExponent;					//Nr of rooms in array
	int** roomIndexArr = new int* [nrOfRooms] {nullptr};			//matrix holding index of rooms
	XMFLOAT2 roomRange = XMFLOAT2(5, 7);							//lowest nr of rooms, highest nr (when radomizing path)
	XMFLOAT2 startIndexXY;											//Randomized start position in room array

	bool reset = false;

	//Room
	float yPoint = -5;												//Refrence Y position (all tiles placed relative to this pos)
	float tileWidth = 5.0f;											//Spacing between tiles
	int xDiv = 18;													//Nr of tiles in X
	int zDiv = 12;													//Nr of tiles in Y
	int currentX;
	int currentZ;
	int nrOfTiles = xDiv * zDiv;									//Total nr of tiles
	XMFLOAT3 midPoint;												//Vector to middle of room
	int currentRoom;												//Currently active Room(unused?)
	int roomWidth = (int)(xDiv * tileWidth);								//Current Room Width
	int roomHeight = (int)(zDiv * tileWidth);								//Current Room Height
	vector<Entity> tiles;
	vector<Mesh> meshes;
	vector<int> activeIndex;										//Index of tiles active for room
	ID3D11Device* deviceReference;

	//Room Actors
	int nrOfstatic = 35;//15											//Max nr of static actors in a room
	int activeStatic = 0;											//Nr of currently active Static actors
	vector<Entity> special;
	vector<XMFLOAT4> specialTilesInfo;								//Array for actor spawn data (Y pos/ Object type/ tileIndexPos/ Rot)
	vector<XMFLOAT3> mapCoordinates;								//[xPos][yPos][roomType]
	XMFLOAT4 dorwayTileIndex = XMFLOAT4(0, 0, 0, 0);				//Left/Right/Up/Down
	vector<int> roomIndexes {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17,18,19,20,21,22,24,25,26,27,28,29,30,31,32};

public:

	RoomHandler(ID3D11Device* device);
	~RoomHandler();

	void setRoom(int roomNrX, int roomNrY, ID3D11DeviceContext* immediateContext, ID3D11Device* device);
	void randomizeWorldLayout(ID3D11Device* device);
	void bindAndDrawTiles(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer);
	void updateTiles(ID3D11DeviceContext* immediateContext);
	void updateTile(ID3D11DeviceContext* immediateContext, int tileIndex);
	bool isTileActive(int tileIndex);
	void raiseTile(int tileIndex);

	void setReset(bool value);
	bool checkReset();

	int getClosestTile(XMFLOAT3 playerPos);
	int getCurrentRoomIndex();
	XMFLOAT3 getMidPoint();
	XMFLOAT2 getRoomDimensions();
	XMFLOAT2 getCurrentRoomDivXY();
	XMFLOAT2 getMaxRoomDivXY();
	XMFLOAT3 getTilePos(int tileIndex);
	XMFLOAT2 getStartingRoom();
	vector<XMFLOAT4> getRoomSpecialTileInfo();
	XMFLOAT4 getDorwayTileIndex();
	Entity* getTile(int index);
	XMFLOAT2 getRoomDivXY();
	int getxDiv();
	int* getRoomInfo();
	int getTileWidth();
	int getNrOfStaticActors();
	Entity* getStaticActors(int index);
	vector<XMFLOAT3> getMap();
};
