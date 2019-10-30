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
CSkill *skill;
CSObject* sobject;

vector<CSObject*> listSobject;
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
		if (!simon->get_isAttack() && !simon->get_isPick() && !simon->get_isJump())
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
	case DIK_P:
		simon->toggleRenderBBox();
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
	if (game->IsKeyDown(DIK_RIGHT) && !simon->get_isAttack())
	{
		if (simon->get_isJump())
		{
			if (simon->nx > 0) simon->SetState(SIMON_STATE_WALKING_RIGHT);
		}
		else
		{
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT) && !simon->get_isAttack())
	{
		if (simon->get_isJump())
		{
			if (simon->nx < 0) simon->SetState(SIMON_STATE_WALKING_LEFT);
		}
		else
		{
			simon->SetState(SIMON_STATE_WALKING_LEFT);
		}
			
	}
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


	LPDIRECT3DTEXTURE9 texBg = textures->Get(ID_TEX_BACKBROUND);


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPANIMATION ani;


#pragma region SObject

	sobject = new CSObject();

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
	sobject->AddAnimation(911);
	sobject->AddAnimation(921);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(90.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(2);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(911);
	sobject->AddAnimation(921);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(220.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(3);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(911);
	sobject->AddAnimation(921);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(345.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(3);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(911);
	sobject->AddAnimation(921);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(475.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(2);
	objects.push_back(sobject);

	sobject = new CSObject();
	sobject->AddAnimation(911);
	sobject->AddAnimation(921);
	sobject->AddAnimation(931);
	sobject->AddAnimation(932);
	sobject->AddAnimation(933);
	sobject->SetPosition(600.0f, 100.0f);
	sobject->SetState(1);
	sobject->SetNextState(4);
	objects.push_back(sobject);

#pragma endregion SObject

	/*===========================================================*/

	CWeapon* weapon = CWeapon::GetInstance();
	objects.push_back(weapon);

	/*===========================================================*/

	skill = CSkill::GetInstance();
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

	simon = new CSimon();
	simon->SetPosition(40.0f, 50.0f);
	objects.push_back(simon);
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
	vector<LPGAMEOBJECT> coObjectsWithSkill;

	for (int i = 0; i < objects.size(); i++)
	{
		coObjectsFull.push_back(objects[i]);

		if (dynamic_cast<CGround *>(objects[i]))
		{
			coObjectGround.push_back(objects[i]);
			coObjects.push_back(objects[i]);
		}
	
		if (dynamic_cast<CSObject *>(objects[i]))
		{
			if (objects[i]->GetState() != 1 && objects[i]->GetState() != 0)
			{
				coObjects.push_back(objects[i]);
			}
			else
			{
				coObjectsWithSkill.push_back(objects[i]);
			}
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

		else if (dynamic_cast<CSkill *>(objects[i]))
		{
			objects[i]->Update(dt, &coObjectsWithSkill);
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