//----------------------------------------------------------------------
// @filename ModeFadeComeBack.cpp
// @date: 2024/03/02
// @author: saito ko
// @explanation
// ステージ遷移時にフェードイン、フェードアウトを行うためのクラス
// ModeFadeと違う点は常にフェードインしてからフェードアウトし、
// 指定のモードを削除しながら遷移する
//----------------------------------------------------------------------
#include "ModeFadeComeBack.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param Time フェード時間
// @param modeName レイヤーを変更するモード名
// @param layer 指定したモードのレイヤーをこの値に変更
// @param IsProcessSkip フェード時プロセスをスキップするかどうか
//----------------------------------------------------------------------
ModeFadeComeBack::ModeFadeComeBack(int Time, std::string modeName, int layer, bool IsProcessSkip) :ModeFade(Time, false) {
	_fadeEnd = 255;
	_fadeStart = 0;
	_deleteMode = nullptr;
	_changeLayer = layer;
	_changeModeName = modeName;
	_IsProcessSkip = IsProcessSkip;
};
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param Time フェード時間
// @param mode 削除するモード名
// @param IsProcessSkip フェード時プロセスをスキップするかどうか
//----------------------------------------------------------------------
ModeFadeComeBack::ModeFadeComeBack(int Time, ModeBase* mode, bool IsProcessSkip) :ModeFade(Time,false){
	_fadeEnd = 255;
    _fadeStart = 0;
	_deleteMode = mode;
	_changeLayer = 0;
	_changeModeName = "";
	_IsProcessSkip = IsProcessSkip;
};
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param Time フェード時間
// @param mode 削除するモード名
// // @param modeName レイヤーを変更するモード名
// @param layer 指定したモードのレイヤーをこの値に変更
// @param IsProcessSkip フェード時プロセスをスキップするかどうか
//----------------------------------------------------------------------
ModeFadeComeBack::ModeFadeComeBack(int Time, ModeBase* mode, std::string modeName, int layer, bool IsProcessSkip) :ModeFade(Time, false) {
	_fadeEnd = 255;
	_fadeStart = 0;
	_deleteMode = mode;
	_changeLayer = layer;
	_changeModeName = modeName;
	_IsProcessSkip = IsProcessSkip;
};
//----------------------------------------------------------------------
// @brief 初期化
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFadeComeBack::Initialize(){
	if (!base::Initialize()) { return false; }
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFadeComeBack::Terminate(){
	base::Terminate();
	_deleteMode = nullptr;
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFadeComeBack::Process(){
	base::Process();
	// プロセスをスキップする場合
	if(_IsProcessSkip){
	   ModeServer::GetInstance()->SkipProcessUnderLayer();
	}
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	int nowTime = GetNowCount() - _currentTime;
	// フェード処理
	_alphaFade = Easing::Linear(nowTime, _fadeStart, _fadeEnd, _fadeTime);
	// フェード終了
	if (_alphaFade >= 255) {
		// 値の入れ替え
		_alphaFade = _fadeEnd;

		int temp = _fadeStart;
        _fadeStart = _fadeEnd;
        _fadeEnd = temp;
		_currentTime = GetNowCount();

		// 削除するモードがあるとき
		if(_deleteMode != nullptr){
		   ModeServer::GetInstance()->Del(_deleteMode);
	    }
		// レイヤーを変更する場合
		if(_changeModeName != ""){
			ModeServer::GetInstance()->ChangeLayer(_changeModeName, _changeLayer);
		}
	}
	else if (_alphaFade < 0) {
		// フェード終了 削除
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeFadeComeBack::Render() {
	base::Render();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaFade);
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaFade);
	return true;
};