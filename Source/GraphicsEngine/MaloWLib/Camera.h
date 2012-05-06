#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "Terrain.h"
#include "GraphicsEngineParameters.h"
#include "Mesh.h"
//#include "..\..\Platform.h"
//#include "..\..\Ball.h"
#include "CursorControl.h"


class Camera
{
protected:
	D3DXMATRIX view;
	D3DXMATRIX projection;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 forward;
	D3DXVECTOR3 up;
	Terrain* terrain;
	Mesh* followTarget;
	
	HWND g_hWnd;

	float angleX;
	float angleY;

	void moveToTerrain();
	void MoveToFollowPosition();

	float speed;
	float sensitivity;
	float DistanceFromTarget;

	GraphicsEngineParams params;

	D3DXVECTOR3 minBoundries;
	D3DXVECTOR3 maxBoundries;
	bool forceBoundries;
	
	D3DXVECTOR3 NormalizeVector(D3DXVECTOR3 vec);

public:
	Camera(HWND g_hWnd, GraphicsEngineParams params);
	virtual ~Camera();

	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();

	D3DXVECTOR3 getPosition();
	float getPosX();
	float getPosY();
	float getPosZ();
	D3DXVECTOR3 getUpVector();
	D3DXVECTOR3 getForward();
	float GetSpeed() const { return this->speed; }
	float GetSensitivity() const { return this->sensitivity; }


	virtual void setPosition(D3DXVECTOR3 pos);
	virtual void setPosX(float x);
	virtual void setPosY(float y);
	virtual void setPosZ(float z);

	virtual void setForward(D3DXVECTOR3 forward);
	virtual void LookAt(D3DXVECTOR3 target);

	virtual void update(float delta);
	virtual void updateSpecific(float delta) = 0;

	virtual void setUpVector(D3DXVECTOR3 up);


	// Movement related
	void move(D3DXVECTOR3 moveBy);

	virtual void moveForward(float diff) = 0;
	virtual void moveBackward(float diff) = 0;
	virtual void moveLeft(float diff) = 0;
	virtual void moveRight(float diff) = 0;

	virtual void SetSpeed(float speed) { this->speed = speed; }
	virtual void SetSensitivity(float sens) { this->sensitivity = sens; }

	virtual void SetBoundries(D3DXVECTOR3 minBoundries, D3DXVECTOR3 maxBoundries);
	virtual void DisableBoundries();

	// Terrain/Mesh interaction
	virtual void WalkOnTerrain(Terrain* terrain) { this->terrain = terrain; }
	virtual void StopWalkingOnTerrain() { this->terrain = NULL; }
	virtual void FollowMesh(Mesh* target) { this->followTarget = target; }
	virtual void StopFollowingMesh() { this->followTarget = NULL; }
	virtual void SetDistanceFromTarget(float distance) { this->DistanceFromTarget = distance; }
	virtual float GetDistanceFromTarget() const { return this->DistanceFromTarget; }

};

#endif