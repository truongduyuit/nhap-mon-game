#include "Candle.h"

void CCandle::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (size == SMALL_CANDLE)
	{
		right = x + CANDLE_BBOX_SMALL_WIDTH;
		bottom = y + CANDLE_BBOX_SMALL_HEIGHT;
	}
	else
	{
		right = x + CANDLE_BBOX_BIG_WIDTH;
		bottom = y + CANDLE_BBOX_BIG_HEIGHT;
	}
}

void CCandle::Render()
{
	int ani;
	if (state == CANDLE_STATE_DES)
		ani = CANDLE_ANI_BIG;
	else
	{
		if (size == SMALL_CANDLE)
		{
			ani = CANDLE_ANI_SMALL;
		}
		else
		{
			ani = CANDLE_ANI_BIG;
		}
	}

	animations[ani]->Render(x, y);

	RenderBoundingBox();
}

void CCandle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CANDLE_STATE_DES:
		break;
	case CANDLE_STATE_EXIST:
		break;
	}
}