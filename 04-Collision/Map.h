#pragma once
#include <Windows.h>
#include "Sprites.h"

#include <d3d9.h>
#include <d3dx9.h>

#define ID_TEX_BACKBROUND		10
#define SCREEN_WIDTH			320
#define SCREEN_HEIGHT			240

class CMap
{
	//static CMap * __instance;

	int end_col = 16;
	int end_row = 11;

public :
	void Load(int id, LPCWSTR filePath, int r, int g, int b);
	void Render();

	//static CMap * GetInstance();
};