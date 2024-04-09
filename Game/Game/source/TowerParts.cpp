//----------------------------------------------------------------------
// @filename TowerParts.h
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �^���[�p�[�c�̐�����s���N���X
// 1�̃^���[�I�u�W�F�N�g�́A������TowerParts�N���X����\�������
//----------------------------------------------------------------------
#include "TowerParts.h"

namespace {
	constexpr float BLAST_SPEED = 60.0f;
	constexpr int BLAST_CNT_MAX = 180;
	constexpr float FALL_CNT_MAX = 30;
}

TowerParts::TowerParts() : ObjectBase::ObjectBase()
{
	_use = true;
	_useCollision = false;

	_blast = false;
	_blastDir = VGet(0.0f, 0.0f, 0.0f);
	_blastCnt = 0;

	_isFalling = false;
	_fallCnt = 0;
	_fallStartPos = VGet(0.0f, 0.0f, 0.0f);
	_fallEndPos = VGet(0.0f, 0.0f, 0.0f);

	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_vRot = VGet(0.0f, 0.0f, 0.0f);

	_sphereCollision.centerPos = VGet(0.0f, 0.0f, 0.0f);
	_sphereCollision.r = 0.0f;
}

TowerParts::~TowerParts()
{
}

void TowerParts::Init(int modelHandle, VECTOR startPos)
{
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);

	_sphereCollision.r = 250.0f;

	_cell->_objType = OBJ_TYPE::TWR_PRT;
}

void TowerParts::Process()
{
	if (_use) {
		// ��������
		if (_isFalling) {
			FallProcess();
		}

		// ������я���
		if (_blast) {
			BlastOffProcess();
			// �����蔻��̍X�V
			UpdateCollision();
		}

		// ���f���̍��W���X�V
		MV1SetPosition(_modelHandle, _pos);
	}
}

void TowerParts::BlastOffProcess()
{
	_pos = VAdd(_pos, VScale(_blastDir, BLAST_SPEED));
	_blastCnt++;
	if (_blastCnt > BLAST_CNT_MAX) {
		_use = false;
		_useCollision = false;
		_collisionManager->ReserveRemovementCell(_cell);
	}
}

void TowerParts::FallProcess()
{
	float x = Easing::InQuint(_fallCnt, _fallStartPos.x, _fallEndPos.x, FALL_CNT_MAX);
	float y = Easing::InQuint(_fallCnt, _fallStartPos.y, _fallEndPos.y, FALL_CNT_MAX);
	float z = Easing::InQuint(_fallCnt, _fallStartPos.z, _fallEndPos.z, FALL_CNT_MAX);
	_pos = VGet(x, y, z);

	_fallCnt++;
	// �������I��������
	if (_fallCnt > FALL_CNT_MAX) {
		_isFalling = false;
	}
}

void TowerParts::Render()
{
	if (_use) {
		MV1SetPosition(_modelHandle, _pos);
		MV1DrawModel(_modelHandle);
	}
}

void TowerParts::UpdateCollision()
{
	_sphereCollision.centerPos =_pos;
	_collisionManager->UpdateCell(_cell);
}

void TowerParts::DrawDebugInfo()
{
	if (_useCollision) {
		_sphereCollision.Render(COLOR_WHITE);
	}
}
