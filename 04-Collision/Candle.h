#pragma once
#include "GameObject.h"

#define SMALL_CANDLE				0
#define BIG_CANDLE					1

#define CANDLE_BBOX_SMALL_WIDTH		8
#define CANDLE_BBOX_SMALL_HEIGHT	17

#define CANDLE_BBOX_BIG_WIDTH		16
#define CANDLE_BBOX_BIG_HEIGHT		31

#define CANDLE_STATE_EXIST			0
#define CANDLE_STATE_DES			100

#define CANDLE_ANI_SMALL			0
#define CANDLE_ANI_BIG				1
#define CANDLE_ANI_DES				2


class CCandle : public CGameObject
{
	int size;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();

public:
	virtual void SetState(int state);
	void SetSize(int size) { size = BIG_CANDLE; }

	CCandle() : CGameObject()
	{
		size = BIG_CANDLE;
		state = CANDLE_STATE_EXIST;
	}
};