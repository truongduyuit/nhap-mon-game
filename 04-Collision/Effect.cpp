#include "Effect.h"
#include "LoadResource.h"
#include "SObject.h"
#include "Map.h"

CEffect::CEffect()
{
	state = STATE_DESTROY;
	isHidden = true;
	item = -1;
	timeE = DESTROY_EFFECT_TIME;
	alpha = 255;
	CLoadResourcesHelper::LoadSprites("data\\effects\\effect_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\effects\\effect_anis.txt", this);
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (item != -1 && isHidden == false)
	{
		CSObject* obj = new CSObject();
		obj->SetState(item);
		obj->SetNextState(item);
		obj->SetPosition(x, y);
		obj->set_isHidden(false);
		obj->BeDestroy();
		CMap::GetInstance()->PushItem(obj);

		isHidden = true;
		isShow = false;
		return;
	}

	CGameObject::Update(dt);

	if (state == STATE_BREAKING_WALL || state == STATE_SPLASH)
	{
		vy += EFFECT_GRAVITY * dt;
	}

	if (this->isShow)
	{
		x += dx;
		y += dy;

		if (GetTickCount() - start_show > timeE)
		{
			if (state == STATE_BOSS_1_DIE)
			{
				CSObject* obj = new CSObject();
				obj->SetState(STATE_CRYSTAL);
				obj->SetNextState(STATE_CRYSTAL);
				obj->SetPosition(CMap::GetInstance()->GetPos_max() - SCREEN_WIDTH/2, Y_MOVE_BEGIN);
				obj->set_isHidden(false);
				obj->BeDestroy();
				CMap::GetInstance()->PushItem(obj);
			}
			isHidden = true;
			isShow = false;
		}		
	}
}

void CEffect::Render()
{

	int ani = 0;
	
	if (state == STATE_DESTROY) ani = STATE_DESTROY;
	else if (state == STATE_BREAKING_WALL) ani = STATE_BREAKING_WALL;
	else if (state == STATE_SPLASH) ani = STATE_SPLASH;
	else if (state == STATE_MONEY_100) ani = STATE_MONEY_100;
	else if (state == STATE_MONEY_400) ani = STATE_MONEY_400;
	else if (state == STATE_MONEY_700) ani = STATE_MONEY_700;
	else if (state == STATE_MONEY_1K) ani = STATE_MONEY_1K;
	else if (state == STATE_BOSS_1_DIE) ani = STATE_BOSS_1_DIE;
	else if (state == STATE_WHITE) ani = STATE_WHITE;
	else if (state == STATE_BE_HIT) ani = STATE_BE_HIT;

	if (!isHidden && state != STATE_HIDDEN)
	{
		if (state == STATE_WHITE)
		{
			alpha += 12;
			if (alpha > 128)
				alpha = 0;
		}
		animations[ani]->Render(x, y, alpha);
		
		//if (renderBBox) RenderBoundingBox();
	}
}



void CEffect::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == STATE_DESTROY) timeE = DESTROY_EFFECT_TIME;
	else if (state == STATE_BOSS_1_DIE) timeE = BOSS_1_DIE_TIME;
	else if (state == STATE_WHITE) 
	{
		timeE = WHITE_TIME;
		alpha = 0;
	}
	else if (state == STATE_BE_HIT) timeE = BE_HIT_TIME;
	else timeE = BREAKING_WALL_TIME;
}

void CEffect::StartShowEffect()
{
	if (isHidden)
	{
		this->isHidden = false;
		this->isShow = true;
		start_show = GetTickCount();	
	}
}

void CEffect::StartShowEffect(int itemN)
{
	if (isHidden)
	{
		this->isHidden = false;

		this->isShow = true;
		start_show = GetTickCount();

		this->item = itemN;
	}
}

void CEffect::StartShowEffect(float vxx, float vyy)
{
	if (isHidden)
	{
		this->isHidden = false;
		vx = vxx;
		vy = vyy;

		this->isShow = true;
		start_show = GetTickCount();
	}
}

void CEffect::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == STATE_DESTROY || state == STATE_BE_HIT)
	{
		right = left + DESTROY_EFFECT_WIDTH;
		bottom = top + DESTROY_EFFECT_HEIGHT;
	}
	else if (state == STATE_BREAKING_WALL)
	{
		right = left + BREAKING_WALL_WIDTH;
		bottom = top + BREAKING_WALL_HEIGHT;
	}
	else if (state == STATE_SPLASH)
	{
		right = left + SPLASH_WIDTH;
		bottom = top + SPLASH_HEIGHT;
	}
	else if (state == STATE_BOSS_1_DIE)
	{
		right = left + BOSS_1_DIE_WIDTH;
		bottom = top + BOSS_1_DIE_HEIGHT;
	}
	else if (state == STATE_WHITE)
	{
		right = left + SCREEN_WIDTH;
		bottom = top + SCREEN_HEIGHT;
	}
	// money
	else
	{
		right = left + MONEY_WIDTH;
		bottom = top + MONEY_HEIGHT;
	}
}