#include "Effect.h"
#include "LoadResource.h"

CEffect::CEffect()
{
	state = STATE_DESTROY;
	isHidden = true;
	CLoadResourcesHelper::LoadSprites("data\\effects\\effect_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\effects\\effect_anis.txt", this);
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	if (this->isShow)
	{
		if (GetTickCount() - start_show > DESTROY_EFFECT_TIME)
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

	if (!isHidden)
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

void CEffect::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;


	right = left + DESTROY_EFFECT_WIDTH;
	bottom = top + DESTROY_EFFECT_HEIGHT;

}

//CEffect* CEffect::__instance = NULL;
//CEffect * CEffect::GetInstance()
//{
//	if (__instance == NULL) __instance = new CEffect();
//	return __instance;
//}