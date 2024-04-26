//----------------------------------------------------------------------
// @filename ModeFade.cpp
// @date: 2024/02/29
// @author: saito ko
// @explanation
// ステージ遷移時にフェードイン、フェードアウトを行うためのクラス
//----------------------------------------------------------------------
#include "ModeFade.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param time フェード時間
// @param isFadeIn フェードインかフェードアウトか
//----------------------------------------------------------------------
ModeFade::ModeFade(int time,bool isFadeIn) {
	_currentTime = GetNowCount();
	_fadeTime = time;
	_isFadeIn = isFadeIn;
	if (isFadeIn) {
		_alphaFade = 255;
	}
	else {
		_alphaFade = 0;
	}
};
//----------------------------------------------------------------------
// @brief 初期化
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFade::Initialize() {
	if (!base::Initialize()) { return false; }
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFade::Terminate() {
	base::Terminate();
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
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

	// 時間経過で削除
	if (GetNowCount() - _currentTime > _fadeTime) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFade::Render() {
	base::Render();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaFade);
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,_alphaFade);
	return true;
};