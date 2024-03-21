#include "ModePause.h"

bool ModePause::Initialize() {
	if (!base::Initialize()) { return false; }

	//�K�E�X�t�B���^�[�̍쐬
	gauss_handle = MakeGraph(1920, 1080);
	GetDrawScreenGraph(0, 0, 1920, 1080, gauss_handle);
	GraphFilter(gauss_handle, DX_GRAPH_FILTER_GAUSS, 16, 1800);

	SetUseASyncLoadFlag(TRUE);

	_handleMap["back"] = ResourceServer::LoadGraph("back","res/ModePause/UI_Menu.png");
	_handleMap["check"] = ResourceServer::LoadGraph("check","res/ModePause/UI_Menu_Check.png");
	_handleMap["checkBox"] = ResourceServer::LoadGraph("checkBox","res/ModePause/UI_Menu_Check_Box.png");
	_handleMap["volumBar"] = ResourceServer::LoadGraph("volumBar","res/ModePause/UI_Menu_Pink_Gauge.png");
	_handleMap["se"] = ResourceServer::LoadGraph("se","res/ModePause/UI_Menu_SE.png");
	_handleMap["bgm"] = ResourceServer::LoadGraph("bgm","res/ModePause/UI_Menu_BGM.png");
	_handleMap["vibration"] = ResourceServer::LoadGraph("vibration","res/ModePause/UI_Menu_Controll.png");
	_handleMap["gide"] = ResourceServer::LoadGraph("gide","res/ModePause/UI_Menu_How_To.png");
	_handleMap["return"] = ResourceServer::LoadGraph("return","res/ModePause/UI_Menu_Back.png");

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
	_input = nullptr;
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
		ModeServer::GetInstance()->Add(NEW ModeInstructions(), 100, "Instructions");
		global._soundServer->DirectPlay("SE_Press");
	}
};

void ModePause::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		ModeServer::GetInstance()->Del("Game");
		ModeServer::GetInstance()->Del(this);
		if (!ModeServer::GetInstance()->Search("Title")) {
			ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "Title");
		}
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
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP) || _input->GetTrg(XINPUT_BUTTON_STICK_UP)) {
		count--;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN) || _input->GetTrg(XINPUT_BUTTON_STICK_DOWN)) {
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
	// ���ʂ̐ݒ�
	global._soundServer->SetSeVolume(_seVolum);
	global._soundServer->SetBgmVolume(_bgmVolum);
	//�I�v�V�����̏I��
	if (_input->GetTrg(XINPUT_BUTTON_START)) {
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
	float handleX, handleY;

	DrawGraph(0, 0, _handleMap["back"], true);
	DrawGraph(1000, 550, _handleMap["checkBox"], true);

	int length[] = { _seVolum,_bgmVolum };
	GetGraphSizeF(_handleMap["volumBar"], &handleX, &handleY);
	for (int i = 0; i < 2; i++) {
		DrawExtendGraph(400, 380 + 110 * i, 400 + (handleX / 255 * length[i]), 380 + 110 * i + handleY, _handleMap["volumBar"],true);
	}

	int x = 180;
	int y = 400;

	std::array<std::string,5> _itemList = { "se","bgm","vibration","gide","return" };

	for (int i = 0; i < MAX_MODE; i++) {
		int _selectedItems = 0;
		float extRate = 1.0f;		
		
		if (_selectItem == i)  extRate = 1.1f; 
		int length = 50;
		switch (i) {
		case 2:
			//y -= 50;
			GetGraphSizeF(_handleMap["check"], &handleX, &handleY);
			if (_isVibration)  DrawGraph(980+handleX/2, y + handleY / 2 - 80,  _handleMap["check"], true);
			break;
		case 4:
			x += 600;
			break;
		}
		GetGraphSizeF(_handleMap[_itemList[i]], &handleX, &handleY);
		DrawRotaGraph(x + handleX/2 , y + handleY/2 , extRate, 0.0f, _handleMap[_itemList[i]], true);
		y += 100;
	}

	return true;
};