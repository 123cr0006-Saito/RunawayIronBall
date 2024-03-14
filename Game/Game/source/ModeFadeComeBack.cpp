#include "ModeFadeComeBack.h"
ModeFadeComeBack::ModeFadeComeBack(int Time, ModeBase* mode) :ModeFade(Time,false){
	_fadeEnd = 255;
    _fadeStart = 0;
	_deleteMode = mode;
};

bool ModeFadeComeBack::Initialize(){
	if (!base::Initialize()) { return false; }
	return true;
};

bool ModeFadeComeBack::Terminate(){
	base::Terminate();
	_deleteMode = nullptr;
	return true;
};

bool ModeFadeComeBack::Process(){
	base::Process();
	//ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	int nowTime = GetNowCount() - _currentTime;

	_alphaFade = Easing::Linear(nowTime, _fadeStart, _fadeEnd, _fadeTime);

	if (_alphaFade >= 255) {
		// 値の入れ替え
		_alphaFade = _fadeEnd;

		int temp = _fadeStart;
        _fadeStart = _fadeEnd;
        _fadeEnd = temp;
		_currentTime = GetNowCount();
		ModeServer::GetInstance()->Del(_deleteMode);
	}
	else if (_alphaFade < 0) {
		// フェード終了 削除
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};

bool ModeFadeComeBack::Render() {
	base::Render();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaFade);
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaFade);
	return true;
};