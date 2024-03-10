#include "ModeLoading.h"
ModeLoading::ModeLoading(bool* flag,std::thread* thread) {
	IsClear = flag;
	_thread = thread;
};

bool ModeLoading::Initialize(){
	return true;
};

bool ModeLoading::Terminate(){
	return true;
};

bool ModeLoading::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	if ((*IsClear)) {
		int time = 4 * 1000;
		ModeServer::GetInstance()->Add(new ModeFade(time,true),1000,"FadeIn");
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};

bool ModeLoading::Render() {
	clsDx();
	printfDx("ƒ[ƒh’†");
	return true;
};