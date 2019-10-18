#include <algorithm>
#include "debug.h"


#include "Simon.h"
#include "Game.h"
#include "Brick.h"
#include "SObject.h"
#include "Weapon.h"

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vy += SIMON_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != SIMON_STATE_DIE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
	}

	CWeapon* weapon = CWeapon::GetInstance();
	if (isAttack)
	{
		weapon->SetPosition(x+SIMON_BBOX_WIDTH-2, y+6);
	}
	else
	{
		weapon->SetPosition(-100, 100);
	}

	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (ny < 0)
		{
			isJump = false;
		}

		// block 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick *>(e->obj))
			{
				x += min_tx * dx + nx * 0.4f;
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

void CSimon::Render()
{
	int ani;
	

	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_DIE_RIGHT;
	}
	else
	{
		DWORD end_time = GetTickCount();
		if (end_time > action_time)
		{
			isAttack = false;
			action_time = 0;
		}

		if (nx > 0)
		{
			if (isJump && !isAttack)
			{
				ani = SIMON_ANI_SIT_RIGHT;
			}
			else if (isAttack)
			{
				ani = SIMON_ANI_ATTACK_RIGHT;
			}
			else if (state == SIMON_STATE_WALKING_RIGHT && !isJump && !isAttack)
			{
				ani = SIMON_ANI_WALKING_RIGHT;
			}
			else if (state == SIMON_STATE_SIT)
			{
				ani = SIMON_ANI_SIT_RIGHT;
			}
			else
			{
				ani = SIMON_ANI_IDLE_RIGHT;
			}
		}
		else
		{
			if (isJump && !isAttack)
			{
				ani = SIMON_ANI_SIT_LEFT;
			}
			else if (isAttack)
			{
				ani = SIMON_ANI_ATTACK_LEFT;
			}
			else if (state == SIMON_STATE_WALKING_LEFT && !isJump && !isAttack)
			{
				ani = SIMON_ANI_WALKING_LEFT;
			}
			else if (state == SIMON_STATE_SIT)
			{
				ani = SIMON_ANI_SIT_LEFT;
			}
			else
			{
				ani = SIMON_ANI_IDLE_LEFT;
			}
		}
		

	}

	//ani = 9;

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
	
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_LEFT:
		if (!isAttack) vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		if (!isAttack) vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_JUMP:
		if(!isJump)
		{
			vy = -SIMON_JUMP_SPEED_Y;
			isJump = true;
		}
		break;
	case SIMON_STATE_ATTACK:
		if (!isAttack)
		{
			isAttack = true;
			vx = 0;
			action_time = 300 + GetTickCount();
		}
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		break;
	default:
		vx = 0;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}