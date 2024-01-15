#pragma once
#include "appframe.h"

class BuildingBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual bool Init();
	virtual bool Process();
	virtual bool Render();

private:
	int _modelHandle;
};