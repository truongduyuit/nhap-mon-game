#pragma once
#include <Windows.h>

#include "LoadResource.h"
#include "TileMat.h"
#include "Effect.h"
#include "Enemy.h"
#include <d3d9.h>
#include <d3dx9.h>

class CMap
{
	static CMap* __instance;

	int max_x;
	int max_y;
	int tile_size_x;
	int tile_size_y;

	vector<CTileMat*> tiles;
	vector<LPGAMEOBJECT> coObjectsFull;
	vector<LPGAMEOBJECT> coObjectGround;
	vector<LPGAMEOBJECT> coObjectFlag;
	vector<LPGAMEOBJECT> coObjectsWithSimon;
	vector<LPGAMEOBJECT> coObjectsWithSkill;
	vector<LPGAMEOBJECT> listEffect;
public :

	CMap();

	void LoadContent(string filePath);

	void LoadMapSprites();
	void DrawMap();
	void ResetListObjects();
	void PushEffect(CEffect* effect);
	void PushObject(LPGAMEOBJECT shoot);

	int GetMaxX() { return this->max_x; }
	int GetMaxY() { return this->max_y; }
	int GetTileSizeX() { return tile_size_x; }

	vector<LPGAMEOBJECT> MergeListCoObject(vector<LPGAMEOBJECT> result, vector<LPGAMEOBJECT> objects);
	vector<LPGAMEOBJECT> Get_coObjectsFull();
	vector<LPGAMEOBJECT> Get_coObjectGround();
	vector<LPGAMEOBJECT> Get_coObjectFlag();
	vector<LPGAMEOBJECT> Get_coObjectsWithSimon();
	vector<LPGAMEOBJECT> Get_coObjectsWithSkill();
	vector<LPGAMEOBJECT> Get_listEffect();

	static CMap * GetInstance();
	~CMap() { ; }
};