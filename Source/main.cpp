#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"
#include "InGameMenu.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif
	
	MaloW::ClearDebug();
	// Create parameters for the graphics engine
	GraphicsEngineParams params;
	params.windowHeight = 900;
	params.windowWidth = 1500;
	params.CamType = RTS;
	
	// Create the graphics engine
	GraphicsEngine* ge = new GraphicsEngine(params, hInstance, nCmdShow);
	gfxeng::eng = ge; // Set the global eng to our engine so that GetGraphicsEngine(); can work.
	ge->CreateSkyBox("Media/skymap.dds");

	/*
	//#define LOLTEST
	#ifdef LOLTEST
	
	// Example of GE useage
	GraphicsEngine* eng = GetGraphicsEngine();
	eng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6));
	eng->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));
	Mesh* testBall = eng->CreateMesh("Media/Ball.obj", D3DXVECTOR3(8, 15, 8));
	Mesh* testCylinder = eng->CreateMesh("Media/Cylinder.obj", D3DXVECTOR3(10, 10, 10));
	Mesh* bth = eng->CreateMesh("Media/bth.obj", D3DXVECTOR3(5, 20, 15));
	bth->Scale(0.1f);
	Image* testImg = eng->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	Light* testLight = eng->CreateLight(D3DXVECTOR3(13, 20, 13));
	Light* testLight2 = eng->CreateLight(D3DXVECTOR3(3, 20, 3));
	testLight->SetLookAt(testBall->GetPosition());
	testLight2->SetLookAt(testBall->GetPosition());
	//Text* text = eng->CreateText("LolAwesome", D3DXVECTOR2(300, 300), 20, "Media/Fonts/1.png");
	while(eng->isRunning())	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		float diff = eng->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.

		testBall->Rotate(D3DXVECTOR3(2*PI, 0, 0) * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.
		//testBall->RotateAxis(D3DXVECTOR3(1, 0, 0),  2* PI * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.

		if(eng->GetKeyListener()->IsPressed('W'))
			eng->GetCamera()->moveForward(diff);
		if(eng->GetKeyListener()->IsPressed(VK_RETURN))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			eng->GetCamera()->moveLeft(diff);
		if(eng->GetKeyListener()->IsPressed('A'))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			eng->GetCamera()->moveLeft(diff);
		if(eng->GetKeyListener()->IsPressed('S'))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			eng->GetCamera()->moveBackward(diff);
		if(eng->GetKeyListener()->IsPressed('D'))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			eng->GetCamera()->moveRight(diff);
		if(eng->GetKeyListener()->IsClicked(1))
			eng->GetCamera()->moveBackward(diff);
	}
	
	#endif
	*/
	// Create the MainMenu and send the graphics engine, and then run Run();
	
	MainMenu* mm = new MainMenu(ge);
	mm->Run();
	delete mm;
	// Delete graphics engine
	delete ge;
	return 0;
}