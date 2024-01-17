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
};