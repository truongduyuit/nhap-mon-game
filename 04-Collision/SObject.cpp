#include "SObject.h"

void CSObject::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == BIG_CANDLE)
	{
		right = x + BBOX_BIG_CANDLE_W;
		bottom = y + BBOX_BIG_CANDLE_H;
	}
	else if (state == SMALL_CANDLE)
	{
		right = x + BBOX_SMALL_CANDLE_W;
		bottom = y + BBOX_SMALL_CANDLE_H;
	}
	else if (state == HEART_ITEM)
	{
		right = x + BBOX_HEART_ITEM_W;
		bottom = y + BBOX_HEART_ITEM_H;
	}
	else if (state == MONEY_ITEM)
	{
		right = x + BBOX_MONEY_ITEM_W;
		bottom = y + BBOX_MONEY_ITEM_H;
	}
	else if (state == CANE_ITEM)
	{
		right = x + BBOX_CANE_ITEM_W;
		bottom = y + BBOX_CANE_ITEM_H;
	}
	else if (state == KNIFE_ITEM)
	{
		right = x + BBOX_KNIFE_ITEM_W;
		bottom = y + BBOX_KNIFE_ITEM_H;
	}
}

void CSObject::Render()
{
	int ani;

	if (state == SMALL_CANDLE)
	{
		ani = SMALL_CANDLE;
	}
	else if (state == BIG_CANDLE)
	{
		ani = BIG_CANDLE;
	}
	else if (state == HEART_ITEM)
	{
		ani = HEART_ITEM;
	}
	else if (state == MONEY_ITEM)
	{
		ani = MONEY_ITEM;
	}
	else if (state == CANE_ITEM)
	{
		ani = CANE_ITEM;
	}
	else if (state == KNIFE_ITEM)
	{
		ani = KNIFE_ITEM;
	}

	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void CSObject::SetState(int state)
{
	CGameObject::SetState(state);

	// something
}

void CSObject::SetNextState(int state)
{
	this->nextState = state;
}

int CSObject::GetNextState()
{
	return this->nextState;
}