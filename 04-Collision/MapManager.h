#pragma once
#include "Map.h"

class CMapManager
{
	static CMapManager* __instance;

	int map_stage;
	CMap* map;
public:
	CMapManager();

	void SetStage(int stage) { this->map_stage = stage; }
	void LoadMapResources();
	void DrawMap();

	static CMapManager * GetInstance();

	~CMapManager() { ; }
};