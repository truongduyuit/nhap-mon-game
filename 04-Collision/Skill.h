#pragma once
#include "GameObject.h"

#define KNIFE_WIDTH	16
#define KNIFE_HEIGHT 9

#define STATE_KNIFE					10

#define ANI_THROW_KNIFE_LEFT		0
#define ANI_THROW_KNIFE_RIGHT		1

#define TIME_THROW					1000

class CSkill : public CGameObject
{
	static CSkill * __instance;

	DWORD timethrow_start;
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