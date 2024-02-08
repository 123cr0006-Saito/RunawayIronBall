#include "ModeInstructions.h"

bool ModeInstructions::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	//������
	_listChoice = 0;
	_listViewNum = 0;

	//�摜�̓ǂݍ���
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
	//�I�����ڂ̈ړ�
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
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
	int y = 0;
	int handle_x, handle_y;
	GetGraphSize(_handle[0].item, &handle_x, &handle_y);
#ifdef NO_IMAGE

	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		int color = GetColor(0, 10 * i, 0);
		if (i >= _listViewNum && i < _listViewNum + _listViewMax) {
			if (i == _listChoice) {
				color = GetColor(255, 0, 255);
				DrawBox(1000, 200, 1800, 500, GetColor(0, 0, 10 * i), true);//���W�͎G�ɐݒ� �摜�����Ă��������
				DrawBox(1000, 700, 1800, 1000, GetColor(0, 10 * i, 0), true);//���W�͎G�ɐݒ� �摜�����Ă��������
			}
			DrawBox(50, y + 100, 350, y + 100 + 100, color, true);
			y += 100; //handle_y + 100; // 100�͌��� ���摜���Ȃ���������handle_y���R�����g�A�E�g
		}
	}

#else
	for (int i = 0; i < LIST_SIZE_MAX; i++) {
		if (i >= _listViewNum && i < _listViewNum + _listViewMax) {
			DrawGraph(50, y, _handle[i].item, true);
			if (i == _listChoice) {
				DrawGraph(1000, 200, _handle[i].image, true);//���W�͎G�ɐݒ� �摜�����Ă��������
				DrawGraph(1000, 700, _handle[i].explanation, true);//���W�͎G�ɐݒ� �摜�����Ă��������
			}
			y += /*handle_y + */100;// 100�͌��� ���摜���Ȃ���������handle_y���R�����g�A�E�g
		}
	}
#endif
	return true;
};