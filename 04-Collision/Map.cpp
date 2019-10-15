#include "Map.h"
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"

void CMap::Load(int id, LPCWSTR filePath, int r , int g, int b)
{
	CTextures * textures = CTextures::GetInstance();
	textures->Add(id, filePath, D3DCOLOR_XRGB(r, g, b));
	LPDIRECT3DTEXTURE9 texBg = textures->Get(id);

	CSprites * sprites = CSprites::GetInstance();
	CGame* game = CGame::GetInstance();
	float x = game->getCamPos_x();
	float y = game->getCamPos_y();

	for (int i = 0; i < end_row; i++)
	{
		for (int j = 0; j < end_col; j++)
		{
			sprites->Add(i * 9000 + j, x + j * 20, y + i * 20, x + j*20 + 20, y + i * 20 + 20, texBg);
		}
	}

	
}

void CMap::Render()
{

	CSprites * sprites = CSprites::GetInstance();
	CGame* game = CGame::GetInstance();
	float x = game->getCamPos_x();
	float y = game->getCamPos_y();

	for (int i = 0; i < end_row; i++)
	{
		for (int j = 0; j < end_col; j++)
		{
			sprites->Get(i*9000 + j)->Draw(x + j*20, y + i*20);
		}
	}
}

//CMap* CMap::GetInstance()
//{
//	if (__instance == NULL) __instance = new CMap();
//	return __instance;
//}