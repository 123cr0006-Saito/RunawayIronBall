
#include "dxlib.h"
#include "../System/Header/Input/XInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Mode//ModeServer.h"

class ApplicationBase
{
public:
	ApplicationBase();
	virtual ~ApplicationBase();

	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }

#ifdef _DEBUG
	virtual int DispSizeW() { return 1280; }
	virtual int DispSizeH() { return 720; }
#else
	virtual int DispSizeW() { return 1920; }
	virtual int DispSizeH() { return 1080; }
#endif

	static	ApplicationBase	*GetInstance() { return _lpInstance; }
	virtual int GetKey() { return _gKey; }
	virtual int GetTrg(int index) { return _gTrg[index]; }
	virtual int GetRel(int index) { return _gRel[index]; }
	virtual XINPUT_STATE GetInput() { return input; }

protected:
	static	ApplicationBase	*_lpInstance;

	int	_gKey, _gTrg[PAD_BUTTON_MAX], _gRel[PAD_BUTTON_MAX];

	ModeServer* _serverMode;

	XINPUT_STATE input;

};
