#include "TileMat.h"



void CTileMat::Render()
{
	CSprites *sprites = CSprites::GetInstance();

	sprites->Get(id)->Draw(index_y*TILE_SIZE, index_x * TILE_SIZE + 20);
}


CTileMat::CTileMat(int id, int x, int y)
{
	this->id = id;
	this->index_x = x;
	this->index_y = y;
}