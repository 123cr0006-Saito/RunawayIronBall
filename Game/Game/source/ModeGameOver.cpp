#include "ModeGameOver.h"
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTest.h"
#include "ModeTitle.h"
bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	_handle["Logo"] = ResourceServer::LoadGraph("Logo","res/ModeGameOver/UI_Game_Over.png");
	_handle["Retry"] = ResourceServer::LoadGraph("Retry","res/ModeGameOver/UI_Game_Over_Retry.png");
	_handle["Give"] = ResourceServer::LoadGraph("Give","res/ModeGameOver/UI_Game_Over_Title_Back.png");
	_selectItem = 0;
	return true;
};

bool ModeGameOver::Terminate() {
	base::Terminate();
	return true;
};

bool ModeGameOver::Process() {
	base::Process();

	//‘I‘ð€–Ú‚ÌØ‚è‘Ö‚¦
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
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "Game");
			ModeServer::GetInstance()->Del(this);
		}
		else {
			ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
			ModeServer::GetInstance()->Del(this);
		}
	}

	return true;
};

bool ModeGameOver::Render() {
	base::Render();
	int handleX, handleY;
	std::vector<std::string> name = { "Logo","Retry","Give" };

	DrawRotaGraph(1920/2, 300, 1.0f, 0.0f, _handle[name[0]], true);
	for (int i = 1; i < name.size(); i++) {
		float exrate = 1.0f;
		if (i == _selectItem + 1)exrate = 1.1f;
		GetGraphSize(_handle[name[i]], &handleX, &handleY);
		DrawRotaGraph(1920 / 2, 1080 / 2 + 175 * i, exrate, 0.0f, _handle[name[i]], true);
	}
	
	return true;
};