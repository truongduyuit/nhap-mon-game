#include <windows.h>
#include<fstream>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Map.h"

#include "LoadResource.h"
#include "Contands.h"

#include "Ground.h"
#include "Simon.h"
#include "SObject.h"
#include "Weapon.h"
#include "Skill.h"

#define WINDOW_CLASS_NAME L"Window"
#define MAIN_WINDOW_TITLE L"Game Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 420
#define SCREEN_HEIGHT 300

#define MAX_FRAME_RATE 120

CGame *game;
CSimon *simon;
CSObject *sobject;
CSkill *skill;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_D:
		if (!simon->get_isAttack() && !simon->get_isPick())
		{
			simon->SetState(SIMON_STATE_ATTACK);
		}
		break;
	case DIK_A:
		if (!simon->get_isAttack() && !simon->get_isPick())
		{
			simon->SetState(SIMON_STATE_THROW);
		}
		break;
	case DIK_DOWN:
		if (!simon->get_isAttack() && !simon->get_isPick())
		{
			simon->SetState(SIMON_STATE_SIT);
		}	
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	//else if (game->IsKeyDown(DIK_D))
	//	simon->SetState(SIMON_STATE_ATTACK);
	//else if (game->IsKeyDown(DIK_A))
	//	simon->SetState(SIMON_STATE_THROW);
	//else if (game->IsKeyDown(DIK_DOWN))
	//	simon->SetState(SIMON_STATE_SIT);
	else
		simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	textures->LoadAllTextures();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texBg = textures->Get(ID_TEX_BACKBROUND);
	LPDIRECT3DTEXTURE9 texObj = textures->Get(ID_TEX_OBJECT);
	LPDIRECT3DTEXTURE9 texBigCandle = textures->Get(ID_TEXT_BIG_CANDLE);
	LPDIRECT3DTEXTURE9 texSmallCandle = textures->Get(ID_TEX_SMALL_CANDLE);
	LPDIRECT3DTEXTURE9 texWeapon = textures->Get(ID_TEX_WEAPON_RIGHT);
	LPDIRECT3DTEXTURE9 texWeapon2 = textures->Get(ID_TEX_WEAPON_LEFT);
	LPDIRECT3DTEXTURE9 texHeart = textures->Get(ID_TEX_BIG_HEART);
	LPDIRECT3DTEXTURE9 texCane = textures->Get(ID_TEX_WHIP);
	LPDIRECT3DTEXTURE9 texKnife = textures->Get(ID_TEX_KNIFE_RIGHT);
	LPDIRECT3DTEXTURE9 texKnifeLeft = textures->Get(ID_TEX_KNIFE_LEFT);



	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	/*===========================================================*/

	sprites->Add(91001, 0, 0, 16, 32, texBigCandle);
	sprites->Add(91002, 16, 0, 32, 32, texBigCandle);

	sprites->Add(91011, 0, 0, 8, 16, texSmallCandle);
	sprites->Add(91012, 8, 0, 16, 16, texSmallCandle);

	sprites->Add(91021, 0, 0, 16, 16, texHeart);
	sprites->Add(91031, 0, 0, 16, 16, texCane);

	sprites->Add(91041, 0, 0, 16, 9, texKnife);
	sprites->Add(91042, 0, 0, 16, 9, texKnifeLeft);

	sprites->Add(92021, 0, 4, 9, 27, texWeapon);
	sprites->Add(92022, 21, 0, 37, 20, texWeapon);
	sprites->Add(92023, 47, 3, 70, 11, texWeapon);
	
	sprites->Add(92031, 0, 33, 9, 58, texWeapon);
	sprites->Add(92032, 21, 30, 37, 50, texWeapon);
	sprites->Add(92033, 47, 36, 70, 42, texWeapon);

	sprites->Add(92041, 0, 63, 9, 88, texWeapon);
	sprites->Add(92042, 0, 93, 9, 118, texWeapon);
	sprites->Add(92043, 0, 123, 9, 148, texWeapon);
	sprites->Add(92044, 0, 153, 9, 178, texWeapon);
	sprites->Add(92045, 21, 60, 37, 80, texWeapon);
	sprites->Add(92046, 21, 90, 37, 110, texWeapon);
	sprites->Add(92047, 21, 120, 37, 140, texWeapon);
	sprites->Add(92048, 21, 150, 37, 170, texWeapon);
	sprites->Add(92049, 47, 66, 87, 72, texWeapon);
	sprites->Add(92050, 47, 96, 87, 102, texWeapon);
	sprites->Add(92051, 47, 126, 87, 132, texWeapon);
	sprites->Add(92052, 47, 156, 87, 162, texWeapon);

	sprites->Add(92121, 77, 4, 86, 27, texWeapon2);
	sprites->Add(92122, 50, 0, 66, 20, texWeapon2);
	sprites->Add(92123, 16, 3, 39, 11, texWeapon2);

	sprites->Add(92131, 77, 33, 86, 58, texWeapon2);
	sprites->Add(92132, 50, 30, 66, 50, texWeapon2);
	sprites->Add(92133, 16, 36, 39, 42, texWeapon2);

	sprites->Add(92141, 77, 63, 86, 88, texWeapon2);
	sprites->Add(92142, 77, 93, 86, 118, texWeapon2);
	sprites->Add(92143, 77, 123, 86, 148, texWeapon2);
	sprites->Add(92144, 77, 153, 86, 178, texWeapon2);
	sprites->Add(92145, 50, 60, 66, 80, texWeapon2);
	sprites->Add(92146, 50, 90, 66, 110, texWeapon2);
	sprites->Add(92147, 50, 120, 66, 140, texWeapon2);
	sprites->Add(92148, 50, 150, 66, 170, texWeapon2);
	sprites->Add(92149, 0, 66, 40, 72, texWeapon2);
	sprites->Add(92150, 0, 96, 40, 102, texWeapon2);
	sprites->Add(92151, 0, 126, 40, 132, texWeapon2);
	sprites->Add(92152, 0, 156, 40, 162, texWeapon2);


	LPANIMATION ani;


#pragma region SObject

	ani = new CAnimation(100);
	ani->Add(91001);
	ani->Add(91002);
	animations->Add(911, ani);

	ani = new CAnimation(100);
	ani->Add(91011);
	ani->Add(91012);
	animations->Add(921, ani);

	ani = new CAnimation(100);
	ani->Add(91021);
	animations->Add(931, ani);

	ani = new CAnimation(100);
	ani->Add(91031);
	animations->Add(932, ani);

	ani = new CAnimation(100);
	ani->Add(91041);
	animations->Add(933, ani);

	sobject = new CSObject();
	sobject->AddAnimation(921);
	sobject->AddAnimation(911);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(90.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(2);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(921);
	sobject->AddAnimation(911);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(220.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(3);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(921);
	sobject->AddAnimation(911);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(345.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(3);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(921);
	sobject->AddAnimation(911);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(475.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(2);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(921);
	sobject->AddAnimation(911);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(600.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(4);
	objects.push_back(sobject);

#pragma endregion SObject


#pragma region Weapon

	ani = new CAnimation(100);
	ani->Add(92021);
	ani->Add(92022);
	ani->Add(92023);
	animations->Add(951, ani);

	ani = new CAnimation(100);
	ani->Add(92031);
	ani->Add(92032);
	ani->Add(92033);
	animations->Add(952, ani);

	ani = new CAnimation(100/6 -1);
	ani->Add(92041);
	ani->Add(92042);
	ani->Add(92043);
	ani->Add(92044);
	ani->Add(92045);
	ani->Add(92046);
	ani->Add(92047);
	ani->Add(92048);
	ani->Add(92049);
	ani->Add(92050);
	ani->Add(92051);
	ani->Add(92052);
	animations->Add(953, ani);

	ani = new CAnimation(100);
	ani->Add(92121);
	ani->Add(92122);
	ani->Add(92123);
	animations->Add(9511, ani);

	ani = new CAnimation(100);
	ani->Add(92131);
	ani->Add(92132);
	ani->Add(92133);
	animations->Add(9512, ani);

	ani = new CAnimation(100 / 6 - 1);
	ani->Add(92141);
	ani->Add(92142);
	ani->Add(92143);
	ani->Add(92144);
	ani->Add(92145);
	ani->Add(92146);
	ani->Add(92147);
	ani->Add(92148);
	ani->Add(92149);
	ani->Add(92150);
	ani->Add(92151);
	ani->Add(92152);
	animations->Add(9513, ani);

	CWeapon* weapon = CWeapon::GetInstance();
	weapon->AddAnimation(951);
	weapon->AddAnimation(952);
	weapon->AddAnimation(953);
	weapon->AddAnimation(9511);
	weapon->AddAnimation(9512);
	weapon->AddAnimation(9513);
	objects.push_back(weapon);

#pragma endregion Weapon

	ani = new CAnimation(100);
	ani->Add(91042);
	animations->Add(111, ani);

	ani = new CAnimation(100);
	ani->Add(91041);
	animations->Add(112, ani);

	skill = CSkill::GetInstance();
	skill->AddAnimation(111);
	skill->AddAnimation(112);

	objects.push_back(skill);

	/*===========================================================*/

	ifstream in;
	in.open("data\\background\\background_sprites.txt", ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load BackgroundSprites failed ! \n");
		return;
	}

	while (!in.eof())
	{
		int id, l, t, r, b;
		in >> id;
		in >> l;
		in >> t;
		in >> r;
		in >> b;
		sprites->Add(id, l, t, r, b, texBg);
	}

	in.close();


	/*===========================================================*/
#pragma region Simon

	in.open("data\\simon\\simon_sprites.txt", ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load SimonSprites failed ! \n");
		return;
	}

	while (!in.eof())
	{
		int id, l, t, r, b;
		in >> id;
		in >> l;
		in >> t;
		in >> r;
		in >> b;
		sprites->Add(id, l, t, r, b, texSimon);
	}

	in.close();


	in.open("data\\simon\\simon_anis.txt", ios::in);

	if (in.fail())
	{
		OutputDebugString(L"[ERROR] Load SimonAnis failed ! \n");
		return;
	}

	simon = new CSimon();

	while (!in.eof())
	{
		int time, n, ani_id;
		in >> time;
		in >> n;

		ani = new CAnimation(time);
		for (int i = 0; i < n; i++)
		{
			int id;
			in >> id;
			ani->Add(id);
		}
		
		in >> ani_id;
		animations->Add(ani_id, ani);
		simon->AddAnimation(ani_id);
	}

	in.close();


	simon->SetPosition(40.0f, 50.0f);
	objects.push_back(simon);

#pragma endregion Simon
	/*===========================================================*/

	CGround* ground = new CGround();
	ground->setSize(720.0f, 10.0f);
	ground->SetPosition(0.0f, 168.0f);
	objects.push_back(ground);
}


void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coObjectGround;
	vector<LPGAMEOBJECT> coObjectsFull;

	for (int i = 0; i < objects.size(); i++)
	{
		coObjectsFull.push_back(objects[i]);

		if (dynamic_cast<CGround *>(objects[i]))
		{
			coObjectGround.push_back(objects[i]);
		}
	
		if (dynamic_cast<CSObject *>(objects[i]))
		{
			if (objects[i]->GetState() != 1 && objects[i]->GetState() != 0)
			{
				coObjects.push_back(objects[i]);
			}
		}
		else
		{
			coObjects.push_back(objects[i]);
		}
	}



	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CSimon *>(objects[i]))
			objects[i]->Update(dt, &coObjects);

		else if (dynamic_cast<CWeapon *>(objects[i]))
		{
			objects[i]->Update(dt, &coObjectsFull);
		}

		else if (dynamic_cast<CSObject *>(objects[i]))
		{
			objects[i]->Update(dt, &coObjectGround);
		}

		else
		{
			objects[i]->Update(dt, &coObjectsFull);
		}
	}


	// Update camera to follow mario
	float cx, cy;
	simon->GetPosition(cx, cy);

	if (cx - SCREEN_WIDTH / 2 < 0)
	{
		cx = 0;
	}
	else if (cx + SCREEN_WIDTH / 2 > 32*23)
	{
		cx = 32 * 23 - SCREEN_WIDTH;
	}
	else
	{
		cx -= SCREEN_WIDTH / 2;
	}

	

	CGame::GetInstance()->SetCamPos(cx,0);
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		CSprites *sprites = CSprites::GetInstance();
	
		ifstream in;
		in.open("data\\background\\background_sprites_position.txt", ios::in);

		if (in.fail())
		{
			OutputDebugString(L"[ERROR] Load BackgroundSprites position failed ! \n");
			return;
		}

		int x = 0, y = 0;
		while (!in.eof())
		{
			int id;
			in >> id;

			sprites->Get(id)->Draw(x, y + 20.0f);

			y += 32.0f;

			if (y > 128)
			{
				x += 32;
				y = 0;
			}
		}

		in.close();

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}