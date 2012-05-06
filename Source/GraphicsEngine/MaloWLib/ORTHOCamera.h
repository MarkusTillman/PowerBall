#ifndef ORTHOCAMERA
#define ORTHOCAMERA

#include "Camera.h"

class ORTHOCamera : public Camera
{
public:
	ORTHOCamera(HWND g_hWnd, GraphicsEngineParams params);
	virtual ~ORTHOCamera();

	virtual void updateSpecific(float delta);

	virtual void moveForward(float diff);
	virtual void moveBackward(float diff);
	virtual void moveLeft(float diff);
	virtual void moveRight(float diff);



	

	// void setPosition(D3DXVECTOR3 pos){}
	// void setPosX(float x){}
	// void setPosY(float y){}
	// void setPosZ(float z){}

	void setForward(D3DXVECTOR3 forward){}
	void LookAt(D3DXVECTOR3 target){}

	virtual void update(float delta){}

	void setUpVector(D3DXVECTOR3 up){}


	// Movement related
	// void move(D3DXVECTOR3 moveBy){}

	// void SetSpeed(float speed) { }
	// void SetSensitivity(float sens) { }

	//void SetBoundries(D3DXVECTOR3 minBoundries, D3DXVECTOR3 maxBoundries){}
	//void DisableBoundries(){}

	// Terrain/Mesh interaction
	// void WalkOnTerrain(Terrain* terrain) { }
	// void StopWalkingOnTerrain() { }
	// void FollowMesh(Mesh* target) { }
	// void StopFollowingMesh() {  }
	// void SetDistanceFromTarget(float distance) {  }
	// float GetDistanceFromTarget() const {return 0;}


};


#endif