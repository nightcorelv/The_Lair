
#include "Movable.h"



/*class Tombstone : public Movable
{
private:

public:
	//Tombstone(Mesh mesh);
	Tombstone(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0));
	bool collision(Entity& entity);
	//move?

};*/
class Plate : public Movable
{
private:
	bool isActivated;

public:
	Plate(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), float* whd = nullptr);
	Plate(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation = XMFLOAT3(0, 0, 0), float* whd = nullptr);
	void overlapCheck(Entity** cull, int used);
	bool getOverlap();
};