//----------------------------------------------------------------------
// @filename BossIronBall.h
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// �{�X�S���̐�����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "Player.h"
#include "AfterImage.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	// ���f���̓ǂݍ���
	void LoadModel();
	// ����������
	void Init(VECTOR* stakePos);
	// �X�V����
	void Process();
	// �`�揈��
	void Render();
	// �c���̕`�揈��
	void RenderAfterImage();

	// �K���X��Ԃɂ���
	void ChangeGlass();
	// �K���X����Ԃ��ǂ������擾����
	bool GetIsGlass() { return _isGlass; }

	// �{�X�S���̍��W���擾
	VECTOR GetPosition() { return _ibPos; }
	// �{�X�S���̍��W��ݒ�
	void SetPosition(VECTOR pos) { _ibPos = pos; }

	// �����蔻��̍X�V
	void UpdateIBCollision();
	// �{�X�S���̓����蔻����擾����
	Sphere GetIBCollision() { return _ibSphereCol; }

	// �S���̓����蔻����X�V
	void UpdateChainCollision();
	// ���̓����蔻����擾����
	Capsule GetChainCollision() { return _chainCapsuleCol; }

	// ��]�U�������ǂ������擾����
	bool GetIsRotationAttack() { return _isRotationAttack; }

	// �X�e�[�W��ɂ��邩�ǂ�����ݒ肷��
	void SetOnStage(bool isOnStage);
	// �X�e�[�W��ɂ��邩�ǂ������擾����
	bool GetOnStage() { return _isOnStage; }

	// ���G��Ԃ��ǂ������擾����
	bool GetIsInvincible() { return _isInvincible; }

	// �����蔻����s�����ǂ������擾����
	bool GetUseCollision() { return _useCollision; }

	// ������Ԃ��ǂ������擾����
	bool GetIsEnhanced() { return _isEnhanced; }
	// ������ԂɈڍs����
	void SetEnhanced() { _changeEnhanced = true; }

	// �n�[�h�m�b�N�o�b�N��Ԃ��ǂ������擾����
	bool CheckHardKnockBack() { return _ibState == IB_STATE::HARD_KNOCK_BACK; }

	// ���ɓ����������ǂ������擾����
	bool GetHitStake() { return _isHitStake; }
	// ���ɓ����������ǂ�����ݒ肷��
	void SetHitStake(bool isHit) { _isHitStake = isHit; }

	// �Y���j���Ԃ��ǂ�����ݒ肷��
	void SetISStakeBroken(bool isBroken) { _isStakeBroken = isBroken; }

	// �v���C���[���S���̒T���͈͓��ɂ��邩�ǂ������擾����
	// // ���G�͈͓��ɂ���ꍇ�͍��G�͈͂̃C���f�b�N�X��Ԃ� 
	// @return: -1: �T���͈͓��ɂ��Ȃ�, 0: �T���͈�0�ɂ���, 1: �T���͈�1�ɂ���
	int CheckPlayerInSearchRange();

	// �m�b�N�o�b�N�̐ݒ���s��
	// @param vDir: �m�b�N�o�b�N����
	// @param speed: �m�b�N�o�b�N���x
	void SetKnockBack(VECTOR vDir, float speed);
	// �m�b�N�o�b�N��Ԃ��ǂ������擾����
	bool CheckKnockBack() { return _isKnockBack; }

	// �X�e�[�W�̔��a��ݒ肷��
	void SetStageRadius(float radius) { _stageRadius = radius; }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();


private:
	// �S�����f���̌������X�V����
	void UpdateModelRotation();

	// �s����Ԃ��X�V����
	void CheckState();

	// ������ԂɈڍs����
	// ������ԂɑJ�ڂ��\�ȃ^�C�~���O�ɌĂяo���A���߂�_changeEnhanced��true�ɂȂ����Ƃ��ɋ�����ԂɈڍs����i���̏ꍇ��_isEnhanced�͕K��false�j
	void CheckChangeEnhanced();

	// �t�F�[�Y�̃��Z�b�g
	// �e�s����Ԃ͕����̃t�F�[�Y�������A�s����Ԃ��ς��ƃt�F�[�Y�����Z�b�g�����
	void ResetPhase();


	// �ҋ@��Ԃ̏���
	void IdleProcess();
	// �ҋ@��ԂɑJ�ڂ���
	void SetIdle();

	// �d����Ԃ̏���
	void StiffenProcess();
	// �d����ԂɑJ�ڂ���
	void SetStiffen(int cnt, bool isInvincible = false, bool playSound = true);


	// �ːi�U���̏���
	void RushProcess();
	// �ːi�U���ɑJ�ڂ���
	void SetRush();

	// �����U���̏���
	void DropProcess();
	// �����U���ɑJ�ڂ���
	void SetDrop();

	// ��]�U���̏���
	void RotationProcess();
	// ��]�U���ɑJ�ڂ���
	void SetRotation();

	// �m�b�N�o�b�N����
	void KnockBackProcess();

	// �n�[�h�m�b�N�o�b�N����
	void HardKnockBackProcess();

	// �d�͏���
	void GravityProcess();

	// ���̍X�V����
	void ChainProcess();

	// �A�j���[�V�����̍X�V����
	void AnimationProcess();

private:
	// �S���̃��f���n���h��
	// ���݂̏�Ԃɉ����ĕύX�����i_ibModelHandleArray�̂ǂ��炩��������j
	int _ibModelHandle;
	// �S���̃��f���̃n���h���z��
	// 0: �ʏ펞, 1: �K���X��Ԏ�
	std::array<int, 2> _ibModelHandleArray;
	// �K���X�����Ă��邩�ǂ���
	bool _isGlass;

	// �S���̍��W
	VECTOR _ibPos;
	// �S���̃��f���̌���
	VECTOR _ibModelForwardDir;
	// �S���̃��f�������Ɍ�����������i���݂̌������琔�t���[�������Č������Ԃ���j
	VECTOR _ibModelNextForwardDir;

	// �S�����f���̌����̏��
	// _ibNextForwardDir�̌������X�V���邩�ǂ���
	enum class IB_MODEL_DIR {
		PLAYER,						// �v���C���[�̕���
		PLAYER_REVERSE,		// �v���C���[�̋t����
		STAKE,						// �Y�̕���
		STAKE_REVERSE,		// �Y�̋t����
		NOT_UPDATE,			// _ibModelNextForwardDir���X�V���Ȃ��i�Ō�ɐݒ肳�ꂽ�l��ێ�����j
	} _ibModelDirState;

	// �S���̓����蔻��
	Sphere _ibSphereCol;
	// ���̓����蔻��
	Capsule _chainCapsuleCol;
	// ��]�U�������ǂ���
	bool _isRotationAttack;

	// �X�e�[�W�̒��ɂ��邩�ǂ���
	bool _isOnStage;

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

	// �s�����
	enum class IB_STATE {
		IDLE,							// �ҋ@
		STIFFEN,					// �d��
		ATTACK_RUSH,			// �ːi�U��
		ATTACK_DROP,			// �����U��
		ATTACK_ROTATION,	// ��]�U��
		KNOCK_BACK,			// �m�b�N�o�b�N
		HARD_KNOCK_BACK,	// �n�[�h�m�b�N�o�b�N�i�d�����Ƀv���C���[�̍U�������������ꍇ�̏��, �Y�܂Œ��ڔ��ł����A�Y�Ƀ_���[�W������j
	} _ibState;

	// �e�s����Ԃ͕����̃t�F�[�Y�������A�s����Ԃ��ς��ƃt�F�[�Y�����Z�b�g�����
	// �e�s����Ԃł̃t�F�[�Y�ԍ�
	int _phase;
	// �t�F�[�Y���̌o�߃t���[����
	int _phaseCnt;

	// �ړ��O�̍��W�i�ړ��̕�ԂɎg�p����j
	VECTOR _posBeforeMoving;
	// �ړ���̍��W�i�ړ��̕�ԂɎg�p����j
	VECTOR _targetPos;

	// ���ɓ����������ǂ���
	bool _isHitStake;

	// �ҋ@��Ԃ̃t���[����
	// �ȉ��͈̔͂Ń����_���ɐݒ肳���
	// IDLE_INTERVAL_BASE + (rand() % IDLE_INTERVAL_ADD_MAX)
	int _ibIdleCnt;

	// �ړ�����
	VECTOR _ibMoveDir;

	// �d����Ԃ̃t���[����
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
	// �����m�̊Ԋu
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

	// �c���N���X
	AfterImage* _afterImage;
	// �V�����c���𐶐����邩�ǂ���
	bool _addAfterImage;

	// SE���Đ����邩�ǂ���
	bool _playSound;

	// �X�e�[�W�̔��a
	float _stageRadius;

	// �v���C���[�̃C���X�^���X
	Player* _player;

};