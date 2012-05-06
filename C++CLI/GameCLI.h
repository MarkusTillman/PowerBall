

#pragma once
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "..\Source\stdafx.h"
#include "..\Source\AI\GameWrapper.h"

using namespace System;
using namespace System::Windows::Interop;
using namespace System::Windows;
using namespace System::Runtime::InteropServices;


namespace System 
{ 
	namespace Windows 
	{ 
		namespace Interop
		{
			public ref class GameCLI
			{
			protected:
				GameWrapper* mGW;
			public:
				GameCLI();
				~GameCLI();

				HRESULT Init(IntPtr hWnd, int width, int height);
				HRESULT Shutdown();

				HRESULT MoveCamera(float moveX, float moveZ);
				
				HRESULT ProcessFrame();
				//HRESULT OnResize(int width, int height);

				String^ ProcessText(String^ text);
			};
		}
	}
}

