#pragma once
#include "ModeFade.h"
class ModeFadeComeBack : public ModeFade
{
	ModeFadeComeBack(int Time);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	int _fadeEnd, _fadeStart;
};

