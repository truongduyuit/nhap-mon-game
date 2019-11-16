#include "SObject.h"
#include "Ground.h"
#include "Simon.h"
#include "LoadResource.h"
#include "Effect.h"
#include "Map.h"

CSObject::CSObject()
{
	CLoadResourcesHelper::LoadSprites("data\\sobjects\\sobject_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\sobjects\\sobject_anis.txt", this);
}

void CSObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);


	if (state == HEART_SMALL_ITEM)
	{
		vy += SMALL_HEART_GRAVITY * dt;
	}
	else if (state != SMALL_CANDLE && state != HEART_SMALL_ITEM)
	{
		vy += SOBJECT_GRAVITY * dt;
	}

	if (state == MONEY_ITEM_1k && isHidden)
	{
		CSimon * simon = CSimon::GetInstance();
		float x, y;
		simon->GetPosition(x, y);
		if (x > xAppear)
		{
			set_isHidden(false);
		}
	}

	if (isDestroy)
	{
		if (GetTickCount() - destroy_start >= SOBJECT_DESTROY_TIME)
		{
			SetState(nextState);
			nextState = SOBJECT_HIDDEN;

			isDestroy = false;
			destroy_start = 0;

			ItemStart();
		}
	}

	if (state == HEART_SMALL_ITEM)
	{
		if (GetTickCount() - appear_start < SMALL_HEART_X_TIME)
		{
			x += 0.5f;
		}
		else if (GetTickCount() - appear_start >= SMALL_HEART_X_TIME && GetTickCount() - appear_start < 2* SMALL_HEART_X_TIME)
		{
			x -= 0.5f;
		}
	}

	if (isAppear)
	{
		if (GetTickCount() - appear_start >= SOBJECT_APPEAR_TIME)
		{
			SetState(SOBJECT_HIDDEN);
			isAppear = false;
		}

	}
	else
	{
		if (nextState == SOBJECT_HIDDEN)
		{
			isAppear = true;
			appear_start = GetTickCount();
		}
	}

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

			if (dynamic_cast<CGround *>(e->obj))
			{
				//x += min_tx * dx + nx * 0.4f;
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
	else if (state == MONEY_ITEM_1k)
	{
		ani = MONEY_ITEM_1k;
	}
	else if (state == CANE_ITEM)
	{
		ani = CANE_ITEM;
	}
	else if (state == KNIFE_ITEM)
	{
		ani = KNIFE_ITEM;
	}
	else if (state == HEART_SMALL_ITEM)
	{
		ani = HEART_SMALL_ITEM;
	}
	else if (state == HOLY_WATER_ITEM)
	{
		ani = HOLY_WATER_ITEM;
	}

	if (state != SOBJECT_HIDDEN && !isHidden)
	{
		animations[ani]->Render(x, y, 255);

		if (renderBBox)RenderBoundingBox();
	}
}

void CSObject::SetState(int state)
{
	CGameObject::SetState(state);
}

void CSObject::ItemStart()
{
	isAppear = true;
	appear_start = GetTickCount();
}

void CSObject::BeDestroy()
{
	/*CEffect * effect = CEffect::GetInstance();*/
	CEffect * effect = new CEffect();
	effect->SetPosition(x, y);
	effect->StartShowEffect();
	effect->SetState(STATE_DESTROY);

	CMap* map = CMap::GetInstance();
	map->PushEffect(effect);

	if (!isDestroy)
	{
		isDestroy = true;
		destroy_start = GetTickCount();

		SetState(SOBJECT_HIDDEN);
	}
}

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
	else if (state == MONEY_ITEM_1k)
	{
		right = x + BBOX_MONEY_1K_W;
		bottom = y + BBOX_MONEY_1K_H;
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
	else if (state == HEART_SMALL_ITEM)
	{
		right = x + BBOX_HEART_SMALL_W;
		bottom = y + BBOX_HEART_SMALL_H;
	}
	else if (state == HOLY_WATER_ITEM)
	{
		right = x + HOLY_WATER_WIDTH;
		bottom = y + HOLY_WATER_HEIGHT;
	}
}