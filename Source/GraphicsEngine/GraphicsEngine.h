#pragma once

/*
Wrapper-class for the graphics engine. Contains the interface that user use from the outside.
Singleton.
*/

#include "DxManager.h"
#include "KeyListener.h"
#include "GraphicsEngineParameters.h"
#include "FPSCamera.h"
#include "RTSCamera.h"
#include "TRDCamera.h"
#include "ORTHOCamera.h"
#include "Image.h"
#include "CamRecording.h"
#include "SoundEngine/SoundEngine.h"


// Class for communication between processes for loading meshes
class LoadMeshEvent : public MaloW::ProcessEvent
{
private:
	string fileName;
	StaticMesh* mesh;
	AnimatedMesh* ani;
	Material* mat;
	bool selfdelete;

public:
	LoadMeshEvent(string fileName, StaticMesh* mesh, AnimatedMesh* ani, Material* mat) 
	{ 
		this->fileName = fileName; 
		this->mesh = mesh; 
		this->mat = mat;
		this->ani = ani;
		this->selfdelete = true;
	}
	virtual ~LoadMeshEvent() 
	{
		if(this->selfdelete) 
		{
			if(this->mesh) 
				delete this->mesh;
			if(this->mat)
				delete this->mat;
			if(this->ani)
				delete this->ani;
		}
	}
	string GetFileName() { return this->fileName; }
	StaticMesh* GetStaticMesh() { this->selfdelete = false; return this->mesh; }
	AnimatedMesh* GetAnimatedMesh() { this->selfdelete = false; return this->ani; }
	Material* GetMaterial() { this->selfdelete = false; return this->mat; }
};

class GraphicsEngine : public MaloW::Process
{
private:
HWND hWndParent;
	static bool initDone;
	static DxManager* dx;
	static HINSTANCE hInstance;
	static HWND hWnd;
	static MaloW::KeyListener* kl;
	static SoundEngine* sound;
	Camera* cam;

	int fpsLast;
	int prevFrameCount;
	float fpsTimer;

	bool keepRunning;
	bool loading;

	GraphicsEngineParams parameters;

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitWindow(HWND parent);
	void InitObjects();


	float PCFreq;
	__int64 prevTimeStamp;


public:
	GraphicsEngine(GraphicsEngineParams params, HINSTANCE hInstance, int nCmdShow);
	/*! For wrapper*/
	GraphicsEngine(GraphicsEngineParams params, HWND hwnd);
	virtual ~GraphicsEngine();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/*! Creates a Mesh and starts rendering it every frame. Return is a pointer to the Mesh created. To stop rendering it call DeleteMesh on the engine with the Mesh as parameter. */
	StaticMesh* CreateStaticMesh(string filename, D3DXVECTOR3 pos, MaterialType material);
	StaticMesh* CreateStaticMesh(string filename, D3DXVECTOR3 pos, Material* material);
	StaticMesh* CreateStaticMesh(string filename, D3DXVECTOR3 pos);

	AnimatedMesh* CreateAnimatedMesh(string filename, D3DXVECTOR3 pos);
	void DeleteAnimatedMesh(AnimatedMesh* mesh) { this->dx->DeleteAnimatedMesh(mesh); }


	Light* CreateLight(D3DXVECTOR3 pos, bool UseShadowMap = true);
	void DeleteLight(Light* light) { this->dx->DeleteLight(light); }
	Terrain* CreateTerrain(D3DXVECTOR3 position, D3DXVECTOR3 dimension, std::string texture, string heightmap, int vertexSize = 256);

	void CreateSkyBox(string texture);

	/*! Stops rendering the Mesh and internally deletes it and the pointer will become NULL. Return depends on if the Mesh was sucessfully removed. */
	void DeleteStaticMesh(StaticMesh* mesh) { this->dx->DeleteStaticMesh(mesh); }

	/*! Create an Image and starts rendering it every frame. Return is a pointer to the image created. Coordinates are in screen-space. 
	To stop rendering it call DeleteImage on the engine with the image as parameter. */
	Image* CreateImage(D3DXVECTOR2 position, D3DXVECTOR2 dimensions, string texture);

	/*! Stops rendering the Image and internally deletes it and the pointer will become NULL. Return depends on if the Image was sucessfully removed. */
	bool DeleteImage(Image* delImage);

	Text* CreateText(string text, D3DXVECTOR2 position, float size, string fontTexturePath);
	bool DeleteText(Text* delText);

	GraphicsEngineParams GetEngineParameters() const { return this->parameters; }

	/*! Updates the Camera and takes care of all key-inputs and returns diff in milliseconds (47.0f as return = 47 ms, IE. NOT SECONDS) */
	float Update();

	/*! 
	Takes control of the thread and renders a loading-screen with a progress bar. Returns once all objects that have been sent to load is loaded.
	To use it first make all CreateMesh()-calls that you need and then call LoadingScreen(.,.) directly after, and it will return once all the meshes are
	created and being rendered in the background. */
	void LoadingScreen(string BackgroundTexture, string ProgressBarTexture);


	bool isRunning();

	// Get's
	Camera* GetCamera() const { return this->dx->GetCamera(); }
	MaloW::KeyListener* GetKeyListener() const { return this->kl; }
	SoundEngine* GetSoundEngine() const { return this->sound; }
	HWND GetWindowHandle() const { return this->hWnd; }

	void CreateSmokeEffect() { this->dx->CreateSmokeEffect(); }

	virtual void Life();
};

// Used as an easy single-ton.
struct gfxeng
{
	static GraphicsEngine* eng;
};

inline GraphicsEngine* GetGraphicsEngine()
{
	return gfxeng::eng;
}