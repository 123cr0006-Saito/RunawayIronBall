#include "House.h"

House::House()
{
	_breakObj = nullptr;
}

House::~House()
{
	SAFE_DELETE(_breakObj);
}

void House::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp, int suppression)
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

void House::Process()
{
	_breakObj->Process();
}

void House::SetHit(VECTOR vDir)
{
	// �j�󏈗��̊J�n
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
