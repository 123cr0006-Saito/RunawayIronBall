#include "BuildingBase.h"

BuildingBase::BuildingBase()
{
	_modelHandle = -1;
}

BuildingBase::~BuildingBase()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

bool BuildingBase::Init()
{
	return true;
}

bool BuildingBase::Process()
{
	return true;
}

bool BuildingBase::Render()
{
	return MV1DrawModel(_modelHandle);
}
