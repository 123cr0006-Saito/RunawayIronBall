
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
	_titleLogo = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/title.png"));
	_comandHandlle[0] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_starta_select.png"));
	_comandHandlle[1] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_option_select.png"));
	_comandHandlle[2] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_quit.png"));
	_comandHandlle[3] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_start.png"));
	_comandHandlle[4] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_option.png"));
	_comandHandlle[5] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_quit_selelct.png"));
	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();
	return true;
}

void ModeTitle::SelectGameStart() {
	ModeServer::GetInstance()->Del(this);
	ModeServer::GetInstance()->Add(new ModeTest(), 1, "test");
};

void ModeTitle::SelectOption() {
	ModeServer::GetInstance()->Add(new ModePause(), 1, "Pause");
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
	printfDx("\n\n%d", _modeCount);
	int x, y;
	//タイトルロゴの描画
	GetGraphSize(_titleLogo, &x, &y);
	x = 1920 / 2 - x / 2;
	DrawGraph(x, 100, _titleLogo, true);

	//それぞれの項目の描画
	GetGraphSize(_comandHandlle[0], &x, &y);
	x = 1920 / 2 - x / 2;
	y = 1080 / 2 - y / 2;

	for (int i = 0; i < 3; i++) {
		int handleNum = i;
		if (i == _modeCount) { handleNum += 3; }
		DrawGraph(x , y + 100 + i * 150, _comandHandlle[handleNum],true);
	}

	return true;
}
