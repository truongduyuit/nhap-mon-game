#pragma once
#include "GameObject.h"

#define EFFECT_GRAVITY			0.0001f

#define STATE_HIDDEN -1
#define STATE_DESTROY			0
#define STATE_BREAKING_WALL		1
#define STATE_SPLASH			2

#define DESTROY_EFFECT_WIDTH	8
#define DESTROY_EFFECT_HEIGHT	16
#define BREAKING_WALL_WIDTH		8
#define BREAKING_WALL_HEIGHT	8
#define SPLASH_WIDTH			8
#define SPLASH_HEIGHT			13

#define DESTROY_EFFECT_TIME		400
#define BREAKING_WALL_TIME		600

class CEffect : public CGameObject
{
	bool isShow;
	DWORD start_show;

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> * colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartShowEffect();
	void StartShowEffect(float vxx, float vyy);

	CEffect();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};