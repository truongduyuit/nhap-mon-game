#pragma once
#include <Windows.h>

#include "LoadResource.h"
#include "TileMat.h"

#include <d3d9.h>
#include <d3dx9.h>

#define ID_TEX_BACKBROUND		10

class CMap
{

	int level;

	int max_x;
	int max_y;

	vector<CTileMat*> tiles;
public :

	CMap();
	CMap(int lv, int maxx, int maxy);

	void LoadMapSprites();
	void LoadTilesPosition();
	void DrawMap();

	~CMap() { ; }
};