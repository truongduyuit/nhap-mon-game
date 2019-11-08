#include "LoadResource.h"
#include "Enemy.h"
#include "Ground.h"

CEnemy::CEnemy()
{
	CLoadResourcesHelper::LoadSprites("data\\enemies\\enemy_sprites.txt");
	CLoadResourcesHelper::LoadAnimations("data\\enemies\\enemy_anis.txt", this);

	nx = -1;
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += ENEMY_GRAVITY * dt;

	nx > 0 ? vx = ENEMY_WALKING_SPEED : vx = -ENEMY_WALKING_SPEED;

	if (x <= x_min)
	{
		nx = ACTION_RIGHT;
	}
	else if (x >= x_max)
	{
		nx = ACTION_LEFT;
	}

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGround *>(e->obj))
			{
				//x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
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

void CEnemy::Render()
{
	int ani;

	if (state == STATE_GHOST)
	{
		nx > 0 ? ani = ANI_GHOST_RIGHT : ani = ANI_GHOST_LEFT;
	}

	if (state != ENEMY_HIDDEN && !isHidden)
	{
		animations[ani]->Render(x, y, 255);

		if (renderBBox)RenderBoundingBox();
	}
}

void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);
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
}