#include "BuildingBase.h"

BuildingBase::BuildingBase()
{
	_modelHandle = -1;
	_breakObj = nullptr;
}

BuildingBase::~BuildingBase()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
	SAFE_DELETE(_breakObj);
}

bool BuildingBase::Init(int modelHandle, VECTOR startPos)
{
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);

	_breakObj = new BreakObject();
	_breakObj->Init(_modelHandle);

	_testCnt = 60;

	return true;
}

bool BuildingBase::Process()
{
	_breakObj->Process();

	_testCnt--;
	if (_testCnt < -90) {
		ActivateBreakObject(false);
		_testCnt = 60;
	}else if (_testCnt < 0) {
		ActivateBreakObject(true, VGet(0.0f, 0.0f, 1.0f));
	}
	return true;
}

bool BuildingBase::Render()
{
	return MV1DrawModel(_modelHandle);
}

void BuildingBase::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakObj->Activate(activate, vDir);
}
