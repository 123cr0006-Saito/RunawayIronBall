#pragma once
#include "appframe.h"

class BreakObject
{
public:
	BreakObject();
	~BreakObject();

	void Init();

	void Process();
	void Render();

	// ������΂��̒��S�������Z�b�g����
	void SetBlastDir(VECTOR vDir);


	void DrawDebugInfo();

private:
	int _modelHandle;
	VECTOR _pos;

	// ������΂��p�[�c��index�ԍ�
	int _breakFrameIndex;

	int _breakCnt;
	
	struct FRAME_INFO {

		int frameIndex;

		// ������΂�

		VECTOR horizontalDir; // ��������
		float horizontalVelocity;
		float verticalVelocity;

		VECTOR rotVector;

		// �f�o�b�O�p
		VECTOR pos, startPos;
	};

	std::vector<FRAME_INFO> _frameInfo;
	VECTOR _blastDir;
	float _blastPower;


	bool _isDrawLocus;
	// �O�Օ\���p�̍��W���
	std::vector<std::vector<VECTOR>> _locus;
};