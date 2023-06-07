#include "Puzzlebox.h"

//Puzzlebox::Puzzlebox(Mesh mesh)
//{
//	setMesh(mesh);
//}


Puzzlebox::Puzzlebox(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation)
	:Movable(device, "../Meshes/PuzzelBoxPurpleGreen",Position, Rotation, 3)/*Puzzlebox_20220502*/
{
	solved = false;
	setID(ID::puzzlebox);
}

Puzzlebox::Puzzlebox(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation)
	:Movable(device, mesh, Position, Rotation, 3)
{
	solved = false;
	setID(ID::puzzlebox);
}

bool Puzzlebox::isSolved() const
{
	return false;
}

bool Puzzlebox::solve()
{
	return false;
}

bool Puzzlebox::collision(Entity& entity)
{
	return false;
}
