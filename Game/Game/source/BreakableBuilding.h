//----------------------------------------------------------------------
// @filename BreakableBuilding.h
// @date: 2024/04/01
// @author: Morozumi Hiroya
// @explanation
// �j��\�Ȍ����̃N���X
//----------------------------------------------------------------------
#pragma once
#include "BuildingBase.h"
#include "BreakModelParts.h"

class BreakableBuilding : public BuildingBase
{
public:
	BreakableBuilding();
	virtual ~BreakableBuilding();

	// ����������
	void Init(int modelHandle,std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp,int suppression);
	// �X�V����
	void Process() override;

	// ��_���[�W�ݒ�
	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) override;

	// �f�o�b�O���̕\��
	void DrawDebugInfo() override;

protected:
	// �j�󏈗��̗L����
	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	// �j�󏈗��N���X
	BreakModelParts* _breakModelParts;
};