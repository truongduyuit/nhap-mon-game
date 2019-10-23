#pragma once
#include "GameObject.h"

#define KNIFE_WIDTH	16
#define KNIFE_HEIGHT 9

#define STATE_HIDDEN				0
#define STATE_KNIFE					10

#define ANI_THROW_KNIFE_LEFT		1
#define ANI_THROW_KNIFE_RIGHT		0

#define TIME_THROW					1000

class CSkill : public CGameObject
{
	static CSkill * __instance;

	int intimethrow;
	DWORD timethrow_start;
public:

	CSkill()
	{
		intimethrow = 0;
	}

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void StartThrow() { intimethrow = 1; timethrow_start = GetTickCount(); }

	static CSkill * GetInstance();
};