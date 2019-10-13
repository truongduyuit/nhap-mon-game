#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED			0.1f
#define SIMON_GRAVITY				0.002f
#define SIMON_JUMP_SPEED_Y			0.4f

#define SIMON_HP_START				100

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT				500

#define SIMON_ANI_WALKING_LEFT		0
#define SIMON_ANI_WALKING_RIGHT		1
#define SIMON_ANI_DIE				2
#define SIMON_ANI_IDLE_LEFT			3
#define SIMON_ANI_IDLE_RIGHT		4
#define SIMON_ANI_SIT_LEFT			5
#define SIMON_ANI_SIT_RIGHT			6
#define SIMON_ANI_JUMP_LEFT			7
#define SIMON_ANI_JUMP_RIGHT		8

#define SIMON_BBOX_WIDTH  17
#define SIMON_BBOX_HEIGHT 31

#define SIMON_UNTOUCHABLE_TIME		4000

class CSimon : public CGameObject
{
	int hp;
	int untouchable;
	DWORD untouchable_start;

public:
	CSimon() : CGameObject()
	{
		hp = SIMON_HP_START;
		untouchable = 0;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetHP(int h) { hp = h; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};