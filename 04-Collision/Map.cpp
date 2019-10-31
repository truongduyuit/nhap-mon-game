#include "Map.h"
#include <fstream>

void CMap::LoadMapSprites()
{
	CLoadResourcesHelper::LoadSprites("data\\background\\background_sprites.txt");
}

void CMap::LoadTilesPosition()
{
	ifstream in;

	in.open("data\\background\\background_tile_position.txt", ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load BackgroundSprites position failed ! \n");
		return;
	}

	in >> max_x;
	in >> max_y;

	CTileMat* tile;

	while (!in.eof())
	{
		for (int i = 0; i < max_y; i++)
		{
			for (int j = 0; j < max_x; j++)
			{
				int id;
				in >> id;

				tile = new CTileMat(id, i, j);
				tiles.push_back(tile);
			}
		}
	}
	in.close();
}

void CMap::DrawMap()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles[i]->Render();
	}
}

CMap::CMap()
{
	this->level = 1;
	this->max_x = MAP1_MAX_X;
	this->max_y = MAP1_MAX_y;
}

CMap::CMap(int lv, int maxx, int maxy)
{
	this->level = lv;
	this->max_x = maxx;
	this->max_y = maxy;
}