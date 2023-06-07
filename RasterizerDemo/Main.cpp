#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <chrono>
#include <thread>
#include <vector>
#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "PipelineHelper.h"
#include "Mesh.h"
#include "PointLight.h"
#include "LightHandler.h"
#include "CubeMapObj.h"
#include "Camera.h"
#include "Tile.h"
#include "RoomHandler.h"
#include "Player.h"
#include "Necromancer.h"
#include "Pile.h"
#include "Puzzlebox.h"
#include "Skull.h"
#include "Tombstone.h"
#include "Wraith.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Time.h"
#include "EnemyHandler.h"
#include "ActorHandler.h"
#include "PickupHandler.h"
#include "Projectile.h"
#include "GuiHandler.h"
#include "Plate.h"
//#include "FileCounter.h"//?
#include "Audio.h"
#include "DXApp.h"


using namespace DirectX;
using namespace std::chrono;
const static UINT WIDTH = 1280;				//Window width 
const static UINT HEIGHT = 720;				//Window height

//--------------------------------------------------------------//Render / Draw//-----------------------------------------------------------------------//
#pragma region RenderFunctions

void Render(ID3D11DeviceContext* immediateContext, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11Buffer* CameraMTXBuffer, 
	ID3D11SamplerState* samplerState, ID3D11Buffer* cameraPosBuffer, ID3D11UnorderedAccessView* backBufferUav, ID3D11ComputeShader* cShader, ID3D11Texture2D** dfTexArr, ID3D11ShaderResourceView** dfSrvArr, ID3D11RenderTargetView** dfRtvArr, 
	vector<Entity*>& entityArray, RoomHandler& roomHandler, LightHandler& ltHandler, CubeMapHandler& reflectCube)
{
	//Clear out rtv's and dsview
	float clearColor[4] = { 0,0,0,0 };
	for (int i = 0; i < 8; i++)
	{
		immediateContext->ClearRenderTargetView(dfRtvArr[i], clearColor);
	}
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//Bind
	immediateContext->IASetInputLayout(inputLayout);											//Set input layout
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);			//Set how topology is rendered
	immediateContext->VSSetShader(vShader, nullptr, 0);											//Set Vshader
	immediateContext->PSSetShader(pShader, nullptr, 0);											//Set Pshader
	immediateContext->RSSetViewports(1, &viewport);												//Set viewport
	immediateContext->PSSetSamplers(0, 1, &samplerState);										//Set sampler
	immediateContext->OMSetRenderTargets(8, dfRtvArr, dsView);									//Set render targets

	//Draw
	
	//ltHandler.bindAndDrawIndexedMesh(immediateContext, CameraMTXBuffer);						//Draw meshes for lights
	for (int i = 0; i < entityArray.size(); i++)
	{
		if (entityArray.at(i) != nullptr)
		{
			
			entityArray.at(i)->draw(immediateContext, CameraMTXBuffer);
								//Draw actors

			if (entityArray.at(i)->getId() == ID::necromancer)                                          // draws enem�es projecctiles
			{
				Necromancer* temp = dynamic_cast<Necromancer*>(entityArray.at(i));
				temp->getProjectileHandeler()->BindAndDrawProjectiles(immediateContext, CameraMTXBuffer);
			}
			else if (entityArray.at(i)->getId() == ID::wraith)
			{
				Wraith* temp = dynamic_cast<Wraith*>(entityArray.at(i));
				temp->getProjectileHandeler()->BindAndDrawProjectiles(immediateContext, CameraMTXBuffer);
			}
		}
	}
	roomHandler.bindAndDrawTiles(immediateContext, CameraMTXBuffer);							//Draw Tiles
	reflectCube.bindAndDrawIndexedMesh(immediateContext, CameraMTXBuffer, cameraPosBuffer);	//Draw cube map
}

void RenderDepthFromLight(ID3D11DeviceContext* immediateContext, ID3D11VertexShader* vShader, ID3D11Buffer* viewBuffer, vector<Entity*>& entityArray, RoomHandler& roomHandler, LightHandler& ltHandler, int ptLightIndex)
{
	ID3D11DepthStencilView* nullDsView{ nullptr };											//Null Variables
	ID3D11RenderTargetView* nullRtv{ nullptr };

	immediateContext->VSSetShader(vShader, nullptr, 0);										//Set Vshader
	immediateContext->PSSetShader(nullptr, nullptr, 0);										//Set Pshader
	ltHandler.bindDepthStencil(immediateContext, ptLightIndex);								//Set depthStencil for light
	for (int i = 0; i < entityArray.size(); i++)
	{
		if (entityArray.at(i) != nullptr)
		{
			entityArray.at(i)->draw(immediateContext, viewBuffer);								//Draw actors
		}
	}

	roomHandler.bindAndDrawTiles(immediateContext, viewBuffer);
	immediateContext->OMSetRenderTargets(1, &nullRtv, nullDsView);							//Unbind render targets & Depth Stencil
}

void AssembleImage(ID3D11DeviceContext* immediateContext, ID3D11ComputeShader* cShader, ID3D11UnorderedAccessView* backBufferUav, ID3D11ShaderResourceView** dfSrvArr, ID3D11Buffer* cameraPosBuffer, ID3D11SamplerState* samplerState, LightHandler ltHandler)
{
	//Unbind
	ID3D11RenderTargetView* nullRtv[8]{ nullptr , nullptr, nullptr, nullptr , nullptr, nullptr, nullptr };
	ID3D11ShaderResourceView* nullSrv[10]{ nullptr , nullptr, nullptr, nullptr , nullptr, nullptr, nullptr };
	ID3D11UnorderedAccessView* nullUav{ nullptr };
	immediateContext->OMSetRenderTargets(8, nullRtv, nullptr);

	//Bind & dispatch
	immediateContext->CSSetShader(cShader, nullptr, 0);
	ltHandler.bindLightBuffer(immediateContext, cameraPosBuffer, dfSrvArr);
	immediateContext->CSSetSamplers(0, 1, &samplerState);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &backBufferUav, nullptr);
	immediateContext->Dispatch(WIDTH / 32, HEIGHT / 18, 1);

	//Unbind
	immediateContext->CSSetUnorderedAccessViews(0, 1, &nullUav, nullptr);
	immediateContext->CSSetShaderResources(0, 10, nullSrv);
}

void GeometryShaderPass(ID3D11DeviceContext* immediateContext, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11GeometryShader* geoShader, ID3D11ComputeShader* particleUpdateShader, ID3D11InputLayout* inputLayout,
	ParticleEmitter& particleEmitter, ID3D11RenderTargetView*& rtv, ID3D11DepthStencilView* dsView, ID3D11Buffer* cameraMatrix, ID3D11Buffer* cameraPosBuffer, ID3D11Buffer* timeBuffer, bool isAura)
{
	//Variables
	vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(cameraMatrix);
	tempBuff.push_back(cameraPosBuffer);

	
	//if (isAura)
	//{
	//	Aura::Bind(immediateContext);
	//}

	//s�tt samma depth buffer fr�n geometripass.
	immediateContext->VSSetShader(vShader, nullptr, 0);								//SetVTXShader
	immediateContext->PSSetShader(pShader, nullptr, 0);								//Set PSShader
	immediateContext->GSSetShader(geoShader, nullptr, 0);							//SetGeoShader
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);	//Set how topology
	immediateContext->IASetInputLayout(inputLayout);								//Input layout = float3 position for each vertex
	immediateContext->GSSetConstantBuffers(0, 2, tempBuff.data());					//Set camera pos for ,Set matrix [world],[view]
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);							//SetRtv


	particleEmitter.BindAndDraw(immediateContext, timeBuffer, particleUpdateShader);//Draw Particles

	//if (isAura)
	//{
	//	Aura::UnBind(immediateContext);
	//}
	Aura::UnBind(immediateContext);

	particleEmitter.Unbind(immediateContext);										//Unbind
}

#pragma endregion
//--------------------------------------------------------------//Update Functions//-----------------------------------------------------------------------// 
#pragma region UpdateFunctions


void updateTimeBuffer(ID3D11DeviceContext* immediateContext, ID3D11Buffer* timeBuffer, float time)
{
	float timer2 = Time::CurrentTime();
	Aura::Bind(immediateContext);

	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &timer2, sizeof(float) * 4);
	immediateContext->Unmap(timeBuffer, 0);
}

bool checkRoomTransition( int & roomX, int& roomY, int& lastframeRoomX, int& lastframeRoomY, XMFLOAT3 midPoint, XMFLOAT3 playerPos, XMFLOAT2 roomDimensions, int & direction)
{	
	//variables
	bool hasTransitioned = false;

	//Check player location relative to room edge 
	if (playerPos.x > midPoint.x + (roomDimensions.x / 2))//R
	{
		roomX++;
		direction = 1;
	}
	if (playerPos.x < midPoint.x - (roomDimensions.x / 2))//L
	{
		roomX--;
		direction = 2;
	}
	if (playerPos.z > midPoint.z + (roomDimensions.y / 2))//U
	{
		roomY--;
		direction = 3;
	}
	if (playerPos.z < midPoint.z - (roomDimensions.y / 2))//D
	{
		roomY++;
		direction = 4;
	}

	//Check last frame value
	if (lastframeRoomX != roomX || lastframeRoomY != roomY)
	{
		hasTransitioned = true;
	}

	//Update roomIndex
	lastframeRoomX = roomX;
	lastframeRoomY = roomY;

	return hasTransitioned;
}

void doRoomTransition(ID3D11DeviceContext* immediateContext, ID3D11Device* device, int roomX, int roomY, XMFLOAT3 & midPoint, int direction, RoomHandler& roomHandler, Entity* player)
{
	roomHandler.setRoom(roomX, roomY, immediateContext, device);
	midPoint = roomHandler.getMidPoint();
	XMFLOAT2 roomDimensions = roomHandler.getRoomDimensions();
	XMFLOAT3 enterPosition(0, roomHandler.getTilePos(player->getTile()).y + 5.7f,0);
	float characterWidth = 3.0f;
	//
	
	if (direction == 1)//R
	{
		enterPosition = XMFLOAT3(midPoint.x - (roomDimensions.x / 2)+1 , enterPosition.y, midPoint.z);
	}
	if (direction == 2)//L
	{
		enterPosition = XMFLOAT3(midPoint.x + (roomDimensions.x / 2)-1, enterPosition.y, midPoint.z);
	}
	if (direction == 3)//U
	{
		enterPosition = XMFLOAT3(midPoint.x, enterPosition.y, midPoint.z - (roomDimensions.y / 2)+1);
	}
	if (direction == 4)//D
	{
		enterPosition = (XMFLOAT3(midPoint.x, enterPosition.y, midPoint.z + (roomDimensions.y / 2)-1));
	}
	if (direction == -1)
	{
		enterPosition = XMFLOAT3(midPoint.x, midPoint.y + 15, midPoint.z);
	}
	player->setPosition(enterPosition);
}

void spawnRoomActors(ID3D11Device* device, vector<Entity*>& entityArray, RoomHandler& roomHandler, EnemyHandler& enemyHandler, ActorHandler & actorHandler, PickupHandler& pickupHandler, 
	vector<Entity*>& gravity, vector<Pile*>& piles, vector<Pickup*>& pickups, vector<Plate*>& plates, int& wraiths)
{
//Variables
	vector<XMFLOAT4> spTile = roomHandler.getRoomSpecialTileInfo(); //format (Y pos/ Object type/ tileIndexPos/ Rot)

	for (int i = 0; i < spTile.size(); i++)//read and spawn actor type
	{
		//Temp Variables
		int invertedRL = 215 - spTile.at(i).z;
		float modulus = (int)(spTile.at(i).z) % 18;
		int realIndex = invertedRL + modulus;

 		XMFLOAT3 tilePos = roomHandler.getTilePos(spTile.at(i).z);
		float actorHeight = spTile.at(i).x;
		XMFLOAT3 actorPos = XMFLOAT3(tilePos.x, actorHeight, tilePos.z);
		XMFLOAT3 actorRot = XMFLOAT3(0, spTile.at(i).w, 0);



		/*
s 0/0/20/0
s 0/1/25/0
s 0/2/36/0
s 0/3/30/0
s 0/4/40/0
s 0/5/45/0
s 0/6/50/0
s 0/7/55/0
s 0/8/60/0
s 0/9/65/0
s 0/10/70/0
s 0/11/75/0
s 0/12/80/0
s 0/13/85/0

s 0/14/20/0
s 0/15/25/0
s 0/16/36/0
s 0/17/30/0
s 0/18/40/0
s 0/19/45/0
s 0/20/50/0
s 0/21/55/0
s 0/24/70/0
s 0/25/75/0
s 0/26/80/0
s 0/27/85/0

s 0/28/20/0
s 0/29/25/0
s 0/30/36/0
s 0/31/30/0
s 0/32/40/0
s 0/33/45/0
s 0/34/50/0
s 0/33/55/0
s 0/35/70/0
s 0/36/75/0
s 0/37/80/0
s 0/38/85/0
s 0/39/91/0
s 0/40/95/0
s 0/41/100/0
s 0/42/105/0

s 0/43/110/0
s 0/44/115/0


		*/

		//Push to EntityArray
		if (spTile.at(i).y == 0)		//Skull
		{
			entityArray.push_back(enemyHandler.getEnemy(ID::skull));
			gravity.push_back(entityArray.back());
		}
		else if (spTile.at(i).y == 1)	//Pile
		{
			entityArray.push_back(enemyHandler.getEnemy(ID::pile));
			gravity.push_back(entityArray.back());
			piles.push_back(dynamic_cast<Pile*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 2)	//Wraith
		{
			wraiths++;
			entityArray.push_back(enemyHandler.getEnemy(ID::wraith));
			gravity.push_back(entityArray.back());
		}
		else if (spTile.at(i).y == 3)	//Necromancer
		{
			entityArray.push_back(enemyHandler.getEnemy(ID::necromancer));
			gravity.push_back(entityArray.back());
		}
		else if (spTile.at(i).y == 4)	//Aura
		{
			entityArray.push_back(pickupHandler.spawnType(ID::aura, XMFLOAT3(0, 0, 0)));			//pickupHandler
			pickups.push_back(dynamic_cast<Pickup*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 5)	//damageboost
		{
			entityArray.push_back(pickupHandler.spawnType(ID::damageboost, XMFLOAT3(0, 0, 0)));
			pickups.push_back(dynamic_cast<Pickup*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 6)	//damagebuff
		{
			entityArray.push_back(pickupHandler.spawnType(ID::damagebuff, XMFLOAT3(0, 0, 0)));
			pickups.push_back(dynamic_cast<Pickup*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 7)	//healthbuff
		{
			entityArray.push_back(pickupHandler.spawnType(ID::healthbuff, XMFLOAT3(0, 0, 0)));
			pickups.push_back(dynamic_cast<Pickup*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 8)	//healthpotion
		{
			entityArray.push_back(pickupHandler.spawnType(ID::healthpotion, XMFLOAT3(0, 0, 0)));
			pickups.push_back(dynamic_cast<Pickup*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 9)	//shield
		{
			entityArray.push_back(pickupHandler.spawnType(ID::shield, XMFLOAT3(0, 0, 0)));
			pickups.push_back(dynamic_cast<Pickup*>(entityArray.back()));
		}
		else if (spTile.at(i).y == 10)	//puzzlebox
		{
			entityArray.push_back(actorHandler.getActor(ID::puzzlebox));
			gravity.push_back(entityArray.back());//actorHandler
		}
		else if (spTile.at(i).y == 11)	//tombstone
		{
			entityArray.push_back(actorHandler.getActor(ID::tombstone));
		}
		else if (spTile.at(i).y == 12)	//Plate
		{
			entityArray.push_back(actorHandler.getActor(ID::plate));
			plates.push_back(dynamic_cast<Plate*>(entityArray.back()));
		}
		else if (spTile.at(i).y >= 13)	//Door
		{
			entityArray.push_back(actorHandler.getActor(ID::door));
		}

		//set position & rotation
		entityArray.back()->setPosition(actorPos);
		entityArray.back()->setRotation(actorRot);
		entityArray.back()->setTile(actorPos.x, actorPos.z, roomHandler.getxDiv(), roomHandler.getTileWidth());
	}
}

void spawnDoors(RoomHandler& roomHandler, vector<Entity*> & entityArray, ActorHandler & actorHandler)
{
	//Variables
	XMFLOAT4 dorwayTileIndex = roomHandler.getDorwayTileIndex();
	XMFLOAT3 tilePos(0, 0, 0);
	XMFLOAT3 actorRot(0, 0, 0);

	if (dorwayTileIndex.x > 0)//rIGHT
	{
		
		tilePos = roomHandler.getTilePos(dorwayTileIndex.x + 18);
		actorRot = XMFLOAT3(0, 1.5, 0);
		entityArray.push_back(actorHandler.getActor(ID::door));
		entityArray.back()->setPosition(tilePos);
		entityArray.back()->setRotation(actorRot);

		entityArray.back()->setTile(tilePos.x, tilePos.z, roomHandler.getxDiv(), roomHandler.getTileWidth());
	}
	if (dorwayTileIndex.y > 0)//Left	
	{
		tilePos = roomHandler.getTilePos(dorwayTileIndex.y);
		actorRot = XMFLOAT3(0, 0.5, 0);
		entityArray.push_back(actorHandler.getActor(ID::door));
		entityArray.back()->setPosition(tilePos);
		entityArray.back()->setRotation(actorRot);
		float* hi = new float[6]{ 3, 5, 4.5, 0, -7, 2.25 };//LEFT?
		entityArray.back()->getMesh()->updateHitbox(hi);
		entityArray.back()->setTile(tilePos.x, tilePos.z, roomHandler.getxDiv(), roomHandler.getTileWidth());
	}
	if (dorwayTileIndex.z > 0)//DOWN
	{
		tilePos = roomHandler.getTilePos(dorwayTileIndex.z+1);
		actorRot = XMFLOAT3(0, 0, 0);
		entityArray.push_back(actorHandler.getActor(ID::door));
		entityArray.back()->setPosition(tilePos);
		entityArray.back()->setRotation(actorRot);
		float* hi = new float[6]{ 4.5, 5, 3, -3.25, -7, 0 };//UD
		entityArray.back()->getMesh()->updateHitbox(hi);
		entityArray.back()->setTile(tilePos.x, tilePos.z, roomHandler.getxDiv(), roomHandler.getTileWidth());
	}
	if (dorwayTileIndex.w > 0)//UP
	{
		tilePos = roomHandler.getTilePos(dorwayTileIndex.w);
		actorRot = XMFLOAT3(0, 1, 0);
		entityArray.push_back(actorHandler.getActor(ID::door));
		entityArray.back()->setPosition(tilePos);
		entityArray.back()->setRotation(actorRot);
		float* hi = new float[6]{ 4.5, 5, 3, -2.25, -7, 0 };//UD
		entityArray.back()->getMesh()->updateHitbox(hi);
		entityArray.back()->setTile(tilePos.x, tilePos.z, roomHandler.getxDiv(), roomHandler.getTileWidth());
	}

}

void clearRoomActors(vector<Entity*>& entityArray, EnemyHandler & enemyHandler, ActorHandler & actorHandler, 
	vector<Entity*>& gravity, vector<Pile*>& piles, vector<Pickup*>& pickups, vector<Plate*>& plates, int& wraiths)
{
	//Variables
	vector<int> indexToRemove;

	//clear 
	gravity.clear();
	piles.clear();
	pickups.clear();
	plates.clear();
	wraiths = 0;

	//Check if index is empty, remove content at index if certain ID
	for (int i = 0; i < entityArray.size(); i++)
	{
		if (entityArray.at(i) != nullptr)
		{
			//get ActorID
			ID actorID = entityArray.at(i)->getId();

			//enemies
			if (actorID == ID::pile || actorID == ID::skull || actorID == ID::wraith || actorID == ID::necromancer)
			{
				if (actorID == ID::pile)
				{
					Pile* temp = dynamic_cast<Pile*>(entityArray.at(i));
					temp->clearAssim();
				}
   		        enemyHandler.removeEnemy(actorID);
				entityArray.at(i) = nullptr;
				indexToRemove.push_back(i);
				
			}
			//Actors
			if (actorID == ID::puzzlebox || actorID == ID::tombstone || actorID == ID::plate || actorID == ID::door)
			{
				actorHandler.removeActor(actorID);
				entityArray.at(i) = nullptr;
				indexToRemove.push_back(i);
			}
			//Pickups
			if (actorID == ID::aura || actorID == ID::damageboost || actorID == ID::damagebuff || actorID == ID::healthbuff || actorID == ID::healthpotion || actorID == ID::shield)
			{
				entityArray.at(i) = nullptr;
				indexToRemove.push_back(i);
			}
			if (actorID == ID::unassigned)
			{
				entityArray.at(i) = nullptr;
				indexToRemove.push_back(i);
			}
		}
	}

	//remove indexes/Shrink array
	for (int i = 0; i < indexToRemove.size(); i++)
	{
		int toRemove = ((int)indexToRemove.size() - i); //testing123
		entityArray.erase(entityArray.begin() + toRemove);
	}
	enemyHandler.reviveEnemies();
}

void removewraith(vector<Entity*>& entityArray, EnemyHandler& enemyHandler, 
	vector<Entity*>& gravity, int& wraiths, int index)
{
	enemyHandler.removeWraith();
	entityArray.at(index) = nullptr;
	entityArray.erase(entityArray.begin() +index);

	gravity.clear();
	
	for (size_t i = 0; i < entityArray.size(); i++)
	{
		if (entityArray.at(i)->getId() == ID::skull|| entityArray.at(i)->getId() == ID::pile || entityArray.at(i)->getId() == ID::wraith || entityArray.at(i)->getId() == ID::necromancer || entityArray.at(i)->getId() == ID::puzzlebox )
		{
			gravity.push_back(entityArray[i]);
		}
	}

	wraiths--;
}

bool cullingCheck(int test, int ref, int xDiv)
{
	for (int i = -1; i < 2; i++)
	{
		for (int k = -1; k < 2; k++)
		{
			if (test == ref + i + (xDiv * k))
			{
				return true;
			}
		}
	}
	return false;
}

bool wraithCullCheck(Entity* wraith, Entity* player, int testT, int testY, int xDiv, int tileWidth)
{
	int diff = abs(wraith->getTile() % xDiv - player->getTile() % xDiv);
	bool temp = false;
	if (player->getTile() % xDiv > wraith->getTile() % xDiv)
	{
		if (wraith->getTile() % xDiv <= testT % xDiv && testT % xDiv <= player->getTile() % xDiv)
		{
			if (player->getPosition().z > wraith->getPosition().z)
			{
				if (wraith->getPosition().z <= testY && testY <= player->getPosition().z)
				{
					temp = true;
				}
			}
			else
			{
				if (wraith->getPosition().z >= testY && testY >= player->getPosition().z)
				{
					temp = true;
				}
			}
		}
	}
	else
	{
		if (wraith->getTile() % xDiv >= testT % xDiv && testT % xDiv >= player->getTile() % xDiv)
		{
			if (player->getPosition().z > wraith->getPosition().z)
			{
				if (wraith->getPosition().z <= testY && testY <= player->getPosition().z)
				{
					temp = true;
				}
			}
			else
			{
				if (wraith->getPosition().z >= testY && testY >= player->getPosition().z)
				{
					temp = true;
				}
			}
		}
	}
	return temp;
}

void assembleCuller(Entity**& target, int& size, int& used, Entity* mesh)
{
	if (mesh->getId() != ID::pile && mesh->getId() != ID::aura && mesh->getId() != ID::damageboost && mesh->getId() != ID::damagebuff &&
		mesh->getId() != ID::healthbuff && mesh->getId() != ID::healthpotion && mesh->getId() != ID::shield && mesh->getId() != ID::plate)
	{
		if (used == size)
		{
			size += 5;
			Entity** temp = new Entity * [size];
			for (int i = 0; i < used; i++)
			{
				temp[i] = target[i];
			}
			delete[]target;
			target = temp;
		}
		target[used++] = mesh;
	}
}

void assembleAtkCuller(Enemy**& target, int& size, int& used, Enemy* enemy)
{
	if (!enemy->isDead())
	{
		if (used == size)
		{
			size += 5;
			Enemy** temp = new Enemy * [size];
			for (int i = 0; i < used; i++)
			{
				temp[i] = target[i];
			}
			delete[]target;
			target = temp;
		}
		target[used++] = enemy;
	}
}

float testCollision(Entity**& culling, int used, XMFLOAT3* rays, int nr, XMFLOAT3 direction, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;
	for (int i = 1; i < used; i++)
	{
		if (culling[i] != ref && culling[i] != self)
		{
			float temp = culling[i]->getMesh()->collision(rays, nr, direction);
			if (temp >= 0 && temp < distance || distance < 0 && temp >= 0)
			{
				obj = i;
				distance = temp;
			}
		}
	}
	return distance;
}

float testGCollision(Entity**& culling, int used, XMFLOAT3* rays, int& obj, Entity* self = nullptr, Entity* ref = nullptr)
{
	float distance = -1;
	for (int i = 0; i < used; i++)
	{
		if (culling[i] != ref && culling[i] != self)
		{
			float temp = culling[i]->getMesh()->gravityCheck(rays);
			if (temp >= 0 && temp < distance || distance < 0 && temp >= 0)
			{
				obj = i;
				distance = temp;
			}
		}
	}
	
	return distance;
}

float hypotenusa(float x, float y)
{
	float h2 = pow(x, 2) + pow(y, 2);
	return sqrt(h2);
}

float compare(XMFLOAT3 p, XMFLOAT3 q)
{
	XMFLOAT3 offset = XMFLOAT3(p.x - q.x, p.y - q.y, p.z - q.z);
	float hyp2 = pow(offset.x, 2) + pow(offset.y, 2) + pow(offset.z, 2);
	return sqrt(hyp2);
}

void handleBox(Entity* box, Entity** cull, int used, RoomHandler* roomHandler, int*& info, float* pos, float rot)
{
	int nrr;
	int close = -1;
	XMFLOAT3 dir;
	XMFLOAT3* boxRays = box->getMesh()->getRays(nrr, dir, rot);
	for (int i = 0; i < 2; i++)
	{
		pos[i] *= 0.5;
	}
	
	float dis = testCollision(cull, used, boxRays, nrr, dir, close, box);
	if (dis > hypotenusa(pos[0], pos[1]) || dis == -1)
	{
		box->setMove(true);
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			pos[i] *= 0;
		}
	}
	delete[]boxRays;
}

void restartMaze(ID3D11Device* device, int& roomX,int& roomY, int& lastframeRoomX, int& lastframeRoomY, RoomHandler& roomHandler, Player* playerCharacter)
{
	roomHandler.randomizeWorldLayout(device);//
	roomX = roomHandler.getStartingRoom().y;
	roomY = roomHandler.getStartingRoom().x;
	lastframeRoomX = roomHandler.getStartingRoom().y;
	lastframeRoomY = roomHandler.getStartingRoom().x;
	XMFLOAT3 midPoint = roomHandler.getMidPoint();
	playerCharacter->setPosition(XMFLOAT3(midPoint.x, midPoint.y + 10, midPoint.z));
	playerCharacter->resetplayer();
	playerCharacter->isNecroManDead = false;
	Time::Reset();
}
#pragma endregion
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################//	  MAIN    //#######################################################################// 
//----------------------------------------------------------------------------------------------------------------------------------------------------------//


int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	/*printLspInfo("../LspMeshes/WraithFinal");
	return 0;*/
	//--------------------------------------------------------------//variables//-----------------------------------------------------------------------//
#pragma region RenderVariables

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//XXX MENU XXX

	DXApp* menus = new DXApp(hInstance);
	if (!menus->Run())
	{

		delete menus;

		return 0;
	}
	delete menus;

	/*CloseWindow(nullptr);*/

		//XXX   x   XXX

	//LspScene myScene("../LspMeshes/test2");

	Audio::Init();

	float timeLastFrame = 0;

	srand((unsigned int)time(0));
	//variables for window class
	HWND window;								//window handle
	float timer = 0.0f;							//counter for main loop (used in setUpipeline to determine angle of quad)
	MSG msg = {};								//Message used in main loop to check window status.
	bool end = false;
	Time::SetFPS(30);		//set fps of the game
	int fps = 0;
	float currentTime = 0.0f;
	int actualFps = 0;
	bool open = false;
	bool enemiesInRoom = false;

	//initialize empty refrences for interface
	ID3D11Device* device;						//Ptr to graphics hardware
	ID3D11DeviceContext* immediateContext;		//will immidiently execute a function called on the hardware
	IDXGISwapChain* swapChain;					//handles swapping between renderbuffers and presenting frames
	ID3D11Texture2D* depthTexture;				//Texture containing the depth data for scene
	ID3D11DepthStencilView* depthView;			//Depth View
	D3D11_VIEWPORT viewport;					//Viewport
	ID3D11SamplerState* samplerState;			//Sampler
	ID3D11SamplerState* shadowSampler;			//Shadow Sampler
	ID3D11InputLayout* inputLayout;				//normal Inputlayout for the Vertex buffer
	ID3D11InputLayout* inputLayoutGeo;			//Inputlayout for Geometry shader pass
	ID3D11VertexShader* vertexShader;			//Vertex shader
	ID3D11VertexShader* VtxShaderGeo;			//Vertex shader for Geometry shader pass
	ID3D11GeometryShader* geoShader;			//Geometry shader
	ID3D11PixelShader* pixelShader;				//Pixel shader
	ID3D11PixelShader* PXShaderGeo;				//Special pixel shader for combining particles with final image
	ID3D11ComputeShader* computeShader;			//Compute shader
	ID3D11ComputeShader* cubeMapShader;			//Special version of regular Compute shader takes RWTexture2DArray instead of RWTexture2D
	ID3D11Buffer* CameraMTXBuffer;				//Stage Camera Matrix buffer
	ID3D11Buffer* CameraPosBuffer;				//Stage Camera POSITION buffer
	ID3D11Buffer* PtListBuffer;					//Points for particles
	ID3D11Buffer* PtworldMatrix;				//
	ID3D11Buffer* timeBuffer;					//Holds elspsed Time [unimplimented]
	ID3D11RenderTargetView* rtv;				//
	ID3D11Texture2D* dfTexArr[8];				//df rendering Main camera variables
	ID3D11ShaderResourceView* dfSrvArr[8];		
	ID3D11RenderTargetView* dfRtvArr[8];
	ID3D11UnorderedAccessView* backBufferUav;
	ID3D11UnorderedAccessView* particleUav;		//UAV for Particle buffer
	ID3D11Texture2D* cTexArr[8];				//For cubemap rendering
	ID3D11ShaderResourceView* cSrvArr[8];
	ID3D11RenderTargetView* cRtvArr[8];
	ID3D11ComputeShader* PT_UpdateShader_Rising;
	ID3D11PixelShader* PT_PixelShader2;

	ID3D11ComputeShader* particleUpdateShader;	//Compute shader for updating particles [unused]


#pragma endregion
	//---------------------------------------------------------------//Setup Functions//-----------------------------------------------------//
#pragma region SetupFunctions
	//Setup Window
	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "failed to setup window: " << std::endl;
		return -1;
	}

	//Setup direct x 11
	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, depthTexture, depthView, viewport, backBufferUav, dfTexArr, dfSrvArr, dfRtvArr, rtv, cTexArr, cSrvArr, cRtvArr))
	{
		std::cerr << "failed to setup D3D11: " << std::endl;
		return -1;
	}
	
	//Setup pipeline
	if (!SetupPipeline(device, vertexShader, VtxShaderGeo, pixelShader, PXShaderGeo, PT_PixelShader2, cubeMapShader, geoShader, particleUpdateShader, PT_UpdateShader_Rising, inputLayout, inputLayoutGeo, CameraMTXBuffer, PtListBuffer, PtworldMatrix, samplerState,
		shadowSampler, CameraPosBuffer, computeShader, particleUav, timeBuffer))
	{
		std::cerr << "failed to setup pipeline: " << std::endl;
		return -1;
	}
#pragma endregion
	//---------------------------------------------------------------//Setup Actors//-----------------------------------------------------//
#pragma region SetupCube/Room/Lights/Particle/Camera


	//Room handler
	RoomHandler roomHandler(device);
	int roomX = (int)roomHandler.getStartingRoom().y;
	int roomY = (int)roomHandler.getStartingRoom().x;
	int lastframeRoomX = (int)roomHandler.getStartingRoom().y;
	int lastframeRoomY = (int)roomHandler.getStartingRoom().x;
	int tileWidth = roomHandler.getTileWidth();




	//Check midpoint & dimensions
	XMFLOAT3 midPoint = roomHandler.getMidPoint();

	//lights
	LightHandler ltHandler(WIDTH,HEIGHT);	 //Pos, Col, LookAt, Up//
	ltHandler.addPointlight(device, XMFLOAT4(midPoint.x - 30, midPoint.y + 70, midPoint.z - 35, 0), XMFLOAT4(1, 1, 1, 0), XMFLOAT4(midPoint.x, midPoint.y, midPoint.z, 0), XMFLOAT4(0, 0, 1, 0));
	ltHandler.finalizeLights(device, immediateContext);

	//Cubemaps
	CubeMapHandler reflectCube(device, immediateContext, XMFLOAT4(midPoint.x, midPoint.y, midPoint.z, 0));


	//Cameras
	Camera stageCam(device, XMFLOAT3(0, 0, -19), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1, 0), WIDTH, HEIGHT);


#pragma endregion


#pragma region Entity/Mesh

	//Entity
	vector<Entity*> entityArray;
	LspMesh bellard;
	bellard.sceneFileName = "../LspMeshes/BellardWithBone";
	LoadMesh(bellard);
	Player* bellamy = new Player(device, bellard, XMFLOAT3(midPoint.x, midPoint.y + (1.5f), midPoint.z), XMFLOAT3(0, 0, 0), 4);
	Aura::Init(device);
	Aura::SetPlayer(bellamy);
	entityArray.emplace_back(bellamy);

	//!!setup sprites and text
	GuiHandler::Init(device, immediateContext, shadowSampler, WIDTH, HEIGHT, bellamy, roomHandler);

	//Vectors for collision checks...
	vector<Pile*> piles;
	vector<Pickup*> pickups;
	vector<Entity*> gravityObj;
	vector<Plate*> plates;
	int nrWraiths = 0;

	//Actors //spawns on entering room
	ActorHandler actorHandler(device);

	//Enemies
	EnemyHandler enemyHandler(device);


	//Pickups
	PickupHandler pickupHandler(device);

	//Set starting room
	doRoomTransition(immediateContext, device, roomX, roomY, midPoint, -1, roomHandler, bellamy);
	spawnRoomActors(device, entityArray, roomHandler, enemyHandler, actorHandler, pickupHandler, gravityObj, piles, pickups, plates, nrWraiths);
	for (int i = 0; i < roomHandler.getNrOfStaticActors(); i++)
	{
		entityArray.push_back(roomHandler.getStaticActors(i));
		entityArray.back()->setTile(entityArray.back()->getPosition().x, entityArray.back()->getPosition().z, roomHandler.getxDiv(), tileWidth);
	}
	//Particles
	ParticleEmitter particleEmitter(device, XMFLOAT3(midPoint.x-140, midPoint.y, midPoint.z), 26, 5);
	//particleEmitter.setPosition(XMFLOAT3(midPoint.x, midPoint.y, midPoint.z));
	//particleEmitter.updateBuffer(immediateContext);

	//start time before game loop
	Time::Start();
	Audio background(L"../Sounds/background.wav");
	background.play(true);
	background.setVolume(0.03f);
	TextureCollection::Load(device, "Damage.png");
#pragma endregion

	//#######################################################// GAME LOOP //#######################################################//
	
	while (msg.message != WM_QUIT && !end)
	{

#pragma region MsgHandling/Timer
		bool necroIsDead = false;
		for (size_t i = 0; i < entityArray.size(); i++)
		{
			if (entityArray.at(i)->getId() == ID::necromancer)
			{
				Necromancer* temp = dynamic_cast<Necromancer*>(entityArray.at(i));
				if (temp != NULL)
				{
					if (temp->isDead())
					{
						necroIsDead = true;
					}
				}
			}
		}
		Audio::Update();

		steady_clock::time_point frameStart = steady_clock::now();	//get time Point
		const auto FrameTimeStart = std::chrono::steady_clock::now();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (GetKeyState(27) & 0x8000)
		{
			end = true;
		}

		//Timer
		if (Time::CurrentTime() - currentTime >= 1.0f)
		{
			actualFps = fps;
			fps = 0;
			currentTime = Time::CurrentTime();
		}
		//tempor�r necromancer health check




		SetWindowTextW(window, to_wstring(actualFps).c_str());
#pragma endregion
		//------------------------------------------------------//Debug//--------------------------------------------------------------//
#pragma region Debug
		//Writing in the debug output
		//char tmp[100];
		//sprintf_s(tmp, "Bellamy's health is %d\n", bellamy->getHP());
		//OutputDebugStringA(tmp);
#pragma endregion		
		//------------------------------------------------------//Render/present//-----------------------------------------------------//
#pragma region Render/present
		//Render geometry
		Render(immediateContext, depthView, viewport, vertexShader, pixelShader, inputLayout, stageCam.getCameraMTXBuffer(),
			samplerState, reflectCube.getPosBuffer(), backBufferUav, computeShader, dfTexArr, dfSrvArr, dfRtvArr, entityArray, roomHandler, ltHandler, reflectCube);
		for (size_t i = 0; i < gravityObj.size(); i++)
		{
			gravityObj.at(i)->getMesh()->textureKey = gravityObj.at(i)->getMesh()->orgTextureKey;
		}
		bellamy->getMesh()->textureKey = bellamy->getMesh()->orgTextureKey;
		//static bool limitRender = false;
		//if (limitRender)
		//{
			//Render Shadows
		for (int i = 0; i < ltHandler.getNrOfLights(); i++)
		{
			RenderDepthFromLight(immediateContext, vertexShader, ltHandler.getLightViewBuffer(i), entityArray, roomHandler, ltHandler, i);
		}
		//}
		//limitRender = !limitRender;

		//Aassembly of final picture
		AssembleImage(immediateContext, computeShader, backBufferUav, dfSrvArr, stageCam.getCameraPosBuffer(), shadowSampler, ltHandler);

		///particleUpdateShader;	//Compute shader for updating particles [unused]
		//GeometryShaderPass/draw Particles
		//GeometryShaderPass(immediateContext, VtxShaderGeo, PXShaderGeo/*PT_PixelShader2*/, geoShader, particleUpdateShader/*PT_UpdateShader_Rising*/, inputLayoutGeo, particleEmitter, rtv, depthView, stageCam.getCameraMTXBuffer(), stageCam.getCameraPosBuffer(), timeBuffer, false);
		GeometryShaderPass(immediateContext, VtxShaderGeo, /*PXShaderGeo*/PT_PixelShader2, geoShader, particleUpdateShader/*PT_UpdateShader_Rising*/, inputLayoutGeo, particleEmitter, rtv, depthView, stageCam.getCameraMTXBuffer(), stageCam.getCameraPosBuffer(), timeBuffer, false);

		//sprite and text pass NEEDS TO RENDER LAST
		GuiHandler::Update();

		//Present
		swapChain->Present(0, 0);
#pragma endregion
		//------------------------------------------------------//Updates//------------------------------------------------------------//

#pragma region RoomTransition

		if (!necroIsDead)
		{

			//Variables
			XMFLOAT3 playerPos = bellamy->getPosition();
			XMFLOAT2 roomDimensions = roomHandler.getRoomDimensions();
			int enterDir = 0;

			//Handle room transitions
			if (checkRoomTransition(roomX, roomY, lastframeRoomX, lastframeRoomY, midPoint, playerPos, roomDimensions, enterDir))
			{
				//reset puzzle bool
				open = false;

				//Do room transition
				doRoomTransition(immediateContext, device, roomX, roomY, midPoint, enterDir, roomHandler, bellamy);

				//clear out room actors
				clearRoomActors(entityArray, enemyHandler, actorHandler, gravityObj, piles, pickups, plates, nrWraiths);

				//Spawn/Set Actors//skicka in en enemy handler & actor handler?
				spawnRoomActors(device, entityArray, roomHandler, enemyHandler, actorHandler, pickupHandler, gravityObj, piles, pickups, plates, nrWraiths);

				//Spawn doors for all dorways
				spawnDoors(roomHandler, entityArray, actorHandler);

				//Get ptr for staticActors (if index of actor to get over 13)
				for (int i = 0; i < roomHandler.getNrOfStaticActors(); i++)
				{
					entityArray.push_back(roomHandler.getStaticActors(i));
					entityArray.back()->setTile(entityArray.back()->getPosition().x, entityArray.back()->getPosition().z, roomHandler.getxDiv(), tileWidth);
				}
			}

#pragma endregion

#pragma region Collision

			//---------------------------------------------------------------------------------------------------------------------------------------------//
			//=================================================================Collision and movement======================================================//
			//---------------------------------------------------------------------------------------------------------------------------------------------//

			//=======================================================================Variables=============================================================//

			for (size_t i = 0; i < gravityObj.size(); i++)
			{
				if (gravityObj.at(i)->getId() == ID::necromancer)
				{

					Necromancer* temp = dynamic_cast<Necromancer*>(gravityObj.at(i));

					if (temp->Spawning() == true)
					{
						ID gotenID = temp->GetenemyID();



						if (enemyHandler.isEnemyFree(gotenID))
						{
							Enemy* spawnedEnemy = enemyHandler.getEnemy(gotenID);
							if (gotenID == ID::pile)
							{
								Pile* Tpile = dynamic_cast<Pile*>(spawnedEnemy);
								piles.push_back(Tpile);
							}
							if (gotenID == ID::wraith)
							{
								nrWraiths++;
							}
							entityArray.push_back(spawnedEnemy);
							XMFLOAT3 temppos{ temp->getPosition().x,  temp->getPosition().y + 2,  temp->getPosition().z };
							entityArray.back()->setPosition(temppos);

							gravityObj.push_back(entityArray.back());
						}

						temp->hasspawned();
					}
				}
			}
			for (size_t i = 0; i < entityArray.size(); i++)
			{
				if (entityArray.at(i)->getId() == ID::wraith)
				{
					Wraith* temp = dynamic_cast<Wraith*>(entityArray.at(i));
					if (temp != nullptr)
					{
						if (temp->getHealth() <= 0)
						{

							removewraith(entityArray, enemyHandler, gravityObj, nrWraiths, i);
							temp->setHpToMax();
							temp->SetDead(false);
						}
					}
				}
			}

			Entity*** gravityCull = new Entity * *[gravityObj.size()];
			float pos[2] = { 0, 0 };
			bellamy->move(stageCam, pos[0], pos[1]);
			float gravity = 0.9;
			int used = 0;
			int size = 10;
			int* info = roomHandler.getRoomInfo();
			int closestObj = -1;
			int objUnder = -1;
			int atkSize = 5;
			int atkUsed = 0;
			int nr;
			int w = 0;
			float smallestDistance = -1;
			float relativeHeight = -1;


			Entity** culling = new Entity * [size];
			Enemy** atkCull = new Enemy * [atkSize];
			Entity*** plateCull = new Entity * *[plates.size()];
			//Entity*** gravityCull = new Entity * *[gravityObj.size()];
			Entity*** wraithCull = new Entity * *[nrWraiths];
			int* pUsed = new int[plates.size()];
			int* pSize = new int[plates.size()];
			int* gUsed = new int[gravityObj.size()];
			int* gSize = new int[gravityObj.size()];
			int* wUsed = new int[nrWraiths];
			int* wSize = new int[nrWraiths];

			XMFLOAT3 direction;
			XMFLOAT3* rays = bellamy->getMesh()->getRays(nr, direction, bellamy->getRotation().y);
			XMFLOAT3* gRays = bellamy->getMesh()->getGravityRays();


			//spawning enemy through necromancer

			//===================================================================Assemble Cullers==========================================================//

			for (int i = 0; i < entityArray.size(); i++)
			{
				if (cullingCheck(entityArray.at(i)->getTile(), bellamy->getTile(), roomHandler.getxDiv()) ||
					entityArray[i]->getId() == ID::skull || entityArray[i]->getId() == ID::door)
				{
					assembleCuller(culling, size, used, entityArray.at(i));
				}
				if (entityArray.at(i)->getId() == ID::skull || entityArray.at(i)->getId() == ID::pile || entityArray.at(i)->getId() == ID::wraith || entityArray.at(i)->getId() == ID::necromancer)
				{
					assembleAtkCuller(atkCull, atkSize, atkUsed, dynamic_cast<Enemy*>(entityArray.at(i)));
				}
				for (int k = 0; k < plates.size(); k++)
				{
					if (i == 0)
					{
						pUsed[k] = 0;
						pSize[k] = 5;
						plateCull[k] = new Entity * [pSize[k]];
					}
					if (entityArray[i]->getId() == ID::puzzlebox || entityArray[i]->getId() == ID::player)
					{
						assembleCuller(plateCull[k], pSize[k], pUsed[k], entityArray[i]);
					}
				}
				for (int k = 0; k < gravityObj.size(); k++)
				{
					if (i == 0)
					{
						gUsed[k] = 0;
						gSize[k] = 5;
						gravityCull[k] = new Entity * [gSize[k]];
						if (gravityObj[k]->getId() == ID::wraith)
						{
							wUsed[w] = 0;
							wSize[w] = 5;
							wraithCull[w] = new Entity * [wSize[w]];
							assembleCuller(wraithCull[w], wSize[w], wUsed[w], bellamy);
						}
					}
					if (cullingCheck(entityArray.at(i)->getTile(), gravityObj[k]->getTile(), roomHandler.getxDiv()))
					{
						assembleCuller(gravityCull[k], gSize[k], gUsed[k], entityArray[i]);
					}
					if (gravityObj[k]->getId() == ID::skull && i == 0 || gravityObj[k]->getId() == ID::necromancer && i == 0)
					{
						assembleCuller(gravityCull[k], gSize[k], gUsed[k], bellamy);
					}
					if (gravityObj[k]->getId() == ID::puzzlebox && i == 0)
					{
						gravityObj[k]->setIndex(k);
					}
					if (gravityObj[k]->getId() == ID::wraith)
					{
						if (wraithCullCheck(gravityObj[k], bellamy, entityArray[i]->getTile(), entityArray[i]->getPosition().z, roomHandler.getxDiv(), roomHandler.getTileWidth()))
						{
							assembleCuller(wraithCull[w], wSize[w], wUsed[w], entityArray[i]);
						}
						w++;
						if (w == nrWraiths)
						{
							w = 0;
						}
					}
				}
			}
			for (int x = 0; x < info[2]; x++)
			{
				for (int z = 0; z < info[3]; z++)
				{
					if (cullingCheck((z * info[0] + x), bellamy->getTile(), roomHandler.getxDiv()))
					{
						assembleCuller(culling, size, used, roomHandler.getTile(z * info[0] + x));
					}
					for (int k = 0; k < gravityObj.size(); k++)
					{
						if (cullingCheck((z * info[0] + x), gravityObj[k]->getTile(), roomHandler.getxDiv()))
						{
							assembleCuller(gravityCull[k], gSize[k], gUsed[k], roomHandler.getTile(z * info[0] + x));
						}
						if (gravityObj[k]->getId() == ID::wraith)
						{
							if (wraithCullCheck(gravityObj[k], bellamy, (z * info[0] + x), roomHandler.getTile(z * info[0] + x)->getPosition().z, roomHandler.getxDiv(), roomHandler.getTileWidth()))
							{
								assembleCuller(wraithCull[w], wSize[w], wUsed[w], roomHandler.getTile(z * info[0] + x));
							}
							w++;
							if (w == nrWraiths)
							{
								w = 0;
							}
						}
					}
				}
			}
			//========================================================================player input=========================================================//
			if (Input::GetKeyDown(Input::Key::R))
			{
				bellamy->releaseBox();
			}
			if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				if (bellamy->canAttack())
				{
					bellamy->atkCountdown();
					bellamy->playPunchaudio();
					bellamy->resetAttackTime();
					char tmp[100];
					sprintf_s(tmp, "attacking..\n");
					OutputDebugStringA(tmp);
					int obj = -1;
					float atkDistance = -1;
					XMFLOAT3* directions = nullptr;
					for (int i = 0; i < atkUsed; i++)
					{
						float temp = atkCull[i]->hitDetection(bellamy, directions);
						if (temp < atkDistance && temp >= 0 || temp >= 0 && atkDistance == -1)
						{
							obj = i;
							atkDistance = temp;
						}
					}
					if (atkDistance < 2 && atkDistance != -1)
					{

						atkCull[obj]->takeDmg(bellamy->getDmg());
						atkCull[obj]->PlaydamageAudio();
						atkCull[obj]->getMesh()->textureKey = "Damage.png";
						sprintf_s(tmp, "hit %i!\n", atkCull[obj]->getId());
						OutputDebugStringA(tmp);
					}
				}
			}
			if (!bellamy->canAttack())
			{
				bellamy->atkCountdown();
			}



			//======================================================================player collision=======================================================//
			//if (/*smallestDistance == -1 && */bellamy->getRotation().y == 1.5)
			//{
			//	float hi = 0.5;
			//}
			smallestDistance = testCollision(culling, used, rays, nr, direction, closestObj, bellamy, bellamy->getBox());
			relativeHeight = testGCollision(culling, used, gRays, objUnder, bellamy, bellamy->getBox());

			for (int i = 0; i < pickups.size(); i++)
			{
				if (pickups[i]->playerOverlap(bellamy))
				{
					if (bellamy->addPickup(pickups[i]))
					{
						pickups[i]->playAudio();
						pickups[i]->setPosition(XMFLOAT3(-10, 0, -10));
						pickupHandler.removePickup(pickups.at(i)->getId());
						pickups.erase(pickups.begin() + i);
					}
				}
			}
			if (smallestDistance == -1)
			{
				//free to move

				if (bellamy->getBox() != nullptr)
				{
					handleBox(bellamy->getBox(), gravityCull[bellamy->getBox()->getIndex()], gUsed[bellamy->getBox()->getIndex()], &roomHandler, info, pos, bellamy->getRotation().y);
				}
			}
			else if (smallestDistance < 0.25 && culling[closestObj]->getId() == ID::puzzlebox)
			{
				//if close to a crate

				if (Input::GetKeyDown(Input::Key::E))
				{
					bellamy->setBox(culling[closestObj]);
				}
				if (bellamy->getBox() != nullptr)
				{
					handleBox(bellamy->getBox(), gravityCull[bellamy->getBox()->getIndex()], gUsed[bellamy->getBox()->getIndex()], &roomHandler, info, pos, bellamy->getRotation().y);
				}
				if (bellamy->getBox() != culling[closestObj])
				{
					handleBox(culling[closestObj], gravityCull[culling[closestObj]->getIndex()], gUsed[culling[closestObj]->getIndex()], &roomHandler, info, pos, bellamy->getRotation().y);
				}
			}
			else
			{
				//if close to an unidentified object

				if (bellamy->getBox() != nullptr)
				{
					handleBox(bellamy->getBox(), gravityCull[bellamy->getBox()->getIndex()], gUsed[bellamy->getBox()->getIndex()], &roomHandler, info, pos, bellamy->getRotation().y);
				}
				smallestDistance -= 0.05;
				if (smallestDistance <= 0)
				{
					smallestDistance = 0;
					pos[0] = 0;
					pos[1] = 0;
				}
				else
				{
					while (hypotenusa(abs(pos[0]), abs(pos[1])) > (smallestDistance * 0.98) && smallestDistance != -1)
					{
						pos[0] *= 0.9;
						pos[1] *= 0.9;
					}
				}

			}
			if (relativeHeight == -1 || relativeHeight >= gravity)
			{
				//if free to fall without obstruction

				bellamy->disableJump();
				if (relativeHeight != -1)
				{
					if (culling[objUnder]->getId() == ID::skull && relativeHeight <= 1.5 || culling[objUnder]->getId() == ID::necromancer && relativeHeight <= 1.5)
					{
						Enemy* temp = dynamic_cast<Enemy*>(culling[objUnder]);
						temp->stopJump();
					}
				}
			}
			else if (relativeHeight < gravity)
			{
				//if player will fall on something

				while (gravity > relativeHeight)
				{
					gravity -= 0.005;
				}
				if (relativeHeight < 0.01)
				{
					bellamy->resetJump();
				}
				if (culling[objUnder]->getId() == ID::skull || culling[objUnder]->getId() == ID::necromancer)
				{
					Enemy* temp = dynamic_cast<Enemy*>(culling[objUnder]);
					temp->stopJump();
				}
			}
			for (int i = 0; i < entityArray.size(); i++)
			{
				if (entityArray.at(i)->getMove())
				{
					entityArray.at(i)->increasePosition(pos[0], 0, pos[1]);
					entityArray.at(i)->updateMesh(immediateContext);
					entityArray.at(i)->setTile(entityArray.at(i)->getPosition().x, entityArray.at(i)->getPosition().z, roomHandler.getxDiv(), tileWidth);
					entityArray.at(i)->setMove(false);
				}
			}
			bellamy->invulCountdown();

			//=========================================================================actor collision & behaviour=====================================================//


			for (size_t i = 0; i < pickups.size(); i++)
			{
				Pickup* temp = dynamic_cast<Pickup*>(pickups.at(i));
				if (temp != NULL)
				{
					temp->rotateItem();
				}
			}
			//tesing some pile to pile action
			for (size_t i = 0; i < piles.size(); i++)
			{
				for (size_t t = 0; t < piles.size(); t++)
				{
					if (i != t)
					{
						if (piles.at(i)->overlap(piles.at(t)) && !piles.at(t)->getAbsorbed() && !piles.at(i)->getAbsorbed() && piles.at(t)->isDead())
						{
							piles.at(t)->setAbsorbed(true);
							piles.at(i)->absorb(piles.at(t));
						}
					}
				}
			}

			for (int i = 0; i < gravityObj.size(); i++)
			{
				if (gravityObj[i]->getId() != ID::puzzlebox && gravityObj[i]->getId() != ID::wraith)
				{
					gravityObj[i]->move(bellamy, gravityCull[i], gUsed[i]);

					if (gravityObj.at(i)->getId() == ID::necromancer)
					{
						Necromancer* temp = dynamic_cast<Necromancer*>(gravityObj[i]);

						temp->getProjectileHandeler()->updateProjectileMesh(immediateContext);


					}
				}
				else if (gravityObj[i]->getId() == ID::wraith)
				{
					Wraith* temp = dynamic_cast<Wraith*>(gravityObj[i]);
					temp->move(bellamy, culling, used, wraithCull[w], wUsed[w]);

					temp->getProjectileHandeler()->updateProjectileMesh(immediateContext);
					w++;
					if (w == nrWraiths)
					{
						w = 0;
					}
				}
				XMFLOAT3* gRay = gravityObj[i]->getMesh()->getGravityRays();
				int obj = -1;
				float gravityTemp = 0.9;
				float distance;
				if (gravityObj[i]->getId() == ID::wraith && gravityObj[i]->getTeleported())
				{
					distance = testGCollision(culling, used, gRay, obj, bellamy);
				}
				else
				{
					distance = testGCollision(gravityCull[i], gUsed[i], gRay, obj, bellamy);
				}
				if (distance == -1 || distance >= gravityTemp)
				{
					gravityObj[i]->increasePosition(0, -gravityTemp, 0);
					if (gravityObj[i]->getId() == ID::skull)
					{
						Skull* temp = dynamic_cast<Skull*>(gravityObj[i]);
						temp->setJump(false);
					}
					else if (gravityObj[i]->getId() == ID::pile)
					{
						Pile* temp = dynamic_cast<Pile*>(gravityObj[i]);
						temp->setJump(false);
					}
					else if (gravityObj[i]->getId() == ID::necromancer)
					{
						Necromancer* temp = dynamic_cast<Necromancer*>(gravityObj[i]);
						temp->setJump(false);
					}
				}
				else if (distance < gravityTemp)
				{
					while (gravityTemp > distance)
					{
						gravityTemp -= 0.005;
					}
					gravityObj[i]->increasePosition(0, -gravityTemp, 0);
					if (gravityObj[i]->getId() == ID::skull)
					{
						Skull* temp = dynamic_cast<Skull*>(gravityObj[i]);
						temp->setJump(true);
					}
					else if (gravityObj[i]->getId() == ID::pile)
					{
						Pile* temp = dynamic_cast<Pile*>(gravityObj[i]);
						temp->setJump(true);
					}
					else if (gravityObj[i]->getId() == ID::necromancer)
					{
						Necromancer* temp = dynamic_cast<Necromancer*>(gravityObj[i]);
						temp->setJump(true);
					}
				}
				gravityObj.at(i)->updateMesh(immediateContext);
				gravityObj[i]->setTile(gravityObj[i]->getPosition().x, gravityObj[i]->getPosition().z, roomHandler.getxDiv(), roomHandler.getTileWidth());
				delete[]gRay;
			}


			open = true;
			for (int i = 0; i < plates.size(); i++)
			{
				plates[i]->overlapCheck(plateCull[i], pUsed[i]);
				if (!plates[i]->getOverlap())
				{
					open = false;
				}
			}

			//==============================================================================clean up=======================================================//

			bellamy->handleAura(gravityObj);

			//Reset character pos if under stage
			if (bellamy->getPosition().y < -30)
			{
				bellamy->setPosition(XMFLOAT3(bellamy->getPosition().x, 20, bellamy->getPosition().z));
			}

			//update player position
			bellamy->increasePosition(pos[0], -gravity, pos[1]);
			bellamy->updateJump();
			if (bellamy->getBox() != nullptr)
			{
				if (compare(bellamy->getPosition(), bellamy->getBox()->getPosition()) > 5)
				{
					bellamy->releaseBox();
				}
			}
			bellamy->setTile(bellamy->getMesh()->getPos().x, bellamy->getMesh()->getPos().z, roomHandler.getxDiv(), tileWidth);
			bellamy->update(immediateContext);

			delete[]rays;
			delete[]gRays;
			delete[]info;
			delete[]culling;
			delete[]atkCull;
			delete[]gUsed;
			delete[]gSize;
			delete[]pSize;
			delete[]pUsed;
			delete[]wUsed;
			delete[]wSize;
			for (int i = 0; i < gravityObj.size(); i++)
			{
				delete[]gravityCull[i];
			}
			for (int i = 0; i < plates.size(); i++)
			{
				delete[]plateCull[i];
			}
			for (int i = 0; i < nrWraiths; i++)
			{
				delete[]wraithCull[i];
			}
			delete[]gravityCull;
			delete[]plateCull;
			delete[]wraithCull;
#pragma endregion

#pragma region EnemyDebug
			bool anyEnemies = false;
			int enemyCounter = 0;
			for (int i = 0; i < entityArray.size(); i++)
			{
				Enemy* tempEnemy = dynamic_cast<Enemy*>(entityArray[i]);
				Skull* tempSkull = dynamic_cast<Skull*>(entityArray[i]);
				Pile* tempPile = dynamic_cast<Pile*>(entityArray[i]);
				Wraith* tempWraith = dynamic_cast<Wraith*>(entityArray[i]);
				if (tempEnemy != nullptr)
				{
					if (!tempEnemy->isDead())
					{
						enemyCounter++;
						anyEnemies = true;
					}
				}
			}
			if (enemyCounter > 0)
			{
				enemiesInRoom = true;
			}
			if (enemyCounter == 0)
			{
				enemiesInRoom = false;
			}


			//Update entityArray objects
			for (int i = 0; i < entityArray.size(); i++)
			{
				if (entityArray.at(i) != nullptr)
				{
					entityArray.at(i)->update(immediateContext);

					if (entityArray.at(i)->getPosition().y < -30)
					{
						entityArray.at(i)->setPosition(XMFLOAT3(entityArray.at(i)->getPosition().x, 20, entityArray.at(i)->getPosition().z));
					}


					//Test door
					Door* temp = dynamic_cast<Door*>(entityArray[i]);
					if (temp != nullptr)
					{
						XMFLOAT3 tilePos = roomHandler.getTilePos(temp->getTile());
						if (plates.size() > 0)
						{
							if (open && temp->isDoorRaised() && !temp->isDoorRaising())
							{
								temp->lowerDoor(tilePos.y);
							}
							if (!open && !temp->isDoorRaised() && !temp->isDoorLowering())
							{
								temp->raiseDoor(tilePos.y, 11);
							}
						}
						else
						{
							if (enemyCounter == 0 && plates.size() == 0 && temp->isDoorRaised() && !temp->isDoorRaising())
							{
								temp->lowerDoor(tilePos.y);
							}
							if (enemyCounter > 0 && plates.size() == 0 && !temp->isDoorRaised() && !temp->isDoorLowering())
							{
								temp->raiseDoor(tilePos.y, 11);
							}
						}



					}
				}
			}
#pragma endregion	

#pragma region Misc
			//Pickup countdown and removal
			bellamy->countdownPickups(timeLastFrame);
			bellamy->removePickup(&pickupHandler);

			//Timer
			timer += 0.02f;
			updateTimeBuffer(immediateContext, timeBuffer, timer);

			/*const auto point1 = std::chrono::steady_clock::now();
			const auto point2 = std::chrono::steady_clock::now();
			std::chrono::duration<double> e1 = point2 - point1;
			char tmp[100];
			sprintf_s(tmp, "%f\n", e1.count());
			OutputDebugStringA(tmp);*/

			//Test Raise tiles (inte klar, ska skrivas om lite)
			//int closestIndex = roomHandler.getClosestTile(bellamy->getPosition());
			//if (closestIndex != -1)
			//{
			//	roomHandler.raiseTile(closestIndex);
			//	roomHandler.updateTile(immediateContext, closestIndex);
			//}

			//Set camera
			XMFLOAT3 charPos = bellamy->getPosition();
			XMFLOAT3 camPos{ charPos.x, charPos.y + 25.0f, charPos.z - 25.0f };
			stageCam.setLookAt(charPos);
			stageCam.setPos(camPos);

			//Update Camera			
			stageCam.updateCameraBuffers(immediateContext);
			Time::Update();

			if (GuiHandler::exit)
			{
				break;
			}

#pragma endregion

#pragma region Reset
		} /// end of game loop if statement
		else if (necroIsDead && !bellamy->isDead())
		{
			bellamy->isNecroManDead = true;

			if (GuiHandler::exit)
			{
				break;
			}
		}
		if (roomHandler.checkReset())
		{
			bellamy->isNecroManDead = false;
			//randomize level
			restartMaze(device, roomX, roomY, lastframeRoomX, lastframeRoomY, roomHandler, bellamy);

			//Do room transition
			doRoomTransition(immediateContext, device, roomX, roomY, midPoint, -1, roomHandler, bellamy);

			//clear out room actors
			clearRoomActors(entityArray, enemyHandler, actorHandler, gravityObj, piles, pickups, plates, nrWraiths);

			//Spawn/Set Actors//skicka in en enemy handler & actor handler?
			spawnRoomActors(device, entityArray, roomHandler, enemyHandler, actorHandler, pickupHandler, gravityObj, piles, pickups, plates, nrWraiths);

			//Spawn doors for all dorways
			spawnDoors(roomHandler, entityArray, actorHandler);

			//Get ptr for staticActors (if index of actor to get over 13)
			for (int i = 0; i < roomHandler.getNrOfStaticActors(); i++)
			{
				entityArray.push_back(roomHandler.getStaticActors(i));
				entityArray.back()->setTile(entityArray.back()->getPosition().x, entityArray.back()->getPosition().z, roomHandler.getxDiv(), tileWidth);
			}

			pickupHandler.resetPickups();

			//reset bool
			roomHandler.setReset(false);
			nrWraiths = 0;
		}
	
		fps++;

		///////DO NOT USE MESSES WITH TIMECLASS!!!!!!
		//steady_clock::time_point frameEnd = steady_clock::now();	//get time Point     
		//duration<double> time_span = duration_cast<duration<double>>(frameEnd - frameStart);
		//if (time_span < 30ms)
		//{
		//	std::this_thread::sleep_for(15ms - time_span);
		//}

		const auto FrameTimeEnd = std::chrono::steady_clock::now();
		std::chrono::duration<double> frameTime = FrameTimeEnd - FrameTimeStart;
		timeLastFrame = (float)frameTime.count();
#pragma endregion

	}
	

	//--------------------------------------------------------------//Relese/Delete//------------------------------------------------------------------//
#pragma region Relese/Delete

	//release sprites stuff and texts
	GuiHandler::Release();
	Audio::Release();

	//Used in Normal rendering 
	for (int i = 0; i < 8; i++)
	{
		dfTexArr[i]->Release();
	}
	for (int i = 0; i < 8; i++)
	{
		dfSrvArr[i]->Release();
	}
	for (int i = 0; i < 8; i++)
	{
		dfRtvArr[i]->Release();
	}
	//Cube rendering
	for (int i = 0; i < 8; i++)
	{
		cTexArr[i]->Release();
	}
	for (int i = 0; i < 8; i++)
	{
		cSrvArr[i]->Release();
	}
	for (int i = 0; i < 8; i++)
	{
		cRtvArr[i]->Release();
	}
	//Release 
	Mesh::Release();
	Aura::Release();
	PT_PixelShader2->Release();
	PT_UpdateShader_Rising->Release();
	particleUpdateShader->Release();
	timeBuffer->Release();
	particleUav->Release();
	PXShaderGeo->Release();
	PtListBuffer->Release();
	PtworldMatrix->Release();
	inputLayoutGeo->Release();
	geoShader->Release();
	VtxShaderGeo->Release();
	cubeMapShader->Release();
	shadowSampler->Release();
	computeShader->Release();
	backBufferUav->Release();
	depthView->Release();
	depthTexture->Release();
	samplerState->Release();
	inputLayout->Release();
	rtv->Release();
	CameraMTXBuffer->Release();
	CameraPosBuffer->Release();
	pixelShader->Release();
	vertexShader->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();

	delete bellamy;

	#pragma endregion
	return 0;

}