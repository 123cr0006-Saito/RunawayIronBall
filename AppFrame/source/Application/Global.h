#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Function/Timer.h"
class Global
{
public:
//�֐�
	Global();
	~Global();
	bool Init();
	void SetExit() { exit_count = true; }
	bool GetExit() { return exit_count; }
//�ϐ��E�N���X
	Timer* _timer;
	bool exit_count;
};
extern Global global;

