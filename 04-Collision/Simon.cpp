#include <algorithm>
#include "debug.h"


#include "Simon.h"
#include "Game.h"

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
		x += min_tx * dx + nx * 0.4f;		
		y += min_ty * dy + ny * 0.4f;

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
		if (vx < 0)
		{
			ani = SIMON_ANI_WALKING_LEFT;
		}
		else if (vx > 0)
		{
			if (isJump)
			{
				if (nx < 0)
				{
					ani = SIMON_ANI_SIT_LEFT;
				}
				else if (nx > 0)
				{
					ani = SIMON_ANI_SIT_RIGHT;
				}
				else
				{
					ani = SIMON_ANI_SIT_RIGHT;
				}
			}
			else
			{
				if (nx < 0)
				{
					ani = SIMON_ANI_WALKING_LEFT;
				}
				else if (nx > 0)
				{
					ani = SIMON_ANI_WALKING_RIGHT;
				}
			}
		}
		else
		{	
			if (this->state == SIMON_STATE_SIT)
			{
				if (nx < 0) ani = SIMON_ANI_SIT_LEFT;
				else ani = SIMON_ANI_SIT_RIGHT;
			}
			else {
				if (nx < 0) ani = SIMON_ANI_IDLE_LEFT;
				else ani = SIMON_ANI_IDLE_RIGHT;
			}
			
		}

	}

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
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_JUMP:
		if(!isJump)
		{
			vy = -SIMON_JUMP_SPEED_Y;
			isJump = true;
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