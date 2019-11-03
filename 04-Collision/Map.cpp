#include "Map.h"
#include <fstream>

#include "Simon.h"
#include "SObject.h"
#include "Ground.h"
#include "Weapon.h"
#include "Skill.h"
#include "Effect.h"

CGround* ground;
CSObject* sobject;

void CMap::LoadMapSprites()
{
	CLoadResourcesHelper::LoadSprites("data\\background\\background_sprites.txt");
}

void CMap::LoadTilesPosition()
{
	if (tiles.size() > 0)
		tiles.clear();

	ifstream in;

	if (this->round == 1)
	{
		in.open("data\\background\\map1_tile_position.txt", ios::in);
	}
	else if (this->round == 2)
	{
		in.open("data\\background\\map2_tile_position.txt", ios::in);
	}
	

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load BackgroundSprites position failed ! \n");
		return;
	}

	in >> max_x >> max_y >> tile_size_x >> tile_size_y;

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
				tile->SetTileSize(tile_size_x, tile_size_y);
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

void CMap::LoadObjects()
{

	if (coObjectsFull.size() > 0)
		coObjectsFull.clear();

	CSimon* simon = CSimon::GetInstance();

	ifstream in;

	if (this->round == 1)
	{
		in.open("data\\background\\map1_objects.txt", ios::in);
	}
	else if (this->round == 2)
	{
		in.open("data\\background\\map2_objects.txt", ios::in);
	}
	//in.open("data\\background\\map1_objects.txt", ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load map objects failed ! \n");
		return;
	}
	while (!in.eof())
	{
		int id_object;
		in >> id_object;

		if (id_object == 0)
		{
			int size_x, size_y, x, y;
			in >> size_x >> size_y >> x >> y;

			ground = new CGround();
			ground->setSize(float(size_x), float(size_y));
			ground->SetPosition(float(x), float(y));
			coObjectsFull.push_back(ground);
		}
		else if (id_object == 1)
		{
			int x, y;
			in >> x >> y;

			simon = CSimon::GetInstance();
			simon->SetPosition(float(x), float(y));
			coObjectsFull.push_back(simon);
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

			coObjectsFull.push_back(sobject);
		}
	}

	in.close();

	CWeapon* weapon = CWeapon::GetInstance();
	coObjectsFull.push_back(weapon);

	CSkill* skill = CSkill::GetInstance();
	coObjectsFull.push_back(skill);

	CEffect* effect = CEffect::GetInstance();
	effect->set_isHidden(true);
	coObjectsFull.push_back(effect);

	/*sobject = new CSObject();
	sobject->SetPosition(float(150), float(135));
	sobject->set_isHidden(true);
	sobject->SetState(5);
	sobject->SetNextState(5);
	coObjectsFull.push_back(sobject);*/
}

vector<LPGAMEOBJECT> CMap::MergeListCoObject(vector<LPGAMEOBJECT> result, vector<LPGAMEOBJECT> objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		result.push_back(objects[i]);
	}

	return result;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectsFull()
{
	return coObjectsFull;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectGround()
{
	vector<LPGAMEOBJECT> coObjectGround;

	for (int i = 0; i < coObjectsFull.size(); i++)
	{
		if (dynamic_cast<CGround *>(coObjectsFull[i]))
		{
			coObjectGround.push_back(coObjectsFull[i]);
		}
	}

	return coObjectGround;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectsWithSimon()
{
	vector<LPGAMEOBJECT> coObjectsWithSimon;

	for (int i = 0; i < coObjectsFull.size(); i++)
	{
		if (dynamic_cast<CSObject *>(coObjectsFull[i]) && !coObjectsFull[i]->get_isHidden())
		{
			if (coObjectsFull[i]->GetState() != 1 && coObjectsFull[i]->GetState() != 0)
			{
				coObjectsWithSimon.push_back(coObjectsFull[i]);
			}
		}
	}

	return MergeListCoObject(coObjectsWithSimon, Get_coObjectGround());
}

vector<LPGAMEOBJECT> CMap::Get_coObjectsWithSkill()
{
	vector<LPGAMEOBJECT> coObjectsWithSkill;

	for (int i = 0; i < coObjectsFull.size(); i++)
	{
		if (dynamic_cast<CSObject *>(coObjectsFull[i]))
		{
			if (coObjectsFull[i]->GetState() == 1 || coObjectsFull[i]->GetState() == 0) 
			{
				coObjectsWithSkill.push_back(coObjectsFull[i]);
			}
		}
	}

	return coObjectsWithSkill;
}

CMap::CMap()
{
	this->round = 1;
}

void CMap::SetRound(int r)
{
	this->round = r;
}

CMap* CMap::__instance = NULL;
CMap * CMap::GetInstance()
{
	if (__instance == NULL) __instance = new CMap();
	return __instance;
}