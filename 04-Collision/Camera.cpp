#include "Camera.h"

void Camera::Update(DWORD dt, CGameObject* simon)
{
	if (isFollowSimon)
	{
		FollowSimon(simon);
	}
	else if (isAuto || nextMap)
	{
		StartAuto();
	}
	else if (changing)
	{
		if (GetTickCount() - delay_change_time > 1000)
		{
			CMapManager* map_manager = CMapManager::GetInstance();
			map_manager->SetStage(map_manager->GetMapStage() + 1);
			map_manager->LoadMapResources();
			map_manager->DrawMap();

			changing = false;
			isFollowSimon = true;
			CSimon::GetInstance()->SetIsBlock(false);
		}
	}
}

void Camera::FollowSimon(CGameObject* simon)
{
	// Update camera to follow simon
	float cx, cy;
	simon->GetPosition(cx, cy);

	pos_max = CMap::GetInstance()->GetPos_max();

	if (cx - SCREEN_WIDTH / 2 < 0)
	{
		cx = 0;
	}
	else if (cx + SCREEN_WIDTH / 2 > pos_max)
	{
		cx = pos_max - SCREEN_WIDTH;
	}
	else
	{
		cx -= SCREEN_WIDTH / 2;
	}


	CGame::GetInstance()->SetCamPos(cx, 0);
}

void Camera::StartAuto()
{
	if (isFollowSimon)
	{
		isFollowSimon = false;
		pos_max += SCREEN_WIDTH / 4;
	}

	if (cam_x < pos_max - SCREEN_WIDTH/2 - 40)
	{
		cam_x = CGame::GetInstance()->getCamPos_x() + 1.0f;
		CGame::GetInstance()->SetCamPos(cam_x, 0);
	}
	else
	{
		if (isAuto)
		{
			pos_max += SCREEN_WIDTH / 2 - 6;
			isAuto = false;
			door->setOpening(true);
		}
		if (nextMap)
		{
			nextMap = false;
			changing = true;
			delay_change_time = GetTickCount();
		}
	}
}

void Camera::CloseDoor()
{
	door->setClose();
}

Camera* Camera::__instance = NULL;
Camera * Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}