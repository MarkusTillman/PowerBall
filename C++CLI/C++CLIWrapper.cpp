#include "C++CLIWrapper.h"
#include <sstream>

namespace System 
{ 
	namespace Windows 
	{ 
		namespace Interop
		{
			CppCLI::CppCLI()
			{
				myInitMemoryCheck();
				this->mGameEngine = new GameEngineWrapper();
			}

			CppCLI::~CppCLI()
			{
				SAFE_DELETE(this->mGameEngine);
				myDumpMemoryLeaks();
			}

			HRESULT CppCLI::Init(IntPtr hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)hWnd;

				return this->mGameEngine->Init(hwnd, width, height);
			}

			HRESULT CppCLI::Shutdown()
			{
				SAFE_DELETE(this->mGameEngine);

				return S_OK;
			}

			HRESULT CppCLI::ProcessFrame()
			{
				return this->mGameEngine->ProcessFrame();
			}

			HRESULT CppCLI::OnResize(int width, int height)
			{
				return this->mGameEngine->OnResize(width, height);
			}

			String^ CppCLI::ProcessText(String^ msg)
			{
				char* lpText = nullptr;
				String^ returnText;

				//Konvertera String^ -> char*
				try
				{
					lpText = (char*)Marshal::StringToHGlobalAnsi(msg).ToPointer();

					returnText = gcnew String(this->mGameEngine->ProcessText(lpText));
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