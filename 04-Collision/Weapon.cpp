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
					}
				}
			}
		}

	}
}

void CWeapon::Render()
{

	int ani;

	isCol = false;

	if (state == STATE_ATTACK_RIGHT)
	{
		if (level == 1)
		{
			ani = 0;
		}
		else if (level == 2)
		{
			ani = 1;
		}
		else if (level == 3)
		{
			ani = 2;
		}
	}
	else if (state == STATE_ATTACK_LEFT)
	{
		if (level == 1)
		{
			ani = 3;
		}
		else if (level == 2)
		{
			ani = 4;
		}
		else if (level == 3)
		{
			ani = 5;
		}
	}


	if (state == STATE_ATTACK_RIGHT)
	{
		if (level != 3)
		{
			if (animations[ani]->GetCurrentFrame() == 0)
			{
				x -= 25;
				y += 2;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				x -= 33;
				y += 2;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				x -= 3;
				y += 2;
				isCol = true;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				x -= 25;
				y += 2;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 8)
			{
				x -= 33;
				y += 2;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() >= 8 && animations[ani]->GetCurrentFrame() < 12)
			{
				x -= 3;
				y += 2;
				isCol = true;
			}
		}
	}
	else if (state == STATE_ATTACK_LEFT)
	{
		if (level != 3)
		{
			if (animations[ani]->GetCurrentFrame() == 0)
			{
				x += 17;
				y += 3;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() == 1)
			{
				x += 17;
				y += 3;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() == 2)
			{
				x -= 39;
				y += 3;
				isCol = true;
			}
		}
		else
		{
			if (animations[ani]->GetCurrentFrame() < 4)
			{
				y += 3;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() >= 4 && animations[ani]->GetCurrentFrame() < 8)
			{
				y += 3;
				isCol = false;
			}
			else if (animations[ani]->GetCurrentFrame() >= 8 && animations[ani]->GetCurrentFrame() < 12)
			{
				x -= 39;
				y += 3;
				isCol = true;
			}
		}
	}
	
	int alpha = 255;
	if (untouchable) alpha = 128;
	
	if (state != STATE_HIDDEN)
	{
		animations[ani]->Render(x, y, alpha);
		//RenderBoundingBox();
	}
	
	
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
}

void CWeapon::SetDirrection(int direction)
{
	this->direction = direction;
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