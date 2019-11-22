#include "Weapon.h"
#include "Simon.h"
#include "SObject.h"
#include "LoadResource.h"
#include "Effect.h"
#include "Enemy.h"

CWeapon::CWeapon()
{
	level = 3;
	x = -100;
	y = -100;
	isHidden = true;
	CLoadResourcesHelper::LoadSprites("data\\weapon\\weapon_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\weapon\\weapon_anis.txt", this);
}


void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	UpdatePositionWithSimon();

	frame == 3 ? isCol = true : isCol = false;

	int ani = GetCurrentAnimation();

	if (animations[ani]->get_countAni() > 0)
	{
		animations[ani]->ResetFrame();
		animations[ani]->set_countAni(0);
		isHidden = true;
	}

	if (!isHidden && isCol)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{

			if (dynamic_cast<CSObject *>(coObjects->at(i)))
			{
				if (isOverlapping(coObjects->at(i)))
				{
					/*if (coObjects->at(i)->GetState() == BIG_CANDLE || coObjects->at(i)->GetState() == SMALL_CANDLE || coObjects->at(i)->GetState() == STATE_WALL_1)
					{
						coObjects->at(i)->BeDestroy();
					}*/

					coObjects->at(i)->BeDestroy();
				}
			}

			if (dynamic_cast<CEnemy *>(coObjects->at(i)))
			{
				if (isOverlapping(coObjects->at(i)))
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
	ani = GetCurrentAnimation();
	
	int alpha = 255;
	//if (untouchable) alpha = 128;
	
	if (!isHidden)
	{
		animations[ani]->Render(x, y, alpha);
		if (renderBBox) RenderBoundingBox();
	}
}

void CWeapon::ResetAttack()
{
	for (unsigned int i = 0; i < animations.size(); i++)
	{
		animations[i]->ResetFrame();
	}
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
}

int CWeapon::GetCurrentAnimation()
{
	int ani;

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

	return ani;
}

void CWeapon::SetPosTemp(float xt, float yt)
{
	xx = xt;
	xy = yt;
}

void CWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level != 3)
	{
		if (frame == 1)
		{
			right = left + WEAPON_BBOX_FRAME_1_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_1_HEIGHT;
		}
		else if (frame == 2)
		{
			right = left + WEAPON_BBOX_FRAME_2_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_2_HEIGHT;
		}
		else if (frame == 3)
		{
			right = left + WEAPON_BBOX_FRAME_3_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_3_HEIGHT;
		}
		else
		{
			right = left;
			bottom = top;
		}
	}
	else
	{
		if (frame == 1)
		{

			right = left + WEAPON_BBOX_FRAME_1_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_1_HEIGHT;
		}
		else if (frame == 2)
		{
			right = left + WEAPON_BBOX_FRAME_2_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_2_HEIGHT;
		}
		else if (frame == 3)
		{
			right = left + WEAPON_BBOX_FRAME_3_3_WIDTH;
			bottom = top + WEAPON_BBOX_FRAME_3_3_HEIGHT;
		}
		else
		{
			right = left;
			bottom = top;
		}
	}
}

CWeapon* CWeapon::__instance = NULL;
CWeapon * CWeapon::GetInstance()
{
	if (__instance == NULL) __instance = new CWeapon();
	return __instance;
}

void CWeapon::UpdatePositionWithSimon()
{
	int ani;
	ani = GetCurrentAnimation();

	x = xx;
	y = xy;

	if (animations[ani]->GetCurrentFrame() == -1)
	{
		x = -100;
		y = -70;
	}

	if (state == STATE_ATTACK_RIGHT)
	{
		if (level != 3)
		{
			if (animations[ani]->GetCurrentFrame() == 0)
			{
				x -= 10;
				y += 7;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				x -= 9;
				y += 7;
				frame = 2;
			}
			else if (animations[ani]->GetCurrentFrame() >= 2)
			{
				nextState == 1 ? x += 22 : x += 15;
				y += 7;
				frame = 3;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				frame = 0;
				x -= 10;
				y += 8;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 7)
			{
				x -= 15;
				y += 7;
				frame = 2;
			}
			else if (animations[ani]->GetCurrentFrame() >= 7)
			{
				nextState == 1 ? x += 22 : x += 15;
				y += 8.5f;
				frame = 3;
			}
		}
	}
	else if (state == STATE_ATTACK_LEFT)
	{
		if (level != 3)
		{
			 if (animations[ani]->GetCurrentFrame() == 0)
			{
				nextState == 1 ? x += 22 : x += 15;
				y += 8;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				nextState == 1 ? x += 22 : x += 15;
				y += 8;
				frame = 2;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				x -= 20;
				y += 6;
				frame = 3;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				nextState == 1 ? x += 22 : x += 15;
				frame = 0;
				y += 8;
				frame = 1;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 7)
			{
				nextState == 1 ? x += 22 : x += 15;
				y += 8;
				frame = 2;
			}
			else if (animations[ani]->GetCurrentFrame() >= 7)
			{
				x -= 37;
				y += 8;
				frame = 3;
			}
		}
	}
}
