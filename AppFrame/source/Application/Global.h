#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Function/Timer.h"
#include "../System/Header/Sound/SoundItem.h"
#include "../System/Header/Sound/SoundServer.h"
class Global
{
public:
//関数
	Global();
	~Global();
	bool Init();
	bool ResourceLoad();
	bool SoundLoad();
	void SetExit() { exit_count = true; }
	bool GetExit() { return exit_count; }
//変数・クラス
	Timer* _timer;
	SoundServer* _soundServer;
	bool exit_count;
};
extern Global global;

