#pragma once
#include "appframe.h"
#include <thread>
#include "ModeFade.h"
class ModeLoading : public ModeBase
{
public : 	
	ModeLoading(bool* flag,std::thread* thread);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	bool* IsClear;
    std::thread* _thread;
};

