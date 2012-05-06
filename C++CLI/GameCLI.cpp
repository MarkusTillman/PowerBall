#include "GameCLI.h"
#include <sstream>

namespace System 
{ 
	namespace Windows 
	{ 
		namespace Interop
		{
			GameCLI::GameCLI()
			{
				this->mGW = new GameWrapper();
			}

			GameCLI::~GameCLI()
			{
				SAFE_DELETE(this->mGW);
			}

			HRESULT GameCLI::Init(IntPtr hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)hWnd;

				
				return this->mGW->Init(hwnd, width, height);
			}

			HRESULT GameCLI::Shutdown()
			{
				HRESULT res = this->mGW->Close();
				SAFE_DELETE(this->mGW);

				return res;
			}

			HRESULT GameCLI::ProcessFrame()
			{
				return this->mGW->Update();
			}

		/*	HRESULT GameCLI::OnResize(int width, int height)
			{
				return this->mGameEngine->OnResize(width, height);
			}
			*/
			
			HRESULT GameCLI::MoveCamera(float moveX, float moveZ)
			{
				return this->mGW->MoveCamera(moveX, moveZ);

			}
			String^ GameCLI::ProcessText(String^ msg)
			{
				char* lpText = nullptr;
				String^ returnText;

				//Konvertera String^ -> char*
				try
				{
					lpText = (char*)Marshal::StringToHGlobalAnsi(msg).ToPointer();

					returnText = gcnew String(this->mGW->ProcessText(lpText));
				}
				finally
				{
					Marshal::FreeHGlobal((IntPtr) const_cast<char*>(lpText)); // Free memory
				}

				return returnText;
			}
		}
	}
}