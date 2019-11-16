#pragma once
#include "GameObject.h"

#define ENEMY_WALKING_SPEED		0.052f
#define WOLF_FLY_SPEED			0.09f
#define WOLF_RUN_SPEED			0.11f
#define BAT_FLY_SPEED			0.025f
#define BAT_FLY_SPEED_Y			0.01f
#define ENEMY_GRAVITY			0.0001f

#define ENEMY_HIDDEN			-1
#define STATE_GHOST				0
#define STATE_WOLF				1
#define STATE_BAT				2
#define STATE_FISH_MONSTER		3

#define ANI_GHOST_LEFT			0
#define ANI_GHOST_RIGHT			1
#define ANI_WOLF_LIE_LEFT		2
#define ANI_WOLF_RUN_LEFT		3
#define ANI_WOLF_FLY_LEFT		4
#define ANI_WOLF_LIE_RIGHT		5
#define ANI_WOLF_RUN_RIGHT		6
#define ANI_WOLF_FLY_RIGHT		7
#define ANI_BAT_FLY_LEFT		8
#define ANI_BAT_FLY_RIGHT		9
#define ANI_FISH_WALKING_LEFT	10
#define ANI_FISH_WALKING_RIGHT	11
#define ANI_FISH_SHOOT_LEFT		12
#define ANI_FISH_SHOOT_RIGHT    13

#define GHOST_WIDTH				16
#define GHOST_HEIGHT			32
#define WOLF_WIDTH				32
#define WOLF_HEIGHT				16
#define BAT_WIDTH				17
#define BAT_HEIGHT				15
#define FISH_MONSTER_WIDTH		16
#define FISH_MONSTER_HEIGHT		33

#define ACTION_LEFT				-1
#define ACTION_RIGHT			1

class CEnemy : public CGameObject
{
	int nxx;
	float x_min;
	float x_max;
	float vyy;

	bool isFly;
	bool isActive;
	DWORD action_time;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();
	virtual void SetState(int state);

	void wolf_update();
	void ghost_update();
	void bat_update();
	bool onFlag();

	void SetMaxMin(float min, float max) { x_min = min; x_max = max; }
	CEnemy();
	~CEnemy() {};
};