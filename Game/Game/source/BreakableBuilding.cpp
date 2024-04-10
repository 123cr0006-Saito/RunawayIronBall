//----------------------------------------------------------------------
// @filename BreakableBuilding.cpp
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �j��\�Ȍ����̃N���X
//----------------------------------------------------------------------
#include "BreakableBuilding.h"

BreakableBuilding::BreakableBuilding()
{
	_breakModelParts = nullptr;
}

BreakableBuilding::~BreakableBuilding()
{
	SAFE_DELETE(_breakModelParts);
}

// ����������
void BreakableBuilding::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp, int suppression)
{
	// ���N���X�̏���������
	BuildingBase::Init(modelHandle, objName,startPos, rotation, scale, obbLength);

	// HP�A�o���l�A�����l�̐ݒ�
	_hp = hp;
	_exp = exp;
	_suppression = suppression;

	// �j��\��Ԃɂ���
	_canBreak = true;

	// �j�󏈗��N���X�̏�����
	_breakModelParts = NEW BreakModelParts();
	_breakModelParts->Init(_modelHandle);
}

// �X�V����
void BreakableBuilding::Process()
{
	// �j�󏈗�
	// �L����Ԃ��ǂ����̔����BreakModelParts�N���X���ōs��
	_breakModelParts->Process();
}

// ��_���[�W�ݒ�
void BreakableBuilding::SetHit(VECTOR vDir)
{
	// �j�󏈗��̊J�n
	ActivateBreakObject(true, vDir);
}

// �j�󏈗��̗L����
void BreakableBuilding::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakModelParts->Activate(activate, vDir);
	SetUseCollision(false);
}

// �f�o�b�O���̕\��
void BreakableBuilding::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
	_breakModelParts->DrawDebugInfo();
}
