//----------------------------------------------------------------------
// @filename ModeGameOver.cpp
// @date: 2024/02/15
// @author: saito ko
// @explanation
// ゲームオーバー時に遷移し、リトライかタイトルに戻るかを選択するモード
//----------------------------------------------------------------------
#include "AppFrame.h"
#include "ModeGameOver.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeFadeComeBack.h"
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	_handle["Logo"] = ResourceServer::LoadGraph("Logo","res/ModeGameOver/UI_Game_Over.png");
	_handle["Retry"] = ResourceServer::LoadGraph("Retry","res/ModeGameOver/UI_Game_Over_Retry.png");
	_handle["Give"] = ResourceServer::LoadGraph("Give","res/ModeGameOver/UI_Game_Over_Title_Back.png");
	_selectItem = 0;
	_selectEnd = false;
	// モデルの読み込み
	_model = ResourceServer::MV1LoadModel("Player","res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	MV1SetPosition(_model, VGet(0, 0, 0));
	// アニメーションのアタッチ
	int animIndex = MV1GetAnimIndex(_model, "MO_PL_Game_Over");
	int attachAnim = MV1AttachAnim(_model, animIndex, -1, false);
	// アニメーションの再生時間を設定
	MV1SetAttachAnimTime(_model, attachAnim, 140);
	//カメラのターゲットを取得
	int frameIndex = MV1SearchFrame(_model, "Character1_Hips");
	_targetPos = MV1GetFramePosition(_model, frameIndex);
	// カメラの位置を設定
	_cameraPos= VGet(0, 500, -500);

	global._soundServer->DirectPlay("PL_GameOver");
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGameOver::Terminate() {
	base::Terminate();
	_input = nullptr;
	_handle.clear();
	return true;
};
//----------------------------------------------------------------------
// @brief アニメーション処理
// @return 無し
//----------------------------------------------------------------------
void ModeGameOver::AnimProcess(){
	MATRIX matrix = MGetRotY(0.02f);
	_cameraPos = VTransform(_cameraPos, matrix);
	VECTOR pos = VAdd(_cameraPos, _targetPos);
	SetCameraPositionAndTarget_UpVecY(pos, _targetPos);
};
//----------------------------------------------------------------------
// @brief 選択処理
// @return 無し
//----------------------------------------------------------------------
void ModeGameOver::SelectProcess(){
	if (_selectEnd) return ; // 選択が終わっっているので処理を終了

	//選択項目の切り替え
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_LEFT) || _input->GetTrg(XINPUT_BUTTON_STICK_LEFT)) {
		_selectItem = 1 - _selectItem;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_RIGHT) || _input->GetTrg(XINPUT_BUTTON_STICK_RIGHT)) {
		_selectItem = 1 - _selectItem;
		global._soundServer->DirectPlay("SE_Select");
	}

	if (_input->GetTrg(XINPUT_BUTTON_A) && !ModeServer::GetInstance()->Search("Fade")) {
		_selectEnd = true;
		global._soundServer->DirectPlay("SE_Press");
		if (_selectItem == 0) {
			global.SetAllExp(global.GetOldExp());
			ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "Game");
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(3000, this), 100, "Fade");
		}
		else {
			ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "Title");
			ModeServer::GetInstance()->Del(this);
		}
	}

};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGameOver::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	// アニメーション処理
	AnimProcess();
	// 選択処理
	SelectProcess();
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeGameOver::Render() {
	base::Render();
    // モデルの描画
	MV1DrawModel(_model);

	int handleX, handleY;
	std::vector<std::string> name = { "Logo","Retry","Give" };

	int ItemX[2] = {600,1330};

	DrawRotaGraph(1920/2, 200, 1.0f, 0.0f, _handle[name[0]], true);
	for (int i = 1; i < name.size(); i++) {
		float extrate = 1.0f;
		if (i == _selectItem + 1)extrate = 1.1f;
		DrawRotaGraph(ItemX[i-1], 800, extrate, 0.0f, _handle[name[i]], true);
	}
	
	return true;
};