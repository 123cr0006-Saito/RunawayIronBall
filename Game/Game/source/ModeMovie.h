#pragma once
#include "appframe.h"
class ModeMovie : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	int _movie;
};

