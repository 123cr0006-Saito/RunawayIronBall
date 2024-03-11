#include "House.h"

House::House()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_useCollision = true;
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

void House::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	BuildingBase::Init(modelHandle, startPos, rotation, scale, obbLength);

	// ”j‰óˆ—ƒNƒ‰ƒX‚Ì‰Šú‰»
	_breakObj = NEW BreakObject();
	_breakObj->Init(_modelHandle);
}

void House::Process()
{
	_breakObj->Process();
}

void House::Render()
{
	MV1DrawModel(_modelHandle);
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
	if (_useCollision) {
		obb.Render(GetColor(255, 255, 255));
	}
	_breakObj->DrawDebugInfo();
}
