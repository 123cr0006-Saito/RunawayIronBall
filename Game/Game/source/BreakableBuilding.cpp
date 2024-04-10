#include "BreakableBuilding.h"

BreakableBuilding::BreakableBuilding()
{
	_breakObj = nullptr;
}

BreakableBuilding::~BreakableBuilding()
{
	SAFE_DELETE(_breakObj);
}

void BreakableBuilding::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp, int suppression)
{
	BuildingBase::Init(modelHandle, objName,startPos, rotation, scale, obbLength);
	_hp = hp;
	_exp = exp;
	_suppression = suppression;
	_canBreak = true;
	// �j�󏈗��N���X�̏�����
	_breakObj = NEW BreakObject();
	_breakObj->Init(_modelHandle);
}

void BreakableBuilding::Process()
{
	_breakObj->Process();
}

void BreakableBuilding::SetHit(VECTOR vDir)
{
	// �j�󏈗��̊J�n
	ActivateBreakObject(true, vDir);
}

void BreakableBuilding::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakObj->Activate(activate, vDir);
	SetUseCollision(false);
}

void BreakableBuilding::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
	_breakObj->DrawDebugInfo();
}
