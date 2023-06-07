#pragma once
#include "Movable.h"

class Puzzlebox : public Movable
{
private:
	bool solved;											//Keeps track on if the solution has been reached or not.
	
public:

	//Puzzlebox(Mesh mesh);
	Puzzlebox(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	Puzzlebox(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	bool isSolved() const;//Is the puzzle solved?
	bool solve();//Solve the puzzle.
	bool collision(Entity& entity);
	//move?
};