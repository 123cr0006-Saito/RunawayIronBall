//----------------------------------------------------------------------
// @filename ModeInstructions.cpp
// ��date: 2024/01/31
// ��author: saito ko
// @explanation
// �|�[�Y��ʂ��瑀�������ʂ�\������N���X
//----------------------------------------------------------------------
#include "ModeInstructions.h"
bool ModeInstructions::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	//������
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
	//�I�����ڂ̈ړ�
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN) || _input->GetTrg(XINPUT_BUTTON_STICK_DOWN)) {
		count++;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP) || _input->GetTrg(XINPUT_BUTTON_STICK_UP)) {
		count--;
		global._soundServer->DirectPlay("SE_Select");
	}
	_listChoice += count;
	_listChoice = (_listChoice + LIST_SIZE_MAX) % LIST_SIZE_MAX;
	//��ԏ�ɂȂ鍀�ڂ̈ړ�
	while (true) {
		if (_listChoice < _listViewNum) { _listViewNum--; continue; }
		if (_listChoice >= _listViewNum + _listViewMax) { _listViewNum++; continue; }
		break;
	}

	//���������ʂ̏I��
	if (_input->GetTrg(XINPUT_BUTTON_START) != 0) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};

bool ModeInstructions::Render() {

	DrawGraph(0,440,_frameHandle, true);

	int handleX[5] = {280,340, 290, 350, 310};
	int handleY[5] = {520 ,605,690,765,850};
	
	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		float Extrate = 1.0f;
		
		if (i == _listChoice) {
			DrawGraph(400, 150, _imageHandle[i], true);//���W�͎G�ɐݒ� �摜�����Ă��������
			Extrate = 1.1f;
		}
		DrawRotaGraph(handleX[i], handleY[i], Extrate, 0.0f, _itemHandle[i], true);
	}
	
	return true;
};