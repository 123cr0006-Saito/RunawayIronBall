#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
ClassificationEffect::ClassificationEffect() {
	_instance = this;
	// �t�@�C���p�X
	std::string filePath = "Data/FrameData/DirectionList.csv";
	//�t�@�C���ǂݍ���
	CFile csvFile(filePath);
	//�t�@�C�����J���Ă�������s
	if (csvFile.Success()) {
		int c = 0;
		const char* p = (const char*)csvFile.Data();
		int size = csvFile.Size(); 
		std::pair<std::string, int> param;
		c += GetString(&p[c], &param.first); // �G�t�F�N�g�����擾
		c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.second); // key���擾
		c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����

		// �f�[�^��ǉ�
		_commandList[param.second] = param.first;
	}
};

ClassificationEffect::~ClassificationEffect() {
	_commandList.clear();
	_instance = nullptr;
};

void ClassificationEffect::SetClassification(CommandParam param) {
	if (param.first == Play_Vibration_X) {
		// �o�C�u���[�V����
	}
	else if (param.first == Play_Vibration_Y) {
		// �o�C�u���[�V����
	}
	else if (param.first == Play_SE) {
		// SE
	}
	else if (param.first == Play_CameraVibration) {
		// �J�����o�C�u���[�V����
	}
	else if (param.first == Play_Effect_PC) {
		// �G�t�F�N�g
	}
	else if (param.first == Play_Effect_IC) {
		// �G�t�F�N�g
	}
	else if (param.first == Play_Effect_IU) {
		// �G�t�F�N�g
	}
	else if (param.first == Play_Effekseer_PC) {
		// �G�t�F�N�V�A
	}
	else if (param.first == Play_Effekseer_IC) {
		// �G�t�F�N�V�A
	}
	else if (param.first == Play_Effekseer_IU) {
		// �G�t�F�N�V�A
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "�G�t�F�N�g�R�}���h��������܂���ł����B", "�G���[", MB_OK);
#endif
	}
};