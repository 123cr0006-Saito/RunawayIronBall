//----------------------------------------------------------------------
// @filename IronBall.h
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �v���C���[�����S���E���̐���E�`����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "Afterglow.h"

#define CHAIN_MAX 12

enum IB_MOVE_STATE {
	FOLLOWING,
	PUTTING_ON_SOCKET,
	INTERPOLATION,
};
class IronBall : public ObjectBase
{
public:
	IronBall();
	virtual ~IronBall();

	void Init();
	// �e���f���̍��W�����Ƃɏ����ʒu��ݒ�
	void InitPosFromParent();
	void Process();

	void MoveProcess();
	void FollowingProcess();
	void PuttingOnSocketProcess();
	void InterpolationProcess();

	void AnimProcess();

	void Render();

	VECTOR GetBallPosition() { return _iPos; }
	void SetBallPosition(VECTOR pos) { _iPos = pos; }

	VECTOR* GetBallPosPtr() { return &_iPos; }

	bool GetEnabledAttackCollision() { return _enabledAttackCollision; }
	void SetEnabledAttackCollision(bool state) { _enabledAttackCollision = state; }

	Sphere GetIBCollision() { return _ibAttackSphereCollision; }
	Sphere GetIBBodyCollision() { return _ibBodySphereCollision; }
	Capsule GetChainCollision() { return _chainCapsuleCollision; }
	void UpdateIBCollision();
	void UpdateChainCollision();
	// �T�C�g�E���쐬���܂����B
	void SetEnabledAfterGlow(bool enable);// �ڂ̎c����\�����邩�ǂ�����ݒ�

	// ���̃I�u�W�F�N�g��ۗL���Ă���e�I�u�W�F�N�g�֘A�̊֐�
	// �v���C���[�̃��f���n���h�����Z�b�g
	void SetPlayerModelHandle(int handle);
	void SetMoveState(IB_MOVE_STATE state) { _moveState = state; }

	void SetParentInstance(ObjectBase* parent) { _parent = parent; }
	ObjectBase* GetParentInstance() { return _parent; }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();


	bool UpdateLevel(float scale);//�v���C���[����擾�����A���x���ōU���͂Ɗg�嗦��ݒ�


private:
	XInput* _input;

	// ��
	int _cModelHandle;
	VECTOR _cPos[CHAIN_MAX];

	// �S��
	int _iModelHandle;
	VECTOR _iPos;
	VECTOR _iForwardDir;

	// �S�������̓����蔻��`��i�n�ʂƂ̓����蔻��Ɏg���j
	Sphere _ibBodySphereCollision;
	// �S�������̍U�������蔻��
	Sphere _ibAttackSphereCollision;
	// �������̓����蔻��
	Capsule _chainCapsuleCollision;
	// �������̓����蔻���CollisionManager�ɓo�^���邽�߂�Cell
	Cell* _chainCell;

	VECTOR _ibDefaultScale;

	// �z�u�\�P�b�g
	int _socketNo[3];


	// ���̃I�u�W�F�N�g��ۗL���Ă���e�̃I�u�W�F�N�g�ւ̃|�C���^
	ObjectBase* _parent;

	int _attackAnimCnt;


	int _animIndex;
	float _animTotalTime;
	float _playTime;


	float _cnt;
	int _attackDir;

	MATRIX _m[CHAIN_MAX];

	float _length;

	bool _followingMode;
	
	IB_MOVE_STATE _moveState;
	bool _enabledAttackCollision;


	int _playerModelHandle;

	std::vector<Afterglow*> _afterglowList;
};