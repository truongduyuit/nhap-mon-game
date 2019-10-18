#pragma once
#include "GameObject.h"

#define SMALL_CANDLE		0
#define BIG_CANDLE			1
#define HEART_ITEM			2
#define MONEY_ITEM			3
#define CANE_ITEM			4
#define KNIFE_ITEM			5

#define BBOX_BIG_CANDLE_W	16
#define BBOX_BIG_CANDLE_H	32

#define BBOX_SMALL_CANDLE_W		16
#define BBOX_SMALL_CANDLE_H		32

#define BBOX_HEART_ITEM_W		16
#define BBOX_HEART_ITEM_H		16

#define BBOX_MONEY_ITEM_W		30
#define BBOX_MONEY_ITEM_H		30

#define	BBOX_CANE_ITEM_W		32
#define BBOX_CANE_ITEM_H		32

#define BBOX_KNIFE_ITEM_W		32
#define BBOX_KNIFE_ITEM_H		18

class CSObject : public CGameObject
{
	int nextState;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();

public :
	virtual void SetState(int state);

	void SetNextState(int type);
	int GetNextState();

	CSObject() : CGameObject()
	{

	}

	CSObject(int nextState) : CGameObject()
	{
		this->nextState = nextState;
	}
};