//----------------------------------------------------------------------
// @filename ModelColor.cpp
// @date: 2024/02/15
// @author: Morozumi Hiroya
// @explanation
// ���f���̔��_�ŏ������s���N���X
//----------------------------------------------------------------------
#include "ModelColor.h"

ModelColor::ModelColor()
{
	_modelHandle = -1;
	_flickerEmissiveColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	_flickerTextureHandle = -1;
}

ModelColor::~ModelColor()
{
	for (auto itr = _defaultMaterial.begin(); itr != _defaultMaterial.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_defaultMaterial.clear();
}

// ����������
void ModelColor::Init(int modelHandle)
{
	_modelHandle = modelHandle;

	// ���f���̃}�e���A�������擾
	for (int i = 0; i < MV1GetMaterialNum(_modelHandle); i++)
	{
		MATERIAL* material = NEW MATERIAL();
		material->index = i;
		material->textureHandle = MV1GetTextureGraphHandle(_modelHandle, i);
		material->emiColor = MV1GetMaterialEmiColor(_modelHandle, i);

		_defaultMaterial.push_back(material);
	}

	_flickerEmissiveColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	_flickerTextureHandle = ResourceServer::LoadGraph("GirlTexWhite","res/Character/cg_player_girl/FlickerTexture.png");
}

// ���_�ŏ���
// �ȉ��̏������J��Ԃ��Ĕ��_�ł��s��
// 1.�e�N�X�`���𔒂��摜�ɍ����ւ��A�G�~�b�V�u�J���[�𔒂ɂ���
// 2.���ɖ߂�
// @param activateFlicker: ���_�ŏ�Ԃ̕ύX�itrue:1�̏�Ԃɂ���, false:2�̏�Ԃɂ���j
void ModelColor::ChangeFlickerTexture(bool activateFlicker)
{
	for (auto itr = _defaultMaterial.begin(); itr != _defaultMaterial.end(); ++itr)
	{
		int nextTextureHandle;
		COLOR_F nextColor;
		// �L����
		if (activateFlicker) {
			// �e�N�X�`���ƃG�~�b�V�u�J���[�𔒂ɍ����ւ�
			nextTextureHandle = _flickerTextureHandle;
			nextColor = _flickerEmissiveColor;
		}
		// ������
		else {
			// �e�N�X�`���ƃG�~�b�V�u�J���[�����ɖ߂�
			nextTextureHandle = (*itr)->textureHandle;
			nextColor = (*itr)->emiColor;
		}
		// ���f���̃}�e���A���Ƀe�N�X�`���ƃG�~�b�V�u�J���[��ݒ肷��
		MV1SetTextureGraphHandle(_modelHandle, (*itr)->index, nextTextureHandle, FALSE);
		MV1SetMaterialEmiColor(_modelHandle, (*itr)->index, nextColor);
	}
}
