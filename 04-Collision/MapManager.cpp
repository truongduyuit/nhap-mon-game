#include <fstream>
#include <string>

#include "MapManager.h"

CMapManager::CMapManager()
{
	map = CMap::GetInstance();
}

void CMapManager::LoadMapResources()
{
	ifstream in;
	in.open("data\\background\\map_manager.txt", ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load map manager failed ! \n");
		return;
	}
	while (!in.eof())
	{
		int stage;
		string filePath;

		in >> stage >> filePath;

		if (stage == this->map_stage)
		{
			map->LoadContent(filePath);
			break;
		}
	}
	in.close();
}

void CMapManager::DrawMap()
{
	map->DrawMap();
}

CMapManager* CMapManager::__instance = NULL;
CMapManager * CMapManager::GetInstance()
{
	if (__instance == NULL) __instance = new CMapManager();
	return __instance;
}