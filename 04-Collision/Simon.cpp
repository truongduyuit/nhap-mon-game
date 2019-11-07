#include <algorithm>
#include "debug.h"


#include "Simon.h"
#include "Ground.h"
#include "Flag.h"
#include "Game.h"
#include "Map.h"
#include "SObject.h"
#include "Weapon.h"
#include "Skill.h"
#include "LoadResource.h"



CSimon::CSimon()
{
	hp = SIMON_HP_START;
	state = SIMON_STATE_WALKING_RIGHT;
	nx = 1;

	isJump = false;
	isAttack = false;
	isthrow = false;

	skill.push_back(10);

	untouchable = 0;

	CLoadResourcesHelper::LoadSprites("data\\simon\\simon_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\simon\\simon_anis.txt", this);
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	if (!onStair)
	{
		vy += SIMON_GRAVITY * dt;
	}
	else
	{
		vy = 0;
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != SIMON_STATE_DIE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
	}

	CWeapon* weapon = CWeapon::GetInstance();
	CMap* map = CMap::GetInstance();

	vector<LPGAMEOBJECT> coObjectFlag;
	coObjectFlag = map->Get_coObjectFlag();

	if (onStair)
	{
		for (unsigned int i = 0; i < coObjectFlag.size(); i++)
		{
			if (isOverlapping(coObjectFlag[i]))
			{
				if (coObjectFlag[i]->state != nx && onStair)
				{
					x = coObjectFlag[i]->x - 15.0f;
					y = coObjectFlag[i]->y - 27.0f;

					onStair = false;
				}
			}
		}
	}

	//

	if (isBeMoving)
	{

		if (x > be_x)
		{
			x -= 0.5f;
			nx = -1;
			SetState(SIMON_STATE_WALKING_LEFT);
		}
		else if (x - be_x < -0.5f)
		{
			x += 0.5f;
			nx = 1;
			SetState(SIMON_STATE_WALKING_RIGHT);
		}
		else
		{
			isBeMoving = false;
			nx = be_nx;

			moveOnStair();
		}
	}

	if (onStair)
	{
		if (GetTickCount() - action_time <= 100)
		{
			if (be_nx == 1)
				x += 0.45f;			 
			if (be_updown == SIMON_UPSTAIR)
				dy = -0.45f;
		}
		else
		{
			onTimeStair = false;
		}
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

	if (isPick)
	{
		weapon->set_isHidden(true);
	}

	// attack
	if (GetTickCount() - action_time > SIMON_ATTACK_TIME)
	{
		isAttack = false;
		action_time = 0;

		weapon->set_isHidden(true);
		weapon->ResetAttack();
		weapon->SetPosTemp(x, y);
	}
	else if (isAttack)
	{
		weapon->SetPosTemp(x, y);
		nx > 0 ? weapon->SetState(STATE_ATTACK_RIGHT) : weapon->SetState(STATE_ATTACK_LEFT);
		dx = 0;
		weapon->set_isHidden(false);
	}

	// throw
	if (GetTickCount() - action_time >= SIMON_ATTACK_TIME)
	{
		isthrow = false;
		action_time = 0;
	}
	else if (isthrow)
	{
		// kết thúc sprite đánh dao mới xuất hiện
		if (animations[SIMON_ANI_ATTACK_RIGHT]->GetCurrentFrame() == 2 || animations[SIMON_ANI_ATTACK_LEFT]->GetCurrentFrame() == 2)
		{
			CSkill* skill = CSkill::GetInstance();

			if (skill->get_isHidden())
			{
				this->skill[0]--;

				skill->SetState(STATE_KNIFE);
				skill->nx = this->nx;

				if (!isJump) skill->nx > 0 ? skill->SetPosition(x + 20, y+6) : skill->SetPosition(x - 3, y+6);
				else skill->nx > 0 ? skill->SetPosition(x + 20, y+13) : skill->SetPosition(x - 3, y+13);

				skill->startThrow();
			}
		}
	}

	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	for (unsigned int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CSObject *>(coObjects->at(i)))
		{
			if (isOverlapping(coObjects->at(i)))
			{
				if (coObjects->at(i)->state == 3)
				{
					startPick();
					weapon->IncreaseLevel();					
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
			if (isJump) upBBox();
			isJump = false;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CFlag *>(e->obj))
			{
				x += dx;
				y += dy;
			}

			if (dynamic_cast<CGround *>(e->obj))
			{
				basicCollision(min_tx, min_ty, nx, ny);
				if (isOverlapping(e->obj)) basicCollision(min_tx, min_ty, nx, ny);
			}

			if (dynamic_cast<CSObject *>(e->obj) && !e->obj->get_isHidden())
			{

				// Lụm roi
				if (e->obj->state == CANE_ITEM)
				{
					startPick();
					weapon->IncreaseLevel();
				}

				// Lụm dao
				else if (e->obj->state == KNIFE_ITEM)
				{
					this->skill[0] += 5;
				}

				e->obj->SetState(SOBJECT_HIDDEN);
			}
		}

		/*if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/	
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CSimon::Render()
{
	int ani;

	if (onStair)
	{
		if (onTimeStair)
		{
			ani = SIMON_ANI_UPSTAIR_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_IDLE_UPSTAIR_RIGHT;
		}
	}
	else
	{
		if (state == SIMON_STATE_DIE)
		{
			ani = SIMON_ANI_DIE_RIGHT;
		}
		else
		{

			if (isPick)
			{
				nx > 0 ? ani = SIMON_ANI_PICK_RIGHT : ani = SIMON_ANI_PICK_LEFT;
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
						isSit ? ani = SIMON_ANI_SIT_ATTACK_RIGHT : ani = SIMON_ANI_ATTACK_RIGHT;
					}
					else if (isthrow)
					{
						isSit ? ani = SIMON_ANI_SIT_ATTACK_RIGHT : ani = SIMON_ANI_ATTACK_RIGHT;
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
						isSit ? ani = SIMON_ANI_SIT_ATTACK_LEFT : ani = SIMON_ANI_ATTACK_LEFT;
					}
					else if (isthrow)
					{
						isSit ? ani = SIMON_ANI_SIT_ATTACK_LEFT : ani = SIMON_ANI_ATTACK_LEFT;
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

	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	if (renderBBox )RenderBoundingBox();
	
}

void CSimon::SetState(int state)
{
	if (!isPick)
	{
		CGameObject::SetState(state);

		switch (state)
		{
		case SIMON_STATE_WALKING_LEFT:
			if (!isAttack)
			{
				vx = -SIMON_WALKING_SPEED;

				if (isSit)
				{
					upBBox();
					isSit = false;
				}
			}
			nx = -1;
			break;
		case SIMON_STATE_WALKING_RIGHT:
			if (onStair) moveOnStair();
			else if (!isAttack)
			{
				vx = SIMON_WALKING_SPEED;

				if (isSit)
				{
					upBBox();
					isSit = false;
				}
			}
			nx = 1;
			break;
		case SIMON_STATE_JUMP:
			startJump();
			break;
		case SIMON_STATE_ATTACK:
			startAttack();
			break;
		case SIMON_STATE_THROW:
			startThrow();
			break;
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

		vx = 0;
	}
}

void CSimon::startJump()
{
	// Đang lụm, đánh, ngồi thì không cho nhảy
	if (!isPick && !isAttack && !isJump && !isSit)
	{
		isJump = true;

		vy = -SIMON_JUMP_SPEED_Y;
	}
}

void CSimon::startPick()
{
	if (!isPick)
	{
		isPick = true;

		isAttack = false;
		isthrow = false;

		action_time = GetTickCount();

		vx = 0;
	}
}

void CSimon::startSit()
{
	// Đang lụm, đánh, ngồi thì không cho ngồi
	if (!isPick && !isAttack && !isSit)
	{
		isSit = true;

		vx = 0;
	}
}

void CSimon::startThrow()
{
	CSkill* skill = CSkill::GetInstance();

	if (!isPick && !isAttack && !isthrow && skill->get_isHidden())
	{
		if (this->skill[0] > 0)
		{
			isthrow = true;

			action_time = GetTickCount();

			vx = 0;
		}
	}
}

void CSimon::beMoving(int bnx, float bx, int updown)
{
	if (!isBeMoving)
	{
		isBeMoving = true;
		be_nx = bnx;
		be_x = bx;
		be_updown = updown;
	}
}

void CSimon::moveOnStair()
{
	if (!onStair)
	{
		onStair = true;	
	}
	if (!onTimeStair)
	{
		onTimeStair = true;
		action_time = GetTickCount();
	}
	
}

void CSimon::upBBox()
{
	if ((isSit || isJump))
	{
		y -= SIMON_RESET_BBOX;
	}
}

void CSimon::SetOnStair(bool stair)
{
	onStair = stair;
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if ((isSit || isJump))
	{
		right = left + SIMON_SIT_BBOX_WIDTH;
		bottom = top + SIMON_SIT_BBOX_HEIGHT;
	}
	else
	{
		right = left + SIMON_BBOX_WIDTH;
		bottom = top + SIMON_BBOX_HEIGHT;
	}
}

CSimon* CSimon::__instance = NULL;
CSimon * CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	return __instance;
}