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

	// ‚Á”ò‚Î‚·ƒp[ƒc‚Ìindex”Ô†
	int _breakFrameIndex;

	int _breakCnt;
};