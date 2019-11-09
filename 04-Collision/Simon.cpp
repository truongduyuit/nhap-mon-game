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
#include "Enemy.h"



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
	CSkill* skill = CSkill::GetInstance();
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
					onTimeStair = false;
				}
			}
		}
	}

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
			else
				x -= 0.45f;

			if (be_updown == SIMON_UPSTAIR)
				dy = -0.45f;
			else
				dy = 0.45f;
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

		
		weapon->ResetAttack();
		weapon->SetPosTemp(x, y);
		weapon->set_isHidden(true);
	}
	else if (isAttack)
	{
		
		nx > 0 ? weapon->SetState(STATE_ATTACK_RIGHT) : weapon->SetState(STATE_ATTACK_LEFT);
		dx = 0;
		weapon->SetPosTemp(x, y);
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
			
			if (skill->get_isHidden())
			{
				this->skill[0]--;

				skill->SetState(STATE_KNIFE);
				skill->nx = this->nx;
				skill->nx > 0 ? skill->SetPosition(x + 10, y + 6) : skill->SetPosition(x - 3, y + 6);
				skill->startThrow();
			}
		}
	}

	// sit
	if (isSit)
	{
		if (GetTickCount() - sit_time >= SIMON_SIT_TIME)
		{
			upBBox();
			isSit = false;
			sit_time = 0;
			
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

		if (untouchable == 0)
		{
			if (dynamic_cast<CEnemy *>(coObjects->at(i)))
			{
				if (isOverlapping(coObjects->at(i)))
				{
					if (hp > 0)
					{
						StartUntouchable();
					}
					else
						state = SIMON_STATE_DIE;
				}
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

	if (onStair)
	{
		if (be_nx > 0)
		{
			if (be_updown == 2)
			{
				onTimeStair ? ani = SIMON_ANI_UPSTAIR_RIGHT : ani = SIMON_ANI_IDLE_UPSTAIR_RIGHT;
			}
			else
			{
				onTimeStair ? ani = SIMON_ANI_DOWNSTAIR_RIGHT : ani = SIMON_ANI_IDLE_DOWNSTAIR_RIGHT;
			}
		}
		else
		{
			if (be_updown == 2)
			{
				onTimeStair ? ani = SIMON_ANI_UPSTAIR_LEFT : ani = SIMON_ANI_IDLE_UPSTAIR_LEFT;
			}
			else
			{
				onTimeStair ? ani = SIMON_ANI_DOWNSTAIR_LEFT : ani = SIMON_ANI_IDLE_DOWNSTAIR_LEFT;
			}
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
					if (isJump && !isAttack && !isthrow)
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
					if (isJump && !isAttack && !isthrow)
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
			if (onStair)
			{
				if (be_nx != -1)
				{
					be_nx = -1;
					be_updown = -be_updown;
				}
				moveOnStair();
			}
			else if (!isAttack)
			{
				vx = -SIMON_WALKING_SPEED;
				
			}
			sit_time = 0;
			nx = -1;
			break;
		case SIMON_STATE_WALKING_RIGHT:
			if (onStair)
			{
				if (be_nx != 1)
				{
					be_nx = 1;
					be_updown = -be_updown;
				}
				moveOnStair();
			}
			else if (!isAttack)
			{
				vx = SIMON_WALKING_SPEED;
			}
			sit_time = 0;
			nx = 1;
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
			if (!isJump) vx = 0;
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
	// Đang lụm, đánh thì không cho nhảy
	if (!isPick && !isAttack && !isJump)
	{

		SetState(SIMON_STATE_JUMP);

		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
	}
}

void CSimon::startJumpMove(bool nxx)
{
	isJump = true;
	vy = -SIMON_JUMP_SPEED_Y;
	nxx ? vx = SIMON_WALKING_SPEED : vx = -SIMON_WALKING_SPEED;
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
	sit_time = GetTickCount();
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
		be_updown = updown;

		if (be_updown == SIMON_DOWNSTAIR)
		{
			be_nx < 0 ? be_x = bx - SIMON_BBOX_WIDTH / 2 - 3 : be_x = bx + SIMON_BBOX_WIDTH / 2 + 1;
		}
		else
			be_x = bx;
		
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

bool CSimon::get_candownstair()
{
	CMap* map = CMap::GetInstance();
	vector<LPGAMEOBJECT> coObjectFlag;
	coObjectFlag = map->Get_coObjectFlag();

	if (!onStair)
	{
		for (unsigned int i = 0; i < coObjectFlag.size(); i++)
		{
			if (isOverlapping(coObjectFlag[i]))
			{
				if (coObjectFlag[i]->nextState == SIMON_DOWNSTAIR)
				{
					return true;
				}
			}
		}
	}
	return false;
}