#include "ORTHOCamera.h"

ORTHOCamera::ORTHOCamera(HWND g_hWnd, GraphicsEngineParams params) : Camera(g_hWnd, params)
{
	this->up = D3DXVECTOR3(0, 0, 1);
	this->DistanceFromTarget = 0.0f;
	this->forward = D3DXVECTOR3(0, -1 , 0);
	this->forward = this->NormalizeVector(this->forward);
	this->pos = D3DXVECTOR3(0, 50, 0);
	//CursorControl cc;
	//cc.SetVisibility(true);
	
	::D3DXMatrixOrthoLH(&this->projection, params.windowWidth*0.05, params.windowHeight*0.05, 1, 1000);
}

ORTHOCamera::~ORTHOCamera()
{
	
}

void ORTHOCamera::updateSpecific(float delta)
{
	//D3DXMatrixPerspectiveFovLH(&this->projection, (float)D3DX_PI * 0.45f, this->params.windowWidth / (float)this->params.windowHeight, 1.0f, 200.0f);
	//this->pos = D3DXVECTOR3(0, 50, 0);
	//::D3DXMatrixOrthoLH(&this->projection, params.windowWidth*0.1, params.windowHeight*0.1, 1, 1000);
}

void ORTHOCamera::moveForward(float diff)
{
	this->pos.z += ((float)diff/100) * this->speed;
}

void ORTHOCamera::moveBackward(float diff)
{
	this->pos.z -= ((float)diff/100) * this->speed;
}

void ORTHOCamera::moveLeft(float diff)
{
	this->pos.x -= ((float)diff/100) * this->speed;
}

void ORTHOCamera::moveRight(float diff)
{
	this->pos.x += ((float)diff/100) * this->speed;
}