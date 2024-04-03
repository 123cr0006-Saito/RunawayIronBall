//----------------------------------------------------------------------
// @filename LoadingPlayer.cpp
// ��date: 2024/03/03
// ��author: saito ko
// @explanation
// loading���̃L�����N�^�[�̂𐧌�E�`����s���N���X
//----------------------------------------------------------------------
#include "LoadingPlayer.h"
LoadingPlayer::LoadingPlayer(){

	_modelHandle = ResourceServer::MV1LoadModel("LoadingPlayer", "res/Character/Loading/Cg_Player_Girl.mv1");
	float rotation = Math::DegToRad(90);// -x �����Ɍ�����
	MV1SetRotationXYZ(_modelHandle, VGet(0,rotation,0));
	for(int i = 0; i < MV1GetMaterialNum(_modelHandle); i++){
		MV1SetMaterialOutLineWidth(_modelHandle, i, 0);
		MV1SetMaterialOutLineDotWidth(_modelHandle, i, 0);
	}


	_pos = VGet(0, 0, 0);
	
	// �S���̏�����
	_chain = NEW LoadingChain();
	_chain->Init();
	_chain->SetPlayerModelHandle(_modelHandle);
	// �S���̈ړ���Ԃ��u�Ǐ]�v�ɐݒ�
	_chain->SetBallPosition(VAdd(_pos, VGet(500, 0, 0)));

	// �F�̒���
	_modelColor = NEW ModelColor();
	_modelColor->Init(_modelHandle);
	_modelColor->ChangeFlickerTexture(true);

	//�A�j���[�V�����̏�����
	std::string frameName;
	if (rand() % 2 == 0) {
		frameName = "MO_PL_Walk";
	}
	else {
		frameName = "MO_PL_Run";
	}

	int animIIdex = MV1GetAnimIndex(_modelHandle, frameName.c_str());

	_attachIndex = MV1AttachAnim(_modelHandle, animIIdex, -1, false);
	_totalTime = MV1GetAttachAnimTotalTime(_modelHandle,_attachIndex);
	_playTime = 0.0f;
};

LoadingPlayer::~LoadingPlayer() {
	MV1DeleteModel(_modelHandle);
	delete _chain; _chain = nullptr;
	delete _modelColor; _modelColor = nullptr;
};

bool LoadingPlayer::Process(){
	_chain->Process();

	_pos.x -= 20;
	MV1SetPosition(_modelHandle, _pos);
	AnimationProcess();
	return true;
};

bool LoadingPlayer::AnimationProcess(){

	MV1SetAttachAnimTime(_modelHandle, _attachIndex, _playTime);

	_playTime += 1.0f;
	if (_playTime > _totalTime) {
		_playTime = 0.0f;
	}

	return true;
};

bool LoadingPlayer::Render() {
	MV1DrawModel(_modelHandle);
	_chain->Render();
	return true;
};