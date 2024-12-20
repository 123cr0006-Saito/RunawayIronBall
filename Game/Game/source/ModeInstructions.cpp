//----------------------------------------------------------------------
// @filename ModeInstructions.cpp
// @date: 2024/01/31
// @author: saito ko
// @explanation
// ポーズ画面から操作説明画面を表示するクラス
//----------------------------------------------------------------------
#include "ModeInstructions.h"
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeInstructions::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	//初期化
	_listChoice = 0;
	_listViewNum = 0;
	_currentTime = GetNowCount();

	_frameHandle = ResourceServer::LoadGraph("InstructionFrame", "res/ModePause/Operation/Frame.png");
	ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5,_imageHandle);
	ResourceServer::LoadMultGraph("OperateItem", "res/ModePause/Operation/UI_Operation", ".png", 5, _itemHandle);
	return true;
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeInstructions::Terminate(){
	base::Terminate();
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeInstructions::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	int count = 0;
	//選択項目の移動
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN) || _input->GetTrg(XINPUT_BUTTON_STICK_DOWN)) {
		count++;
		global._soundServer->DirectPlay("SE_Select");
		_currentTime = GetNowCount();
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP) || _input->GetTrg(XINPUT_BUTTON_STICK_UP)) {
		count--;
		global._soundServer->DirectPlay("SE_Select");
		_currentTime = GetNowCount();
	}
	_listChoice += count;
	_listChoice = (_listChoice + LIST_SIZE_MAX) % LIST_SIZE_MAX;
	//一番上になる項目の移動
	while (true) {
		if (_listChoice < _listViewNum) { _listViewNum--; continue; }
		if (_listChoice >= _listViewNum + _listViewMax) { _listViewNum++; continue; }
		break;
	}

	//操作説明画面の終了
	if (_input->GetTrg(XINPUT_BUTTON_START) != 0) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool ModeInstructions::Render() {

	DrawGraph(0,440,_frameHandle, true);

	int handleX[5] = {280,340, 290, 350, 310};
	int handleY[5] = {520 ,605,690,765,850};
	
	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		float Extrate = 1.0f;
		
		if (i == _listChoice) {
			DrawGraph(400, 150, _imageHandle[i], true);//座標は雑に設定 画像が来てから微調整
			Extrate = 1.0f + 0.1f * sin(2.0f * DX_PI * (GetNowCount() - _currentTime) /2000.0f);
		}
		DrawRotaGraph(handleX[i], handleY[i], Extrate, 0.0f, _itemHandle[i], true);
	}
	
	return true;
};