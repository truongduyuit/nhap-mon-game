#include "Weapon.h"
#include "Simon.h"
#include "SObject.h"

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	for (UINT i = 0; i < coObjects->size(); i++)
	{

		if (dynamic_cast<CSObject *>(coObjects->at(i)))
		{
			if (isCol)
			{
				if (isOverlapping(coObjects->at(i)))
				{
					if (coObjects->at(i)->GetState() == 1)
					{
						
						int nextState = coObjects->at(i)->GetNextState();
						coObjects->at(i)->SetState(nextState);

						//coObjects->at(i)->SetNextState(-1);
					}
				}
			}
		}

	}
}

void CWeapon::Render()
{

	int ani;
	ani = 2;
	level = 3;
	isCol = false;

	//if (state == STATE_ATTACK_RIGHT)
	//	ani = STATE_ATTACK_RIGHT;
	//else if (state == STATE_ATTACK_LEFT)
	//	ani = STATE_ATTACK_LEFT;

	//float wy = y;
	if (level != 3)
	{
		if (animations[ani]->GetCurrentFrame() == 0)
		{
			x -= 25;
			y += 3;
			isCol = false;
		}
		else if (animations[ani]->GetCurrentFrame() == 1)
		{
			x -= 33;
			y += 3;
			isCol = false;
		}
		else if (animations[ani]->GetCurrentFrame() == 2)
		{
			y += 3;
			isCol = true;
		}
	}
	else
	{
		if (animations[ani]->GetCurrentFrame() < 4)
		{
			x -= 25;
			y += 3;
			isCol = false;
		}
		else if (animations[ani]->GetCurrentFrame() < 8)
		{
			x -= 33;
			y += 3;
			isCol = false;
		}
		else if (animations[ani]->GetCurrentFrame() < 12)
		{
			y += 3;
			isCol = true;
		}
	}


	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);
	
	RenderBoundingBox();
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
}

void CWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + 24;
	bottom = y + 18;
}

CWeapon* CWeapon::__instance = NULL;
CWeapon * CWeapon::GetInstance()
{
	if (__instance == NULL) __instance = new CWeapon();
	return __instance;
}