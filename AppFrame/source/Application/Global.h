#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Effect/Effect.h"
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
	ResourceServer* resource;
	Effect* effect;
	bool exit_count;
};
extern Global global;

