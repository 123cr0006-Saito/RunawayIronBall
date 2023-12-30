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

	// ������΂��̒��S�������w��
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
		// ���f���̋N�_���W���猩���t���[���̕���
		VECTOR dir;

		// �f�o�b�O�p
		VECTOR pos, startPos;
	};

	std::vector<FRAME_INFO> _frameInfo;
	VECTOR _blastDir;
	float _blastPower;


	// �f�o�b�O�p
	//struct FRAME_POSITION
	//{
	//	VECTOR pos, startPos;
	//};
	//std::vector<FRAME_POSITION> _framePos;
};