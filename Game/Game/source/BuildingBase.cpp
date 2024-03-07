#include "BuildingBase.h"

BuildingBase::BuildingBase()
{
	_useCollision = true;
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
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

bool BuildingBase::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);
	MV1SetRotationXYZ(_modelHandle, rotation);
	MV1SetScale(_modelHandle, scale);

	_breakObj = NEW BreakObject();
	_breakObj->Init(_modelHandle);

	return true;
}

bool BuildingBase::Process()
{
	_breakObj->Process();
	return true;
}

bool BuildingBase::Render()
{
	return MV1DrawModel(_modelHandle);
}

void BuildingBase::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakObj->Activate(activate, vDir);
	SetUseCollision(false);
}

bool BuildingBase::DrawDebugInfo()
{
	return true;
}
