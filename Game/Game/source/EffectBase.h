#pragma once
#include "appframe.h"
class EffectBase
{
public : 
	EffectBase();
	virtual ~EffectBase();
	virtual bool Process();
	virtual bool Render();
	bool GetFlag() { return _IsPlay; }
protected : 		
	bool _IsPlay;
};

