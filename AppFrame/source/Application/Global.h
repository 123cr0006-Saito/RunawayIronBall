#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Function/Timer.h"
class Global
{
public:
//ŠÖ”
	Global();
	~Global();
	bool Init();
	void SetExit() { exit_count = true; }
	bool GetExit() { return exit_count; }
//•Ï”EƒNƒ‰ƒX
	Timer* _timer;
	ResourceServer* resource;
	bool exit_count;

	int time;
};
extern Global global;

