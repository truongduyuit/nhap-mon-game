#include <windows.h>
#include<fstream>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Map.h"

#include "Brick.h"
#include "Simon.h"
#include "SObject.h"
#include "Weapon.h"

#define WINDOW_CLASS_NAME L"Window"
#define MAIN_WINDOW_TITLE L"Game Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 420
#define SCREEN_HEIGHT 300

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON			0
#define ID_TEX_BACKBROUND		10
#define ID_TEX_OBJECTS			20

CGame *game;
CSimon *simon;

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
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_D:
		simon->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_DOWN:
		simon->SetState(SIMON_STATE_SIT);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_D))
		simon->SetState(SIMON_STATE_ATTACK);
	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_SIT);
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

	textures->Add(ID_TEX_SIMON, L"textures\\simon.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_BACKBROUND, L"textures\\background.png", D3DCOLOR_XRGB(255,255,255));
	textures->Add(ID_TEX_OBJECTS, L"textures\\objects.png", D3DCOLOR_XRGB(34,177,76));
	textures->Add(105, L"textures\\sprite\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	textures->Add(100, L"textures\\sprite\\ground\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texBigCandle = textures->Get(100);

	textures->Add(101, L"textures\\sprite\\ground\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texSmallCandle = textures->Get(101);

	textures->Add(102, L"textures\\weapon.png", D3DCOLOR_XRGB(255,255,255));
	LPDIRECT3DTEXTURE9 texWeapon = textures->Get(102);

	textures->Add(103, L"textures\\weapon2.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texWeapon2 = textures->Get(103);

	textures->Add(111, L"textures\\sprite\\item\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texHeart = textures->Get(111);

	textures->Add(112, L"textures\\sprite\\item\\3.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texCane = textures->Get(112);

	textures->Add(113, L"textures\\sprite\\item\\4.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texKnife = textures->Get(113);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texBg = textures->Get(ID_TEX_BACKBROUND);
	LPDIRECT3DTEXTURE9 texObj = textures->Get(ID_TEX_OBJECTS);

	/*===========================================================*/
		//brick
	sprites->Add(90000, 0, 153, 9, 160, texBg);

	sprites->Add(91001, 0, 0, 16, 32, texBigCandle);
	sprites->Add(91002, 16, 0, 32, 32, texBigCandle);

	sprites->Add(91011, 0, 0, 8, 16, texSmallCandle);
	sprites->Add(91012, 8, 0, 16, 16, texSmallCandle);

	sprites->Add(91021, 0, 0, 8, 8, texHeart);
	sprites->Add(91031, 0, 0, 16, 16, texCane);
	sprites->Add(91041, 0, 0, 16, 9, texKnife);


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
	sprites->Add(92052, 47, 1566, 87, 162, texWeapon);


	//sprites->Add(92021, 244, 9, 252, 34, texWeapon2);
	//sprites->Add(92022, 164, 7, 180, 25, texWeapon2);
	//sprites->Add(92023, 120, 8, 144, 16, texWeapon2);



	LPANIMATION ani;

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

	for (int i = 0; i < 3; i++)
	{
		CSObject *sobject = new CSObject();
		sobject->AddAnimation(921);
		sobject->AddAnimation(911);
		sobject->AddAnimation(931);
		sobject->AddAnimation(932);
		sobject->AddAnimation(933);
		sobject->SetPosition(100 + i * 100, 135);

		sobject->SetState(1);
		sobject->SetNextState(i+2);

		objects.push_back(sobject);
	}

	ani = new CAnimation(100);
	ani->Add(92021);
	ani->Add(92022);
	ani->Add(92023);
	animations->Add(951, ani);

	ani = new CAnimation(250);
	ani->Add(92031);
	ani->Add(92032);
	ani->Add(92033);
	animations->Add(952, ani);

	ani = new CAnimation(150);
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

	CWeapon* weapon = CWeapon::GetInstance();
	weapon->AddAnimation(951);
	weapon->AddAnimation(952);
	weapon->AddAnimation(953);

	weapon->SetPosition(-100, -100);
	objects.push_back(weapon);

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


	/*===========================================================*/

	ani = new CAnimation(100);		// brick
	ani->Add(90000);
	animations->Add(901, ani);

	for (int i = 0; i < 90; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(901);
		brick->SetPosition(0 + i * 8.0f, 173);
		objects.push_back(brick);
	}


}


void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coObjectsFull;

	for (int i = 0; i < objects.size(); i++)
	{
		coObjectsFull.push_back(objects[i]);
	
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