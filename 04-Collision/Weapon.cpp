#include "Weapon.h"
#include "Simon.h"
#include "SObject.h"

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

}

void CWeapon::Render()
{

	int ani;
	ani = 0;
	//if (state == STATE_ATTACK_RIGHT)
	//	ani = STATE_ATTACK_RIGHT;
	//else if (state == STATE_ATTACK_LEFT)
	//	ani = STATE_ATTACK_LEFT;

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
}

void CWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + 24;
	bottom = y + 18;
}

CWeapon* CWeapon::__instance = NULL;
CWeapon * CWeapon::GetInstance()
{
	if (__instance == NULL) __instance = new CWeapon();
	return __instance;
}