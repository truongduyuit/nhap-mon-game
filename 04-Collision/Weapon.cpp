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
			if (isOverlapping(coObjects->at(i)))
			{
				if (coObjects->at(i)->GetState() == 1)
				{

					int nextState = coObjects->at(i)->GetNextState();
					coObjects->at(i)->SetState(nextState);
				}
			}
		}

	}
}

void CWeapon::Render()
{

	int ani;

	float xx = x, xy = y;

	if (state == STATE_ATTACK_RIGHT)
	{
		if (level == 1)
		{
			ani = WEAPON_ANI_1_RIGHT;
		}
		else if (level == 2)
		{
			ani = WEAPON_ANI_2_RIGHT;
		}
		else if (level == 3)
		{
			ani = WEAPON_ANI_3_RIGHT;
		}
	}
	else if (state == STATE_ATTACK_LEFT)
	{
		if (level == 1)
		{
			ani = WEAPON_ANI_1_LEFT;
		}
		else if (level == 2)
		{
			ani = WEAPON_ANI_2_LEFT;
		}
		else if (level == 3)
		{
			ani = WEAPON_ANI_3_LEFT;
		}
	}


	if (state == STATE_ATTACK_RIGHT)
	{
		if (level != 3)
		{
			if (animations[ani]->GetCurrentFrame() == 0)
			{
				xx -= 8;
				xy += 6;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				xx -= 16;
				xy += 6;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				xx += 15;
				xy += 6;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				xx -= 8;
				xy += 6;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 8)
			{
				xx -= 17;
				xy += 6;
			}
			else if (animations[ani]->GetCurrentFrame() >= 8 && animations[ani]->GetCurrentFrame() < 12)
			{
				xx += 15;
				xy += 6;
			}
		}
	}
	else if (state == STATE_ATTACK_LEFT)
	{
		if (level != 3)
		{
			if (animations[ani]->GetCurrentFrame() == 0)
			{
				xx += 15;
				xy += 6;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				xx += 15;
				xy += 6;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				//xx -= 39;
				xy += 6;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				xy += 3;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 8)
			{
				xy += 3;
			}
			else if (animations[ani]->GetCurrentFrame() >= 8 && animations[ani]->GetCurrentFrame() < 12)
			{
				xx -= 39;
				xy += 3;
			}
		}
	}
	
	int alpha = 255;
	//if (untouchable) alpha = 128;
	
	if (!isHidden)
	{
		animations[ani]->Render(xx, xy, alpha);
		//RenderBoundingBox();
	}
}

void CWeapon::ResetAttack()
{
	for (int i = 0; i < animations.size(); i++)
	{
		animations[i]->ResetFrame();
	}
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
}

void CWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level != 3)
	{
		right = x + 24;
		bottom = y + 18;
	}
	else
	{
		right = x + 39;
		bottom = y + 18;
	}
}

CWeapon* CWeapon::__instance = NULL;
CWeapon * CWeapon::GetInstance()
{
	if (__instance == NULL) __instance = new CWeapon();
	return __instance;
}