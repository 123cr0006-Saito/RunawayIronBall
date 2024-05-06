//----------------------------------------------------------------------
// @filename BreakModelParts.h
// @date: 2024/01/07
// @author: Morozumi Hiroya
// @explanation
// �������f�����p�[�c���Ƃɐ�����΂��������s���N���X
// ���炩���߃��f���̃p�[�c�𕪊����Ă����A���̃p�[�c�����𗘗p���Đ�����΂��������s��
// �p�[�c�̖��O�̐擪�ɁuBRK�v�����Ă�����̂𐁂���΂��ΏۂƂ��A����ȊO�̃p�[�c�͂��̏�ɂƂǂ܂� 
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

class BreakModelParts
{
public:
	BreakModelParts();
	~BreakModelParts();

	// ����������
	void Init(int modelHandle);
	// �X�V����
	void Process();

	// ������΂������̗L����Ԃ��ǂ������擾����
	bool GetIsActive() { return _isActive; }

	// ������΂������̗L�����ݒ�
	// @param activate : ������΂�������L�������邩�ǂ���
	// @param _blastDir : ������΂�����
	void Activate(bool activate, VECTOR blastDir = VGet(0.0f,0.0f,0.0f));

	// �f�o�b�O���̕\��
	void DrawDebugInfo();

private:

	// ������΂��������Z�b�g����
	// �p�[�c���ƂɁA������΂�������1�t���[��������̉�]�n��ݒ肷��
	// @param vDir : ������΂��̒��S����
	void SetBlastDir(VECTOR vDir);
	// �p�[�c���ƂɓK�������s������Z�b�g���A���f����������Ԃɖ߂�
	void ResetFrameMatrix();


private:
	// �p�[�c�̐�����΂��������s�����ǂ���
	bool _isActive;
	// �p�[�c�̐�����΂��������I���������ǂ���
	bool _finishedBreakAll;

	// ���f���n���h��
	// �e�����N���X�̃C���X�^���X����󂯎��i���̃C���X�^���X�ƃ��f���n���h�������L����j
	int _modelHandle;
	// ���f���̉�]�l
	VECTOR _modelRotation;

	struct FRAME_INFO {
		// 3D���f���̃p�[�c�̃C���f�b�N�X�ԍ�
		int frameIndex;

		// ������΂��������I���������ǂ���
		bool finishedBreak;

		// --------------------------------------------------------------------------------------------
		// ������΂����
		VECTOR horizontalDir; // ��������
		float horizontalVelocity; // ���������̑��x
		float verticalVelocity; // ���������̑��x
		VECTOR vRot; // �e���ɂ�����1�t���[���̉�]�l
		// --------------------------------------------------------------------------------------------
	};

	std::vector<FRAME_INFO*> _frameInfo;


	// �f�o�b�O�\���p
	// ������΂�����
	VECTOR _blastDir;
	// �O�Օ\�������邩�ǂ���
	bool _isDrawLocus;
	// �O�Օ\���p�̍��W���
	std::vector<std::vector<VECTOR>> _locus;
};
