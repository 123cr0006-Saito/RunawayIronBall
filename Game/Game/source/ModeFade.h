#pragma once
#include "appframe.h"

class ModeFade : public ModeBase
{
protected: 
	typedef ModeBase base;
public:
	ModeFade(int Time,bool FadeIn = false);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process()override;
	virtual bool Render()override;
protected:
	int _alphaFade;
	int _currentTime;
	int _fadeTime;
    bool _isFadeIn;
};

