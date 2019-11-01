#pragma once
#include <Windows.h>

#include "LoadResource.h"
#include "TileMat.h"

#include <d3d9.h>
#include <d3dx9.h>

class CMap
{
	static CMap* __instance;

	int round;

	int max_x;
	int max_y;

	vector<CTileMat*> tiles;
	vector<LPGAMEOBJECT> coObjectsFull;

	int tile_size_x;
	int tile_size_y;
public :

	CMap();

	void LoadMapSprites();
	void LoadTilesPosition();
	void LoadObjects();
	void DrawMap();
	void SetRound(int r);
	int GetRound() { return this->round; }

	vector<LPGAMEOBJECT> MergeListCoObject(vector<LPGAMEOBJECT> result, vector<LPGAMEOBJECT> objects);
	vector<LPGAMEOBJECT> Get_coObjectsFull();
	vector<LPGAMEOBJECT> Get_coObjectGround();
	vector<LPGAMEOBJECT> Get_coObjectsWithSimon();
	vector<LPGAMEOBJECT> Get_coObjectsWithSkill();

	static CMap * GetInstance();
	~CMap() { ; }
};