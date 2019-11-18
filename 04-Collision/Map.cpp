#include "Map.h"
#include <fstream>
#include "Game.h"
#include "Simon.h"
#include "SObject.h"
#include "Ground.h"
#include "Flag.h"
#include "Weapon.h"
#include "Skill.h"

CGround* ground;
CSObject* sobject;
CEnemy* enemy;

void CMap::LoadMapSprites()
{
	CLoadResourcesHelper::LoadSprites("data\\background\\background_sprites.txt");
}

void CMap::LoadContent(string filePath)
{
	ResetListObjects();

	ifstream in;
	in.open(filePath, ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load map content failed ! \n");
		return;
	}

	CTileMat* tile;
	mapObjects.clear();
	gridObject.clear();
	mapObjectId = 2;

	while (!in.eof())
	{

		// background
		in >> this->max_x >> this->max_y >> this->tile_size_x >> this->tile_size_y;

		for (int i = 0; i < max_y; i++)
		{
			for (int j = 0; j < max_x; j++)
			{
				int id;
				in >> id;

				tile = new CTileMat(id, i, j);
				tile->SetTileSize(tile_size_x, tile_size_y);
				tiles.push_back(tile);
			}
		}

		// objects
		CWeapon* weapon = CWeapon::GetInstance();

		CSimon* simon = CSimon::GetInstance();
		simon->nx = 1;

		while (!in.eof())
		{
			mapObjectId++;

			int id_object;
			in >> id_object;

			if (id_object == 0)
			{
				int size_x, size_y, x, y;
				in >> size_x >> size_y >> x >> y;

				ground = new CGround();
				ground->setSize(size_x, size_y);
				ground->SetPosition(float(x), float(y));

				mapObjects[mapObjectId] = ground;
				vector<int> goundGrid = GetGridNumber(float(x), size_x);
				for (int i = 0; i < goundGrid.size(); i++)
				{
					AddGridObject(goundGrid[i], mapObjectId);
				}
			}
			else if (id_object == 1)
			{
				int x, y;
				in >> x >> y;

				simon = CSimon::GetInstance();
				simon->SetPosition(float(x), float(y));
			}
			else if (id_object == 2)
			{
				int x, y, state, nextState;
				in >> x >> y >> state >> nextState;

				sobject = new CSObject();
				sobject->SetPosition(float(x), float(y));
				sobject->SetState(state);
				sobject->SetNextState(nextState);

				if (state == 5)
				{
					int xAppear;
					in >> xAppear;
					sobject->SetxAppear(xAppear);
					sobject->set_isHidden(true);
				}

				mapObjects[mapObjectId] = sobject;
				AddGridObject(GetGridNumber(x), mapObjectId);
			}
			else if (id_object == 3)
			{
				int size_x, size_y, x, y, state, nextState;
				in >> size_x >> size_y >> x >> y >> state >> nextState;

				CFlag* flag = new CFlag();
				flag->setSize(size_x, size_y);
				flag->SetPosition(float(x), float(y));
				flag->SetState(state);
				flag->SetNextState(nextState);

				mapObjects[mapObjectId] = flag;
				AddGridObject(GetGridNumber(x), mapObjectId);
			}
			else if (id_object == 4)
			{
				int x, y, state, nextState, nxx;
				float x_min, x_max;

				in >> x >> y >> state >> nextState >> nxx;
				in >> x_min >> x_max;

				enemy = new CEnemy();
				enemy->SetPosition(float(x), float(y));
				enemy->SetState(state);
				enemy->SetNextState(nextState);
				enemy->nx = nxx;
				enemy->SetMaxMin(x_min, x_max);

				mapObjects[mapObjectId] = enemy;
				AddGridObject(GetGridNumber(x), mapObjectId);
			}
		}
	
		mapObjects[0] = weapon;
		mapObjects[1] = simon;
		CSkill* skill = CSkill::GetInstance();
		mapObjects[2] = skill;
		for (int i = 0; i < gridObject.size(); i++)
		{
			AddGridObject(i, 0); // weapon
			AddGridObject(i, 1); // simon
			AddGridObject(i, 2); // skill
		}
	}
	in.close();

	//OutputDebugString(L"a");
}

void CMap::DrawMap()
{
	int tile_x_begin = CGame::GetInstance()->getCamPos_x() / tile_size_x;

	int tile_x_end = tile_x_begin + SCREEN_WIDTH / tile_size_x + 1;

	//int ncount = 0;

	for (unsigned int i = tile_x_begin; i < tiles.size(); i++)
	{
		tiles[i]->Render();
		//ncount++;

		if (i >= tile_x_end)
		{
			i += max_x - SCREEN_WIDTH / tile_size_x - 2 ;
			tile_x_end += max_x;
		}
	}

	//OutputDebugString(L"a");
}

void CMap::ResetListObjects()
{
	tiles.clear();
	listEffect.clear();
}

vector<LPGAMEOBJECT> CMap::Get_coObjectFlag()
{
	vector<LPGAMEOBJECT> coObjectFlag;
	vector<int> currentGrid = GetGridNumber(CGame::GetInstance()->getCamPos_x(), SCREEN_WIDTH - 1);
	for (int i = 0; i < currentGrid.size(); i++)
	{
		vector<int> idObjects = gridObject[currentGrid[i]].Get_Arr();

		for (int j = 0; j < idObjects.size(); j++)
		{
			if (dynamic_cast<CFlag *>(mapObjects[idObjects[j]]) && !mapObjects[idObjects[j]]->get_isHidden())
			{
				coObjectFlag.push_back(mapObjects[idObjects[j]]);
			}
		}
	}
	return coObjectFlag;
}

vector<LPGAMEOBJECT> CMap::Get_listEffect()
{
	vector<LPGAMEOBJECT> effects;

	for (unsigned int i = 0; i < listEffect.size(); i++)
	{
		if (!listEffect[i]->isHidden)
		{
			effects.push_back(listEffect[i]);
		}
	}

	if (effects.size() == 0) listEffect.clear();

	return effects;
}

void CMap::PushEffect(CEffect* effect)
{
	listEffect.push_back(effect);
}

void CMap::PushObject(LPGAMEOBJECT shoot)
{
	int numGrid = GetGridNumber(shoot->x);
	mapObjectId++;
	mapObjects[mapObjectId] = shoot;
	gridObject.at(numGrid).push(mapObjectId);
}

void CMap::Cross_Enemy()
{
	vector<int> currentGrid = GetGridNumber(CGame::GetInstance()->getCamPos_x(), SCREEN_WIDTH - 1);
	for (int i = 0; i < currentGrid.size(); i++)
	{
		vector<int> idObjects = gridObject[currentGrid[i]].Get_Arr();

		for (int j = 0; j < idObjects.size(); j++)
		{
			if (dynamic_cast<CEnemy *>(mapObjects[idObjects[j]]) && !mapObjects[idObjects[j]]->get_isHidden())
			{
				mapObjects[idObjects[j]]->BeDestroy();
			}
		}
	}
}

void CMap::Stop_Enemy()
{
	vector<int> currentGrid = GetGridNumber(CGame::GetInstance()->getCamPos_x(), SCREEN_WIDTH - 1);
	for (int i = 0; i < currentGrid.size(); i++)
	{
		vector<int> idObjects = gridObject[currentGrid[i]].Get_Arr();

		for (int j = 0; j < idObjects.size(); j++)
		{
			if (dynamic_cast<CEnemy *>(mapObjects[idObjects[j]]) && !mapObjects[idObjects[j]]->get_isHidden())
			{
				mapObjects[idObjects[j]]->isStop = true;
			}
		}
	}
}

CMap::CMap()
{
	LoadMapSprites();
}

CMap* CMap::__instance = NULL;
CMap * CMap::GetInstance()
{
	if (__instance == NULL) __instance = new CMap();
	return __instance;
}

LPGAMEOBJECT CMap::Get_mapObject(unsigned int id)
{
	return mapObjects[id];
}

void CMap::AddMapObject(int id, LPGAMEOBJECT obj)
{
	mapObjects[id] = obj;
}

void CMap::AddGridObject(int numGrid, int id)
{
	while (gridObject.size() <= numGrid)
	{
		int_c temp;
		gridObject.push_back(temp);
	}

	gridObject.at(numGrid).push(id);
}

int CMap::GetGridNumber(int pos_x)
{
	return pos_x / SCREEN_WIDTH;
}

vector<int> CMap::GetGridNumber(float pos_x, int size_x)
{
	int min = pos_x / SCREEN_WIDTH;
	int max = (pos_x + size_x) / SCREEN_WIDTH;
	vector<int> result;

	if (min < max)
	{
		for (int i = min; i <= max; i++)
		{
			result.push_back(i);
		}
	}
	else
		result.push_back(min);

	return result;
}

void CMap::Get_gridObjects(
	vector<LPGAMEOBJECT> &coObjectsFull,
	vector<LPGAMEOBJECT> &coObjectGround,
	vector<LPGAMEOBJECT> &coObjectFlag,
	vector<LPGAMEOBJECT> &coObjectsWithSimon,
	vector<LPGAMEOBJECT> &coObjectsWithSkill
)
{
	coObjectsFull.clear();
	coObjectGround.clear();
	coObjectFlag.clear();
	coObjectsWithSimon.clear();
	coObjectsWithSkill.clear();

	vector<int> currentGrid = GetGridNumber(CGame::GetInstance()->getCamPos_x(), SCREEN_WIDTH - 1);
	unordered_map<int, int> checkUnique;
	vector<int> idObjects;

	for (int i = 0; i < currentGrid.size(); i++)
	{
		if (gridObject.size() < currentGrid[i] - 1)
		{
			int_c temp;
			gridObject.push_back(temp);
		}
		idObjects = gridObject[currentGrid[i]].Get_Arr();

		vector<int> b = gridObject[currentGrid[i]].Get_Arr();
		int a = currentGrid[i];

		for (int j = 0; j < idObjects.size(); j++)
		{
			if (checkUnique[idObjects[j]] == NULL)
			{
				coObjectsFull.push_back(mapObjects[idObjects[j]]);
				checkUnique[idObjects[j]] = idObjects[j];

				if (dynamic_cast<CSObject *>(mapObjects[idObjects[j]]))
				{
					if (mapObjects[idObjects[j]]->GetState() == SMALL_CANDLE || mapObjects[idObjects[j]]->GetState() == BIG_CANDLE)
					{
						coObjectsWithSkill.push_back(mapObjects[idObjects[j]]);
					}
					else
					{
						coObjectsWithSimon.push_back(mapObjects[idObjects[j]]);
					}
				}
				else if (dynamic_cast<CGround *>(mapObjects[idObjects[j]]))
				{
					coObjectGround.push_back(mapObjects[idObjects[j]]);
					coObjectsWithSkill.push_back(mapObjects[idObjects[j]]);
					coObjectsWithSimon.push_back(mapObjects[idObjects[j]]);
				}
				else if (dynamic_cast<CEnemy *>(mapObjects[idObjects[j]]) && !mapObjects[idObjects[j]]->get_isHidden())
				{
					coObjectsWithSkill.push_back(mapObjects[idObjects[j]]);
					coObjectsWithSimon.push_back(mapObjects[idObjects[j]]);
				}
				else if (dynamic_cast<CFlag *>(mapObjects[idObjects[j]]))
				{
					coObjectFlag.push_back(mapObjects[idObjects[j]]);
					//coObjectsWithSimon.push_back(mapObjects[idObjects[j]]);
				}
			}
		}
	}
}