#pragma once
#include <algorithm>
#include <d3dx9.h>
#include <d3d9.h>

#include "Map.h"
#include "Game.h"
#include "GameObject.h"
#include "Flag.h"
#include "MapManager.h"
#include "Simon.h"

class Camera
{
	static Camera* __instance;

	float cam_x;
	float pos_max;
	int pos_door;
	int id_nextMap;

	bool isFollowSimon;
	bool isAuto;
	bool isBlockBoss;
	bool nextMap;
	bool changing;

	CFlag* door;


	DWORD delay_change_time;
public:

	void Update(DWORD dt, CGameObject* simon);
	void FollowSimon(CGameObject* simon);
	void StartAuto();
	void SetDoor(LPGAMEOBJECT f) {door = (CFlag*)f; }
	void CloseDoor();

	void SetFollowSimon() { isFollowSimon = true; isAuto = false; isBlockBoss = false; }
	void SetAuto() { isAuto = true;}
	void SetNextMap() { nextMap = true; }
	void SetBlockBoss(bool block) { isBlockBoss = block; }
	void Set_idnextMap(int map) { id_nextMap = map; }
	bool GetCameraAuto() { return isAuto; }
	bool GetFollowSimon() { return isFollowSimon; }
	float GetCamX() { return cam_x; }


	static Camera* GetInstance();
	Camera() { isFollowSimon = true; }
};