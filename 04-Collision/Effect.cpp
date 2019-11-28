#include "Effect.h"
#include "LoadResource.h"
#include "SObject.h"
#include "Map.h"

CEffect::CEffect()
{
	state = STATE_DESTROY;
	isHidden = true;
	item = -1;
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

		DWORD timeE;
		if (state == STATE_DESTROY) timeE = DESTROY_EFFECT_TIME;
		else timeE = BREAKING_WALL_TIME;

		if (GetTickCount() - start_show > timeE)
		{
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

	if (!isHidden && state != STATE_HIDDEN)
	{
		animations[ani]->Render(x, y);
		
		if (renderBBox) RenderBoundingBox();
	}
}



void CEffect::SetState(int state)
{
	CGameObject::SetState(state);
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

	if (state == STATE_DESTROY)
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
	// money
	else
	{
		right = left + MONEY_WIDTH;
		bottom = top + MONEY_HEIGHT;
	}
}