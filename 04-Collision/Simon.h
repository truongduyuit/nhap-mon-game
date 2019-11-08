#pragma once
#include "GameObject.h"
#include "LoadResource.h"

#define SIMON_WALKING_SPEED			0.07f
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
#define SIMON_STATE_THROW			800
#define SIMON_STATE_UP_STAIR		900
#define SIMON_STATE_DOWN_STAIR		901

#define SIMON_ANI_IDLE_LEFT					0
#define SIMON_ANI_IDLE_RIGHT				1
#define SIMON_ANI_BACK						2
#define SIMON_ANI_INJURE_LEFT				3
#define SIMON_ANI_INJURE_RIGHT				4
#define SIMON_ANI_DIE_LEFT					5
#define SIMON_ANI_DIE_RIGHT					6
#define SIMON_ANI_WALKING_LEFT				7
#define SIMON_ANI_WALKING_RIGHT				8
#define SIMON_ANI_SIT_LEFT					9
#define SIMON_ANI_SIT_RIGHT					10
#define SIMON_ANI_ATTACK_LEFT				11
#define SIMON_ANI_ATTACK_RIGHT				12
#define SIMON_ANI_PICK_LEFT					13
#define SIMON_ANI_PICK_RIGHT				14
#define SIMON_ANI_SIT_ATTACK_LEFT			15
#define SIMON_ANI_SIT_ATTACK_RIGHT			16
#define SIMON_ANI_IDLE_UPSTAIR_RIGHT		17
#define SIMON_ANI_IDLE_DOWNSTAIR_RIGHT		18
#define SIMON_ANI_UPSTAIR_RIGHT				19
#define SIMON_ANI_DOWNSTAIR_RIGHT			20
#define SIMON_ANI_IDLE_UPSTAIR_LEFT			21
#define SIMON_ANI_IDLE_DOWNSTAIR_LEFT		22
#define SIMON_ANI_UPSTAIR_LEFT				23
#define SIMON_ANI_DOWNSTAIR_LEFT			24

#define SIMON_BBOX_WIDTH		17
#define SIMON_BBOX_HEIGHT		31
#define SIMON_SIT_BBOX_WIDTH	17
#define SIMON_SIT_BBOX_HEIGHT	24
#define SIMON_RESET_BBOX		7

#define SIMON_ATTACK_TIME			300
#define SIMON_PICK_TIME				1600
#define SIMON_THROW_TIME			1000
#define SIMON_UNTOUCHABLE_TIME		3000

#define SIMON_DOWNSTAIR		-2
#define SIMON_UPSTAIR		2

class CSimon : public CGameObject
{
	static CSimon * __instance;

	int hp;
	vector<int> skill;
	int sthrow;


	int untouchable;
	DWORD untouchable_start;

	bool isJump;
	bool isAttack;
	bool isPick;
	bool isthrow;
	bool isSit;
	bool onStair;
	bool onTimeStair;

	bool isBeMoving;
	float be_x;
	int be_nx;
	int be_updown;

	DWORD action_time;
public:

	CSimon();
	~CSimon() { ; };

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetOnStair(bool stair);
	void SetHP(int h) { hp = h; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	bool get_isAttack() { return isAttack; }
	bool get_isJump() { return isJump; }
	bool get_isPick() { return isPick; }
	bool get_isThrow() { return isthrow; }
	bool get_isSit() { return isSit; }
	bool get_onstair() { return onStair; }

	void startAttack();
	void startJump();
	void startPick();
	void startThrow();
	void startSit();
	void upBBox();
	void beMoving(int bnx, float bx, int updown);
	void moveOnStair();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	static CSimon * GetInstance();
};