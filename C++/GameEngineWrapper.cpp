//File: GameEngineWrapper.cpp

#include "GameEngineWrapper.h"
#include "GameEngine.h"

GameEngineWrapper::GameEngineWrapper()
{
	this->mGameEngine = new GameEngine();
}
GameEngineWrapper::~GameEngineWrapper()
{
	SAFE_DELETE(this->mGameEngine);
}

HRESULT GameEngineWrapper::Init(HWND hWnd, int width, int height)	
{
	return this->mGameEngine->Init(hWnd, width, height);
}
HRESULT GameEngineWrapper::Shutdown()
{
	SAFE_DELETE(this->mGameEngine);

	return S_OK;
}

HRESULT GameEngineWrapper::ProcessFrame()
{
	return this->mGameEngine->ProcessFrame();
}
HRESULT GameEngineWrapper::OnResize(int width, int height)
{
	return this->mGameEngine->OnResize(width, height);
}

char* GameEngineWrapper::ProcessText(char* msg)
{
	return this->mGameEngine->ProcessText(msg);
}