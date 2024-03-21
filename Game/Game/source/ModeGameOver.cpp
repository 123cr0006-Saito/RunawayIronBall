#include "AppFrame.h"
#include "ModeGameOver.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeFadeComeBack.h"

ModeGameOver::ModeGameOver(ModeGame* mode) {
	_modeGame = mode;
	_modeBossBattle = nullptr;
};

ModeGameOver::ModeGameOver(ModeBossBattle* mode){
	_modeGame = nullptr;
	_modeBossBattle = mode;

};

bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
	_handle["Logo"] = ResourceServer::LoadGraph("Logo","res/ModeGameOver/UI_Game_Over.png");
	_handle["Retry"] = ResourceServer::LoadGraph("Retry","res/ModeGameOver/UI_Game_Over_Retry.png");
	_handle["Give"] = ResourceServer::LoadGraph("Give","res/ModeGameOver/UI_Game_Over_Title_Back.png");
	_selectItem = 0;
	_selectEnd = false;
	// ���f���̓ǂݍ���
	_model = ResourceServer::MV1LoadModel("Player","res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	MV1SetPosition(_model, VGet(0, 0, 0));
	// �A�j���[�V�����̃A�^�b�`
	int animIndex = MV1GetAnimIndex(_model, "MO_PL_Game_Over");
	int attachAnim = MV1AttachAnim(_model, animIndex, -1, false);
	// �A�j���[�V�����̍Đ����Ԃ�ݒ�
	MV1SetAttachAnimTime(_model, attachAnim, 140);
	//�J�����̃^�[�Q�b�g���擾
	int frameIndex = MV1SearchFrame(_model, "Character1_Hips");
	_targetPos = MV1GetFramePosition(_model, frameIndex);
	// �J�����̈ʒu��ݒ�
	_cameraPos= VGet(0, 500, -500);

	global._soundServer->DirectPlay("PL_GameOver");
	return true;
};

bool ModeGameOver::Terminate() {
	base::Terminate();
	_input = nullptr;
	_handle.clear();
	return true;
};

void ModeGameOver::AnimProcess(){
	MATRIX matrix = MGetRotY(0.02f);
	_cameraPos = VTransform(_cameraPos, matrix);
	VECTOR pos = VAdd(_cameraPos, _targetPos);
	SetCameraPositionAndTarget_UpVecY(pos, _targetPos);
};

void ModeGameOver::SelectProcess(){
	if (_selectEnd) return ; // �I�����I������Ă���̂ŏ������I��

	//�I�����ڂ̐؂�ւ�
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_LEFT) || _input->GetTrg(XINPUT_BUTTON_STICK_LEFT)) {
		_selectItem = 1 - _selectItem;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_RIGHT) || _input->GetTrg(XINPUT_BUTTON_STICK_RIGHT)) {
		_selectItem = 1 - _selectItem;
		global._soundServer->DirectPlay("SE_Select");
	}

	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		_selectEnd = true;
		global._soundServer->DirectPlay("SE_Press");
		if (_selectItem == 0) {
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(3000, this), 100, "Fade");
			if (_modeGame != nullptr) {
				_modeGame->NewStage();
				Player::GetInstance()->MaxHeal();
			}
			else if(_modeBossBattle != nullptr){
				Player::GetInstance()->MaxHeal();
			}
		}
		else {
			ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
			ModeServer::GetInstance()->Del(_modeGame);
			ModeServer::GetInstance()->Del(this);
		}
	}

};

bool ModeGameOver::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	AnimProcess();
	SelectProcess();


	return true;
};

bool ModeGameOver::Render() {
	base::Render();
    // ���f���̕`��
	MV1DrawModel(_model);

	int handleX, handleY;
	std::vector<std::string> name = { "Logo","Retry","Give" };

	int ItemX[2] = {600,1330};

	DrawRotaGraph(1920/2, 200, 1.0f, 0.0f, _handle[name[0]], true);
	for (int i = 1; i < name.size(); i++) {
		float extrate = 1.0f;
		if (i == _selectItem + 1)extrate = 1.1f;
		DrawRotaGraph(ItemX[i-1], 800, extrate, 0.0f, _handle[name[i]], true);
	}
	
	return true;
};