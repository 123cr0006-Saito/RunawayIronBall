#include "ModePause.h"

bool ModePause::Initialize() {
	if (!base::Initialize()) { return false; }

	//�K�E�X�t�B���^�[�̍쐬
	gauss_handle = MakeGraph(1920, 1080);
	GetDrawScreenGraph(0, 0, 1920, 1080, gauss_handle);
	GraphFilter(gauss_handle, DX_GRAPH_FILTER_GAUSS, 16, 1800);

	SetUseASyncLoadFlag(TRUE);

	_handleMap["back"] = ResourceServer::LoadGraph("res/ModePause/UI_Menu.png");
	_handleMap["check"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_check.png");
	_handleMap["checkBox"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/munu_ui_check_box.png");
	_handleMap["volumBar"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/UI_Menu_Bar.png");
	_handleMap["se"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_se.png");
	_handleMap["bgm"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_bgm.png");
	_handleMap["vibration"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_controller_vibration.png");
	_handleMap["gide"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_operation_gide.png");
	_handleMap["return"] = ResourceServer::LoadGraph("res/TemporaryMaterials/Pause/menu_ui_title_return.png");

	SetUseASyncLoadFlag(FALSE);

	//input���쐬
	_input = XInput::GetInstance();

	//������
	_selectItem = 0;
	_seVolum = global._soundServer->GetSeVolume();
	_bgmVolum = global._soundServer->GetBgmVolume();
	return true;
};

bool ModePause::Terminate() {
	base::Terminate();
	DeleteGraph(gauss_handle);
	return true;
};

void ModePause::SelectSetVolum(int& setVolum) {
	if (_input->GetKey(XINPUT_BUTTON_DPAD_RIGHT)) {
		if (setVolum < 255) setVolum++;
	}
	else if (_input->GetKey(XINPUT_BUTTON_DPAD_LEFT)) {
		if (setVolum > 0) setVolum--;
	}
};

void ModePause::SelectSetVibration() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		_isVibration = 1 - _isVibration;
		global._soundServer->DirectPlay("SE_Press");
	}
};

void ModePause::SelectOperationInstructions() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		//��������p�̃T�[�o�[���쐬
		//����������I���������悤�ɂ��̃T�[�o�[�͍폜���Ȃ�
		//�폜���Ă��ǂ����A��������ł��̃T�[�o�[���쐬���Ă���폜����@���K�E�X�̔w�i�͂��������Ȃ�Ǝv����
		ModeServer::GetInstance()->Add(new ModeInstructions(), 100, "Instructions");
		global._soundServer->DirectPlay("SE_Press");
	}
};

void ModePause::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
		global._soundServer->DirectPlay("SE_Press");
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
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
		global._soundServer->DirectPlay("SE_Select");
	}

	_selectItem += count;
	_selectItem = (_selectItem + MAX_MODE) % MAX_MODE;


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
		global._soundServer->SetSeVolume(_seVolum);
		global._soundServer->SetBgmVolume(_bgmVolum);
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};

bool ModePause::Render() {
	base::Render();
	//�Q�[���{�҂�`�悵�Ȃ�
	//�w�i�̕`��
	DrawGraph(0, 0, gauss_handle, true);

	//----------------------------------------------------------------------------------
	//�{�����[���Ƃ��Ƃ��̕`��i���j
	int handleX, handleY;

	DrawGraph(0, 0, _handleMap["back"], true);
	DrawGraph(900, 460, _handleMap["checkBox"], true);

	int length[] = { _seVolum,_bgmVolum };
	GetGraphSize(_handleMap["volumBar"], &handleX, &handleY);
	for (int i = 0; i < 2; i++) {
		DrawExtendGraph(500, 250 + 110 * i, 500 + (handleX / 255 * length[i]), 250 + 110 * i + handleY, _handleMap["volumBar"],true);
	}

	std::array<std::string,5> _itemList = { "se","bgm","vibration","gide","return" };

	for (int i = 0; i < MAX_MODE; i++) {
		int _selectedItems = 0;
		int _gameEnd = 0;
		float extRate = 1.0f;
		
		if (_selectItem == i)  extRate = 1.1f; 
		int length = 50;
		switch (i) {
		case 2:
			GetGraphSize(_handleMap["check"], &handleX, &handleY);
			if (_isVibration)  DrawGraph(900 + (70 - handleX) / 2, 460 + (70 - handleY) / 2, _handleMap["check"], true);
			break;
		case 4:
			_gameEnd = 190;
			break;
		}
		GetGraphSize(_handleMap[_itemList[i]], &handleX, &handleY);
		
	//	DrawGraph(180 + _selectedItems, 250 + 110 * i + _gameEnd, _itemHandle[i], true);
		DrawRotaGraph(180 + handleX/2, 250 + handleY/2 + 110 * i + _gameEnd, extRate, 0.0f, _handleMap[_itemList[i]], true);
	}

	return true;
};