#include <random>

#include "LoadResource.h"
#include "Enemy.h"
#include "Camera.h"
#include "Ground.h"
#include "Map.h"
#include "Simon.h"
#include "Contands.h"
#include "SObject.h"
#include "MathHelper.h"


CEnemy::CEnemy()
{
	hp = HP_DEFAULT;
	CLoadResourcesHelper::LoadSprites("data\\enemies\\enemy_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\enemies\\enemy_anis.txt", this);
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (x < Camera::GetInstance()->GetCamX() || x > CMap::GetInstance()->GetPos_max() && state != STATE_WOLF && state != STATE_BOSS_1) isHidden = true;
	if (isHidden) return;
	if (GetTickCount() - behit_time >= BOSS_BIHIT_TIME) behit = false;
	if (behit) return;

	CGameObject::Update(dt);

	if (state != STATE_BOSS_1) vy += ENEMY_GRAVITY * dt;

	if (state == STATE_GHOST) ghost_update();
	else if (state == STATE_WOLF) wolf_update();
	else if (state == STATE_BAT) bat_update();
	else if (state == STATE_FISH_MONSTER) fish_update();
	else if (state == STATE_BULLET) bullet_update();
	else if (state == STATE_BOSS_1) boss_1_update();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (ny < 0 && !onFlag())
		{
			if (isFly)
			{
				nxx = -nxx;
				isFly = false;
			}
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGround *>(e->obj))
			{
				if (ny < 0)
				{
					y += min_ty * dy + ny * 0.2f;

					if (state == STATE_FISH_MONSTER && !isFishMove && !isShoot)
					{
						isFishMove = true;
						action_time = GetTickCount();
					}
				}
				else
				{
					y += dy;
				}

				x += dx;
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

void CEnemy::Render()
{
	int ani;

	if (state == STATE_GHOST)
	{
		nxx > 0 ? ani = ANI_GHOST_RIGHT : ani = ANI_GHOST_LEFT;
	}
	else if (state == STATE_WOLF)
	{
		if (!isActive) nxx > 0 ? ani = ANI_WOLF_LIE_RIGHT : ani = ANI_WOLF_LIE_LEFT;
		else
		{
			if (isFly) nxx > 0 ? ani = ANI_WOLF_FLY_RIGHT : ani = ANI_WOLF_FLY_LEFT;
			else nxx > 0 ? ani = ANI_WOLF_RUN_RIGHT : ani = ANI_WOLF_RUN_LEFT;
		}
	}
	else if (state == STATE_BAT) nxx > 0 ? ani = ANI_BAT_FLY_RIGHT : ani = ANI_BAT_FLY_LEFT;
	else if (state == STATE_FISH_MONSTER)
	{
		if (!isActive) nxx > 0 ? ani = ANI_FISH_INACTIVE_RIGHT : ani = ANI_FISH_INACTIVE_LEFT;
		else
		{
			if (isShoot) nxx > 0 ? ani = ANI_FISH_SHOOT_RIGHT : ani = ANI_FISH_SHOOT_LEFT;
			else if (isFishMove) nxx > 0 ? ani = ANI_FISH_WALKING_RIGHT : ani = ANI_FISH_WALKING_LEFT;
			else nxx > 0 ? ani = ANI_FISH_INACTIVE_RIGHT : ani = ANI_FISH_INACTIVE_LEFT;
		}
	}
	else if (state == STATE_BULLET) nxx > 0 ? ani = ANI_BULLET_RIGHT : ani = ANI_BULLET_LEFT;
	else if (state == STATE_BOSS_1) isActive ? ani = ANI_BOSS_1_ACTIVE : ani = ANI_BOSS_1_INACTIVE;

	if (isStop) animations[ani]->ResetFrame();

	if (state != ENEMY_HIDDEN && !isHidden)
	{
		animations[ani]->Render(x, y, 255);

		if (renderBBox)RenderBoundingBox();
	}
}

void CEnemy::BeDestroy()
{
	CEffect * effect = new CEffect();
	effect->SetPosition(x + 5.0f, y);

	if (state == STATE_BOSS_1)
	{
		//effect->StartShowEffect(STATE_CRYSTAL);
		effect->SetState(STATE_BOSS_1_DIE);
	}

	effect->StartShowEffect();

	CMap* map = CMap::GetInstance();
	map->PushEffect(effect);

	if (state == STATE_GHOST && isHidden == false)
	{
		int item = RandomItem();
		effect = new CEffect();
		effect->SetPosition(x, y);
		effect->SetState(-1);
		effect->StartShowEffect(item);
		map->PushEffect(effect);
	}

	isHidden = true;
	CSimon::GetInstance()->increaseScore(100);
}

int CEnemy::RandomItem()
{
	vector<int> items = {
		SOBJECT_HIDDEN,
		HEART_ITEM,
		//CANE_ITEM,
		SOBJECT_HIDDEN,
		//HEART_SMALL_ITEM,
		SOBJECT_HIDDEN,
		MONEY_ITEM_100,
		SOBJECT_HIDDEN,
		MONEY_ITEM_100,
		SOBJECT_HIDDEN,
		STOPWATCH_ITEM,
		SOBJECT_HIDDEN,
		//MONEY_ITEM_400,
		SOBJECT_HIDDEN
	};

	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(1, items.size());
	auto n = uni(rng);


	return items[n - 1];
}

bool CEnemy::onFlag()
{
	CMap* map = CMap::GetInstance();

	vector<LPGAMEOBJECT> coObjectFlag;
	coObjectFlag = map->Get_coObjectFlag();

	for (unsigned int i = 0; i < coObjectFlag.size(); i++)
	{
		if (isOverlapping(coObjectFlag[i]))
		{
			if (coObjectFlag[i]->nextState == -2 && coObjectFlag[i]->state == nxx) return true;
			if (coObjectFlag[i]->nextState == 2 && coObjectFlag[i]->state == -1) return true;
		}
	}

	return false;
}

void CEnemy::ghost_update()
{
	nxx > 0 ? vx = ENEMY_WALKING_SPEED : vx = -ENEMY_WALKING_SPEED;
}

void CEnemy::wolf_update()
{
	CSimon *simon = CSimon::GetInstance();
	if (x - simon->x < 100 && x - simon->x > 0)
	{
		if (!isActive)
		{
			isActive = true;
			simon->x > x ? nxx = 1 : nxx = -1;
		}
	}

	if (isActive)
	{
		if (!isFly) nxx > 0 ? vx = WOLF_RUN_SPEED : vx = -WOLF_RUN_SPEED;
		else nxx > 0 ? vx = WOLF_FLY_SPEED : vx = -WOLF_FLY_SPEED;
	}

	if (isActive && !isFly)
	{
		if (onFlag())
		{
			isFly = true;
			vy = -0.03f;
		}
	}
}

void CEnemy::bat_update()
{

	if (!isActive)
	{
		isActive = true;
		action_time = GetTickCount();
		vyy = BAT_FLY_SPEED_Y;
	}
	else
	{
		nxx > 0 ? vx = BAT_FLY_SPEED : vx = -BAT_FLY_SPEED;


		if (GetTickCount() - action_time > 1000)
		{
			vyy = -vyy;
			action_time = GetTickCount();
		}

		vy = vyy;
	}
}

void CEnemy::fish_update()
{

	if (!isActive)
	{
		vy = -FISH_INATIVE;

		if (y < 161 && vy < 0 && !splash)
		{
			splash = true;

			CMap* map = CMap::GetInstance();
			CEffect * effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(-0.0295f, -0.025f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);


			effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(0.0295f, -0.025f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);

			effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(-0.0095f, -0.055f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);

			effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(0.0095f, -0.055f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);
		}

		if (y < x_min)
		{
			isActive = true;
			nxx = nx;

			random_device rd;
			mt19937 rng(rd());
			uniform_int_distribution<int> uni(15, 20);
			auto time = uni(rng);
			move_time = time * 100;
		}
	}
	else
	{
		vy = FISH_INATIVE/2;
		if (y > 141 && dy > 0 && splash)
		{
			splash = false;

			CMap* map = CMap::GetInstance();
			CEffect * effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(-0.0295f, -0.025f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);


			effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(0.0295f, -0.025f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);

			effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(-0.0095f, -0.055f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);

			effect = new CEffect();
			effect->SetPosition(x, y);
			effect->StartShowEffect(0.0295f, -0.055f);
			effect->SetState(STATE_SPLASH);
			map->PushEffect(effect);

			isHidden = true;
		}
		if (isFishMove)
		{
			nxx > 0 ? vx = FISH_WALKING_SPEED : vx = -FISH_WALKING_SPEED;

			if (GetTickCount() - action_time > move_time)
			{
				isFishMove = false;
				isShoot = true;
				action_time = GetTickCount();

				CEnemy* ene = new CEnemy();
				ene->SetState(STATE_BULLET);
				ene->nx = nxx;
				nxx > 0 ? ene->SetPosition(x + FISH_MONSTER_WIDTH + 2, y + 5) : ene->SetPosition(x - 2, y + 5);
				ene->set_isHidden(false);
				CMap* map = CMap::GetInstance();
				map->PushObject(ene);
			}
			else  if (x > x_max && nxx > 0)
			{
				isFishMove = false;
				isShoot = true;
				action_time = GetTickCount();
			}
			else if (x < x_max - 200 && nxx < 0)
			{
				isFishMove = false;
				isShoot = true;
				action_time = GetTickCount();
			}
		}
		else if (isShoot)
		{
			vx = 0;
			if (GetTickCount() - action_time > 800)
			{
				isFishMove = true;
				isShoot = false;
				action_time = GetTickCount();

				nxx = -nxx;

				random_device rd;
				mt19937 rng(rd());
				uniform_int_distribution<int> uni(15, 20);
				auto time = uni(rng);
				move_time = time * 100;
			}
		}
	}
}

void CEnemy::bullet_update()
{
	nxx = nx;
	nxx > 0 ? vx = SHOOT_SPEED : vx = -SHOOT_SPEED;
	vy = 0;

	CSimon * simon = CSimon::GetInstance();
	if (x < simon->x - SCREEN_WIDTH || x > simon->x + SCREEN_WIDTH) isHidden = true;
}

void CEnemy::boss_1_update()
{
	Camera* camera = Camera::GetInstance();
	CMap* map = CMap::GetInstance();

	if (!isActive) 
	{
		xOld = x;
		xMove = x - 70;
		yOld = y;
		yMove = y + 50;
	}
	else
	{		
		if (!waiting)
		{
			xMove = CMathHelper::RandomInt(camera->GetCamX() + SPACE_ERROR_MIN, map->GetPos_max() - 2*SPACE_ERROR_MIN);
			yMove = CMathHelper::RandomInt(Y_MOVE_BEGIN, Y_MOVE_END);

			waiting = true;
			moving_straight = true;
		}
		else if (moving_straight)
		{
			boss_move_straight();
		}
		
		if (moving_bezier)
		{
			boss_move_bezier();
		}
	}

	if (x < camera->GetCamX())
		x = camera->GetCamX() + 0.1f;
	else if (x >map->GetPos_max() - 2 * SIMON_BBOX_WIDTH)
		x = map->GetPos_max() - 2 * SIMON_BBOX_WIDTH - 8.1f;
}

void CEnemy::boss_move_straight()
{
	xOld = x;
	yOld = y;

	vx = (xMove - xOld) / 1000;
	vy = (yMove - yOld) / 1000;

	if (abs(x - xMove) < 2.0f && abs(y - yMove) < 2.0f)
	{
		moving_straight = false;
		moving_bezier = true;

		CSimon::GetInstance()->GetPosition(xSimon, ySimon);
		Camera* camera = Camera::GetInstance();
		CMap* map = CMap::GetInstance();

		if (x < xSimon) xMove = CMathHelper::RandomInt(xSimon + SPACE_ERROR_MIN, map->GetPos_max() - 2*SPACE_ERROR_MIN);
		else xMove = CMathHelper::RandomInt(camera->GetCamX() + 2*SPACE_ERROR_MIN, xSimon - SPACE_ERROR_MIN);
		yMove = CMathHelper::RandomInt(Y_MOVE_BEGIN, ySimon + SIMON_BBOX_HEIGHT + SPACE_ERROR_MIN);

		bezier_time = 0.01f;
	}
}

void CEnemy::boss_move_bezier()
{
	if (bezier_time < 1)
	{
		float xNew = CMathHelper::GetBezier3P(xOld, xSimon + SIMON_BBOX_HEIGHT, xMove, bezier_time);
		float yNew = CMathHelper::GetBezier3P(yOld, ySimon + SIMON_BBOX_HEIGHT, yMove, bezier_time);
		vx = (xNew - xOld) / 1000;
		vy = (yNew - yOld) / 1000;

		bezier_time += JUMP_TIME_BEZIER;
	}
	else
	{
		moving_bezier = false;
		waiting = false;
	}
}

void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == STATE_BOSS_1)
		hp = HP_BOSS;
}

void CEnemy::be_hit()
{
	if (!behit)
	{
		
		hp -= HIT_HP;
		OutputDebugString(L"a");
		if (hp <= 0) BeDestroy();
		else
		{
			CMap* map = CMap::GetInstance();
			map->PushEffect(x + BOSS_1_WIDTH / 2, y + BOSS_1_HEIGHT / 2 - 3.0f);
		}
		behit = true;
		behit_time = GetTickCount();
	}
}

void CEnemy::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == STATE_GHOST)
	{
		right = left + GHOST_WIDTH;
		bottom = top + GHOST_HEIGHT;
	}
	else if (state == STATE_WOLF)
	{
		right = left + WOLF_WIDTH;
		bottom = top + WOLF_HEIGHT;
	}
	else if (state == STATE_BAT)
	{
		right = left + BAT_WIDTH;
		bottom = top + BAT_HEIGHT;
	}
	else if (state == STATE_FISH_MONSTER)
	{
		right = left + FISH_MONSTER_WIDTH;
		bottom = top + FISH_MONSTER_HEIGHT;
	}
	else if (state == STATE_BULLET)
	{
		right = left + BULLET_WIDTH;
		bottom = top + BULLET_HEIGHT;
	}
	else if (state == STATE_BOSS_1)
	{
		right = left + BOSS_1_WIDTH;
		bottom = top + BOSS_1_HEIGHT;
	}
}