#include "GameEngine.h"

GameEngine::GameEngine()
{
	this->mHGE = NULL;
	this->mScreenWidth = 0;
	this->mScreenHeight = 0;
}
GameEngine::~GameEngine()
{
	this->mHGE->System_Shutdown();
	this->mHGE->Release();
}

HRESULT GameEngine::Init(HWND hWnd, int width, int height)
{
	this->mScreenWidth = width;
	this->mScreenHeight = height;

	this->mHGE = hgeCreate(HGE_VERSION);
	//this->mHGE->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	//this->mHGE->System_SetState(HGE_RENDERFUNC, RenderFunc);
	this->mHGE->System_SetState(HGE_FPS, 100);
	this->mHGE->System_SetState(HGE_WINDOWED, true);
	this->mHGE->System_SetState(HGE_SCREENWIDTH, width);
	this->mHGE->System_SetState(HGE_SCREENHEIGHT, height);
	this->mHGE->System_SetState(HGE_SCREENBPP, 32);
	this->mHGE->System_SetState(HGE_HWNDPARENT, hWnd);

	if(this->mHGE->System_Initiate())
	{
		//Init hge stuff and so on
	}

	return S_OK;
}
HRESULT GameEngine::Shutdown()
{
	return S_OK;
}

HRESULT GameEngine::ProcessFrame()
{
	FrameFunc();
	RenderFunc();
	return S_OK;
}
HRESULT GameEngine::OnResize(int width, int height)
{
	MoveWindow(this->mHGE->System_GetState(HGE_HWND), 0, 0, width, height, true);

	this->mScreenWidth = width;
	this->mScreenHeight = height;

	return S_OK;
}

bool GameEngine::FrameFunc()
{
	if(this->mHGE->Input_KeyDown(HGEK_ESCAPE))
		return true;

	return false;
}

void GameEngine::RenderFunc()
{
	this->mHGE->Gfx_BeginScene();
	this->mHGE->Gfx_Clear(ARGB(255, 0, 0, 255));

	this->mHGE->Gfx_EndScene();
}

char* GameEngine::ProcessText(char* msg)
{
	return msg;
}