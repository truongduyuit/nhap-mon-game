#include <algorithm>
#include "debug.h"
#include <random>

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
#include "Camera.h"
#include "BoardGame.h"

CSimon::CSimon()
{
	skill.push_back(5);
	life = 3;
	score = 0;
	stage = 1;

	untouchable = 0;
	alpha = 255;
	hp = SIMON_HP_START;

	CLoadResourcesHelper::LoadSprites("data\\simon\\simon_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\simon\\simon_anis.txt", this);
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);

	if (isBlock && !isBeMoving)
	{
		SetState(SIMON_STATE_IDLE);
		return;
	}
	else if (isBlock && isBeMoving)
	{
		SetState(SIMON_STATE_WALKING_RIGHT);
		vx = SIMON_WALKING_SPEED_AUTO;
	}


	if (isBeMoving && !isBlock && !onStair)
	{

		if (x - be_x > 0.5f)
		{
			vx = -SIMON_WALKING_SPEED_MOVING;
			nx = -1;
			state = SIMON_STATE_WALKING_LEFT;
		}
		else if (x - be_x < -0.5f)
		{
			vx = SIMON_WALKING_SPEED_MOVING;
			nx = 1;
			state = SIMON_STATE_WALKING_RIGHT;
		}
		else
		{
			nx = be_nx;

			moveOnStair();
			isBeMoving = false;
		}
	}


	if (!onStair) vy += SIMON_GRAVITY * dt;
	else vy = 0;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	CWeapon* weapon = CWeapon::GetInstance();
	CSkill* skill = CSkill::GetInstance();
	CMap* map = CMap::GetInstance();

	vector<LPGAMEOBJECT> coObjectFlag;
	coObjectFlag = map->Get_coObjectFlag();

	if (create_enemy_right)
	{
		if (GetTickCount() - create_time_right > CREATE_ENEMY_TIME)
		{
			create_enemy_right = false;
			create_enemy = !create_enemy;
		}
	}
	if (create_enemy_left)
	{
		if (GetTickCount() - create_time_left > CREATE_ENEMY_TIME)
		{
			create_enemy_left = false;
			create_enemy = !create_enemy;
		}
	}

	if (onStair)
	{
		for (unsigned int i = 0; i < coObjectFlag.size(); i++)
		{
			if (isOverlapping(coObjectFlag[i]))
			{
				if (coObjectFlag[i]->state < 5)
				{
					if (coObjectFlag[i]->state == -nx && onStair)
					{
						onStair = false;
						onTimeStair = false;
						if (nx > 0)
						{
							coObjectFlag[i]->nextState == 2 ? x = coObjectFlag[i]->x + 7.0f : x = coObjectFlag[i]->x - 15.0f;
						}
						else
						{
							coObjectFlag[i]->nextState == -2 ? x = coObjectFlag[i]->x + 13.0f : x = coObjectFlag[i]->x + 2.0f;
						}
						//y = coObjectFlag[i]->y - 27.0f;
						if (coObjectFlag[i]->nextState == -2) y = coObjectFlag[i]->y;

						SetState(SIMON_STATE_IDLE);
					}
				}

				else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 8)
				{
					CMapManager::GetInstance()->ChangeMap(coObjectFlag[i]->nextState);
				}
			}
		}
	}
	else
	{
		canActiveMoney1k = false;
		for (unsigned int i = 0; i < coObjectFlag.size(); i++)
		{
			if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state >= 10)
			{
				if (coObjectFlag[i]->state == 10)
				{
					if (coObjectFlag[i]->nextState == 1)
					{
						if (!create_enemy_right)
						{
							create_enemy_right = true;
							create_time_right = GetTickCount();

							random_device rd;
							mt19937 rng(rd());
							uniform_int_distribution<int> uni(0, 2);
							auto n = uni(rng);

							for (int j = 0; j <= n; j++)
							{
								createEnemy(coObjectFlag[i]->state - 10, -coObjectFlag[i]->nextState, x + SCREEN_WIDTH / 2 - j * 35, coObjectFlag[i]->state == 10 ? 148 + BOARDGAME_HEIGHT : y - 2);
							}
						}
					}
					else
					{
						if (!create_enemy_left)
						{
							create_enemy_left = true;
							create_time_left = GetTickCount();

							createEnemy(coObjectFlag[i]->state - 10, -coObjectFlag[i]->nextState, x - SCREEN_WIDTH / 2 , coObjectFlag[i]->state == 10 ? 148 + BOARDGAME_HEIGHT : y - 2);
						}
					}
				}
				else if (coObjectFlag[i]->state == 12)
				{
					if (coObjectFlag[i]->nextState == 1)
					{
						if (!create_enemy_right)
						{
							create_enemy_right = true;
							create_time_right = GetTickCount();


							createEnemy(coObjectFlag[i]->state - 10, -coObjectFlag[i]->nextState, x + SCREEN_WIDTH / 2, y + 10.0f);
						}
					}
					else
					{
						if (!create_enemy_left)
						{
							create_enemy_left = true;
							create_time_left = GetTickCount();

							createEnemy(coObjectFlag[i]->state - 10, -coObjectFlag[i]->nextState, x - SCREEN_WIDTH / 2, y + 10.0f);
						}
					}
				}

				else if (coObjectFlag[i]->state == 13 && CMap::GetInstance()->Get_numFishMonster() < 2)
				{
					if (coObjectFlag[i]->nextState == -1 && !create_enemy_left)
					{
						create_enemy_left = true;
						create_time_left = GetTickCount();

						CEnemy* enemy = new CEnemy();
						enemy->SetPosition(x - 60, 148);
						enemy->SetMaxMin(50, x + 120);

						enemy->SetState(coObjectFlag[i]->state - 10);
						enemy->setNxx(coObjectFlag[i]->nextState);

						CMap* map = CMap::GetInstance();
						map->PushEnemy(enemy);
					}
					 if (coObjectFlag[i]->nextState == 1 && !create_enemy_right)
					{
						create_enemy_right = true;
						create_time_right = GetTickCount();

						CEnemy* enemy = new CEnemy();
						enemy->SetPosition(x + 60, 148);
						enemy->SetMaxMin(50, x + 120);

						enemy->SetState(coObjectFlag[i]->state - 10);
						enemy->setNxx(coObjectFlag[i]->nextState);

						CMap* map = CMap::GetInstance();
						map->PushEnemy(enemy);
					}

				}
			}

			else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 0)
			{
				if (Camera::GetInstance()->GetFollowSimon() && !isJump)
				{
					Camera::GetInstance()->SetDoor(coObjectFlag[i]);
					Camera::GetInstance()->SetAuto();
					SetState(SIMON_STATE_IDLE);
					nx = 1;
					isBlock = true;
					isPausetime = true;
				}
				else if (Camera::GetInstance()->GetFollowSimon())
				{
					dx = 0;
					vx = 0;
				}
			}

			else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 9)
			{
				isBeMoving = false;
				Camera::GetInstance()->CloseDoor();
				Camera::GetInstance()->Set_idnextMap(coObjectFlag[i]->nextState);
			}

			else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 8 && !isJump)
			{
				CMapManager::GetInstance()->ChangeMap(coObjectFlag[i]->nextState);
				isJump = false;
				isSit = false;
				dy = 0;
			}

			else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 7)
			{
				CMap* map = CMap::GetInstance();
				if (map->Get_Boss() != NULL)
				{
					map->Get_Boss()->isActive = true;
					Camera::GetInstance()->SetBlockBoss(true);
				}
			}
		
			else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 6)
			{
				canActiveMoney1k = true;
				if (activeMoney1k) CMap::GetInstance()->ActiveMoney1K();
			}

			else if (isOverlapping(coObjectFlag[i]) && coObjectFlag[i]->state == 5)
			{
				stage = coObjectFlag[i]->nextState;
			}
		}
	}

	if (onStair && !isAttack && !isthrow && onTimeStair)
	{
		if (GetTickCount() - action_time <= 300)
		{
			if (be_nx == 1)
				dx = SIMON_AUTO_STAIR_SPEED;
			else
				dx = -SIMON_AUTO_STAIR_SPEED;
			if (be_updown == SIMON_UPSTAIR)
				dy = -SIMON_AUTO_STAIR_SPEED_Y;
			else
				dy = SIMON_AUTO_STAIR_SPEED_Y;

			this->x += dx;
			this->y += dy;
		}
		else
		{
			onTimeStair = false;
			action_time = 0;
		}
	}


	// pick
	if (GetTickCount() - action_time >= SIMON_PICK_TIME)
	{
		isPick = false;
		action_time = 0;
	}
	else if (isPick)
	{
		dx = 0;
	}

	if (isPick)
	{
		weapon->set_isHidden(true);
	}
	if (isSit) { dx = 0; }
	// attack
	if (GetTickCount() - action_time > SIMON_ATTACK_TIME)
	{
		isAttack = false;
		action_time = 0;


		weapon->SetPosTemp(x, y);
		resetAttack();
	}
	else if (isAttack)
	{
		weapon->SetPosTemp(x, y);
		if (!isJump && !isOnJump) dx = 0;
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
				skill->nx = this->nx;
				//skill->nx > 0 ? skill->SetPosition(x + 10, y + 6) : skill->SetPosition(x - 3, y + 6);
				skill->SetPosition(x, y+5);
				skill->startThrow();
			}
		}
	}

	if (isDie && GetTickCount() - action_time > SIMON_DIE_TIME) resetDie();

	if (y > 1000) resetDie();

	// Injure
	if (GetTickCount() - injure_time > SIMON_ENJURE_TIME)
	{
		if (isInJure)
		{
			StartUntouchable();
			isInJure = false;
			action_time = 0;

			if (hp <= 0)
			{
				SetState(SIMON_STATE_DIE);
				isDie = true;
				action_time = GetTickCount();
			}
		}
	}
	else if (isInJure)
	{
		if (!isBeMoving)
		{
			weapon->set_isHidden(true);
			weapon->ResetAttack();
			isJump = false;

			if (!onStair && !isBeMoving && vy != 0)
			{
				vy = -SIMON_INJURE_Y;
				nx = be_nx;
				dx = -be_nx * SIMON_INJURE_X;
			}
			else
			{
				injure_time = 0;
			}
		}
	}

	// invisible
	if (isInvisible)
	{
		if (GetTickCount() - invisible_time > SIMON_INVISIBLE_TIME)
		{
			isInvisible = false;
			invisible_time = 0;

			StartUntouchable();			
		}
	}

	if (untouchable == 1)
	{
		if (GetTickCount() - untouchable_start > untouchable_time)
		{
			untouchable_start = 0;
			untouchable = 0;
			alpha = 255;
		}
	}


	if (isJump && vy > 0)
	{
		if (!jumpingMove && !isFly)
		{
			isFly = true;
			fly_time = GetTickCount();
		}
	}

	if (isFly)
	{
		if (GetTickCount() - fly_time > SIMON_FLY_TIME)
		{
			isJump = false;
			isOnJump = true;
			isFly = false;
		}
		else
		{
			vy = 0;
			dy = 0;
		}
	}

	if (isOnJump && jumpingMove)
	{
		jump_nx ? vx = SIMON_WALKING_SPEED : vx = -SIMON_WALKING_SPEED;
	}

	for (unsigned int i = 0; i < coObjects->size(); i++)
	{

		if (dynamic_cast<CSObject *>(coObjects->at(i)) && coObjects->at(i)->state != SOBJECT_HIDDEN)
		{
			if (isOverlapping(coObjects->at(i)))
			{
				// Lụm roi
				if (coObjects->at(i)->state == CANE_ITEM)
				{
					startPick();
					weapon->IncreaseLevel();
				}

				// Lụm dao
				else if (coObjects->at(i)->state == KNIFE_ITEM)
				{
					skill->SetState(STATE_KNIFE);
					skill->SetNextState(STATE_KNIFE);
					canStop = false;
				}

				// lụm nước thánh
				else if (coObjects->at(i)->state == HOLY_WATER_ITEM)
				{
					skill->SetState(STATE_HOLY_WATER);
					skill->SetNextState(STATE_HOLY_WATER);
					canStop = false;
				}
				// lụm rìu
				else if (coObjects->at(i)->state == ACE_ITEM)
				{
					skill->SetState(STATE_ACE);
					skill->SetNextState(STATE_ACE);
					canStop = false;
				}
				else if (coObjects->at(i)->state == CROSS_ITEM)
				{
					map->Cross_Enemy();

					CEffect * effect = new CEffect();
					effect->SetPosition(Camera::GetInstance()->GetCamX(), 0);
					effect->SetState(STATE_WHITE);
					effect->StartShowEffect();
					CMap::GetInstance()->PushEffect(effect);

				}
				else if (coObjects->at(i)->state == OTHER_STUFF_1)
					isHasOtherStuff = true;
				else if (coObjects->at(i)->state == STOPWATCH_ITEM)
				{
					skill->SetState(STATE_TOPWATCH);
					canStop = true;
				}
				else if (coObjects->at(i)->state == POT_ROAST_ITEM)
				{
					hp = SIMON_HP_START;
				}
				else if (coObjects->at(i)->state == STATE_INVINCIBILITY_POTION)
				{
					startInvisible();
				}
				else if (coObjects->at(i)->state == STATE_WALL_1)
				{
					vy = 0;
				}
				else if (coObjects->at(i)->state == HEART_SMALL_ITEM)
				{
					this->skill[0] += INCREASE_SKILL_SMALL;
				}
				else if (coObjects->at(i)->state == HEART_ITEM)
				{
					this->skill[0] += INCREASE_SKILL_BIG;
				}
				else if (coObjects->at(i)->state == STATE_CRYSTAL) hp = SIMON_HP_START;
				if (coObjects->at(i)->state != STATE_WALL_1 && coObjects->at(i)->state != STATE_BLACK && coObjects->at(i)->state != STATE_WALL_2 && coObjects->at(i)->state != STATE_WALL_3)
				{
					if (coObjects->at(i)->state == MONEY_ITEM_100)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_100);
						CMap::GetInstance()->PushEffect(effect);

						score += 100;
					}
					else if (coObjects->at(i)->state == MONEY_ITEM_400)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_400);
						CMap::GetInstance()->PushEffect(effect);

						score += 400;
					}
					else if (coObjects->at(i)->state == MONEY_ITEM_700)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_700);
						CMap::GetInstance()->PushEffect(effect);

						score += 700;
					}
					else if (coObjects->at(i)->state == MONEY_ITEM_1k)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_1K);
						CMap::GetInstance()->PushEffect(effect);

						score += 1000;
					}
					coObjects->at(i)->isHidden = true;
				}
			}
		}

		if (untouchable == 0 && !isInJure && !isInvisible)
		{
			if (dynamic_cast<CEnemy *>(coObjects->at(i)))
			{
				if (isOverlapping(coObjects->at(i)))
				{
					if (coObjects->at(i)->x > x)
					{
						startInjure(1);
						weapon->ResetAttack();
					}
					else
					{
						startInjure(-1);
						weapon->ResetAttack();
					}

					if (coObjects->at(i)->state == STATE_BAT)
						coObjects->at(i)->BeDestroy();
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
			if (isJump)
			{
				isJump = false;
				isOnJump = false;
				isSit = true;
			}
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
				if (ny < 0)
				{
					if (isJump) isSit = true;
					isOnJump = false;
						basicCollision(min_tx, min_ty, nx, ny);
					//if (isOverlapping(e->obj)) basicCollision(min_tx, min_ty, nx, ny);
				}
				else
				{
					x += dx;
					y += dy;
				}
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
					skill->SetState(STATE_KNIFE);
					skill->SetNextState(STATE_KNIFE);
					canStop = false;
				}

				// lụm nước thánh
				else if (e->obj->state == HOLY_WATER_ITEM)
				{
					skill->SetState(STATE_HOLY_WATER);
					skill->SetNextState(STATE_HOLY_WATER);
					canStop = false;
				}
				// lụm rìu
				else if (e->obj->state == ACE_ITEM)
				{
					skill->SetState(STATE_ACE);
					skill->SetNextState(STATE_ACE);
					canStop = false;
				}
				else if (e->obj->state == STOPWATCH_ITEM)
				{
					skill->SetState(STATE_TOPWATCH);
					canStop = true;
				}
				else if (e->obj->state == CROSS_ITEM)
				{
					CEffect * effect = new CEffect();
					effect->SetPosition(Camera::GetInstance()->GetCamX(), 0);
					effect->SetState(STATE_WHITE);
					effect->StartShowEffect();
					CMap::GetInstance()->PushEffect(effect);

					map->Cross_Enemy();
				}
				else if (e->obj->state == HEART_ITEM)
				{
					this->skill[0] += INCREASE_SKILL_BIG;
				}
				else if (e->obj->state == HEART_SMALL_ITEM)
				{
					this->skill[0] += INCREASE_SKILL_SMALL;
				}
				else if (e->obj->state == POT_ROAST_ITEM)
				{
					hp = SIMON_HP_START;
				}
				else if (e->obj->state == OTHER_STUFF_1)
				{
					isHasOtherStuff = true;
				}
				else if (e->obj->state == STATE_INVINCIBILITY_POTION)
				{
					startInvisible();
				}
				else if (e->obj->state == STATE_CRYSTAL) hp = SIMON_HP_START;
				else if (e->obj->state != STATE_WALL_2 && e->obj->state != STATE_WALL_3)
				{
					x += dx;
					vy = 0;
				}

				if (e->obj->state != STATE_WALL_1 && e->obj->state != STATE_BLACK && e->obj->state != STATE_WALL_2 && e->obj->state != STATE_WALL_3)
				{
					if (e->obj->state == MONEY_ITEM_100)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_100);
						CMap::GetInstance()->PushEffect(effect);

						score += 100;
					}
					else if (e->obj->state == MONEY_ITEM_400)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_400);
						CMap::GetInstance()->PushEffect(effect);

						score += 400;
					}
					else if (e->obj->state == MONEY_ITEM_700)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_700);
						CMap::GetInstance()->PushEffect(effect);

						score += 700;
					}
					else if (e->obj->state == MONEY_ITEM_1k)
					{
						CEffect * effect = new CEffect();
						effect->SetPosition(x - 5.0f, y - 10.0f);
						effect->StartShowEffect();
						effect->SetState(STATE_MONEY_1K);
						CMap::GetInstance()->PushEffect(effect);

						score += 1000;
					}
					e->obj->SetState(SOBJECT_HIDDEN);
					e->obj->isHidden = true;
				}
			}

			if (untouchable == 0 && !isInJure && !isInvisible)
			{
				if (dynamic_cast<CEnemy *>(e->obj))
				{
					if (e->obj->state != STATE_BULLET)
					{
						startInjure(e->obj->x > 0 ? 1 : -1);
						weapon->ResetAttack();
					}

					if (e->obj->state == STATE_BAT)
						e->obj->BeDestroy();
				}
			}
		}

		/*		if (nx != 0) vx = 0;
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
		if (isAttack || isthrow)
		{
			if (nx > 0) be_updown == 2 ? ani = SIMON_ANI_RIGHT_UP_ATTACK : ani = SIMON_ANI_RIGHT_DOWN_ATTACK;
			else be_updown == 2 ? ani = SIMON_ANI_LEFT_UP_ATTACK : ani = SIMON_ANI_LEFT_DOWN_ATTACK;
		}
		else
		{
			if (be_nx > 0)
			{
				if (be_updown == 2) onTimeStair ? ani = SIMON_ANI_UPSTAIR_RIGHT : ani = SIMON_ANI_IDLE_UPSTAIR_RIGHT;
				else onTimeStair ? ani = SIMON_ANI_DOWNSTAIR_RIGHT : ani = SIMON_ANI_IDLE_DOWNSTAIR_RIGHT;
			}
			else
			{
				if (be_updown == 2) onTimeStair ? ani = SIMON_ANI_UPSTAIR_LEFT : ani = SIMON_ANI_IDLE_UPSTAIR_LEFT;
				else onTimeStair ? ani = SIMON_ANI_DOWNSTAIR_LEFT : ani = SIMON_ANI_IDLE_DOWNSTAIR_LEFT;
			}
		}
	}
	else if (isInJure) nx > 0 ? ani = SIMON_ANI_INJURE_RIGHT : ani = SIMON_ANI_INJURE_LEFT;
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
					if (isJump && !isAttack && !isthrow) ani = SIMON_ANI_SIT_RIGHT;
					else if (isAttack || isthrow) isSit ? ani = SIMON_ANI_SIT_ATTACK_RIGHT : ani = SIMON_ANI_ATTACK_RIGHT;
					else if (state == SIMON_STATE_WALKING_RIGHT && !isJump && !isAttack) ani = SIMON_ANI_WALKING_RIGHT;
					else if (isSit) ani = SIMON_ANI_SIT_RIGHT;
					else ani = SIMON_ANI_IDLE_RIGHT;
				}
				else
				{
					if (isJump && !isAttack && !isthrow) ani = SIMON_ANI_SIT_LEFT;
					else if (isAttack || isthrow) isSit ? ani = SIMON_ANI_SIT_ATTACK_LEFT : ani = SIMON_ANI_ATTACK_LEFT;
					else if (state == SIMON_STATE_WALKING_LEFT && !isJump && !isAttack) ani = SIMON_ANI_WALKING_LEFT;
					else if (isSit) ani = SIMON_ANI_SIT_LEFT;
					else ani = SIMON_ANI_IDLE_LEFT;
				}
			}
		}
	}

	if (isthrow && canStop) nx > 0 ? ani = SIMON_ANI_IDLE_RIGHT : ani = SIMON_ANI_IDLE_LEFT;

	if (untouchable) alpha == 255 ? alpha = 128 : alpha = 255;
	if (isInvisible) alpha = 0;
	animations[ani]->Render(x, y, alpha);

	if (renderBBox)RenderBoundingBox();

}

void CSimon::SetState(int state)
{

	CGameObject::SetState(state);

	if (isBeMoving || isInJure)
		return;

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
			isSit ? resetSit() : vx = -SIMON_WALKING_SPEED;
		}
		resetSit();
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
			isSit ? resetSit() : vx = SIMON_WALKING_SPEED;
		}

		nx = 1;
		break;
	case SIMON_STATE_ATTACK:
		if (isJump && vx != 0)
		{
			isOnJump = true;
			jumpingMove = true;
		}
		startAttack();
		break;
	case SIMON_STATE_THROW:
		startThrow();
		break;
	case SIMON_STATE_PICK:
		startPick();
		break;
	default:
		if (!isJump && !isOnJump) vx = 0;
		if (!isAttack) resetSit();
		break;
	}
}

void CSimon::startAttack()
{
	if (!isPick && !isAttack)
	{
		isAttack = true;
		resetAttack();
		action_time = GetTickCount();

		CWeapon* weapon = CWeapon::GetInstance();
		nx > 0 ? weapon->SetState(STATE_ATTACK_RIGHT) : weapon->SetState(STATE_ATTACK_LEFT);

		weapon->ResetAttack();
		weapon->SetPosTemp(x, y);
		weapon->set_isHidden(false);

		onStair ? weapon->SetNextState(1) : weapon->SetNextState(-1);
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
		jumpingMove = false;
	}
}

void CSimon::startJumpMove(bool nxx)
{
	isJump = true;
	vy = -SIMON_JUMP_SPEED_Y;
	nxx ? vx = SIMON_WALKING_SPEED : vx = -SIMON_WALKING_SPEED;
	jumpingMove = true;
	jump_nx = nxx;
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

void CSimon::startSit(bool sit)
{
	// Đang lụm, đánh, ngồi thì không cho ngồi
	if (!isPick && !isAttack && !isSit)
	{
		isSit = true;
		dx = 0;
		y += SIMON_RESET_BBOX;
	}
}

void CSimon::resetSit()
{
	if (isSit && !onStair)
	{
		upBBox();
		isSit = false;
	}
}

void CSimon::startInjure(int nxx)
{
	if (!isInJure)
	{
		hp -= SIMON_HP_INJURE;
		isInJure = true;
		injure_time = GetTickCount();

		if (!isBeMoving && !onStair) be_nx = nxx;
		resetAttack();

		untouchable_time = SIMON_UNTOUCHABLE_TIME;
	}
}

void CSimon::resetAttack()
{

	vector<int> a = { SIMON_ANI_ATTACK_RIGHT, SIMON_ANI_ATTACK_LEFT, SIMON_ANI_SIT_ATTACK_LEFT, SIMON_ANI_SIT_ATTACK_RIGHT, SIMON_ANI_RIGHT_UP_ATTACK, SIMON_ANI_RIGHT_DOWN_ATTACK, SIMON_ANI_LEFT_DOWN_ATTACK, SIMON_ANI_LEFT_UP_ATTACK };
	for (unsigned int i = 0; i < a.size(); i++)
	{
		int ani = a[i];
		animations[ani]->ResetFrame();
	}
}

void CSimon::startThrow()
{
	CSkill* skill = CSkill::GetInstance();
	if (skill->state == STATE_HIDDEN) return;

	if (canStop)
	{
		CMap* map = CMap::GetInstance();
		map->Stop_Enemy();
		isthrow = true;
	}
	else
	{
		

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
}

void CSimon::startInvisible()
{
	if (!isInvisible)
	{
		isInvisible = true;

		invisible_time = GetTickCount();
		untouchable_time = SIMON_UNTOUCHABLE_INVISIBLE_TIME;
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
			be_nx < 0 ? be_x = bx - SIMON_BBOX_WIDTH / 2 - 7.0f : be_x = bx + SIMON_BBOX_WIDTH / 2 + 2.0f;
		}
		else
		{
			be_nx == -1 ? be_x = bx + 3.0f : be_x = bx - 5.0f;
		}
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

	if (isSit && !onStair)
	{
		right = left + SIMON_SIT_BBOX_WIDTH;
		bottom = top + SIMON_SIT_BBOX_HEIGHT;
	}
	else if (isJump && state)
	{
		right = left + SIMON_SIT_BBOX_WIDTH;
		bottom = top + SIMON_SIT_BBOX_HEIGHT;
	}
	else if (state == SIMON_STATE_DIE)
	{
		right = left + SIMON_DIE_BBOX_WIDTH;
		bottom = top + SIMON_DIE_BBOX_HEIGHT;
	}
	else
	{
		right = left + SIMON_BBOX_WIDTH;
		bottom = top + SIMON_BBOX_HEIGHT;
	}
}

void CSimon::SetIsBlock(bool block)
{
	isBlock = block;
}

CSimon* CSimon::__instance = NULL;
CSimon * CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	return __instance;
}

void CSimon::createEnemy(int state, int nxx, float xx, float yy)
{
	CEnemy* enemy = new CEnemy();
	enemy->SetPosition(xx, yy);

	enemy->SetState(state);
	enemy->setNxx(nxx);

	CMap* map = CMap::GetInstance();
	map->PushEnemy(enemy);
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

void CSimon::resetDie()
{
	Camera::GetInstance()->SetFollowSimon();
	nx = 1;
	SetState(SIMON_STATE_IDLE);
	isDie = false;
	hp = SIMON_HP_START;
	onStair = false;

	CMapManager* mapManager = CMapManager::GetInstance();
	mapManager->ChangeMap(mapManager->GetMapStage());
	if (!isHasOtherStuff)
	{
		this->skill[0] = 0;

	}
	isHasOtherStuff = false;
	CWeapon::GetInstance()->SetLevel(1);

	CBoardGame::GetInstance()->resetTime();
	life - 1 > 0 ? life -= 1 : life = 3;
	vy = 0;
}