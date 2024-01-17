#pragma once
#include "appframe.h"
#include "BreakObject.h"

class BuildingBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual bool Init(int modelHandle);
	virtual bool Process();
	virtual bool Render();

private:
	int _modelHandle;

	BreakObject* _breakObj;





	// デバッグ用
	// パーツの吹っ飛び処理を行うまでのフレーム数
	int _testCnt;
};