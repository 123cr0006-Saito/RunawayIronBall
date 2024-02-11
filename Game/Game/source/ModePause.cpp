#include "ModePause.h"

bool ModePause::Initialize() {
	if (!base::Initialize()) { return false; }

	//ガウスフィルターの作成
	gauss_handle = MakeGraph(1920, 1080);
	GetDrawScreenGraph(0, 0, 1920, 1080, gauss_handle);
	GraphFilter(gauss_handle, DX_GRAPH_FILTER_GAUSS, 16, 1800);

	SetUseASyncLoadFlag(TRUE);
	 _backHandle = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu.png");
	 _optionHandle = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_operation_gide.png");
	 _checkHandle = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_check.png");
	 _checkBoxHandle = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/munu_ui_check_box.png");
	_itemHandle[0] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_bgm.png");
	_itemHandle[1] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_se.png");
	_itemHandle[2] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_controller_vibration.png");
	_itemHandle[3] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_operation_gide.png");
	_itemHandle[4] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_title_return.png");
	SetUseASyncLoadFlag(FALSE);

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

	DrawGraph(100, 65, _backHandle, true);
	DrawGraph(180, 110, _optionHandle, true);
	DrawGraph(900, 460, _checkBoxHandle, true);

	for (int i = 0; i < MAX_MODE; i++) {
		int _selectedItems = 0;
		int _gameEnd = 0;
		if (_selectItem == i) _selectedItems = 50;
		int length = 50;
		switch (i) {
		case 0:
			length = _seVolum;
			break;
		case 1:
			length = _bgmVolum;
			break;
		case 2:
			int x, y;
			GetGraphSize(_checkHandle, &x, &y);
			if (_isVibration)  DrawGraph(900 + (70 - x) / 2, 460 + (70 - y) / 2, _checkHandle, true);
			break;
		case 4:
			_gameEnd = 190;
			break;
		}
		
		
		DrawGraph(180 + _selectedItems, 250 + 110 * i + _gameEnd, _itemHandle[i], true);
	}

	return true;
};