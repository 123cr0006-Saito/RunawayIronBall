#pragma once
#include "appframe.h"

class BreakObject
{
public:
	BreakObject();
	~BreakObject();

	void Init(int modelHandle);

	void Process();
	void Render();


	bool GetIsActive() { return _isActive; }
	void Activate(bool activate, VECTOR _blastDir = VGet(0.0f,0.0f,0.0f));




	void DrawDebugInfo();



private:

	// ������΂��̒��S�������Z�b�g����
	void SetBlastDir(VECTOR vDir);
	void ResetFrameMatrix();


private:
	// �p�[�c�̐�����я������s�����ǂ���
	bool _isActive;
	// �p�[�c�̐�����я������I���������ǂ���
	bool _finishedBreakAll;

	// ���f���n���h��
	// �e�����N���X�̃C���X�^���X����󂯎��i���̃C���X�^���X�ƃ��f���n���h�������L����j
	int _modelHandle;
	// ���f���̉�]�l
	VECTOR _modelRotation;

	int _breakCnt;

	struct FRAME_INFO {
		// 3D���f���̃p�[�c�̃C���f�b�N�X�ԍ�
		int frameIndex;

		bool finishedBreak;

		// ������΂����
		VECTOR horizontalDir; // ��������
		float horizontalVelocity; // ���������̑��x
		float verticalVelocity; // ���������̑��x
		VECTOR vRot; // �e���ɂ�����1�t���[���̉�]�l
	};

	std::vector<FRAME_INFO*> _frameInfo;


	/*  */
	// ������ѕ���
	VECTOR _blastDir;


	// �O�Օ\�������邩�ǂ���
	bool _isDrawLocus;
	// �O�Օ\���p�̍��W���
	std::vector<std::vector<VECTOR>> _locus;
};
