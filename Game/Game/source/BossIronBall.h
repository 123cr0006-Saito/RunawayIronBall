#pragma once
#include "appframe.h"
#include "Player.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	void LoadModel();
	void Init(VECTOR* stakePos);

	void Process();
	void Render();

	VECTOR GetPosition() { return _ibPos; }
	void SetPosition(VECTOR pos) { _ibPos = pos; }


	void UpdateIBCollision();
	Sphere GetIBCollision() { return _ibSphereCol; }

	void SetHitStake(bool isHit) { _isHitStake = isHit; }
	bool GetHitStake() { return _isHitStake; }


	int CheckPlayerInSearchRange();

	// �f�o�b�O���̕\��
	void DrawDebugInfo();


private:
	void CheckState();

	void ResetPhase();


	// �ҋ@��Ԃ̏���
	void IdleProcess();

	// �d����Ԃ̏���
	void StiffenProcess();
	void SetStiffen(int cnt) { _ibState = IB_STATE::STIFFEN; _ibStiffenCnt = cnt; }


	// �ːi�U��
	void RushProcess();
	void SetRush();

	// �����U��
	void DropProcess();
	void SetDrop();

	// ��]�U��
	void RotationProcess();
	void RotationAcceleration();

private:
	// �S���̃��f���n���h��
	int _ibModelHandle;
	// �S���̍��W
	VECTOR _ibPos;
	// �S���̓����蔻��
	Sphere _ibSphereCol;

	enum class IB_STATE {
		IDLE,							// �ҋ@
		STIFFEN,					// �d��
		ATTACK_RUSH,			// �ːi�U��
		ATTACK_DROP,			// �����U��
		ATTACK_ROTATION,	// ��]�U��
	} _ibState;

	// �e�X�e�[�g���ł̃t�F�[�Y�ԍ�
	int _phase;
	int _phaseCnt;

	VECTOR _posBeforeMoving;
	VECTOR _targetPos;

	bool _isHitStake;

	//
	int _ibIdleCnt;
	VECTOR _ibMoveDir;

	int _ibStiffenCnt;



	// 
	bool _activeRotationAcceleration;
	int _rotationAccelerationCnt;
	float _rotAngularVelocity;
	float _rotAngle;

	// ���̃��f���n���h��
	int _chainModelHandle;
	// ���̍��W
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// �Y�̍��W
	VECTOR* _stakePos;

	// ���_����Y�̍��W�ւ̕��s�ړ��s��
	MATRIX _mStakePos;
	// _mStakePos�̋t�s��
	MATRIX _mStakePosInv;



	Player* _player;


	// �f�o�b�O�p
	// ����̃A�N�V�����̃t���[����
	int debugFrame = 30;
	int debugFrameMax = 30;

	float debugValue = 100.0f;
	float debugValueMax = 100.0f;
};