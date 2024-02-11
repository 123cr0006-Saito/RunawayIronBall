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
	// ”ñ“¯Šú“Ç‚İ‚İİ’è
	SetUseASyncLoadFlag(TRUE);
	//_soundServer->Add("", new SoundItemBGM(""));
	// ”ñ“¯Šú“Ç‚İ‚İI—¹
	SetUseASyncLoadFlag(FALSE);
	return true;
}