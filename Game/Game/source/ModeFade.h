#pragma once
#include "appframe.h"

class ModeFade : public ModeBase
{
	typedef ModeBase base;
public:
	ModeFade(int Time,bool FadeIn = false);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	int _alphaFade;
	int _currentTime;
	int _fadeTime;
    bool _isFadeIn;
};

