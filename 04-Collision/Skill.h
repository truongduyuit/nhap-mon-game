#pragma once
#include "GameObject.h"

#define KNIFE_WIDTH	16
#define KNIFE_HEIGHT 9
#define HOLY_WATER_SKILL_WIDTH			16
#define HOLY_WATER_SKILL_HEIGHT			13

#define STATE_KNIFE					10
#define STATE_HOLY_WATER			11
#define STATE_HOLY_FIRE				12

#define ANI_THROW_KNIFE_LEFT		0
#define ANI_THROW_KNIFE_RIGHT		1
#define ANI_HOLY_WATER_LEFT			2
#define ANI_HOLY_WATER_RIGHT		3
#define ANI_HOLY_FIRE				4

#define TIME_THROW					2000

class CSkill : public CGameObject
{
	static CSkill * __instance;

	bool isFailing;

	DWORD timethrow_start;
	DWORD timeshow;
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void startThrow();
	void updateThrow();

	static CSkill * GetInstance();

	CSkill();
};