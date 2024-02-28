#include "ClassificationEffect.h"
ClassificationEffect* ClassificationEffect::_instance = nullptr;
ClassificationEffect::ClassificationEffect() {
	_instance = this;
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
			std::pair<std::string, int> param;
			c += GetString(&p[c], &param.first); // �G�t�F�N�g�����擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &param.second); // key���擾
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			// �f�[�^��ǉ�
			_commandList[param.second] = param.first;
		}
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "DirectionList ���J���܂���ł����B", "�G���[", MB_OK);
#endif
	}
};

ClassificationEffect::~ClassificationEffect() {
	_commandList.clear();
	_instance = nullptr;
};

void ClassificationEffect::SetClassification(CommandParam param) {
	if (param.first == Play_Vibration) {
		// �o�C�u���[�V����
	}
	else if (param.first == Play_SE) {
		// SE
		int seName = static_cast<int>(param.second);
		global._soundServer->DirectPlay(_commandList[seName]);
	}
	else if (param.first == Play_CameraVibration_X) {
		// �J�����o�C�u���[�V���� X
		ScreenVibration::GetInstance()->SetVibrationX(param.second, 20);
	}
	else if (param.first == Play_CameraVibration_Y) {
		// �J�����o�C�u���[�V���� Y
		ScreenVibration::GetInstance()->SetVibrationY(param.second, 20);
	}
	else if (param.first == Play_Effect_Dust) {
		// �G�t�F�N�g
		int effectName = static_cast<int>(param.second);
		ResourceServer::Mult handle = ResourceServer::SearchMult(_commandList[effectName].c_str());
		if (handle.AllNum != 0) {
			VECTOR pos = VGet(0, 0, 0);
			int size = 200;
			float animSpeed = 1.0f / 60.0f * 1000;
			PlaneEffect::BoardPolygonDust* dust = new PlaneEffect::BoardPolygonDust(pos, size, handle.handle, handle.AllNum, animSpeed);
			PlaneEffect::PlaneEffectManeger::GetInstance()->LoadVertical(dust);
		}
	}
	else if (param.first == Play_Effekseer_PC) {
		// �G�t�F�N�V�A �v���C���[���S
		VECTOR vec = Player::GetInstance()->GetPosition();
		CreateEffeckseer(param.second, &vec);
	}
	else if (param.first == Play_Effekseer_IC) {
		// �G�t�F�N�V�A �S�����S
		VECTOR pos = VGet(0, 0, 0);
		CreateEffeckseer(param.second, &pos);
	}
	else if (param.first == Play_Effekseer_IU) {
		// �G�t�F�N�V�A �S������
		VECTOR pos = VGet(0, 0, 0);
		CreateEffeckseer(param.second, &pos);
	}
	else {
#ifdef _DEBUG
		MessageBox(NULL, "�G�t�F�N�g�R�}���h��������܂���ł����B", "�G���[", MB_OK);
#endif
	}
};

void ClassificationEffect::CreateEffeckseer(float param, VECTOR* pos) {
	int effectName = static_cast<int>(param);
	int handle = ResourceServer::SearchSingle(_commandList[effectName].c_str(), ResourceServer::TYPE::Efk);
	EffekseerPosSynchro* effekseer = new EffekseerPosSynchro(handle, pos,1.0f);
};