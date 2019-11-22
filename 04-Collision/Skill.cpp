#include "Skill.h"
#include "SObject.h"
#include "Simon.h"
#include "Effect.h"
#include "Ground.h"
#include "Enemy.h"

CSkill::CSkill()
{
	CLoadResourcesHelper::LoadSprites("data\\skills\\skill_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\skills\\skill_anis.txt", this);
	state = STATE_KNIFE;
	nextState = STATE_KNIFE;
}

void CSkill::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (state != STATE_KNIFE)
		vy += 0.00015f * dt;
	else
		vy = 0;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();



	if (state != STATE_KNIFE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
	}

	if (state == STATE_HOLY_WATER && !isFailing)
	{
		vy = -0.03f;
		isFailing = true;
	}

	if (GetTickCount() - timethrow_start > timeshow)
	{

		set_isHidden(true);
		timethrow_start = 0;
		isFailing = false;
	}
	else
	{
		updateThrow();
	}


	if (state != STATE_HOLY_WATER && !isHidden)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CSObject *>(coObjects->at(i)))
			{
				if (isOverlapping(coObjects->at(i)))
				{
					if (coObjects->at(i)->GetState() == BIG_CANDLE || coObjects->at(i)->GetState() == SMALL_CANDLE)
					{
						// Load effect destroy
						coObjects->at(i)->BeDestroy();

						//set_isHidden(true);
						SetPosition(-100.0f, -100.0f);
					}
				}
			}

			if (dynamic_cast<CEnemy *>(coObjects->at(i)))
			{
				if (isOverlapping(coObjects->at(i)))
				{
					coObjects->at(i)->BeDestroy();

					//set_isHidden(true);
					SetPosition(-100.0f, -100.0f);
				}
			}
		}
	}

	if (coEvents.size() == 0 )
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
				isFailing = false;
				SetState(STATE_HOLY_FIRE);

				basicCollision(min_tx, min_ty, nx, ny);
				if (isOverlapping(e->obj)) basicCollision(min_tx, min_ty, nx, ny);

			}
			else
			{
				x += dx;
				y += dy;
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

void CSkill::Render()
{
	int ani;


	if (state == STATE_KNIFE)
	{
		nx < 0 ? ani = ANI_THROW_KNIFE_LEFT : ani = ANI_THROW_KNIFE_RIGHT;
	}
	else if (state == STATE_HOLY_WATER)
	{
		nx < 0 ? ani = ANI_HOLY_WATER_LEFT : ani = ANI_HOLY_WATER_RIGHT;
	}
	else if (state == STATE_HOLY_FIRE)
	{
		ani = ANI_HOLY_FIRE;
	}

	if (!isHidden)
	{
		animations[ani]->Render(x, y);
		if (renderBBox)RenderBoundingBox();
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
	else if (state == STATE_HOLY_WATER)
	{
		right = x + HOLY_WATER_SKILL_WIDTH;
		bottom = y + HOLY_WATER_SKILL_HEIGHT;
	}
	else if (state == STATE_HOLY_FIRE)
	{
		right = x + HOLY_WATER_SKILL_WIDTH;
		bottom = y + HOLY_WATER_SKILL_HEIGHT;
	}
}

void CSkill::startThrow()
{
	if (isHidden)
	{
		isHidden = false;

		timethrow_start = GetTickCount();

		SetState(nextState);
	}
}

void CSkill::updateThrow()
{
	if (state == STATE_KNIFE && !isHidden)
	{
		nx > 0 ? dx = 3 : dx = -3;
	}
	else if (state == STATE_HOLY_WATER)
	{
		nx > 0 ? vx = 0.095f : vx = -0.095f;
	}
	else if (state == STATE_HOLY_FIRE)
	{
		vx = 0;
	}
}

void CSkill::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_HOLY_WATER:
		timeshow = 10000;
	default:
		timeshow = TIME_THROW;
		break;
	}
}

CSkill* CSkill::__instance = NULL;
CSkill * CSkill::GetInstance()
{
	if (__instance == NULL) __instance = new CSkill();
	return __instance;
}