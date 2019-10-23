#pragma once
#include "GameObject.h"

#define SOBJECT_GRAVITY		0.001f

#define SOBJECT_HIDDEN		-1
#define SMALL_CANDLE		0
#define BIG_CANDLE			1
#define HEART_ITEM			2
#define CANE_ITEM			3
#define KNIFE_ITEM			4
#define MONEY_ITEM			5


#define BBOX_BIG_CANDLE_W	16
#define BBOX_BIG_CANDLE_H	32

#define BBOX_SMALL_CANDLE_W		8
#define BBOX_SMALL_CANDLE_H		8

#define BBOX_HEART_ITEM_W		16
#define BBOX_HEART_ITEM_H		16

#define BBOX_MONEY_ITEM_W		30
#define BBOX_MONEY_ITEM_H		30

#define	BBOX_CANE_ITEM_W		16
#define BBOX_CANE_ITEM_H		16

#define BBOX_KNIFE_ITEM_W		16
#define BBOX_KNIFE_ITEM_H		9

class CSObject : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();

public :
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	CSObject() : CGameObject()
	{

	}
};