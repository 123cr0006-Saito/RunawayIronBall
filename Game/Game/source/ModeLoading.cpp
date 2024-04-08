//----------------------------------------------------------------------
// @filename ModeLoading.cpp
// ＠date: 2024/03/06
// ＠author: saito ko
// @explanation
// loading時のキャラクターや鉄球などの処理・描画を行うクラス
//----------------------------------------------------------------------
#include "ModeLoading.h"
#include "ModeScenario.h"
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeLoading::Initialize(){
	_chara = NEW LoadingPlayer();
	// 3Ｄ空間の画面の中心点を移動
	int sizeX, sizeY, colorBit;
	GetScreenState(&sizeX, &sizeY, &colorBit);
	SetCameraScreenCenter(sizeX - 500, sizeY - 150);
	global.ResourceLoad();
	global.Init();
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeLoading::Terminate(){
	// 3Ｄ空間の画面の中心点を移動
	int sizeX, sizeY, colorBit;
	GetScreenState(&sizeX, &sizeY, &colorBit);
	SetCameraScreenCenter(sizeX / 2, sizeY / 2);
	delete _chara; _chara = nullptr;
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeLoading::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	// ロードが終わったらシナリオモードを追加
	if (GetASyncLoadNum() <= 0 && !ModeServer::GetInstance()->Search("Fade")) {
		ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario01.csv",	1), 50, "Scenario");
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(1000,this),1000,"Fade");
	}
	// キャラクターの処理
	_chara->Process();
	// カメラの位置を設定
	VECTOR target = _chara->GetPos();
	VECTOR cameraPos = VAdd(target, VGet(0, 0, -1000));
	SetCameraPositionAndTarget_UpVecY(cameraPos, target);

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeLoading::Render() {
	_chara->Render();
	return true;
};