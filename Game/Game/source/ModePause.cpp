//----------------------------------------------------------------------
// @filename ModePause.cpp
// @date: 2023/12/25
// @author: saito ko
// @explanation
// ���ʐݒ��U���ݒ�A���������ʁA�Q�[���I����I������|�[�Y��ʂ̃N���X
//----------------------------------------------------------------------
#include "ModePause.h"

const unsigned short ModePause::vertex[6]{ 0,1,2,2,1,3 };
//----------------------------------------------------------------------------------
// @brief: ����������
// @return: �����������ǂ���
//----------------------------------------------------------------------------------
bool ModePause::Initialize() {
	if (!base::Initialize()) { return false; }

	//�K�E�X�t�B���^�[�̍쐬
	_gaussHandle = MakeGraph(1920, 1080);
	GetDrawScreenGraph(0, 0, 1920, 1080, _gaussHandle);
	GraphFilter(_gaussHandle, DX_GRAPH_FILTER_GAUSS, 16, 1800);

	SetUseASyncLoadFlag(TRUE);

	_handleMap["back"] = ResourceServer::LoadGraph("back","res/ModePause/UI_Menu.png");
	_handleMap["check"] = ResourceServer::LoadGraph("check","res/ModePause/UI_Menu_Check.png");
	_handleMap["checkBox"] = ResourceServer::LoadGraph("checkBox","res/ModePause/UI_Menu_Check_Box.png");
	_handleMap["volumBarFrame"] = ResourceServer::LoadGraph("volumBarFrame", "res/ModePause/UI_Menu_Black_Gauge.png");
	_handleMap["volumBar"] = ResourceServer::LoadGraph("volumBar","res/ModePause/UI_Menu_Pink_Gauge.png");
	_handleMap["se"] = ResourceServer::LoadGraph("se","res/ModePause/UI_Menu_SE.png");
	_handleMap["bgm"] = ResourceServer::LoadGraph("bgm","res/ModePause/UI_Menu_BGM.png");
	_handleMap["vibration"] = ResourceServer::LoadGraph("vibration","res/ModePause/UI_Menu_Controll.png");
	_handleMap["gide"] = ResourceServer::LoadGraph("gide","res/ModePause/UI_Menu_How_To.png");
	_handleMap["return"] = ResourceServer::LoadGraph("return","res/ModePause/UI_Menu_Back.png");

	SetUseASyncLoadFlag(FALSE);

	//input���쐬
	_input = XInput::GetInstance();

	//�ϐ��̏�����
	_selectItem = 0;
	_seVolum = global._soundServer->GetSeVolume();
	_bgmVolum = global._soundServer->GetBgmVolume();
	//���_�̏�����
	SetVertex();
	return true;
};
//----------------------------------------------------------------------------------
// @brief: ���_�̐ݒ�
// @return: ����
//----------------------------------------------------------------------------------
void ModePause::SetVertex(){
	// �摜�̔����̃T�C�Y���擾
	int cx, cy;
	GetGraphSize(_handleMap["volumBar"], &cx, &cy);
	cx = cx / 2;
	cy = cy / 2;
	// �K�v�ȕϐ����쐬
	//     x,      y,    u,    v
	float posTbl[][4] = {
		{-cx,-cy,0.0f,0.0f},
		{cx,-cy,1.0f,0.0f},
		{-cx,cy,0.0f,1.0f},
		{cx,cy,1.0f,1.0f}
	};
	VECTOR standardPos[2] = {VGet(641, 432, 0),VGet(630, 542, 0)};

	// ���_�̐ݒ�
	for (int i = 0; i < 4; i++) {

		VECTOR sePos = (VGet(posTbl[i][0], posTbl[i][1], 0));
		VECTOR bgmPos = (VGet(posTbl[i][0], posTbl[i][1], 0));

		_seGauge[i].pos = VAdd(standardPos[0], sePos);
		_seGauge[i].u = posTbl[i][2];
		_seGauge[i].v = posTbl[i][3];
		_seGauge[i].dif = GetColorU8(255, 255, 255, 255);
		_seGauge[i].rhw = 1.0f;

		_bgmGauge[i].pos = VAdd(standardPos[1], bgmPos);
		_bgmGauge[i].u = posTbl[i][2];
		_bgmGauge[i].v = posTbl[i][3];
		_bgmGauge[i].dif = GetColorU8(255, 255, 255, 255);
		_bgmGauge[i].rhw = 1.0f;
	}
};
//----------------------------------------------------------------------------------
// @brief: �I������
// @return: �����������ǂ���
//----------------------------------------------------------------------------------
bool ModePause::Terminate() {
	base::Terminate();
	DeleteGraph(_gaussHandle);
	_input = nullptr;
	return true;
};
//----------------------------------------------------------------------------------
// @brief: ���ʂ̐ݒ�
// @param: setVolum �ݒ肷�鉹��
// @return: ����
//----------------------------------------------------------------------------------
void ModePause::SelectSetVolum(int& setVolum) {
	if (_input->GetKey(XINPUT_BUTTON_DPAD_RIGHT)) {
		if (setVolum < 255) setVolum++;
	}
	else if (_input->GetKey(XINPUT_BUTTON_DPAD_LEFT)) {
		if (setVolum > 0) setVolum--;
	}
};
//----------------------------------------------------------------------------------
// @brief: �U���̐ݒ�
// @return: ����
//----------------------------------------------------------------------------------
void ModePause::SelectSetVibration() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.SetVibration();
		global._soundServer->DirectPlay("SE_Press");
	}
};
//----------------------------------------------------------------------------------
// @brief: ��������̑I��
// @return: ����
//----------------------------------------------------------------------------------
void ModePause::SelectOperationInstructions() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		//��������p�̃T�[�o�[���쐬
		//����������I���������悤�ɂ��̃T�[�o�[�͍폜���Ȃ�
		//�폜���Ă��ǂ����A��������ł��̃T�[�o�[���쐬���Ă���폜����@���K�E�X�̔w�i�͂��������Ȃ�Ǝv����
		ModeServer::GetInstance()->Add(NEW ModeInstructions(), 100, "Instructions");
		global._soundServer->DirectPlay("SE_Press");
	}
};
//----------------------------------------------------------------------------------
// @brief: �Q�[���I���̑I��
// @return: ����
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------
// @brief: ���_�̍X�V
// @return: ����
//----------------------------------------------------------------------------------
void ModePause::VertexProcess() {
	int handleX, handleY;
	GetGraphSize(_handleMap["volumBar"], &handleX, &handleY);
	float seRatio = static_cast<float>(_seVolum) / 255;
	float bgmRatio = static_cast<float>(_bgmVolum) / 255;

	for (int i = 0; i < 2; i++) {
		_seGauge[1 + 2 * i].pos.x = _seGauge[i * 2].pos.x + handleX * seRatio;
		_seGauge[1 + 2 * i].u = seRatio;
		_bgmGauge[1 + 2 * i].pos.x = _seGauge[2 * i].pos.x + handleX * bgmRatio;
		_bgmGauge[1 + 2 * i].u = bgmRatio;
	}
};
//----------------------------------------------------------------------------------
// @brief: �X�V����
// @return: �����������ǂ���
//----------------------------------------------------------------------------------
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
	global._soundServer->SetVoiceVolume(_seVolum);
	global._soundServer->SetBgmVolume(_bgmVolum);
	//���_�̍X�V
	VertexProcess();

	//�I�v�V�����̏I��
	if (_input->GetTrg(XINPUT_BUTTON_START)) {
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};
//----------------------------------------------------------------------------------
// @brief: �`�揈��
// @return: �����������ǂ���
//----------------------------------------------------------------------------------
bool ModePause::Render() {
	base::Render();
	//�Q�[���{�҂�`�悵�Ȃ�
	//�w�i�̕`��
	DrawGraph(0, 0, _gaussHandle, true);
	
	float handleX, handleY;
	//�w�i�̕`��
	DrawGraph(0, 0, _handleMap["back"], true);
	//�`�F�b�N�{�b�N�X�̕`��
	DrawGraph(800, 550, _handleMap["checkBox"], true);
	//�U���`�F�b�N�}�[�N�̐ݒ�
	if (global.GetVibration())  DrawGraph(800, 550, _handleMap["check"], true);
	//���ʃo�[�̒��_�f�[�^���R�s�[���s��ŉ�]
	int length[] = { _seVolum,_bgmVolum };
	GetGraphSizeF(_handleMap["volumBar"], &handleX, &handleY);
	VERTEX2D seGauge[4];
	VERTEX2D bgmGauge[4];
	float angle = -5 * DX_PI / 180;
	MATRIX matrix = MGetRotZ(angle);
	for(int i = 0 ; i < 4; i++){
	    seGauge[i] = _seGauge[i];
	    seGauge[i].pos = VTransform(_seGauge[i].pos,matrix);
	    bgmGauge[i] = _bgmGauge[i];
	    bgmGauge[i].pos = VTransform(_bgmGauge[i].pos, matrix);
	}
	//���ʃo�[�̃t���[���̕`��
	VECTOR framePos[2] = { VGet(680, 380, 0),VGet(680, 490, 0) };
	for(int i = 0 ; i < 2; i++){
	    DrawRotaGraph(framePos[i].x,framePos[i].y,1.0f,angle, _handleMap["volumBarFrame"],true);
	}
	//���ʃo�[�̕`��
	DrawPrimitiveIndexed2D(seGauge, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handleMap["volumBar"], true);
	DrawPrimitiveIndexed2D(bgmGauge, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handleMap["volumBar"], true);


	std::array<std::string,5> _itemList = { "se","bgm","vibration","gide","return" };

	int selectItemX[5] ={260,280,370,320,920};
	int selectItemY[5] = { 430,540,650,790,840 };
	//���ڂ̕`��
	for (int i = 0; i < MAX_MODE; i++) {
		float extRate = 1.0f;		
		if (_selectItem == i)  extRate = 1.1f; 
		GetGraphSizeF(_handleMap[_itemList[i]], &handleX, &handleY);
		DrawRotaGraph(selectItemX[i], selectItemY[i], extRate, 0.0f, _handleMap[_itemList[i]], true);
	}

	return true;
};