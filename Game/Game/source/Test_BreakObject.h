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

	// 吹っ飛ばすパーツのindex番号
	int _breakFrameIndex;

	int _breakCnt;
};