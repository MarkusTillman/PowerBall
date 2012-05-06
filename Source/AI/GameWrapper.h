#ifndef GAME_WRAPPER
#define GAME_WRAPPER

#include "..\Source\stdafx.h"
#include "..\Source\MainMenu.h"
#include "..\Source\GraphicsEngine\GraphicsEngine.h"
#include "..\Source\SoundEngine\SoundEngine.h"
#include "..\Source\GameManager.h"
class DLL_USAGE GameWrapper
{
private:
	GraphicsEngine*		mGE;
	GameManager*		mGM;
public:
				GameWrapper();
				~GameWrapper();


	HRESULT		Init(HWND hWnd, int width, int height);

	HRESULT		Update();
	HRESULT		MoveCamera(float moveX, float moveZ);
	char*		ProcessText(char* msg);
	HRESULT		Close();


};
#endif