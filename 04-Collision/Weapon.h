#pragma once
#include "GameObject.h"

#define STATE_ATTACK_RIGHT 0
#define STATE_ATTACK_LEFT	1

class CWeapon : public CGameObject
{
	static CWeapon * __instance;

	int level;
	int untouchable;
	DWORD untouchable_start;

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> * colliable_objects = NULL);
	virtual void Render();
	void SetState(int statCSpritese);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	static CWeapon * GetInstance();
};