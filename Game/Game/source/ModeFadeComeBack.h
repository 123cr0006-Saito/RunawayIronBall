#pragma once
#include "ModeFade.h"

class ModeFadeComeBack : public ModeFade
{
public:
	ModeFadeComeBack(int Time);
	ModeFadeComeBack(int Time, ModeBase* mode);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process()override;
	virtual bool Render()override;
protected:
	int _fadeEnd, _fadeStart;
	ModeBase* _deleteMode;
};

