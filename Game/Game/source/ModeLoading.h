#pragma once
#include "appframe.h"
#include "ModeFade.h"
#include "LoadingPlayer.h"
#include "ModeGame.h"
class ModeLoading : public ModeBase
{
public : 	
	ModeLoading(bool* flag ,ModeGame* mode = nullptr);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	bool* IsClear;
	LoadingPlayer* _chara;
	ModeGame* _mode;
};

