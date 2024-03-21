#pragma once
#include "appframe.h"
#include "ModeFadeComeBack.h"
#include "LoadingPlayer.h"
class ModeLoading : public ModeBase
{
public : 	
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	LoadingPlayer* _chara;
};

