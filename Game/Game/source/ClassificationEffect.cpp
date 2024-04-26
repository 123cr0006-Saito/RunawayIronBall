//----------------------------------------------------------------------
// @filename ClassificationEffect.cpp
// @date: 2024/02/28
// @author: saito ko
// @explanation
// FrameData����̃R�}���h���󂯎��A����ɉ������G�t�F�N�g��SE���Đ�����N���X
//----------------------------------------------------------------------
#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
ClassificationEffect::ClassificationEffect() {
	_instance = this;
	vib = NEW Vibration(DX_INPUT_PAD1);
	_sVib = NEW ScreenVibration();
	// �t�@�C���p�X
	std::string filePath = "Data/DirectionList/DirectionList.csv";
	//�t�@�C���ǂݍ���
	CFile csvFile(filePath);
	//�t�@�C�����J���Ă�������s
	if (csvFile.Success()) {
		int c = 0;
		const char* p = (const char*)csvFile.Data();
		int size = csvFile.Size();
		while (c < size) {
			int key = 0;
			std::pair<std::string, int> param;
			c += GetString(&p[c], &param.first); // �G�t�F�N�g�����擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &key); // key���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.second); // �p�����[�^�[���擾
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			// �f�[�^��ǉ�
			_commandList[key] = param;
		}
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "DirectionList ���J���܂���ł����B", "�G���[", MB_OK);
#endif
	}
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
ClassificationEffect::~ClassificationEffect() {
	_commandList.clear();
	_instance = nullptr;
	delete vib ; vib = nullptr;
	delete _sVib; _sVib = nullptr;
};
//----------------------------------------------------------------------
// @brief ���t���[���s������
// @return �Ȃ�
//----------------------------------------------------------------------
void ClassificationEffect::Process(){
	_sVib->UpdateScreenVibration();
};
//----------------------------------------------------------------------
// @brief �R�}���h�ɉ������������s��
// @param param �R�}���h�p�����[�^
// @return �Ȃ�
//----------------------------------------------------------------------
void ClassificationEffect::SetClassification(CommandParam param) {
	// �R�}���h�ɉ������������s��
	// �R���g���[���[�U��
	if (param.first == Play_Vibration) {
		if(global.GetVibration()) vib->SetVibrationController(param.second,1000);
	}
	// SE�Đ�
	else if (param.first == Play_SE) {
		int voiceNum = static_cast<int>(param.second);
		std::string voiceName = _commandList[voiceNum].first;
		int randomMax = _commandList[voiceNum].second;
		if (randomMax != 0) {
			int randomNum = rand() % randomMax + 1;
			voiceName += std::to_string(randomNum);
		}
		global._soundServer->DirectPlay(voiceName);
	}
	// ��ʐU��(x��)
	else if (param.first == Play_CameraVibration_X) {
		ScreenVibration::GetInstance()->SetVibrationX(param.second, 20);
	}
	// ��ʐU��(y��)
	else if (param.first == Play_CameraVibration_Y) {
		ScreenVibration::GetInstance()->SetVibrationY(param.second, 20);
	}
	// �����G�t�F�N�g
	else if (param.first == Play_Effect_Dust) {
		int effectName = static_cast<int>(param.second);
		ResourceServer::Mult handle = ResourceServer::SearchMult(_commandList[effectName].first.c_str());
		if (handle.AllNum != 0) {
			VECTOR pos = Player::GetInstance()->GetIBPos();
			float animSpeed = 1.0f / 60.0f * 1000;
			BoardPolygonDust* dust = NEW BoardPolygonDust(pos, _commandList[effectName].second, handle.handle, handle.AllNum, animSpeed);
			EffectManeger::GetInstance()->LoadEffect(dust);
		}
	}
	// �G�t�F�N�V�A (�v���C���[���S)
	else if (param.first == Play_Effekseer_PC) {	
		VECTOR* vec = Player::GetInstance()->GetPositionPtr();
		VECTOR* dir = Player::GetInstance()->GetForwardDirPtr();
		float height = Player::GetInstance()->GetCollision().up_pos.y / 2.0f; // �����̔���������o��
		CreateEffeckseer(param.second, vec,height,*dir);
	}
	// �G�t�F�N�V�A(�S�����S)
	else if (param.first == Play_Effekseer_IC) {
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		CreateEffeckseer(param.second, pos);
	}
	// �G�t�F�N�V�A (�S������)
	else if (param.first == Play_Effekseer_IU) {
		VECTOR* pos = Player::GetInstance()->GetIBPosPtr();
		float height = Player::GetInstance()->GetIBBodyCollision().r * -1.0f; // �����̔���������o�� ����͍����Ԃ񉺂���̂� -1���|���� IBbodyCollision�̔��a���擾
		CreateEffeckseer(param.second, pos,height);
	}
	// �G�t�F�N�V�A (��]�U��)
	else if (param.first == Play_Effekseer_Rotation) {
		int effectName = static_cast<int>(param.second);
		int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
		if (handle != -1) {
			VECTOR* pos = Player::GetInstance()->GetPositionPtr();
			VECTOR* dir = Player::GetInstance()->GetForwardDirPtr();
			float height = Player::GetInstance()->GetCollision().up_pos.y; // �����̔���������o��
			EffekseerRotation* effect = NEW EffekseerRotation(handle, pos, _commandList[effectName].second, dir, height);
			EffectManeger::GetInstance()->LoadEffect(effect);
		}
	}
	// �G���[
	else {
#ifdef _DEBUG
		MessageBox(NULL, "�G�t�F�N�g�R�}���h��������܂���ł����B", "�G���[", MB_OK);
#endif
	}
};
//----------------------------------------------------------------------
// @brief �G�t�F�N�g�̐���
// @param param ��������G�t�F�N�g�̔ԍ�
// @param pos ���W
// @param height ����W����+y�������ɂ��炷����
// @param rotation ��]�l
// @return �Ȃ�
//----------------------------------------------------------------------
void ClassificationEffect::CreateEffeckseer(float param, VECTOR* pos, float height , VECTOR rotation ) {
	int effectName = static_cast<int>(param);
	int handle = ResourceServer::SearchSingle(_commandList[effectName].first.c_str(), ResourceServer::TYPE::Efk);
	EffekseerBase* effekseer = NEW EffekseerPosSynchro(handle, pos, _commandList[effectName].second, rotation,height);
	EffectManeger::GetInstance()->LoadEffect(effekseer);
};