#pragma once
#include "..\stdafx.h"

#include <hge.h>
#include <hgesprite.h>

class GameEngine
{
	private:
		HGE *	mHGE;
		int		mScreenWidth;
		int		mScreenHeight;

	private:
		void RenderFunc();
		bool FrameFunc();

	public:
		GameEngine();
		~GameEngine();

		HRESULT Init(HWND hWnd, int width, int height);
		HRESULT Shutdown();

		HRESULT ProcessFrame();
		HRESULT OnResize(int width, int height);

		char* ProcessText(char* msg);
};