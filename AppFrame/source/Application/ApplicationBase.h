
#include "dxlib.h"
#include "../System/Header/Input/XInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Mode//ModeServer.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

#define FULLSCREEN

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
	virtual bool BeforeDXLib_Init() { return true; }	// DXLib_Init() 実行前に行いたい処理があればオーバーライド

#if !defined(_DEBUG) || defined(FULLSCREEN)
	virtual int DispSizeW() { return 1920; }
	virtual int DispSizeH() { return 1080; }
#else
	virtual int DispSizeW() { return 1280; }
	virtual int DispSizeH() { return 720; }
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
