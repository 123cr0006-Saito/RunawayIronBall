#include "ModeFade.h"

ModeFade::ModeFade(int time,bool FadeIn) {
	_currentTime = GetNowCount();
	_fadeTime = time;
	_isFadeIn = FadeIn;
	if (FadeIn) {
		_alphaFade = 255;
	}
	else {
		_alphaFade = 0;
	}
};

bool ModeFade::Initialize() {
	if (!base::Initialize()) { return false; }
	return true;
};

bool ModeFade::Terminate() {
	base::Terminate();
	return true;
};

bool ModeFade::Process() {
	base::Process();
	if (_isFadeIn) {
	    // FadeIn
		_alphaFade = Easing::Linear(GetNowCount() - _currentTime,255,0,_fadeTime);
	}
	else {
		// FadeOut
		_alphaFade = Easing::Linear(GetNowCount() - _currentTime, 0, 255, _fadeTime);
	}

	// ŽžŠÔŒo‰ß‚Åíœ
	if (GetNowCount() - _currentTime > _fadeTime) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};

bool ModeFade::Render() {
	base::Render();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaFade);
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,_alphaFade);
	return true;
};