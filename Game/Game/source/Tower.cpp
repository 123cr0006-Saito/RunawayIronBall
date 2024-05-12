//----------------------------------------------------------------------
// @filename Tower.cpp
// @date: 2024/02/05
// @author: Morozumi Hiroya
// @explanation
// �^���[�I�u�W�F�N�g�̐���E�`����s���N���X
// 1�̃^���[�I�u�W�F�N�g�́A������TowerParts�N���X����\�������
//----------------------------------------------------------------------
#include "Tower.h"

namespace {
	// �v���C���[����̍U������𖳌�������t���[����
	constexpr int INVINCIBLE_CNT_MAX = 30; 
}

Tower::Tower() : ObjectBase::ObjectBase()
{
	_use = true;
	_pos = VGet(0.0f, 0.0f, 0.0f);

	_towerParts.clear();
	_partsNum = 0;
	_bottomIndex = 0;

	_checkFallEnd = false;
	_invincibleCnt = -1;

	_canBlast = true;	

	_sphereCollision.centerPos = VGet(0.0f, 0.0f, 0.0f);
	_sphereCollision.r = 0.0f;
}

Tower::~Tower()
{
	for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		delete *itr;
	}
	_towerParts.clear();
}

// ����������
// @param modelHandle: ���f���n���h���i�z��̗v�f�����^���[�̒i���ƂȂ�j
// @param startPos: �N�_���W
// @param rotation: ���f���̉�]�l
// @param scale: ���f���̊g�嗦
void Tower::Init(std::vector<int> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	_pos = startPos;

	_partsNum = modelHandle.size();
	// �^���[�p�[�c�𐶐�
	for (int i = 0; i < _partsNum; i++) {
		TowerParts* tp = NEW TowerParts();
		VECTOR tmpPos = VGet(0.0f, 0.0f, 0.0f);
		// �ŉ����̃p�[�c�́AstartPos�ɔz�u����
		if (i == 0) {
			tmpPos = startPos;
		}
		// ����ȊO�̃p�[�c�́A����̃p�[�c�̃\�P�b�g�i�W���C���g�j���W�ɔz�u����
		else {
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m = MV1GetFrameLocalWorldMatrix(_towerParts[i - 1]->GetModelHandle(), 1);
			tmpPos = VTransform(vOrigin, m);
		}
		
		tp->Init(modelHandle[i], tmpPos, rotation, scale);
		_towerParts.push_back(tp);
	}

	// �^���[�{�̂̓����蔻���ݒ�
	_sphereCollision.centerPos = _pos;
	_sphereCollision.r = 250.0f;
	_cell->_objType = OBJ_TYPE::TWR;
	_collisionManager->UpdateCell(_cell);
}

// �X�V����
void Tower::Process()
{
	if (_use) {
		// �p�[�c�̗����I������
		if (_checkFallEnd) {
			// �c���Ă���S�Ẵp�[�c�������I���������ǂ����𔻒肷��
			bool finishedFalling = true;
			for (int i = _bottomIndex; i < _partsNum; i++) {
				if(_towerParts[i]->GetUse() == false) continue;

				finishedFalling = finishedFalling && !(_towerParts[i]->GetIsFalling());
			}
			// �S�Ẵp�[�c�������I��������
			if (finishedFalling) {
				// �����I���������߂�
				_checkFallEnd = false;
				// �p�[�c�𐁂���΂��\��Ԃɂ���
				_canBlast = true;
			}
		}

		// �v���C���[����̍U������𖳌������̏���
		if (_invincibleCnt >= 0) {
			_invincibleCnt--;
			if (_invincibleCnt < 0) {
				_checkFallEnd = true;
			}
		}

	}

	// �p�[�c�̍X�V����
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Process();
	}
}

// �`�揈��
void Tower::Render()
{
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Render();
	}
}

// �^���[�p�[�c�̐�����΂��ݒ�
// @param vDir: ������΂�����
void Tower::SetBlast(VECTOR vDir)
{
	// �^���[��������΂��\��ԂȂ�
	if (_canBlast) {
		_invincibleCnt = INVINCIBLE_CNT_MAX;
		_canBlast = false;

		for (int i = _bottomIndex; i < _partsNum; i++) {
			// �ŉ����̃p�[�c�̂ݐ�����я���
			if (i == _bottomIndex) {
				_towerParts[i]->SetBlast(VNorm(vDir));
			}
			// ����ȊO�̃p�[�c�͗�������
			else {
				// ����̃p�[�c�̐�����΂��O�̍��W���A�����I����̍��W�Ƃ��Đݒ�
				_towerParts[i]->SetFalling(_towerParts[i - 1]->GetPos());
			}
		}
		_bottomIndex++;

		//�p�[�c���c���Ă��Ȃ�������A�^���[�{�̂̓����蔻��𖳌���
		if (_bottomIndex >= _partsNum) {
			_use = false;
			_collisionManager->ReserveRemovementCell(_cell);
		}
	}
}

// �f�o�b�O���̕\��
void Tower::DrawDebugInfo()
{
	if (_use) {
		_sphereCollision.Render(COLOR_GREEN);
	}
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->DrawDebugInfo();
	}
}
