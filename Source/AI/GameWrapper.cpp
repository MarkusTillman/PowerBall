#include "GameWrapper.h"

GameWrapper::GameWrapper()
{
	this->mGE = NULL;
	this->mGM = NULL;
}
GameWrapper::~GameWrapper()
{
	SAFE_DELETE(this->mGE);
	SAFE_DELETE(this->mGM);

}


HRESULT GameWrapper::Init(HWND hWnd, int width, int height)
{
	GraphicsEngineParams params;
	params.windowHeight = height;
	params.windowWidth = width;
	params.FXAAQuality = 0;			// 0 - 4
	params.ShadowMapSettings = 0;	// 0 - 10 (works with higher but VERY consuming)
	params.CamType = ORTHO;
	//this->mGE->CreateSkyBox("Media/skymap.dds");
	
	// Create the graphics engine
	//change so that the gfx engine can also accept a handle thus not creating one on its own.
	//add orthogonal view to the camera
	this->mGE = new GraphicsEngine(params, hWnd);

	
	gfxeng::eng = this->mGE; // Set the global eng to our engine so that GetGraphicsEngine(); can work.
	this->mGE->CreateSkyBox("Media/skymap.dds");
	this->mGM = new GameManager(this->mGE);
	this->mGM->UpdateEditor();
	//this->mGE->Suspend();
	return S_OK;
}
HRESULT GameWrapper::MoveCamera(float moveX, float moveZ)
{
	Camera* cam = this->mGE->GetCamera();

	cam->setPosition(cam->getPosition() + D3DXVECTOR3(moveX, 0, moveZ));
	return S_OK;
}
HRESULT GameWrapper::Update()
{
	this->mGM->UpdateEditor();
	return S_OK;
}
char* GameWrapper::ProcessText(char* msg)
{
	return msg;
}
HRESULT GameWrapper::Close()
{
	this->mGE->Close(); //will automatically close GameManager cuz it stops running if the gfx engine isnt running.
	return S_OK;
}