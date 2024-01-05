#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Function/Timer.h"
class Global
{
public:
//関数
	Global();
	~Global();
	bool Init();
	void SetExit() { exit_count = true; }
	bool GetExit() { return exit_count; }
//変数・クラス
	Timer* _timer;
	ResourceServer* resource;
	bool exit_count;

	int time;
};
extern Global global;

