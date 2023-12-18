#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Effect/Effect.h"
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
	ResourceServer* resource;
	Effect* effect;
	bool exit_count;
};
extern Global global;

