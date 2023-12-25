#include "ModePause.h"

bool ModePause::Initialize() {
	if (!base::Initialize()) { return false; }

	//�K�E�X�t�B���^�[�̍쐬
	gauss_handle = MakeGraph(1920, 1080);
	GetDrawScreenGraph(0, 0, 1920, 1080, gauss_handle);
	GraphFilter(gauss_handle, DX_GRAPH_FILTER_GAUSS, 16, 1800);

	//input���쐬
	_input = XInput::GetInstance();

	//������
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

};

void ModePause::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
	}
};

bool ModePause::Process() {
	base::Process();
	//�I�v�V������ʂł̓Q�[���{�҂͎��s���Ȃ�
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	int count = 0;

	//�I�����ڂ̐؂�ւ�
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
	}

	_selectItem += count;
	_selectItem = _selectItem % MAX_MODE;

	if (_selectItem < 0) {
		_selectItem = MAX_MODE - 1;
	}

	//���ڂ̑I��
	switch (_selectItem) {
	case 0:
		//se�̉��ʐݒ�
		SelectSetVolum(_seVolum);
		break;
	case 1:
		//bgm�̉��ʐݒ�
		SelectSetVolum(_bgmVolum);
		break;
	case 2:
		//�Q�[���p�b�h�̐U���ݒ�
		SelectSetVibration();
		break;
	case 3:
		//�������
		SelectOperationInstructions();
		break;
	case 4:
		//�Q�[���I�����
		SelectGameEnd();
		break;
	}

	//�I�v�V�����̏I��
	if (_input->GetTrg(XINPUT_BUTTON_START)) {
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};

bool ModePause::Render() {
	base::Render();
	//�Q�[���{�҂�`�悵�Ȃ�
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	//�w�i�̕`��
	DrawGraph(0, 0, gauss_handle, true);

	//----------------------------------------------------------------------------------
	//�{�����[���Ƃ��Ƃ��̕`��i���j
	int x = 1920 / 2;
	int y = 1080 / 2;

	for (int i = 0; i < 3; i++) {
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
		DrawBox(x - 122, y - 50 + i * 200, x - 122 + length, y + 50 + i * 200, color, true);
	}

	return true;
};