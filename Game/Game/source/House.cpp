#include "House.h"

House::House()
{
	_breakObj = nullptr;
}

House::~House()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
	SAFE_DELETE(_breakObj);
}

void House::Init(int modelHandle, std::string name, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	BuildingBase::Init(modelHandle, name,startPos, rotation, scale, obbLength);

	_canBreak = true;
	// ”j‰óˆ—ƒNƒ‰ƒX‚Ì‰Šú‰»
	_breakObj = NEW BreakObject();
	_breakObj->Init(_modelHandle);
}

void House::Process()
{
	_breakObj->Process();
}

void House::SetHit(VECTOR vDir)
{
	// ”j‰óˆ—‚ÌŠJŽn
	ActivateBreakObject(true, vDir);
}

void House::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakObj->Activate(activate, vDir);
	SetUseCollision(false);
}

void House::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
	_breakObj->DrawDebugInfo();
}
