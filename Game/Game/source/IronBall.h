#pragma once
#include "appframe.h"
#include "ObjectBase.h"

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
	~IronBall();

	void Init();
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
	Capsule GetChainCollision() { return _chainCapsuleCollision; }
	void UpdateIBCollision();
	void UpdateChainCollision();


	// ���̃I�u�W�F�N�g��ۗL���Ă���e�I�u�W�F�N�g�֘A�̊֐�
	// �v���C���[�̃��f���n���h�����Z�b�g
	void SetPlayerModelHandle(int handle);
	void SetMoveState(IB_MOVE_STATE state) { _moveState = state; }

	void SetParentInstance(ObjectBase* parent) { _parent = parent; }
	ObjectBase* GetParentInstance() { return _parent; }
	void SetParentPosPtr(VECTOR* pos) { _parentPos = pos; }
	VECTOR* GetParentPosPtr() { return _parentPos; }

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
	// ���̃I�u�W�F�N�g��ۗL���Ă���e�̍��W�ւ̃|�C���^
	VECTOR* _parentPos;


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

	//-------------------
	// �V�����쐬�����ϐ��ł��B
	std::map<int, std::pair<int, float>> _powerAndScale;//�U���͂Ɗg�嗦���i�[�����R���e�i�ł��B
};