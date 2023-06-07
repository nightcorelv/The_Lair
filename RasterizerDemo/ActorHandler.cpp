#include "ActorHandler.h"
#include "Plate.h"

ActorHandler::ActorHandler(ID3D11Device * device, int maxArrSize)
	:arrSize(maxArrSize)
{
	//Initiate arrays
	for (int i = 0; i < arrSize; i++)
	{
		LspMesh box;		box.sceneFileName = "../LspMeshes/Puzzlebox";			LoadMesh(box);
		LspMesh tombStone;	tombStone.sceneFileName = "../LspMeshes/TombStone1";	LoadMesh(tombStone);
		LspMesh plate;		plate.sceneFileName = "../LspMeshes/puzzlePlate";	LoadMesh(plate);
		LspMesh door;		door.sceneFileName = "../LspMeshes/door";			LoadMesh(door);
		puzzleBoxArr.push_back(new Puzzlebox(device, box, resetPosition));
		tombstoneArr.push_back(new Tombstone(device, tombStone, resetPosition));
		plateArr.push_back(new Plate(device, plate, XMFLOAT3(0,0,200)));
		doorArr.push_back(new Door(device, door, resetPosition, XMFLOAT3(0,0,0),0 /*hi*/));
	}
}
//float* hi = new float[6]{ 3, 5, 4.5, 0, -7, 2.25 };//LEFT?
//float* hi = new float[6]{ 4.5, 5, 1, -2.25, -7, 0 };//UD
ActorHandler::~ActorHandler()
{
	for (int i = 0; i < arrSize; i++)
	{
		delete puzzleBoxArr.at(i);
		delete tombstoneArr.at(i);
		delete plateArr.at(i);
		delete doorArr.at(i);
	}
}

Entity * ActorHandler::getActor(ID actorID)
{
	//Assumes ptr is actually added to external array thus Index++.
	Entity * ptrToReturn = nullptr;

	
	if (actorID == ID::puzzlebox && pb_Index < arrSize)
	{
		ptrToReturn = puzzleBoxArr.at(pb_Index);
		pb_Index++;
	}
	else if (actorID == ID::tombstone && ts_Index < arrSize)
	{
		ptrToReturn = tombstoneArr.at(ts_Index);
		ts_Index++;
	}
	else if (actorID == ID::plate && pl_Index < arrSize)
	{
		ptrToReturn = plateArr.at(pl_Index);
		pl_Index++;
	}
	else if (actorID == ID::door && dr_Index < arrSize)
	{
		ptrToReturn = doorArr.at(dr_Index);
		dr_Index++;
	}

	return ptrToReturn;
}

void ActorHandler::removeActor(ID actorID)
{
	//Sets enemy position & decreses index.
	if (actorID == ID::puzzlebox && pb_Index < arrSize)
	{
		puzzleBoxArr.at(pb_Index)->setPosition(resetPosition);
		pb_Index--;
	}
	else if (actorID == ID::tombstone && ts_Index < arrSize)
	{
		tombstoneArr.at(ts_Index)->setPosition(resetPosition);
		ts_Index--;
	}
	else if (actorID == ID::plate && pl_Index <= arrSize)
	{
		plateArr.at(pl_Index)->setPosition(resetPosition);
		pl_Index--;
	}
	else if (actorID == ID::door && dr_Index < arrSize)
	{
		doorArr.at(dr_Index)->setPosition(resetPosition);
		dynamic_cast<Door*>(doorArr.at(dr_Index))->reset();
		dr_Index--;
	}
}
