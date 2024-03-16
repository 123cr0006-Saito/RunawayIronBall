#include "House.h"

House::House()
{
	_breakObj = nullptr;
}

House::~House()
{
	if (_modelHandle != -1) {
		//MV1DeleteModel(_modelHandle);
		//_modelHandle = -1;
	}
	SAFE_DELETE(_breakObj);
}

void House::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	BuildingBase::Init(modelHandle, startPos, rotation, scale, obbLength);

	_canBreak = true;
	// 破壊処理クラスの初期化
	_breakObj = NEW BreakObject();
	_breakObj->Init(_modelHandle);
}

void House::Process()
{
	_breakObj->Process();
}

void House::SetHit(VECTOR vDir)
{
	// 破壊処理の開始
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
