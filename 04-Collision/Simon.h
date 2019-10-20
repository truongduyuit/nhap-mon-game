#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED			0.1f
#define SIMON_GRAVITY				0.001f
#define SIMON_JUMP_SPEED_Y			0.3f

#define SIMON_HP_START				100

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT				500
#define SIMON_STATE_ATTACK			600
#define SIMON_STATE_PICK			700

#define SIMON_ANI_IDLE_LEFT			0
#define SIMON_ANI_IDLE_RIGHT		1
#define SIMON_ANI_BACK				2
#define SIMON_ANI_INJURE_LEFT		3
#define SIMON_ANI_INJURE_RIGHT		4
#define SIMON_ANI_DIE_LEFT			5
#define SIMON_ANI_DIE_RIGHT			6
#define SIMON_ANI_WALKING_LEFT		7
#define SIMON_ANI_WALKING_RIGHT		8
#define SIMON_ANI_SIT_LEFT			9
#define SIMON_ANI_SIT_RIGHT			10
#define SIMON_ANI_ATTACK_LEFT		11
#define SIMON_ANI_ATTACK_RIGHT		12
#define SIMON_ANI_PICK_LEFT			13
#define SIMON_ANI_PICK_RIGHT		14

#define SIMON_BBOX_WIDTH  17
#define SIMON_BBOX_HEIGHT 31

#define SIMON_UNTOUCHABLE_TIME		3000

class CSimon : public CGameObject
{
	int hp;
	int untouchable;
	DWORD untouchable_start;

	bool isJump;
	bool isAttack;
	bool isPick;

	DWORD action_time;
public:
	CSimon() : CGameObject()
	{
		hp = SIMON_HP_START;
		state = SIMON_STATE_WALKING_RIGHT;
		nx = 1;

		isJump = false;
		isAttack = false;

		untouchable = 0;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetHP(int h) { hp = h; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};