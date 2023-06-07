#pragma once
#include "puzzlebox.h"
#include "tombstone.h"
#include "Door.h"
class Plate;



class ActorHandler
{
private:
	XMFLOAT3 resetPosition = XMFLOAT3(0, 80.0f, 0);
	vector<Puzzlebox*> puzzleBoxArr;
	vector<Tombstone*> tombstoneArr;
	vector<Plate*> plateArr;
	vector<Door*> doorArr;

	int arrSize;

	int pb_Index = 0;
	int ts_Index = 0;
	int pl_Index = 0;
	int dr_Index = 0;

public:

	ActorHandler(ID3D11Device* device, int maxArrSize = 4);
	~ActorHandler();
	Entity* getActor(ID actorID);
	void removeActor(ID actorID);
};