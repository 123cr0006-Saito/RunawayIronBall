//----------------------------------------------------------------------
// @filename ModeTutorial.cpp
// ＠date: 2024/03/17
// ＠author: saito ko
// @explanation
// ステージの最初にチュートリアル画面を表示するクラス
//----------------------------------------------------------------------
#include "ModeTutorial.h"
#include "TimeLimit.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param handle チュートリアル画像のハンドル
// @param size チュートリアル画像の数
//----------------------------------------------------------------------
ModeTutorial::ModeTutorial(int* handle, int size){
	_pageSize = size;
	_handle = NEW int[_pageSize];
	for(int i = 0; i < _pageSize; i++){
		_handle[i] = handle[i];
	}
	_selectItem = 0;
	_input = XInput::GetInstance();
};
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeTutorial::Initialize(){
	TimeLimit::GetInstance()->Stop();
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeTutorial::Terminate(){
	delete[] _handle;
	_input = nullptr;
	TimeLimit::GetInstance()->Restart();
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
bool ModeTutorial::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	// 次の画像へ
	if (_input->GetTrg(XINPUT_BUTTON_A) ) {
		if (_selectItem < _pageSize) {
			_selectItem++;
			if (_selectItem == _pageSize) {
				ModeServer::GetInstance()->Del(this);
			}
		}
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
bool ModeTutorial::Render(){
	// このmodeより上にmodeが無い場合、チュートリアル画像の描画
	if (!ModeServer::GetInstance()->IsAboutLayer(this)) {
		int x, y;
		GetGraphSize(_handle[_selectItem], &x, &y);
		int ScreenX, ScreenY, ScreenDepth;
		GetScreenState(&ScreenX, &ScreenY, &ScreenDepth);
		DrawGraph((ScreenX - x) / 2, (ScreenY - y) / 2, _handle[_selectItem], true);
	}
	return true;
};
