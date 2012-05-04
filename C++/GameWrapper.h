#pragma once

#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"
#include "SoundEngine\SoundEngine.h"
class DLL_USAGE GameWrapper
{
private:
	GraphicsEngine*		mGE;
	GameManager*		mGM;
public:
				GameWrapper();
	virtual		~GameWrapper();


	void		InitGraphicsEngine(HWND hWnd, int width, int height);

	void		Update();
	void		MoveCamera(float moveX, float moveZ);
	void		ProcessMSG(char* msg);
	void		Close();


};