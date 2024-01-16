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

	// ������΂��̒��S�������Z�b�g����
	void SetBlastDir(VECTOR vDir);


	void DrawDebugInfo();

private:
	int _modelHandle;
	VECTOR _pos;

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
	VECTOR _blastDir;
	float _blastPower;


	// �O�Օ\�������邩�ǂ���
	bool _isDrawLocus;
	// �O�Օ\���p�̍��W���
	std::vector<std::vector<VECTOR>> _locus;
};
