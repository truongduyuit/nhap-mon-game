#include "SObject.h"
#include "Brick.h"
#include "Simon.h"

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

void CSObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vy += SOBJECT_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick *>(e->obj))
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
			}
		
		}

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
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

	if (state != SOBJECT_HIDDEN)
	{
		animations[ani]->Render(x, y);
		RenderBoundingBox();
	}
}

void CSObject::SetState(int state)
{
	CGameObject::SetState(state);
}
