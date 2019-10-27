#include <algorithm>
#include "debug.h"


#include "Simon.h"
#include "Game.h"
#include "Brick.h"
#include "SObject.h"
#include "Weapon.h"
#include "Skill.h"

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


	// attack
	if (GetTickCount() - action_time >= SIMON_ATTACK_TIME)
	{
		isAttack = false;
		action_time = 0;

		weapon->set_isHidden(true);
		weapon->ResetAttack();
	}
	else
	{
		weapon->set_isHidden(false);
		weapon->SetPosition(x, y);
		nx > 0 ? weapon->SetState(STATE_ATTACK_RIGHT) : weapon->SetState(STATE_ATTACK_LEFT);
		dx = 0;
	}

	// pick
	if (GetTickCount() - action_time >= SIMON_PICK_TIME)
	{
		isPick = false;
		action_time = 0;
	}
	else
	{
		dx = 0;
	}


	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	for (int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CSObject *>(coObjects->at(i)))
		{
			if (isOverlapping(coObjects->at(i)))
			{
				if (coObjects->at(i)->state == 3)
				{
					SetState(SIMON_STATE_PICK);
					weapon->IncreaseLevel();
					weapon->SetState(STATE_HIDDEN);					
				}

				coObjects->at(i)->SetState(SOBJECT_HIDDEN);
			}
		}
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick *>(e->obj))
			{
				if (isOverlapping(e->obj))
				{
					y = e->obj->y - 31;
				}
			}

			if (dynamic_cast<CSObject *>(e->obj))
			{

				// Lụm roi
				if (e->obj->state == CANE_ITEM)
				{
					startPick();
					weapon->IncreaseLevel();
				}

				// Lụm giao
				else if (e->obj->state == KNIFE_ITEM)
				{
					this->skill[0] += 5;
				}

				e->obj->SetState(SOBJECT_HIDDEN);
			}
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick *>(e->obj))
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (isOverlapping(e->obj))
				{
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

				}
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

		CWeapon* weapon = CWeapon::GetInstance();
		CSkill* skill = CSkill::GetInstance();
		
		if (isPick)
		{
			nx > 0 ? ani = SIMON_ANI_PICK_RIGHT: ani = SIMON_ANI_PICK_LEFT;
			untouchable = true;
		}
		else
		{
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
				else if (isSit)
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
				else if (isSit)
				{
					ani = SIMON_ANI_SIT_LEFT;
				}
				else
				{
					ani = SIMON_ANI_IDLE_LEFT;
				}
			}
		}
	}


	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	//RenderBoundingBox();
	
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	if (!isPick)
	{
		switch (state)
		{
		case SIMON_STATE_WALKING_LEFT:
			if (!isAttack)
			{
				vx = -SIMON_WALKING_SPEED;
				isSit = false;
			}
			nx = -1;
			break;
		case SIMON_STATE_WALKING_RIGHT:
			if (!isAttack)
			{
				vx = SIMON_WALKING_SPEED;
				isSit = false;
			}
			nx = 1;
			break;
		case SIMON_STATE_JUMP:
			startJump();
			break;
		case SIMON_STATE_ATTACK:
			startAttack();
			break;
		/*case SIMON_STATE_THROW:
			startThrow();
			break;*/
		case SIMON_STATE_PICK:
			startPick();
			break;
		case SIMON_STATE_SIT:
			startSit();
			break;
		default:
			vx = 0;
			break;
		}
	}
}

void CSimon::startAttack()
{
	if (!isPick && !isAttack)
	{
		isAttack = true;
		action_time = GetTickCount();
		canAction = false;

		vx = 0;
	}
}

void CSimon::startJump()
{
	if (!isPick && !isAttack && !isJump)
	{
		isJump = true;
		canAction = true;

		vy = -SIMON_JUMP_SPEED_Y;
	}
}

void CSimon::startPick()
{
	isPick = true;
	action_time = GetTickCount();

	vx = 0;
}

void CSimon::startSit()
{
	if (!isPick && !isAttack && !isSit)
	{
		isSit = true;

		vx = 0;
	}
}

void CSimon::startThrow()
{
	if (!isPick && !isAttack && !isthrow)
	{
		isthrow = true;

		vx = 0;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}