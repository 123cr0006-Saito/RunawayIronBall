
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeTest.h"
#include "ModePause.h"
#include "math.h"


bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }

	_input = XInput::GetInstance();
	_modeCount = 0;
	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();
	return true;
}

void ModeTitle::SelectGameStart() {
	ModeServer::GetInstance()->Del(this);
	ModeServer::GetInstance()->Add(NEW ModeTest(), 1, "test");
};

void ModeTitle::SelectOption() {
	ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
};

void ModeTitle::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
	}
};

bool ModeTitle::Process() {
	base::Process();
	int count = 0;

	//モード選択の切り替え
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
	}

	_modeCount += count;
	_modeCount = (_modeCount +3)% 3;

	//モードの選択
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		switch (_modeCount) {
		case 0:
			SelectGameStart();
			break;
		case 1:
			SelectOption();
			break;
		case 2:
			SelectGameEnd();
			break;
		}
	}

	return true;
}

bool ModeTitle::Render() {
	base::Render();
	clsDx();
	printfDx("%d", _modeCount);
	int x = 1920 / 2;
	int y = 1080 / 2;

	for (int i = 0; i < 3; i++) {
		int color = GetColor(255, 0, 0);
		if (i == _modeCount) { color = GetColor(0, 255, 0); }
		DrawBox(x - 200, y - 50 + i * 200, x + 200, y + 50 + i * 200, color, true);
	}

	return true;
}
