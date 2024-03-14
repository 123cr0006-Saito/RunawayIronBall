#include "ModeFadeComeBack.h"
ModeFadeComeBack::ModeFadeComeBack(int Time) :ModeFade(Time,false){
	_fadeEnd = 255;
    _fadeStart = 0;
};

bool ModeFadeComeBack::Initialize(){
	if (!base::Initialize()) { return false; }
	return true;
};

bool ModeFadeComeBack::Terminate(){
	base::Terminate();
	return true;
};

bool ModeFadeComeBack::Process(){
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	_alphaFade = Easing::Linear(GetNowCount() - _currentTime, _fadeStart, _fadeEnd, _fadeTime);

	if (_alphaFade >= _fadeEnd) {
		// 値の入れ替え
		int temp = _fadeStart;
        _fadeStart = _fadeEnd;
        _fadeEnd = temp;
		_currentTime = GetNowCount();
	}
	else if (_alphaFade <= _fadeEnd) {
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