#include "Skill.h"
#include "SObject.h"
#include "Simon.h"
#include "Effect.h"
#include "Enemy.h"

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

	x += dx;
	y += dy;
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
	if (state == STATE_KNIFE && !isHidden)
	{
		nx > 0 ? dx = 3 : dx = -3;
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