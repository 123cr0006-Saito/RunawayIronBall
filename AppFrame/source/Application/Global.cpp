#include "Global.h"

Global global;

Global::Global() {
	_timer = new Timer();
	_soundServer = new SoundServer();
	_timer->Start();
	exit_count = false;
	Init();
};

Global::~Global() {
};

bool Global::Init() {
	SoundLoad();
	return true;
};

bool Global::SoundLoad() {
	// �񓯊��ǂݍ��ݐݒ�
	SetUseASyncLoadFlag(TRUE);
	//_soundServer->Add("", new SoundItemBGM(""));
	// �񓯊��ǂݍ��ݏI��
	SetUseASyncLoadFlag(FALSE);
	return true;
}