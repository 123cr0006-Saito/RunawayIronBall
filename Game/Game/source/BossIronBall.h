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

	bool GetIsInvincible() { return _isInvincible; }
	bool GetUseCollision() { return _useCollision; }

	bool GetIsEnhanced() { return _isEnhanced; }
	void SetEnhanced() { _changeEnhanced = true; }


	bool CheckHardKnockBack() { return _ibState == IB_STATE::HARD_KNOCK_BACK; }

	void SetHitStake(bool isHit) { _isHitStake = isHit; }
	bool GetHitStake() { return _isHitStake; }

	void SetISStakeBroken(bool isBroken) { _isStakeBroken = isBroken; }


	int CheckPlayerInSearchRange();

	void SetKnockBack(VECTOR vDir, float speed);
	bool CheckKnockBack() { return _isKnockBack; }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();


private:
	// �S�����f���̌������X�V����
	void UpdateModelRotation();

	void CheckState();

	// ������ԂɈڍs����
	// ������ԂɑJ�ڂ��\�ȃ^�C�~���O�ɌĂяo���A���߂�_changeEnhanced��true�ɂȂ����Ƃ��ɋ�����ԂɈڍs����i���̏ꍇ��_isEnhanced�͕K��false�j
	void CheckChangeEnhanced();

	void ResetPhase();


	// �ҋ@��Ԃ̏���
	void IdleProcess();
	void SetIdle();

	// �d����Ԃ̏���
	void StiffenProcess();
	// �d����ԂɑJ�ڂ���
	void SetStiffen(int cnt, bool isInvincible = false);


	// �ːi�U��
	void RushProcess();
	void SetRush();

	// �����U��
	void DropProcess();
	void SetDrop();

	// ��]�U��
	void RotationProcess();
	void SetRotation();

	// �m�b�N�o�b�N����
	void KnockBackProcess();

	// �n�[�h�m�b�N�o�b�N����
	void HardKnockBackProcess();

	// 
	void GravityProcess();

	void ChainProcess();

	// �A�j���[�V�����̍Đ����Ԃ��X�V
	void AnimationProcess();

private:
	// �S���̃��f���n���h��
	int _ibModelHandle;
	// �S���̍��W
	VECTOR _ibPos;
	// �S���̃��f���̌���
	VECTOR _ibModelForwardDir;
	// �S���̃��f�������Ɍ�����������i���݂̌������琔�t���[�������Č������Ԃ���j
	VECTOR _ibModelNextForwardDir;

	enum class IB_MODEL_DIR {
		PLAYER,		// �v���C���[�̕���
		PLAYER_REVERSE,		// �v���C���[�̋t����
		STAKE,		// �Y�̕���
		STAKE_REVERSE,		// �Y�̋t����
		NOT_UPDATE,		// �X�V���Ȃ�
	} _ibModelDirState;

	// �S���̓����蔻��
	Sphere _ibSphereCol;
	// ���G��Ԃ��ǂ���
	bool _isInvincible;
	// �����蔻����s�����ǂ���
	bool _useCollision;
	// ������Ԃ��ǂ���
	bool _isEnhanced;
	// ������Ԃɐ؂�ւ��邩�ǂ���
	// �Y��HP�������ȉ��ɂȂ����Ƃ���true�ɂ���
	// ��xtrue�ɂȂ�����false�ɂ͖߂�Ȃ�
	bool _changeEnhanced;
	// ������Ԃł̍U���̉�
	int _enhancedAttackCnt;

	enum class IB_STATE {
		IDLE,							// �ҋ@
		STIFFEN,					// �d��
		ATTACK_RUSH,			// �ːi�U��
		ATTACK_DROP,			// �����U��
		ATTACK_ROTATION,	// ��]�U��
		KNOCK_BACK,			// �m�b�N�o�b�N
		HARD_KNOCK_BACK,	// �n�[�h�m�b�N�o�b�N�i�d�����Ƀv���C���[�̍U�������������ꍇ�̏��, �Y�܂Œ��ڔ��ł����j
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



	// ��]�U��
	//  ������x�N�g��
	VECTOR _rotBaseDir;
	// �p���x
	float _rotAngularVelocity;
	// _rotBaseDir����Ƃ�����]�p�x
	float _rotAngle;
	// ��]�̔��a
	float _rotRadius;


	// �m�b�N�o�b�N��Ԃ��ǂ���
	bool _isKnockBack;
	// �m�b�N�o�b�N����
	VECTOR _knockBackDir;
	// �m�b�N�o�b�N���x
	float _knockBackSpeed;
	// �m�b�N�o�b�N���s���̎c��t���[����
	int _knockBackCnt;

	float _gravity;

	// ���̃��f���n���h��
	int _chainModelHandle;
	// ���̍��W
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// �Y�̍��W
	VECTOR* _stakePos;
	// �Y���j�󂳂ꂽ���ǂ���
	bool _isStakeBroken;

	// ���_����Y�̍��W�ւ̕��s�ړ��s��
	MATRIX _mStakePos;
	// _mStakePos�̋t�s��
	MATRIX _mStakePosInv;


	// �A�j���[�V�����֘A
	// �A�j���[�V�����̃C���f�b�N�X
	int _animIndex;
	// �A�j���[�V�����̍��v����
	float _animTotalTime;
	// �Đ�����
	float _playTime;



	Player* _player;


	// �f�o�b�O�p
	// ����̃A�N�V�����̃t���[����
	int debugFrame = 30;
	int debugFrameMax = 30;

	float debugValue = 100.0f;
	float debugValueMax = 100.0f;
};