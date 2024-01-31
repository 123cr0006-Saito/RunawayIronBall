#include "ModePause.h"

bool ModePause::Initialize() {
	if (!base::Initialize()) { return false; }

	//ガウスフィルターの作成
	gauss_handle = MakeGraph(1920, 1080);
	GetDrawScreenGraph(0, 0, 1920, 1080, gauss_handle);
	GraphFilter(gauss_handle, DX_GRAPH_FILTER_GAUSS, 16, 1800);

	//inputを作成
	_input = XInput::GetInstance();

	//初期化
	_selectItem = 0;
	_seVolum = 255;
	_bgmVolum = 255;
	return true;
};

bool ModePause::Terminate() {
	base::Terminate();
	DeleteGraph(gauss_handle);
	return true;
};

void ModePause::SelectSetVolum(int& setVolum) {
	if (_input->GetLx() > 0) {
		if (setVolum < 255) setVolum++;
	}
	else if (_input->GetLx() < 0) {
		if (setVolum > 0) setVolum--;
	}
};

void ModePause::SelectSetVibration() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		_isVibration = 1 - _isVibration;
	}
};

void ModePause::SelectOperationInstructions() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		//操作説明用のサーバーを作成
		//操作説明が終了した時ようにこのサーバーは削除しない
		//削除しても良いが、操作説明でこのサーバーを作成してから削除する　※ガウスの背景はおかしくなると思われる
		ModeServer::GetInstance()->Add(new ModeInstructions(), 100, "Instructions");
	}
};

void ModePause::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
	}
};

bool ModePause::Process() {
	base::Process();
	//オプション画面ではゲーム本編は実行しない
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	int count = 0;

	//選択項目の切り替え
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
	}

	_selectItem += count;
	_selectItem = (_selectItem + MAX_MODE) % MAX_MODE;


	//項目の選択
	switch (_selectItem) {
	case 0:
		//seの音量設定
		SelectSetVolum(_seVolum);
		break;
	case 1:
		//bgmの音量設定
		SelectSetVolum(_bgmVolum);
		break;
	case 2:
		//ゲームパッドの振動設定
		SelectSetVibration();
		break;
	case 3:
		//操作説明
		SelectOperationInstructions();
		break;
	case 4:
		//ゲーム終了画面
		SelectGameEnd();
		break;
	}

	//オプションの終了
	if (_input->GetTrg(XINPUT_BUTTON_START)) {
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};

bool ModePause::Render() {
	base::Render();
	//ゲーム本編を描画しない
	//背景の描画
	DrawGraph(0, 0, gauss_handle, true);

	//----------------------------------------------------------------------------------
	//ボリュームとかとかの描画（仮）
	int x = 1920 / 2;
	int y = 1080 / 2;

	for (int i = 0; i < MAX_MODE; i++) {
		int color = GetColor(255, 0, 0);
		int length = 50;
		switch (i) {
		case 0:
			length = _seVolum;
			break;
		case 1:
			length = _bgmVolum;
			break;
		case 2:
			_isVibration ? color = GetColor(0, 255, 0) : NULL;
			break;
		}
		if (_selectItem == i) color = GetColor(0, 0, 255);
		DrawBox(x - 122, y - 50 + i * 100, x - 122 + length, y + 50 + i * 100, color, true);
	}

	return true;
};