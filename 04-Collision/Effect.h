#pragma once
#include "GameObject.h"

#define STATE_HIDDEN -1

#define DESTROY_EFFECT_WIDTH	8
#define DESTROY_EFFECT_HEIGHT	16

#define DESTROY_EFFECT_TIME	400

class CEffect : public CGameObject
{
	static CEffect * __instance;

	bool isShow;
	DWORD start_show;

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> * colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartShowEffect();

	CEffect();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	static CEffect * GetInstance();
};