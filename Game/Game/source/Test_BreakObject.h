#pragma once
#include "appframe.h"

class BreakObject
{
public:
	BreakObject();
	~BreakObject();

	void Process();
	void Render();

private:
	int _modelHandle;
	VECTOR _pos;

	// ������΂��p�[�c��index�ԍ�
	int _breakFrameIndex;

	int _breakCnt;
};