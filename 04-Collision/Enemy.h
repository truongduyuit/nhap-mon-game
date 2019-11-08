#pragma once
#include "GameObject.h"

#define ENEMY_WALKING_SPEED		0.07f
#define ENEMY_GRAVITY			0.001f

#define ENEMY_HIDDEN			-1
#define STATE_GHOST				0

#define ANI_GHOST_LEFT			0
#define ANI_GHOST_RIGHT			1

#define GHOST_WIDTH				16
#define GHOST_HEIGHT			32

#define ACTION_LEFT				-1
#define ACTION_RIGHT			1

class CEnemy : public CGameObject
{
	float x_min;
	float x_max;

	bool isActive;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();
	virtual void SetState(int state);

	void SetMaxMin(float min, float max) { x_min = min; x_max = max; }
	CEnemy();
	~CEnemy() {};
};