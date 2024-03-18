#include "AppFrame.h"
#include "ModeGameOver.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeFade.h"

bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	_handle["Logo"] = ResourceServer::LoadGraph("Logo","res/ModeGameOver/UI_Game_Over.png");
	_handle["Retry"] = ResourceServer::LoadGraph("Retry","res/ModeGameOver/UI_Game_Over_Retry.png");
	_handle["Give"] = ResourceServer::LoadGraph("Give","res/ModeGameOver/UI_Game_Over_Title_Back.png");
	_selectItem = 0;

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

	ModeServer::GetInstance()->Add(new ModeFade(3000, true), 10, "Fade");

	global._soundServer->DirectPlay("PL_GameOver");
	return true;
};

bool ModeGameOver::Terminate() {
	base::Terminate();
	_input = nullptr;
	_handle.clear();
	return true;
};

bool ModeGameOver::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	//選択項目の切り替え
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		_selectItem = 1 - _selectItem;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		_selectItem = 1 - _selectItem;
		global._soundServer->DirectPlay("SE_Select");
	}

	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global._soundServer->DirectPlay("SE_Press");
		if (_selectItem == 0) {
			if (_mode != nullptr) {
				_mode->NewStage();
			}
			ModeServer::GetInstance()->Del(this);
		}
		else {
			ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
			ModeServer::GetInstance()->Del(this);
		}
	}

	MATRIX matrix = MGetRotY(0.02f);
	_cameraPos = VTransform(_cameraPos, matrix);
	VECTOR pos = VAdd(_cameraPos, _targetPos);
	SetCameraPositionAndTarget_UpVecY(pos, _targetPos);

	return true;
};

bool ModeGameOver::Render() {
	base::Render();
    // モデルの描画
	MV1DrawModel(_model);

	int handleX, handleY;
	std::vector<std::string> name = { "Logo","Retry","Give" };

	DrawRotaGraph(1920/2, 200, 1.0f, 0.0f, _handle[name[0]], true);
	for (int i = 1; i < name.size(); i++) {
		float exrate = 1.0f;
		if (i == _selectItem + 1)exrate = 1.1f;
		GetGraphSize(_handle[name[i]], &handleX, &handleY);
		DrawRotaGraph(1920 / 2, 1080 / 2 + 175 * i, exrate, 0.0f, _handle[name[i]], true);
	}

	
	return true;
};