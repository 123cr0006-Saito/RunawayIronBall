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

};