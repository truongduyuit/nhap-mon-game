#include "Map.h"
#include <fstream>

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

	while (!in.eof())
	{
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

		CWeapon* weapon = CWeapon::GetInstance();
		coObjectsFull.push_back(weapon);
		CSimon* simon = CSimon::GetInstance();
		simon->nx = 1;

		while (!in.eof())
		{
			int id_object;
			in >> id_object;

			if (id_object == 0)
			{
				int size_x, size_y, x, y;
				in >> size_x >> size_y >> x >> y;

				ground = new CGround();
				ground->setSize(size_x, size_y);
				ground->SetPosition(float(x), float(y));

				coObjectGround.push_back(ground);
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
			else if (id_object == 3)
			{
				int size_x, size_y, x, y, state, nextState;
				in >> size_x >> size_y >> x >> y >> state >> nextState;

				CFlag* flag = new CFlag();
				flag->setSize(size_x, size_y);
				flag->SetPosition(float(x), float(y));
				flag->SetState(state);
				flag->SetNextState(nextState);

				coObjectFlag.push_back(flag);
				coObjectsFull.push_back(flag);
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
				coObjectsFull.push_back(enemy);
			}
		}
	}
	in.close();

	CSkill* skill = CSkill::GetInstance();
	coObjectsFull.push_back(skill);
}

void CMap::DrawMap()
{
	for (unsigned int i = 0; i < tiles.size(); i++)
	{
		tiles[i]->Render();
	}
}

void CMap::ResetListObjects()
{
	tiles.clear();
	listEffect.clear();
	coObjectsFull.clear();
	coObjectGround.clear();
	coObjectFlag.clear();
	coObjectsWithSimon.clear();
	coObjectsWithSkill.clear();
}

vector<LPGAMEOBJECT> CMap::MergeListCoObject(vector<LPGAMEOBJECT> result, vector<LPGAMEOBJECT> objects)
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		result.push_back(objects[i]);
	}

	return result;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectsFull()
{

	vector<LPGAMEOBJECT> Full;

	for (unsigned int i = 0; i < this->coObjectsFull.size(); i++)
	{
		if (!coObjectsFull[i]->isHidden)
		{
			Full.push_back(coObjectsFull[i]);
		}
	}
	return Full;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectGround()
{
	return coObjectGround;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectFlag()
{
	return coObjectFlag;
}

vector<LPGAMEOBJECT> CMap::Get_coObjectsWithSimon()
{
	vector<LPGAMEOBJECT> coObjectsWithSimon;

	for (unsigned int i = 0; i < coObjectsFull.size(); i++)
	{
		if (dynamic_cast<CSObject *>(coObjectsFull[i]) && !coObjectsFull[i]->get_isHidden())
		{
			if (coObjectsFull[i]->GetState() != 1 && coObjectsFull[i]->GetState() != 0)
			{
				coObjectsWithSimon.push_back(coObjectsFull[i]);
			}
		}
		if (dynamic_cast<CEnemy *>(coObjectsFull[i]) && !coObjectsFull[i]->get_isHidden())
		{
			coObjectsWithSimon.push_back(coObjectsFull[i]);
		}
	}

	return MergeListCoObject(coObjectsWithSimon, Get_coObjectGround());
}

vector<LPGAMEOBJECT> CMap::Get_coObjectsWithSkill()
{
	vector<LPGAMEOBJECT> coObjectsWithSkill;

	for (unsigned int i = 0; i < coObjectsFull.size(); i++)
	{
		if (dynamic_cast<CSObject *>(coObjectsFull[i]) && !coObjectsFull[i]->get_isHidden())
		{
			if (coObjectsFull[i]->GetState() == 1 || coObjectsFull[i]->GetState() == 0)
			{
				coObjectsWithSkill.push_back(coObjectsFull[i]);
			}
		}
		if (dynamic_cast<CEnemy *>(coObjectsFull[i]) && !coObjectsFull[i]->get_isHidden())
		{
			coObjectsWithSkill.push_back(coObjectsFull[i]);
		}
	}

	return MergeListCoObject(coObjectsWithSkill, Get_coObjectGround());
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
	coObjectsFull.push_back(shoot);
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