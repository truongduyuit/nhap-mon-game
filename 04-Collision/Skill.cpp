#include "Skill.h"
#include "SObject.h"
#include "Simon.h"

CSkill::CSkill()
{
	CLoadResourcesHelper::LoadSprites("data\\skills\\skill_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\skills\\skill_anis.txt", this);
}

void CSkill::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (GetTickCount() - timethrow_start >= TIME_THROW)
	{
		set_isHidden(true);

		timethrow_start = 0;
	}
	else
	{
		updateThrow();
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
					e->obj->SetNextState(SOBJECT_HIDDEN);

					set_isHidden(true);
					SetPosition(-100.0f, -100.0f);
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
					coObjects->at(i)->SetNextState(SOBJECT_HIDDEN);

					set_isHidden(true);
					SetPosition(-100.0f, -100.0f);
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
		ani = ANI_THROW_KNIFE_LEFT;
	}
	else if (nx > 0)
	{
		ani = ANI_THROW_KNIFE_RIGHT;
	}
	
	if (!isHidden)
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

void CSkill::startThrow()
{
	if (isHidden)
	{
		isHidden = false;

		timethrow_start = GetTickCount();
	}
}

void CSkill::updateThrow()
{
	if (state == STATE_KNIFE)
	{
		nx > 0 ? dx += 3 : dx -= 3;
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