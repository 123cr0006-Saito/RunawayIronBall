//----------------------------------------------------------------------
// @filename ModeTitle.cpp
// ＠date: 2023/12/25
// ＠author: saito ko
// @explanation
// タイトル画面の処理を行うクラス
//----------------------------------------------------------------------
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeLoading.h"
#include "ModePause.h"
#include "math.h"
//----------------------------------------------------------------------
// @brief 初期化処理
// ＠return 成功しているかどうか
//----------------------------------------------------------------------
bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	_modeCount = 0;
	//画像の読み込み
	_handleMap["BackGround"] = ResourceServer::LoadGraph("T_BackGround", _T("res/ModeTitle/title_base.png"));
	_handleMap["Title"] = ResourceServer::LoadGraph("T_Title",_T("res/ModeTitle/UI_Title.png"));
	_handleMap["Start"] = ResourceServer::LoadGraph("T_Start",_T("res/ModeTitle/UI_Start.png"));
	_handleMap["Option"] = ResourceServer::LoadGraph("T_Option",_T("res/ModeTitle/UI_Option.png"));
	_handleMap["Quit"] = ResourceServer::LoadGraph("T_Quit",_T("res/ModeTitle/UI_Quit.png"));
	_handleMap["Logo"] = ResourceServer::LoadGraph("T_Logo", _T("res/ModeTitle/UI_Logo.png"));

	_IsGameStart = false;
	//割れる処理の初期化
	 _modelHandle = ResourceServer::MV1LoadModel("Board", "res/ModeTitle/board.mv1");
	 _currentTime = 0;
	 _IsBreak = false;
	 _frameSize = MV1GetFrameNum(_modelHandle);
	 _MoveVec = NEW VECTOR[_frameSize];
	 _rotVec = NEW VECTOR[_frameSize];

	 VECTOR breakPos = VGet(0,0, 0);//割れる一の中心点
	 for (int i = 0; i < _frameSize; i++) {
	
		 VECTOR fPos = MV1GetFramePosition(_modelHandle, i);
		 _MoveVec[i] = VSub(fPos, breakPos);
		 _MoveVec[i] = VScale(_MoveVec[i], 0.05f);
		 _rotVec[i].x = rand() % 100 - 50;
		 _rotVec[i].y = rand() % 100 - 50;
		 _rotVec[i].z = rand() % 100 - 50;
		 _rotVec[i] = VScale(_rotVec[i], 0.0002f);
	 }

	 MV1SetPosition(_modelHandle, VGet(0, 0, 0)); 
	 SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -1850), VGet(0, 0, 0));

	 // bgmの設定
	 global._soundServer->DirectPlay("Title");

	 // タイトルコール
	 std::string voiceNum[1] = {"IB_Title"};
	 global._soundServer->DirectPlay(voiceNum[0]);

	return true;
}
//----------------------------------------------------------------------
// @brief 終了処理
// ＠return 成功しているか
//----------------------------------------------------------------------
bool ModeTitle::Terminate() {
	base::Terminate();
	delete[] _MoveVec;
	delete[] _rotVec;
	_input = nullptr;
	_handleMap.clear();
	return true;
}
//----------------------------------------------------------------------
// @brief ゲーム開始処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::SelectGameStart() {
	ModeServer::GetInstance()->Del(this);
	ModeServer::GetInstance()->Add(NEW ModeLoading(),100,"Loading");
};
//----------------------------------------------------------------------
// @brief オプション選択処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::SelectOption() {
	ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
};
//----------------------------------------------------------------------
// @brief ゲーム終了処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
	}
};
//----------------------------------------------------------------------
// @brief 選択項目の更新処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::UpdateSelectItems(){
	int count = 0;

	//モード選択の切り替え
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP) || _input->GetTrg(XINPUT_BUTTON_STICK_UP)) {
		count--;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN) || _input->GetTrg(XINPUT_BUTTON_STICK_DOWN)) {
		count++;
		global._soundServer->DirectPlay("SE_Select");
	}

	_modeCount += count;
	_modeCount = (_modeCount + 3) % 3;

	//モードの選択
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		int textureHandle = MakeGraph(1920, 1080);
		switch (_modeCount) {
		case 0:
			global._soundServer->DirectPlay("SE_Break");
			UpdateCrackedScreen();
			GetDrawScreenGraph( 0, 0, 1920, 1080, textureHandle);
			MV1SetTextureGraphHandle(_modelHandle, 0, textureHandle, false);
			_IsGameStart = true;
			_currentTime = GetNowCount();
			break;
		case 1:
			global._soundServer->DirectPlay("SE_Press");
			SelectOption();
			break;
		case 2:
			global._soundServer->DirectPlay("SE_Press");
			SelectGameEnd();
			break;
		}
	}
};
//----------------------------------------------------------------------
// @brief 画面が割れる演出とゲーム開始の更新処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::UpdateSelectToGameStart() {
	int nowTime = GetNowCount() - _currentTime;
	if (nowTime >= 1000) {
		UpdateCrackedScreen();
		if (nowTime >= 5000) {
			SelectGameStart();
		}
	}
};
//----------------------------------------------------------------------
// @brief 画面が割れる演出の処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::UpdateCrackedScreen(){
	for (int i = 0; i < _frameSize; i++) {
		MATRIX matrix = MGetIdent();
		matrix = MGetRotX(_rotVec[i].x);
		matrix = MMult(matrix, MGetRotY(_rotVec[i].y));
		matrix = MMult(matrix, MGetRotZ(_rotVec[i].z));
		matrix = MMult(matrix, MGetTranslate(_MoveVec[i]));
		_MoveVec[i].y -= 2.5f;
		matrix = MMult(MV1GetFrameLocalMatrix(_modelHandle, i), matrix);
		MV1SetFrameUserLocalMatrix(_modelHandle, i, matrix);
	}
};
//----------------------------------------------------------------------
// @brief タイトル画面の描画処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::DrawTitleItems(){
	DrawGraph(0, 0, _handleMap["BackGround"], true);
	int handleX, handleY;
	//タイトルロゴの描画
	GetGraphSize(_handleMap["Title"], &handleX, &handleY);
	//x = 1920 / 2 - x / 2;
	handleX = 840;
	DrawGraph(handleX, 0, _handleMap["Title"], true);
	//チームロゴの描画
	GetGraphSize(_handleMap["Logo"], &handleX, &handleY);
	DrawGraph(0, 1080 - handleY, _handleMap["Logo"], true);
	//それぞれの項目の描画
	
	int centerX;
	int centerY[3] = { 550,720,910 };
	centerX = 1200;

	std::array<std::string,3> _handleNameList = { "Start","Option","Quit" };

	for (int i = 0; i < 3; i++) {
		int handleNum = i;
		float extRate = 1.0f;
		GetGraphSize(_handleMap[_handleNameList[handleNum]], &handleX, &handleY);
		if (i == _modeCount) { extRate = 1.1f; }
		DrawRotaGraph(centerX + handleX / 2, centerY[i], extRate, 0.0f, _handleMap[_handleNameList[handleNum]], true);
	}
};
//----------------------------------------------------------------------
// @brief 割れる画面の描画処理
// ＠return 無し
//----------------------------------------------------------------------
void ModeTitle::DrawCrackedScreen() {
	MV1DrawModel(_modelHandle);
};
//----------------------------------------------------------------------
// @brief 更新処理
// ＠return 成功しているか
//----------------------------------------------------------------------
bool ModeTitle::Process() {
	base::Process();
	if (!_IsGameStart) {
		//タイトル画面の更新
		UpdateSelectItems();
	}
	else {
		//割れる画面の更新
		UpdateSelectToGameStart();
	}

	return true;
}
//----------------------------------------------------------------------
// @brief 描画処理
// ＠return 成功しているか
//----------------------------------------------------------------------
bool ModeTitle::Render() {
	base::Render();
	if (!_IsGameStart) {
		//タイトル画面の描画
		DrawTitleItems();
	}
	else {
		//割れる画面の描画
		DrawCrackedScreen();
	}
	return true;
}
