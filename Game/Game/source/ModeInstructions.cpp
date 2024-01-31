#include "ModeInstructions.h"

bool ModeInstructions::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	//初期化
	_listChoice = 0;
	_listViewNum = 0;

	//画像の読み込み
	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		_handle[i].item = 0;
		_handle[i].image = 0;
		_handle[i].explanation = 0;
	}
	return true;
};

bool ModeInstructions::Terminate(){
	base::Terminate();
	return true;
};

bool ModeInstructions::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	int count = 0;
	//選択項目の移動
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
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

bool ModeInstructions::Render() {
	int y = 0;
	int handle_x, handle_y;
	GetGraphSize(_handle[0].item, &handle_x, &handle_y);
#ifdef NO_IMAGE

	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		int color = GetColor(0, 10 * i, 0);
		if (i >= _listViewNum && i < _listViewNum + _listViewMax) {
			if (i == _listChoice) {
				color = GetColor(255, 0, 255);
				DrawBox(1000, 200, 1800, 500, GetColor(0, 0, 10 * i), true);//座標は雑に設定 画像が来てから微調整
				DrawBox(1000, 700, 1800, 1000, GetColor(0, 10 * i, 0), true);//座標は雑に設定 画像が来てから微調整
			}
			DrawBox(50, y + 100, 350, y + 100 + 100, color, true);
			y += 100; //handle_y + 100; // 100は隙間 仮画像がなかったためhandle_yをコメントアウト
		}
	}

#else
	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		if (i >= _listViewNum && i < _listViewNum + _listViewMax) {
			DrawGraph(50, y, _handle[i].item, true);
			if (i == _listChoice) {
				DrawGraph(1000, 200, _handle[i].image, true);//座標は雑に設定 画像が来てから微調整
				DrawGraph(1000, 700, _handle[i].explanation, true);//座標は雑に設定 画像が来てから微調整
			}
			y += /*handle_y + */100;// 100は隙間 仮画像がなかったためhandle_yをコメントアウト
		}
	}
#endif
	return true;
};