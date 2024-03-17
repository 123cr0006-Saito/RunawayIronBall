#include "ModeFadeComeBack.h"

ModeFadeComeBack::ModeFadeComeBack(int Time, std::string modeName, int layer, bool IsProcessSkip) :ModeFade(Time, false) {
	_fadeEnd = 255;
	_fadeStart = 0;
	_deleteMode = nullptr;
	_changeLayer = layer;
	_changeModeName = modeName;
	_IsProcessSkip = IsProcessSkip;
};

ModeFadeComeBack::ModeFadeComeBack(int Time, ModeBase* mode, bool IsProcessSkip) :ModeFade(Time,false){
	_fadeEnd = 255;
    _fadeStart = 0;
	_deleteMode = mode;
	_changeLayer = 0;
	_changeModeName = "";
	_IsProcessSkip = IsProcessSkip;
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
	if(_IsProcessSkip){
	   ModeServer::GetInstance()->SkipProcessUnderLayer();
	}
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	int nowTime = GetNowCount() - _currentTime;

	_alphaFade = Easing::Linear(nowTime, _fadeStart, _fadeEnd, _fadeTime);

	if (_alphaFade >= 255) {
		// �l�̓���ւ�
		_alphaFade = _fadeEnd;

		int temp = _fadeStart;
        _fadeStart = _fadeEnd;
        _fadeEnd = temp;
		_currentTime = GetNowCount();

		// �폜���郂�[�h������Ƃ�
		if(_deleteMode != nullptr){
		   ModeServer::GetInstance()->Del(_deleteMode);
	    }
		// ���C���[��ύX����ꍇ
		else if(_changeModeName != ""){
			ModeServer::GetInstance()->ChangeLayer(_changeModeName, _changeLayer);
		}
	}
	else if (_alphaFade < 0) {
		// �t�F�[�h�I�� �폜
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