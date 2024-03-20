#include "ModeInstructions.h"

bool ModeInstructions::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	//‰Šú‰»
	_listChoice = 0;
	_listViewNum = 0;

	_frameHandle = ResourceServer::LoadGraph("InstructionFrame", "res/ModePause/Operation/Frame.png");
	ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5,_imageHandle);
	ResourceServer::LoadMultGraph("OperateItem", "res/ModePause/Operation/UI_Operation", ".png", 5, _itemHandle);
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
	//‘I‘ğ€–Ú‚ÌˆÚ“®
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN) && _input->GetTrg(XINPUT_BUTTON_STICK_DOWN)) {
		count++;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP) && _input->GetTrg(XINPUT_BUTTON_STICK_UP)) {
		count--;
		global._soundServer->DirectPlay("SE_Select");
	}
	_listChoice += count;
	_listChoice = (_listChoice + LIST_SIZE_MAX) % LIST_SIZE_MAX;
	//ˆê”Ôã‚É‚È‚é€–Ú‚ÌˆÚ“®
	while (true) {
		if (_listChoice < _listViewNum) { _listViewNum--; continue; }
		if (_listChoice >= _listViewNum + _listViewMax) { _listViewNum++; continue; }
		break;
	}

	//‘€ìà–¾‰æ–Ê‚ÌI—¹
	if (_input->GetTrg(XINPUT_BUTTON_START) != 0) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};

bool ModeInstructions::Render() {
	int x = -30,y = 380;
	int handleX, handleY;
	DrawGraph(0,300,_frameHandle, true);
	
	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		float Extrate = 1.0f;
		GetGraphSize(_itemHandle[i], &handleX, &handleY);
		
		if (i == _listChoice) {
			DrawGraph(500, 200, _imageHandle[i], true);//À•W‚ÍG‚Éİ’è ‰æ‘œ‚ª—ˆ‚Ä‚©‚ç”÷’²®
			Extrate = 1.1f;
		}
		DrawRotaGraph(300 + x, y, Extrate, 0.0f, _itemHandle[i], true);
		y += 85;
		x *= -1;
	}
	

	return true;
};