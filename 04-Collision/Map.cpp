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

void CMap::LoadObjects(vector<LPGAMEOBJECT>& objects)
{
	CSimon* simon;

	ifstream in;
	in.open("data\\background\\map1_objects.txt", ios::in);

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
			objects.push_back(ground);
		}
		else if (id_object == 1)
		{
			int x, y;
			in >> x >> y;

			simon = CSimon::GetInstance();
			simon->SetPosition(float(x), float(y));
			objects.push_back(simon);
		}
		else if (id_object == 2)
		{
			int x, y, state, nextState;
			in >> x >> y >> state >> nextState;

			sobject = new CSObject();
			sobject->SetPosition(float(x), float(y));
			sobject->SetState(state);
			sobject->SetNextState(nextState);
			objects.push_back(sobject);
		}
	}

	in.close();

	CWeapon* weapon = CWeapon::GetInstance();
	objects.push_back(weapon);
	CSkill* skill = CSkill::GetInstance();
	objects.push_back(skill);
	CEffect* effect = CEffect::GetInstance();
	effect->set_isHidden(true);
	objects.push_back(effect);
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