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
	// 非同期読み込み設定
	SetUseASyncLoadFlag(TRUE);
	//_soundServer->Add("", new SoundItemBGM(""));
	// 非同期読み込み終了
	SetUseASyncLoadFlag(FALSE);
	return true;
}