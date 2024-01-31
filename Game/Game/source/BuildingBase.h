#pragma once
#include "appframe.h"
#include "BreakObject.h"

class BuildingBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual bool Init(int modelHandle, VECTOR startPos);
	virtual bool Process();
	virtual bool Render();

	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	// デバッグ情報の表示
	virtual bool DrawDebugInfo();

protected:
	int _modelHandle;
	VECTOR _pos;

	BreakObject* _breakObj;
};