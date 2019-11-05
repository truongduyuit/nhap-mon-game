#pragma once
#include "GameObject.h"

class CEnemy : public CGameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();
};