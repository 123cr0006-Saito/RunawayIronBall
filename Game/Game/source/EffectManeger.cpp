//----------------------------------------------------------------------
// @filename EffectManeger.cpp
// ��date: 2024/01/12
// ��author: saito ko
// @explanation
// �S�ẴG�t�F�N�g���Ǘ����邽�߂̃N���X
//----------------------------------------------------------------------
#include "EffectManeger.h"
EffectManeger* EffectManeger::_instance = nullptr;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
EffectManeger::EffectManeger(){
	_instance = this;
	_effect.clear();
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
EffectManeger::~EffectManeger() {
	_instance = nullptr;
	for (auto&& effect : _effect) {
		delete effect;
	}
	_effect.clear();
};
//----------------------------------------------------------------------
// @brief �G�t�F�N�g��ǉ�
// @return ����
//----------------------------------------------------------------------
void EffectManeger::LoadEffect(EffectBase* effect) {
	_effect.push_back(effect);
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool EffectManeger::Update(){
	for (auto itr = _effect.begin(); itr != _effect.end(); ) {
		if ((*itr)->GetFlag()) {
			(*itr)->Process();
			++itr;
		}
		else {
			delete (*itr);
			itr = _effect.erase(itr);
		}
	}
	UpdateEffekseer3D();
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool EffectManeger::Render() {
	//�r���{�[�h�`�撆�̓��C�g�̌v�Z���ꎞ��~  ���C�g�Ɣ��Ε����������ƍ����Ȃ�
	
	SetUseLighting(FALSE);
	for (auto itr = _effect.begin(); itr != _effect.end(); ++itr) {
		(*itr)->Render();
	}
	SetUseLighting(TRUE);

	// �G�t�F�N�V�A�̕`��
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();

	return true;
};