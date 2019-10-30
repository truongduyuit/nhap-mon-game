#include "Weapon.h"
#include "Simon.h"
#include "SObject.h"
#include "LoadResource.h"
#include "Effect.h"

CWeapon::CWeapon()
{
	level = 1;

	CLoadResourcesHelper::LoadSprites("data\\weapon\\weapon_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\weapon\\weapon_anis.txt", this);
}

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
					coObjects->at(i)->BeDestroy();
				}
			}
		}

	}
}

void CWeapon::Render()
{

	int ani;

	x = xx;
	y = xy;

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
				x -= 8;
				y += 6;
				frame = 0;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				x -= 16;
				y += 6;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				x += 15;
				y += 6;
				frame = 2;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				x -= 8;
				y += 6;
				frame = 0;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 8)
			{
				x -= 17;
				y += 6;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() >= 8 && animations[ani]->GetCurrentFrame() < 12)
			{
				x += 15;
				y += 6;
				frame = 2;
			}
		}
	}
	else if (state == STATE_ATTACK_LEFT)
	{
		if (level != 3)
		{
			if (animations[ani]->GetCurrentFrame() == 0)
			{
				x += 15;
				y += 6;
				frame = 0;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				x += 15;
				y += 6;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				x -= 20;
				y += 6;
				frame = 2;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				x += 15;
				y += 6;
				frame = 0;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 7)
			{
				x += 15;
				y += 6;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() >= 7 && animations[ani]->GetCurrentFrame() < 12)
			{
				x -= 40;
				y += 6;
				frame = 2;
			}
		}
	}
	
	int alpha = 255;
	//if (untouchable) alpha = 128;
	
	if (!isHidden)
	{
		animations[ani]->Render(x, y, alpha);
		RenderBoundingBox();
	}
}

void CWeapon::ResetAttack()
{
	for (int i = 0; i < animations.size(); i++)
	{
		animations[i]->ResetFrame();
	}
}

void CWeapon::SetPositionTemp(float x, float y)
{
	xx = x;
	xy = y;
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
		if (frame == 0)
		{
			right = left + WEAPON_BBOX_FRAME_1_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_1_HEIGHT;
		}
		else if (frame == 1)
		{
			right = left + WEAPON_BBOX_FRAME_2_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_2_HEIGHT;
		}
		else
		{
			right = left + WEAPON_BBOX_FRAME_3_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_3_HEIGHT;
		}
	}
	else
	{
		if (frame == 0)
		{
			right = left + WEAPON_BBOX_FRAME_1_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_1_HEIGHT;
		}
		else if (frame == 1)
		{
			right = left + WEAPON_BBOX_FRAME_2_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_2_HEIGHT;
		}
		else
		{
			right = left + WEAPON_BBOX_FRAME_3_3_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_3_3_HEIGHT;
		}
	}

}

CWeapon* CWeapon::__instance = NULL;
CWeapon * CWeapon::GetInstance()
{
	if (__instance == NULL) __instance = new CWeapon();
	return __instance;
}