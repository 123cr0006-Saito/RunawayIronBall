#pragma once
#include "../System/Header/Resource/ResourceServer.h"
#include "../System/Header/Function/Timer.h"
#include "../System/Header/Sound/SoundItem.h"
#include "../System/Header/Sound/SoundServer.h"
class Global
{
public:
//�֐�
	Global();
	~Global();
	bool Init();
	bool ResourceLoad();
	bool SoundLoad();
	void SetExit() { exit_count = true; }
	bool GetExit() { return exit_count; }

	void AddStageNum() { _stageNum++; }
	int GetStageNum() { return _stageNum; }
	void SetAllExp(int allExp) { _allExp = allExp; }
	void SetAllExpAdd(int exp) { _allExp += exp; }
	int GetAllExp() { return _allExp; }
	int GetOldExp() { return _oldExp; }
	void SetOldExp(int exp) { _oldExp = exp; }

//�ϐ��E�N���X
	Timer* _timer;
	SoundServer* _soundServer;
	bool exit_count;
	int _stageNum;
	int _allExp; //�S�Ă̌o���l���i�[���܂��B
	int _oldExp; //�X�e�[�W���n�܂������̌o���l���i�[���܂��B
};
extern Global global;

