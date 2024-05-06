//----------------------------------------------------------------------
// @filename UnbreakableBuilding.cpp
// @date: 2024/03/05
// @author: Morozumi Hiroya
// @explanation
// �j��s�\�Ȍ����̃N���X
//----------------------------------------------------------------------
#include "UnbreakableBuilding.h"

UnbreakableBuilding::UnbreakableBuilding()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
}

UnbreakableBuilding::~UnbreakableBuilding()
{
}

// ����������
void UnbreakableBuilding::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	// ���N���X�̏���������
	BuildingBase::Init(modelHandle, objName, startPos, rotation, scale, obbLength);
	// �j��s�\��Ԃɂ���
	_canBreak = false;
}

// �X�V����
void UnbreakableBuilding::Process()
{
}

// �f�o�b�O���̕\��
void UnbreakableBuilding::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
}
