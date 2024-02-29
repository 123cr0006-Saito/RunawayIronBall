#pragma once
#include "appframe.h"
#include"ModeGame.h"

class ApplicationMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }

protected:
	XInput* _input;
	Vibration* _vibration;

	Fps* _fpsController;
}; 
