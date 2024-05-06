//----------------------------------------------------------------------
// @filename ModelColor.h
// @date: 2024/02/15
// @author: Morozumi Hiroya
// @explanation
// ���f���̔��_�ŏ������s���N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

class ModelColor
{
public:
	ModelColor();
	~ModelColor();

	// ����������
	void Init(int modelHandle);

	// ���_�ŏ���
	// �ȉ��̏������J��Ԃ��Ĕ��_�ł��s��
	// 1.�e�N�X�`���𔒂��摜�ɍ����ւ��A�G�~�b�V�u�J���[�𔒂ɂ���
	// 2.���ɖ߂�
	// @param activateFlicker: ���_�ŏ�Ԃ̕ύX�itrue:1�̏�Ԃɂ���, false:2�̏�Ԃɂ���j
	void ChangeFlickerTexture(bool activateFlicker);

private:
	// ���f���n���h��
	int _modelHandle;

	// �}�e���A�����
	struct MATERIAL
	{
		// �}�e���A���̃C���f�b�N�X�ԍ�
		int index;
		// �e�N�X�`���摜�n���h��
		int textureHandle;
		// �G�~�b�V�u�J���[
		COLOR_F emiColor;
	};
	std::vector<MATERIAL*> _defaultMaterial;

	// ���_�ŗp�̃e�N�X�`���摜�n���h��
	int _flickerTextureHandle;
	// ���_�ŗp�̃G�~�b�V�u�J���[
	COLOR_F _flickerEmissiveColor;
};