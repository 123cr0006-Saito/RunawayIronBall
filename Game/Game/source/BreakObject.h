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
	void SetIsActive(bool activate, VECTOR _blastDir = VGet(0.0f,0.0f,0.0f));

	// ������΂��̒��S�������Z�b�g����
	void SetBlastDir(VECTOR vDir);


	void DrawDebugInfo();

private:
	// �p�[�c�̐�����я������s�����ǂ���
	bool _isActive;

	// ���f���n���h��
	// �e�����N���X�̃C���X�^���X����󂯎��i���̃C���X�^���X�ƃ��f���n���h�������L����j
	int _modelHandle;
	

	int _breakCnt;

	struct FRAME_INFO {
		// 3D���f���̃p�[�c�̃C���f�b�N�X�ԍ�
		int frameIndex;

		// ������΂����

		VECTOR horizontalDir; // ��������
		float horizontalVelocity; // ���������̑��x
		float verticalVelocity; // ���������̑��x

		VECTOR rotVector; // �e���ɂ�����1�t���[���̉�]�l
	};

	std::vector<FRAME_INFO> _frameInfo;


	/*  */
	VECTOR _pos;
	// ������ѕ���
	VECTOR _blastDir;
	// ������т̋���
	float _blastPower;


	// �O�Օ\�������邩�ǂ���
	bool _isDrawLocus;
	// �O�Օ\���p�̍��W���
	std::vector<std::vector<VECTOR>> _locus;
};
