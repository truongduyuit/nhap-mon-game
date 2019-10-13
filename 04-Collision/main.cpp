#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Brick.h"
#include "Simon.h"
#include "Candle.h"

#define WINDOW_CLASS_NAME L"Window"
#define MAIN_WINDOW_TITLE L"Game Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

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
	case DIK_A:
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


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texBg = textures->Get(ID_TEX_BACKBROUND);
	LPDIRECT3DTEXTURE9 texObj = textures->Get(ID_TEX_OBJECTS);
	
	// walking left
	sprites->Add(10001, 123, 40, 140, 71, texSimon);
	sprites->Add(10002, 83, 40, 100, 71, texSimon);
	sprites->Add(10003, 43, 40, 60, 71, texSimon);

	// walking right
	sprites->Add(10011, 163, 40, 180, 71, texSimon);
	sprites->Add(10012, 203, 40, 220, 71, texSimon);
	sprites->Add(10013, 243, 40, 260, 71, texSimon);

	// die
	sprites->Add(10098, 83, 0, 100, 31, texSimon);
	sprites->Add(10099, 36, 0, 68, 31, texSimon);

	// idle left
	sprites->Add(10031, 123, 40, 140, 71, texSimon);

	// idle right
	sprites->Add(10032, 163, 40, 180, 71, texSimon);

	// sit left
	sprites->Add(10051, 4, 43, 20, 67, texSimon);

	// sit right
	sprites->Add(10061, 284, 43, 300, 67, texSimon);

	// jump left
	sprites->Add(10071, 84, 0, 100, 30, texSimon);

	// jump right
	sprites->Add(10081, 204, 0, 220, 30, texSimon);

	//brick
	sprites->Add(90000, 0, 153, 9, 160, texBg);


	sprites->Add(50001, 17, 30, 25, 47, texObj); 	// small candle 
	sprites->Add(50002, 31, 30, 39, 47, texObj);

	sprites->Add(50011, 48, 25, 64, 56, texObj); 	// big candle 
	sprites->Add(50012, 75, 25, 91, 56, texObj);

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(101, ani);

	ani = new CAnimation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(102, ani);

	ani = new CAnimation(200);
	ani->Add(10098);
	ani->Add(10099);
	animations->Add(99, ani);

	ani = new CAnimation(300);
	ani->Add(10031);
	animations->Add(111, ani);

	ani = new CAnimation(300);
	ani->Add(10032);
	animations->Add(112, ani);

	ani = new CAnimation(300);
	ani->Add(10051);
	animations->Add(105, ani);

	ani = new CAnimation(300);
	ani->Add(10061);
	animations->Add(105, ani);

	ani = new CAnimation(200);
	ani->Add(10071);
	ani->Add(10051);
	animations->Add(106, ani);

	ani = new CAnimation(200);
	ani->Add(10081);
	ani->Add(10061);
	animations->Add(107, ani);

	simon = new CSimon();
	simon->AddAnimation(101);	//	walking left
	simon->AddAnimation(102);	//	walking right
	simon->AddAnimation(99);	//	die
	simon->AddAnimation(111);	//	idle left
	simon->AddAnimation(112);	//	idle right
	simon->AddAnimation(105);	//	sit left
	simon->AddAnimation(106);	//	sit right
	simon->AddAnimation(107);	//	jump left
	simon->AddAnimation(107);	//	jump right

	simon->SetPosition(40.0f, 50.0f);
	objects.push_back(simon);

	/*===========================================================*/

	ani = new CAnimation(100);		// brick
	ani->Add(90000);
	animations->Add(901, ani);


	for (int i = 0; i < 20; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(901);
		brick->SetPosition(-9.0f, 150 - i*8.0f);
		objects.push_back(brick);
	}

	for (int i = 0; i < 20; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(901);
		brick->SetPosition(760.0f, 150 - i * 8.0f);
		objects.push_back(brick);
	}

	for (int i = 0; i < 95; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(901);
		brick->SetPosition(0 + i * 8.0f, 150);
		objects.push_back(brick);
	}

	/*===========================================================*/

	ani = new CAnimation(100);		// small candle
	ani->Add(50001);
	ani->Add(50002);
	animations->Add(501, ani);

	ani = new CAnimation(100);		// big candles
	ani->Add(50011);
	ani->Add(50012);
	animations->Add(511, ani);

	CCandle *candle = new CCandle();
	candle->AddAnimation(501);
	candle->AddAnimation(511);

	candle->SetPosition(85.0f, 119.0f);
	objects.push_back(candle);
}

void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow mario
	float cx, cy;
	simon->GetPosition(cx, cy);

	if (cx - SCREEN_WIDTH / 2 < 0)
	{
		cx = 0;
	}
	else
	{
		cx -= SCREEN_WIDTH / 2;
	}

	if (cy - SCREEN_HEIGHT / 2 < 0)
	{
		cy = 0;
	}
	else
	{
		cy -= SCREEN_HEIGHT / 2;
	}

	CGame::GetInstance()->SetCamPos(cx,cy);
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