//----------------------------------------------------------------------
// @filename UnbreakableBuilding.h
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �j��s�\�Ȍ����̃N���X
//----------------------------------------------------------------------
#pragma once
#include "BuildingBase.h"

class UnbreakableBuilding : public BuildingBase
{
public:
	UnbreakableBuilding();
	 virtual ~UnbreakableBuilding();

	// ����������
	void Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	// �X�V����
	void Process() override;

	// �f�o�b�O���̕\��
	void DrawDebugInfo() override;

protected:

};