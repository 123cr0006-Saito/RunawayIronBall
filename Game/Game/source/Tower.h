//----------------------------------------------------------------------
// @filename Tower.h
// @date: 2024/02/05
// @author: Morozumi Hiroya
// @explanation
// �^���[�I�u�W�F�N�g�̐���E�`����s���N���X
// 1�̃^���[�I�u�W�F�N�g�́A������TowerParts�N���X����\�������
//----------------------------------------------------------------------
#pragma once
#include "TowerParts.h"
#include "ObjectBase.h"

class Tower : public ObjectBase
{
public:
	Tower();
	virtual ~Tower();

	// ����������
	// @param modelHandle: ���f���n���h���i�z��̗v�f�����^���[�̒i���ƂȂ�j
	// @param startPos: �N�_���W
	// @param rotation: ���f���̉�]�l
	// @param scale: ���f���̊g�嗦
	void Init(std::vector<int> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	// �X�V����
	void Process();
	// �`�揈��
	void Render();

	// ���̃^���[���g�p�����ǂ������擾
	bool GetUse() { return _use; }

	// �N�_���W���擾
	VECTOR GetPos() { return _pos; }

	// �^���[�p�[�c�̐�����΂��ݒ�
	// @param vDir: ������΂�����
	void SetBlast(VECTOR vDir);

	// �^���[�p�[�c��������΂��\��Ԃ��ǂ������擾
	bool GetCanBlast() { return _canBlast; }

	// �^���[�{�̂̓����蔻��`����擾
	Sphere GetCollision() { return _sphereCollision; }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();

protected:
	// ���̃^���[���g�p�����ǂ����i�^���[�p�[�c���c���Ă��邩�ǂ����j
	bool _use;
	// �N�_���W
	VECTOR _pos;

	// �p�[�c�̏��
	std::vector<TowerParts*> _towerParts;
	// �p�[�c�̏����̐�
	int _partsNum;
	// �ŉ����̃p�[�c�̃C���f�b�N�X
	int _bottomIndex;
	
	// �p�[�c�̗����I��������s�����ǂ���
	bool _checkFallEnd;

	// �v���C���[����̍U������𖳌������̃J�E���g
	int _invincibleCnt;

	// �p�[�c�𐁂���΂��\���ǂ���
	bool _canBlast;

	// �^���[�{�̂̓����蔻��`��
	// 1. �L�����N�^�[�̑̂Ƃ̓����蔻��Ɏg�p
	// 2. �v���C���[����̍U������Ɏg�p
	Sphere _sphereCollision;
};