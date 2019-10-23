#include "Skill.h"
#include "SObject.h"
#include "Simon.h"

void CSkill::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (GetTickCount() - timethrow_start > TIME_THROW)
	{
		state = STATE_HIDDEN;

		timethrow_start = 0;
		intimethrow = 0;
	}
	else
	{
		x += 2;
	}

	if (intimethrow == 0)
	{
		StartThrow();
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

			if (dynamic_cast<CSObject *>(e->obj))
			{
				if (e->obj->GetState() == BIG_CANDLE)
				{
					int nextState = e->obj->GetNextState();
					e->obj->SetState(nextState);

					this->SetState(STATE_HIDDEN);
				}
			}
		}
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{

		if (dynamic_cast<CSObject *>(coObjects->at(i)))
		{
			if (isOverlapping(coObjects->at(i)))
			{
				if (coObjects->at(i)->GetState() == 1)
				{

					int nextState = coObjects->at(i)->GetNextState();
					coObjects->at(i)->SetState(nextState);

					this->SetState(STATE_HIDDEN);
				}
			}
		}

	}
}

void CSkill::Render()
{
	int ani;

	if (nx < 0)
	{
		ani = 0;
	}
	else if (nx > 0)
	{
		ani = 0;
	}
	
	if (state != STATE_HIDDEN)
	{
		animations[ani]->Render(x, y);
		RenderBoundingBox();
	}
}

void CSkill::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == STATE_KNIFE)
	{
		right = x + KNIFE_WIDTH;
		bottom = y + KNIFE_HEIGHT;
	}
}

void CSkill::SetState(int state)
{
	this->state = state;
}

CSkill* CSkill::__instance = NULL;
CSkill * CSkill::GetInstance()
{
	if (__instance == NULL) __instance = new CSkill();
	return __instance;
}