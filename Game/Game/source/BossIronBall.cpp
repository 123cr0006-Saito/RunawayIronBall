//----------------------------------------------------------------------
// @filename BossIronBall.cpp
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// �{�X�S���̐�����s���N���X
// �{�X�S���̐���́ABossIronBall�N���X�ōs��
//----------------------------------------------------------------------
#include "BossIronBall.h"
#include "BossIronBall.h"

namespace {
	constexpr int BOSS_CHAIN_MAX = 12;
	constexpr float BOSS_CHAIN_TOTAL_LENGTH = 3000.0f;


	constexpr float SEARCH_RANGE[2] = { 2000.0f, 4000.0f };

	// --------- 

	// �ҋ@���
	// �W�����v�ړ����s���t���[����
	constexpr int IDLE_CNT_MAX = 30;
	// 1�t���[��������̈ړ���
	constexpr float IDLE_MOVE_SPEED = 180.0f / IDLE_CNT_MAX; // 30�t���[����1.8m�ړ�
	// ���W�����v��Ɏ��̍s���Ɉڂ�܂ł̃t���[�����i120 ~ 180�j
	// ��l 
	constexpr int IDLE_INTERVAL_BASE = 120;
	// �����Œǉ������t���[�����̍ő�l
	constexpr int IDLE_INTERVAL_ADD_MAX = 60;

	// �ːi�U��
	// �Y�̕t�߂ɓ��B����܂ł̃t���[����
	constexpr int RU_REACH_STAKE_CNT = 60;
	// ���ߎ���
	constexpr int RU_CHARGE_CNT = 60;
	// �ːi���s���t���[����
	constexpr int RU_ATTACK_CNT = 30;
	// �ːi�ł̈ړ�����
	constexpr float RU_MOVE_DISTANCE = 3700.0f;
	// �ːi��̍d�����ԁi�ʏ펞�j
	constexpr int RU_STIFFEN_CNT = 180;
	// �ːi��̍d�����ԁi�������j
	constexpr int RU_STIFFEN_CNT_ENHANCED = 90;

	// �����U��
	// ��яオ���Ă���A�ō��n�_�ɓ��B����܂ł̃t���[����
	constexpr int DR_REACH_HIGHEST_CNT = 60;
	// �ō��n�_�ɓ��B���Ă���A�n�ʂɒ��n����܂ł̃t���[����
	constexpr int DR_REACH_GROUND_CNT = 20;
	// �n�ʂɒ��n��̍d�����ԁi�ʏ펞�j
	constexpr int DR_STIFFEN_CNT = 90;
	// �n�ʂɒ��n��̍d�����ԁi������Ԏ��j
	constexpr int DR_STIFFEN_CNT_ENHANCED = 60;

	// ��]�U��
	// ��]�J�n�O�ɍY�̕t�߂ɓ��B����܂ł̃t���[����
	constexpr int RO_REACH_STAKE_CNT = 60;
	// ��]����
	constexpr float RO_ROTATION_DIR = -1.0f; // �����v���
	// 1�t���[��������̉�]�p	�ŏ��l
	constexpr float RO_ANGULAR_VELOCITY_MIN = (2.0f * DX_PI) / 90.0f; // 90�t���[���ň��]
	// 1�t���[��������̉�]�p	�ő�l
	constexpr float RO_ANGULAR_VELOCITY_MAX = (2.0f * DX_PI) / 20.0f; // 20�t���[���ň��]
	// ��]���a�̍ŏ��l
	constexpr float RO_ROTAION_RADIUS_MIN = 500.0f;
	// ��]���a�̍ő�l
	constexpr float RO_ROTAION_RADIUS_MAX = 3100.0f;

	// �ő呬�x�ɓ��B����܂ł̃t���[����
	constexpr int RO_ACCELERATION_CNT_MAX = 600;
	// �ő呬�x���ێ�����t���[����
	constexpr int RO_MAINTAIN_MAXSPEED_CNT = 210;
	// �ő呬�x����A���x0�܂Ō���������܂ł̃t���[����
	constexpr int RO_DECELERATION_CNT_MAX = 90;
	// ��]�U����̍d������
	constexpr int RO_STIFFEN_CNT = 300;

	// �m�b�N�o�b�N
	// �K���X��Ԃł̃m�b�N�o�b�N���x�{��
	constexpr float KB_SPEED_RATE_GLASS = 20.0f;

	// �n�[�h�m�b�N�o�b�N
	// �Y�ɓ��B����܂ł̃t���[����
	constexpr int HK_REACH_STAKE_CNT = 60;
	// �Y�ɂ����蒵�˕Ԃ���s���t���[����
	constexpr int HK_BOUNCE_CNT = 45;
	// ���˕Ԃ苗��
	constexpr float HK_BOUNCE_DISTANCE = 2000.0f;
	// ���˕Ԃ��̍d������
	constexpr int HK_STIFFEN_CNT = 60;

	// ---------

	// ���f���̊g�嗦
	constexpr float MODEL_SCALE = 12.8f;
	// �S�������̓����蔻��̔��a
	constexpr float IB_SPHERE_RADIUS = 20.0f * MODEL_SCALE;
	// �������̓����蔻��̔��a
	constexpr float CH_CAPSULE_RADIUS = 100.0f;
}

BossIronBall::BossIronBall()
{
	_ibModelHandle = -1;
	for (int i = 0; i < 2; i++) {
		_ibModelHandleArray[i] = -1;
	}
	_isGlass = false;

	_ibPos = VGet(0.0f, 0.0f, 0.0f);
	_ibModelForwardDir = VGet(0.0f, 0.0f, -1.0f);
	_ibModelNextForwardDir = VGet(0.0f, 0.0f, -1.0f);
	_ibModelDirState = IB_MODEL_DIR::PLAYER;

	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 0.0f;

	_chainCapsuleCol.down_pos = VGet(0.0f, 0.0f, 0.0f);
	_chainCapsuleCol.up_pos = VGet(0.0f, 0.0f, 0.0f);
	_chainCapsuleCol.r = 0.0f;
	_isRotationAttack = false;

	_isOnStage = true;

	_isInvincible = false;
	_useCollision = true;
	_isEnhanced = false;
	_changeEnhanced = false;
	_enhancedAttackCnt = 0;

	_ibState = IB_STATE::IDLE;

	_phase = 0;
	_phaseCnt = 0;

	_posBeforeMoving = VGet(0.0f, 0.0f, 0.0f);
	_targetPos = VGet(0.0f, 0.0f, 0.0f);

	_isHitStake = false;

	_ibIdleCnt = 0;
	_ibMoveDir = VGet(0.0f, 0.0f, 0.0f);


	_ibStiffenCnt = 0;

	_rotBaseDir = VGet(0.0f, 0.0f, 0.0f);
	_rotAngularVelocity = 0.0f;
	_rotAngle = 0.0f;
	_rotRadius = 0.0f;

	_isKnockBack = false;
	_knockBackDir = VGet(0.0f, 0.0f, -1.0f);
	_knockBackSpeed = 0.0f;
	_knockBackCnt = 0;
	_gravity = 0.0f;

	_chainModelHandle = -1;
	_chainPos.clear();
	_chainDistance = 0.0f;

	_stakePos = nullptr;
	_isStakeBroken = false;
	_mStakePos = MGetIdent();
	_mStakePosInv = MGetIdent();


	_animIndex = 0;
	_animTotalTime = 0.0f;
	_playTime = 0.0f;

	_playSound = false;

	_stageRadius = 0.0f;

	_player = nullptr;
}

BossIronBall::~BossIronBall()
{
	_stakePos = nullptr;
	_player = nullptr;
}

// ���f���̓ǂݍ���
void BossIronBall::LoadModel()
{
	// �ʏ펞�̓S�����f��
	_ibModelHandleArray[0] = ResourceServer::MV1LoadModel("Bossnake", "res/Enemy/Cg_Enemy_Bossnake/Cg_Enemy_Bossnake.mv1");
	// �K���X��Ԃ̓S�����f��
	_ibModelHandleArray[1] = ResourceServer::MV1LoadModel("BossnakeEnhanced", "res/Enemy/Cg_Enemy_Bossnake/Bossnake_Glass.mv1");
	// ���̃��f��
	_chainModelHandle = ResourceServer::MV1LoadModel("Chain", "res/Chain/Cg_Chain.mv1");
}

// ����������
void BossIronBall::Init(VECTOR* stakePos)
{
	// �Y�̍��W��ݒ�
	_stakePos = stakePos;

	// �����m�̊Ԋu��ݒ�
	_chainDistance = BOSS_CHAIN_TOTAL_LENGTH / BOSS_CHAIN_MAX;
	// ���̍��W��ݒ�
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		_chainPos.push_back(VAdd(*_stakePos, VGet(0.0f, 100.0f, -80.0f * i)));
	}

	// �S���̓����蔻���ݒ�
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = IB_SPHERE_RADIUS;
	_chainCapsuleCol.down_pos = *_stakePos;
	_chainCapsuleCol.up_pos = _ibPos;
	_chainCapsuleCol.r = CH_CAPSULE_RADIUS;

	//	�S���̏������W��ݒ�
	_ibPos = VGet(0, 0, -1000);
	_ibPos.y = _ibSphereCol.r;

	// �S���̃��f���̊g�嗦��ݒ�
	for (int i = 0; i < 2; i++) {
		MV1SetScale(_ibModelHandleArray[i], VScale(VGet(1.0f, 1.0f, 1.0f), MODEL_SCALE));
	}
	// ���݂̓S�����f����ݒ�
	_ibModelHandle = _ibModelHandleArray[0];

	// �s����Ԃ�������
	_ibState = IB_STATE::IDLE;

	// ���_����Y�̍��W�ւ̕��s�ړ��s����擾
	_mStakePos = MGetTranslate(*_stakePos);
	// _mStakePos�̋t�s����擾
	_mStakePosInv = MInverse(_mStakePos);


	_rotAngle = 0.0f;

	//--------------------------------
	// �A�j���[�V�������̏�����
	_animIndex = MV1AttachAnim(_ibModelHandle, 0, -1, FALSE);
	_animTotalTime = MV1GetAttachAnimTotalTime(_ibModelHandle, _animIndex);
	_playTime = 0.0f;
	//--------------------------------

	// �v���C���[�̃C���X�^���X���擾
	_player = Player::GetInstance();
}

// �X�V����
void BossIronBall::Process()
{
	// �X�e�[�W��ɂ���ꍇ�̏���
	if (_isOnStage) {

		// �s����Ԃɉ������������s��
		switch (_ibState)
		{
		// �ҋ@
		case BossIronBall::IB_STATE::IDLE:
			IdleProcess();
			CheckChangeEnhanced();
			break;
		// �d��
		case BossIronBall::IB_STATE::STIFFEN:
			StiffenProcess();
			CheckChangeEnhanced();
			break;
		// �ːi�U��
		case BossIronBall::IB_STATE::ATTACK_RUSH:
			RushProcess();
			break;
		// �����U��
		case BossIronBall::IB_STATE::ATTACK_DROP:
			DropProcess();
			break;
		// ��]�U��
		case BossIronBall::IB_STATE::ATTACK_ROTATION:
			RotationProcess();
			break;
		// �m�b�N�o�b�N
		case BossIronBall::IB_STATE::KNOCK_BACK:
			KnockBackProcess();
			break;
		// �n�[�h�m�b�N�o�b�N
		case BossIronBall::IB_STATE::HARD_KNOCK_BACK:
			HardKnockBackProcess();
			break;
		}


		_ibPos.y -= 16.0f;
		if (_ibPos.y - _ibSphereCol.r < 0.0f) _ibPos.y = _ibSphereCol.r;
	}
	else {
		_ibPos = VAdd(_ibPos, VScale(_knockBackDir, 30.0f));
		GravityProcess();
	}

	// �S���̓����蔻����X�V
	UpdateIBCollision();

	// ��]�U�����̏ꍇ�A���̓����蔻����X�V
	if (_isRotationAttack) {
		UpdateChainCollision();
		global._soundServer->DirectPlay("SE_BOSS_Rotate");
	}

	// �S�����f���̌������X�V
	UpdateModelRotation();

	// �A�j���[�V�����̍X�V����
	AnimationProcess();

	_isHitStake = false;

	// ���̍X�V����
	ChainProcess();
}

// �`�揈��
void BossIronBall::Render()
{
	// ���̕`��
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		MV1SetPosition(_chainModelHandle, _chainPos[i]);
		MV1DrawModel(_chainModelHandle);
	}

	// �S���̕`��
	MV1SetPosition(_ibModelHandle, _ibPos);
	MV1DrawModel(_ibModelHandle);
}

// �K���X��Ԃɂ���
void BossIronBall::ChangeGlass()
{
	if (!_isGlass) {
		// �K���X��Ԃ̓S�����f���ɕύX
		_ibModelHandle = _ibModelHandleArray[1];

		// �A�j���[�V�������̐ݒ�
		_animIndex = MV1AttachAnim(_ibModelHandle, 0, -1, FALSE);
		_animTotalTime = MV1GetAttachAnimTotalTime(_ibModelHandle, _animIndex);
		_playTime = 0.0f;

		// SE�Đ�
		global._soundServer->DirectPlay("SE_BOSS_Glass");

		// �K���X��Ԃɐݒ�
		_isGlass = true;
	}
}

// �S���̓����蔻����X�V
void BossIronBall::UpdateIBCollision()
{
	_ibSphereCol.centerPos = _ibPos;
}

// ���̓����蔻����X�V
void BossIronBall::UpdateChainCollision()
{
	_chainCapsuleCol.down_pos = *_stakePos;
	_chainCapsuleCol.down_pos.y = _chainCapsuleCol.r;
	_chainCapsuleCol.up_pos = _ibPos;
	_chainCapsuleCol.up_pos.y = _chainCapsuleCol.r;
}

// �X�e�[�W��ɂ��邩�ǂ�����ݒ肷��
void BossIronBall::SetOnStage(bool isOnStage)
{
	bool oldState = _isOnStage;
	_isOnStage = isOnStage;
	if (!oldState && isOnStage) {
		_gravity = 0.0f;
	}
}

// �S�����f���̌������X�V����
void BossIronBall::UpdateModelRotation()
{
	bool isUpdate = false;
	// IB_MODEL_DIR�ɉ�����������ݒ�
	switch (_ibModelDirState)
	{
	// �v���C���[�̕���
	case IB_MODEL_DIR::PLAYER:
		_ibModelNextForwardDir = VSub(_player->GetPosition(), _ibPos);
		isUpdate = true;
		break;
	// �v���C���[�̋t����
	case IB_MODEL_DIR::PLAYER_REVERSE:
		_ibModelNextForwardDir = VSub(_ibPos, _player->GetPosition());
		isUpdate = true;
		break;
	// �Y�̕���
	case IB_MODEL_DIR::STAKE:
		_ibModelNextForwardDir = VSub(*_stakePos, _ibPos);
		isUpdate = true;
		break;
	// �Y�̋t����
	case IB_MODEL_DIR::STAKE_REVERSE:
		_ibModelNextForwardDir = VSub(_ibPos, *_stakePos);
		isUpdate = true;
		break;
	// _ibModelNextForwardDir���X�V���Ȃ��i�Ō�ɐݒ肳�ꂽ�l��ێ�����j
	case IB_MODEL_DIR::NOT_UPDATE:
		//return;
		break;
	}

	// �������X�V����ꍇ
	if (isUpdate) {
		_ibModelNextForwardDir.y = 0.0f;
		_ibModelNextForwardDir = VNorm(_ibModelNextForwardDir);
	}

	// �S�����f�������炩�ɉ�]������
	// ���f���̌��݂̐��ʕ����x�N�g���ƕύX��̕����x�N�g���̊p�x���v�Z����
	float angle = Math::CalcVectorAngle(_ibModelForwardDir, _ibModelNextForwardDir);
	// 1�t���[��������̉�]�p
	float rotRad = (2.0f * DX_PI_F) / 20.0f;
	// angle��rotRad��菬�����ꍇ��_ibModelNextForwardDir�����f���̐��ʕ����ɂ���
	if (rotRad > angle) {
		_ibModelForwardDir = _ibModelNextForwardDir;
	}
	else {
		// rotRad�̊p�x������]������
		VECTOR vN = VCross(_ibModelForwardDir, _ibModelNextForwardDir);
		_ibModelForwardDir = VTransform(_ibModelForwardDir, MGetRotAxis(vN, rotRad));
	}
	// ���f�����w��̕����Ɍ�������
	Math::SetModelForward_RotationY(_ibModelHandle, _ibModelForwardDir);
}

// �s����Ԃ��X�V����
void BossIronBall::CheckState()
{
	// �v���C���[���S���̍��G�͈͂̓��A�ǂ͈̔͂ɂ��邩���擾
	int searchRangeIndex = -1;
	// �Y���j�󂳂�Ă��Ȃ��ꍇ
	if (!_isStakeBroken) {
		searchRangeIndex = CheckPlayerInSearchRange();
		if (searchRangeIndex == -1) {
			_ibState = IB_STATE::IDLE;
		}
		else if (searchRangeIndex == 0) {
			_ibState = IB_STATE::ATTACK_DROP;
		}
		else {
			_ibState = IB_STATE::ATTACK_RUSH;
		}
	}
	else {
		_ibState = IB_STATE::IDLE;
	}

	// �s����Ԃɉ������������������s��
	switch (_ibState)
	{
	// �ҋ@
	case BossIronBall::IB_STATE::IDLE:
		SetIdle();
		break;
	// �d��
	case BossIronBall::IB_STATE::STIFFEN:
		break;
	// �ːi�U��
	case BossIronBall::IB_STATE::ATTACK_RUSH:
		// ������Ԃ̏ꍇ�̘A���U����
		_enhancedAttackCnt = 3;
		SetRush();
		break;
	// �����U��
	case BossIronBall::IB_STATE::ATTACK_DROP:
		// ������Ԃ̏ꍇ�̘A���U����
		_enhancedAttackCnt = 3;
		SetDrop();
		break;
	// ��]�U��
	case BossIronBall::IB_STATE::ATTACK_ROTATION:
		SetRotation();
		break;
	}
}

// �t�F�[�Y�̃��Z�b�g
// �e�s����Ԃ͕����̃t�F�[�Y�������A�s����Ԃ��ς��ƃt�F�[�Y�����Z�b�g�����
void BossIronBall::ResetPhase()
{
	_phase = 0;
	_phaseCnt = 0;
}

// �ҋ@��Ԃ̏���
void BossIronBall::IdleProcess()
{
	switch (_phase)
	{
	// �t�F�[�Y0: �ړ�
	case 0:
		_ibPos = VAdd(_ibPos, VScale(_ibMoveDir, IDLE_MOVE_SPEED));
		_ibPos.y = sinf(_phaseCnt / 30.0f * DX_PI) * 100.0f + _ibSphereCol.r;

		_phaseCnt++;

		// �ړ��t���[��������萔�ɒB�����玟�̃t�F�[�Y��
		if (_phaseCnt > IDLE_CNT_MAX) {
			// ���̃t�F�[�Y�̐ݒ�

			// �ҋ@���Ԃ�ݒ�
			_ibIdleCnt = IDLE_INTERVAL_BASE + (rand() % IDLE_INTERVAL_ADD_MAX);

			// SE�Đ�
			global._soundServer->DirectPlay("SE_Boss_Stay");

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y1: �ҋ@
	case 1:
		_phaseCnt++;
		// �ҋ@���Ԃ��o�߂����玟�̍s����Ԃ�
		if (_phaseCnt > _ibIdleCnt) {
			ResetPhase();
			CheckState();
		}
		break;
	}
}

// �ҋ@��ԂɑJ�ڂ���
void BossIronBall::SetIdle()
{
	ResetPhase();
	_isInvincible = false;

	_ibState = IB_STATE::IDLE;
	_ibIdleCnt = 0;

	// -100 ~ 100�͈̔͂Ń����_���Ɉړ�����������
	float x = (rand() % 201) - 100;
	float z = (rand() % 201) - 100;
	_ibMoveDir = VGet(x, 0.0f, z);
	_ibMoveDir = VNorm(_ibMoveDir);

	// ���Ƀ��f����������������ړ������ɐݒ�
	_ibModelNextForwardDir = _ibMoveDir;
	// ��̍s�Őݒ肵��_ibModelNextForwardDir���ێ�����悤�ɃX�e�[�g��ݒ�
	_ibModelDirState = IB_MODEL_DIR::NOT_UPDATE;
}

// �d����Ԃ̏���
void BossIronBall::StiffenProcess()
{
	// SE���Đ�
	if (_playSound) {
		global._soundServer->DirectPlay("SE_BOSS_Confusion");
		_playSound = false;
	}

	_ibStiffenCnt--;
	// �d�����Ԃ��o�߂�����ҋ@��ԂɑJ��
	if (_ibStiffenCnt < 0) {
		_ibStiffenCnt = 0;
		SetIdle();
	}
}

// �d����ԂɑJ�ڂ���
// isInvincible: ���G��Ԃɂ��邩�ǂ��� �f�t�H���g��false
// playSound: SE���Đ����邩�ǂ��� �f�t�H���g��true
void BossIronBall::SetStiffen(int cnt, bool isInvincible, bool playSound)
{
	_ibState = IB_STATE::STIFFEN;
	_ibStiffenCnt = cnt;
	_isInvincible = isInvincible;
	_playSound = playSound;
}

// �ːi�U���̏���
void BossIronBall::RushProcess()
{
	switch (_phase)
	{
	// �t�F�[�Y0: �Y�̕t�߂ֈړ�
	case 0:
		{
			// �ړ�����
			// ���������͐��`��ԁA���������̓T�C���g�ňړ�
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RU_REACH_STAKE_CNT);
			v.y = _ibSphereCol.r + 500.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(RU_REACH_STAKE_CNT)));
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RU_REACH_STAKE_CNT);
			_ibPos = v;
		}

		// ���f���̌������Y�̕����������悤�ɐݒ�
		_ibModelDirState = IB_MODEL_DIR::STAKE;

		_phaseCnt++;
		// �Y�̕t�߂ɓ��B�����玟�̃t�F�[�Y��
		if (_phaseCnt > RU_REACH_STAKE_CNT) {
			// ���̃t�F�[�Y�̐ݒ�

			// ���f���̌������v���C���[�̕����ɐݒ�
			VECTOR vDir = VSub(_player->GetPosition(), _ibPos);
			vDir.y = 0.0f;
			vDir = VNorm(vDir);
			_ibMoveDir = vDir;
			_ibModelDirState = IB_MODEL_DIR::PLAYER;

			// �ړ��O�̍��W��ۑ�
			_posBeforeMoving = _ibPos;
			// �ړ���̍��W��ݒ�
			// �t�F�[�Y0�I�����Ƀv���C���[�����������ڕW�����ɐݒ肷��
			_targetPos = VScale(_ibMoveDir, RU_MOVE_DISTANCE);

			// SE�Đ�
			global._soundServer->DirectPlay("SE_Boss_Stay");

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y1: ����
	case 1:
		// �O�ɐݒ肵��_ibModelNextForwardDir���X�V���Ȃ����Ȃ��悤�ɐݒ�i�t�F�[�Y0�I�����Ƀv���C���[����������������������j
		_ibModelDirState = IB_MODEL_DIR::NOT_UPDATE;

		_phaseCnt++;
		// ���ߎ��Ԃ��o�߂����玟�̃t�F�[�Y��
		if (_phaseCnt > RU_CHARGE_CNT) {
			// ���̃t�F�[�Y�̐ݒ�

			_playSound = true;

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y2: �ːi
	case 2:
		{
			// �t�F�[�Y0�I�����Ƀv���C���[��������������ڕW�����ƂȂ�
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RU_ATTACK_CNT);
			v.y = _ibSphereCol.r;
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RU_ATTACK_CNT);
			_ibPos = v;
		}

		if (_playSound) {
			global._soundServer->DirectPlay("SE_BOSS_Rush");
			_playSound = false;
		}

		_phaseCnt++;
		// �ːi���I�������玟�̃t�F�[�Y��
		if (_phaseCnt > RU_ATTACK_CNT) {
			ResetPhase();

			// ������Ԃ̏ꍇ�͘A���U��
			if (_isEnhanced) {
				_enhancedAttackCnt--;
				// �A���U���񐔂��c���Ă���ꍇ�͍ēx�ːi�U��
				if (_enhancedAttackCnt > 0) {
					SetRush();
				}
				// �A���U���񐔂��Ȃ��ꍇ�͍d��
				else {
					SetStiffen(RU_STIFFEN_CNT_ENHANCED);
				}
			}
			else {
				// ������ԂłȂ��ꍇ�͍d��
				SetStiffen(RU_STIFFEN_CNT);
			}
		}
		break;
	}
}

// �ːi�U���ɑJ�ڂ���
void BossIronBall::SetRush()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_RUSH;

	// �ړ��O�̍��W��ۑ�
	_posBeforeMoving = _ibPos;
	// �ړ���̍��W��ݒ�i�Y�̕t�߂Ɉړ�����j
	_targetPos = VGet(_stakePos->x, _ibSphereCol.r, _stakePos->z);
	VECTOR vDir = VSub(_posBeforeMoving, _targetPos);
	vDir.y = 0.0f;
	vDir = VNorm(vDir);
	_targetPos = VAdd(_targetPos, VScale(vDir, 500.0f));
}

// �����U���̏���
void BossIronBall::DropProcess()
{
	switch (_phase)
	{
	// �t�F�[�Y0: ��яオ��
	case 0:
	{
		// ��яオ�菈��
		// SetDrop()���s���ɁA�v���C���[���������W�̏��ɔ�яオ��
		VECTOR highestPos = VAdd(_targetPos, VGet(0.0f, 500.0f, 0.0f));
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::OutExpo(_phaseCnt, _posBeforeMoving.x, highestPos.x, DR_REACH_HIGHEST_CNT);
		v.y = Easing::OutExpo(_phaseCnt, _posBeforeMoving.y, highestPos.y, DR_REACH_HIGHEST_CNT);
		v.z = Easing::OutExpo(_phaseCnt, _posBeforeMoving.z, highestPos.z, DR_REACH_HIGHEST_CNT);
		_ibPos = v;

		// ���f���̌������v���C���[�̕����ɐݒ�
		_ibModelDirState = IB_MODEL_DIR::PLAYER;

		// SE�Đ�
		if (_playSound) {
			global._soundServer->DirectPlay("SE_BOSS_Jump_Attack_01");
			_playSound = false;
		}

		_phaseCnt++;
		// ��яオ�肪�I�������玟�̃t�F�[�Y��
		if (_phaseCnt > DR_REACH_HIGHEST_CNT) {
			// ���̃t�F�[�Y�̐ݒ�

			_playSound = true;

			// �ړ��O�̍��W��ۑ��i�v���C���[�̏��j
			_posBeforeMoving = _ibPos;

			_phaseCnt = 0;
			_phase++;
		}
	}
	break;

	// �t�F�[�Y1: ���n
	case 1:
	{
		// ���n����
		// SetDrop()���s���ɁA�v���C���[���������W�ɗ����U�����s��
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.x, _targetPos.x, DR_REACH_GROUND_CNT);
		v.y = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.y, _targetPos.y, DR_REACH_GROUND_CNT);
		v.z = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.z, _targetPos.z, DR_REACH_GROUND_CNT);
		_ibPos = v;

		// �O�ɐݒ肵��_ibModelNextForwardDir���X�V���Ȃ����Ȃ��悤�ɐݒ� �i�t�F�[�Y0�I�����Ƀv���C���[����������������������j
		_ibModelDirState = IB_MODEL_DIR::NOT_UPDATE;

		// SE�Đ�
		if (_playSound) {
			if (_ibPos.y - _ibSphereCol.r < 0.0f) {
				global._soundServer->DirectPlay("SE_BOSS_Jump_Attack_02");
				_playSound = false;
			}
		}

		_phaseCnt++;
		// ���n���I�������玟�̃t�F�[�Y��
		if (_phaseCnt > DR_REACH_GROUND_CNT) {

			ResetPhase();

			// ������Ԃ̏ꍇ�͘A���U��
			if (_isEnhanced) {
				_enhancedAttackCnt--;
				// �A���U���񐔂��c���Ă���ꍇ�͍ēx�����U��
				if (_enhancedAttackCnt > 0) {
					SetDrop();
				}
				// �A���U���񐔂��Ȃ��ꍇ�͍d��
				else { 
					SetStiffen(DR_STIFFEN_CNT_ENHANCED);
				}
			}
			else {
				// ������ԂłȂ��ꍇ�͍d��
				SetStiffen(DR_STIFFEN_CNT);
			}
		}
	}
	break;
	}
}

// �����U���ɑJ�ڂ���
void BossIronBall::SetDrop()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_DROP;

	// �ړ��O�̍��W��ۑ�
	_posBeforeMoving = _ibPos;
	// �ړ���̍��W��ݒ�i�v���C���[�̏��ɔ�яオ��j
	_targetPos = _player->GetPosition();
	_targetPos.y = _ibSphereCol.r;

	_playSound = true;
}

// ��]�U���̏���
void BossIronBall::RotationProcess()
{
	// �t�F�[�Y0�ȊO�i1~3�j�ł̉�]����
	if (_phase != 0) {
		// �Y�𒆐S�Ƃ��ĉ�]����
		float angle = _rotAngle * RO_ROTATION_DIR;
		VECTOR v = VScale(_rotBaseDir, _rotRadius);
		v.y = _ibSphereCol.r;
		MATRIX mR = MGetRotY(angle);
		MATRIX m = MMult(_mStakePosInv, mR); // �Y�̍��W�𒆐S�ɉ�]����
		m = MMult(m, _mStakePos);
		_ibPos = VTransform(v, m);

		// ���f���̌�����ݒ�
		MV1SetRotationXYZ(_ibModelHandle, VGet(0.0f, angle, 0.0f));
	}

	// ��]���x�̍X�V����
	switch (_phase)
	{
	// �t�F�[�Y0: �Y�̕t�߂܂Ŗ߂�
	case 0:
		// �ړ�����
		// ���������͐��`��ԁA���������̓T�C���g�ňړ�
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RO_REACH_STAKE_CNT);
		v.y = _ibSphereCol.r + 500.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(RO_REACH_STAKE_CNT)));
		v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RO_REACH_STAKE_CNT);
		_ibPos = v;

		// ���f���̌������Y�̕����������悤�ɐݒ�
		_ibModelDirState = IB_MODEL_DIR::STAKE;

		_phaseCnt++;
		// �Y�̕t�߂ɓ��B�����玟�̃t�F�[�Y��
		if (_phaseCnt > RO_REACH_STAKE_CNT) {
			_rotRadius = RO_ROTAION_RADIUS_MIN;

			// ���f���̌������Y�̋t�����ɐݒ�
			_ibModelDirState = IB_MODEL_DIR::STAKE_REVERSE;

			// SE�Đ�
			global._soundServer->DirectPlay("SE_Boss_Stay");

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y1: ��]�U���i�����j
	case 1:
		// �p���x����`��ԂōX�V
		_rotAngularVelocity = Easing::Linear(_phaseCnt, RO_ANGULAR_VELOCITY_MIN, RO_ANGULAR_VELOCITY_MAX, RO_ACCELERATION_CNT_MAX);
		// ��]���a����`��ԂōX�V
		_rotRadius = Easing::Linear(_phaseCnt, RO_ROTAION_RADIUS_MIN, RO_ROTAION_RADIUS_MAX, RO_ACCELERATION_CNT_MAX);

		_isRotationAttack = true;

		_phaseCnt++;
		// �������I�������玟�̃t�F�[�Y��
		if (_phaseCnt > RO_ACCELERATION_CNT_MAX) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y2: ��]�U���i�ő呬�x�ێ��j
	case 2:
		_phaseCnt++;
		// ��莞�Ԍo�߂����玟�̃t�F�[�Y��
		if (_phaseCnt > RO_MAINTAIN_MAXSPEED_CNT) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y3: ��]�U���i�����j
	case 3:
		// �p���x����`��ԂōX�V
		_rotAngularVelocity = Easing::Linear(_phaseCnt, RO_ANGULAR_VELOCITY_MAX, 0.0f, RO_DECELERATION_CNT_MAX);

		_phaseCnt++;
		// �������I�������玟�̃t�F�[�Y��
		if (_phaseCnt > RO_DECELERATION_CNT_MAX) {
			_isRotationAttack = false;

			ResetPhase();
			// �d����ԂɑJ��
			SetStiffen(RO_STIFFEN_CNT);
			return;
		}
		break;
	}

	// ���̃t���[���ł̉�]�p�x���X�V
	_rotAngle += _rotAngularVelocity;
}

// ��]�U���ɑJ�ڂ���
void BossIronBall::SetRotation()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_ROTATION;

	// �ړ��O�̍��W��ۑ�
	_posBeforeMoving = _ibPos;

	// ��]�̎n�_�ƂȂ������ݒ�
	_rotBaseDir = VSub(_ibPos, *_stakePos);
	_rotBaseDir.y = 0.0f;
	_rotBaseDir = VNorm(_rotBaseDir);

	// �ړ���̍��W��ݒ�i�Y�̕t�߂Ɉړ�����j
	_targetPos = VGet(_stakePos->x, _ibSphereCol.r, _stakePos->z);
	_targetPos = VAdd(_targetPos, VScale(_rotBaseDir, RO_ROTAION_RADIUS_MIN));

	_rotAngularVelocity = 0;
	_rotAngle = RO_ANGULAR_VELOCITY_MIN;
	_rotRadius = 0.0f;

	_playSound = true;
}

// �d�͏���
void BossIronBall::GravityProcess()
{
	_ibPos.y += _gravity;
	if (_isOnStage &&  _ibPos.y - _ibSphereCol.r < 0.0f) {
		_gravity = 40.0f;
		global._soundServer->DirectPlay("SE_Boss_Stay");
	}
	else {
		_gravity -= 6.0f;
	}
}

// ���̍X�V����
void BossIronBall::ChainProcess()
{
	_chainPos[BOSS_CHAIN_MAX - 1] = _ibPos;
	for (int i = 0; i < BOSS_CHAIN_MAX - 1; i++) {
		if (i == 0 && !_isStakeBroken) continue;
		_chainPos[i].y -= 16.0f;
		if (_chainPos[i].y - 20.0f < 0.0f) {
			//�S�����X�e�[�W��ɂ���Ƃ��́A�n�ʂ���̉����o���������s��
			if (_isOnStage) {
				_chainPos[i].y = 20.0f;
			}
			// �S�����X�e�[�W��ɂ��Ȃ��Ƃ��́A�X�e�[�W�̏�ɂ��鍽�݂̂������o��
			else {
				VECTOR v = _chainPos[i];
				v.y = 0.0f;
				float squareLength = VSquareSize(v);
				squareLength -= powf(_ibSphereCol.r + 230.0f, 2);
				if (squareLength < powf(_stageRadius, 2)) {
					_chainPos[i].y = 20.0f;
				}
			}
		}
	}

	// �����m�̊Ԋu�����ɕۂ���
	for (int i = BOSS_CHAIN_MAX - 1; i != 0; i--) {

		VECTOR vNext = _chainPos[i - 1];
		VECTOR vDelta = VSub(vNext, _chainPos[i]);
		float distance = VSize(vDelta);
		float difference = _chainDistance - distance;

		float offsetX = (difference * vDelta.x / distance) * 0.9f;
		float offsetY = (difference * vDelta.y / distance) * 0.9f;
		float offsetZ = (difference * vDelta.z / distance) * 0.9f;

		if (i != BOSS_CHAIN_MAX - 1) {
			_chainPos[i].x -= offsetX;
			_chainPos[i].y -= offsetY;
			_chainPos[i].z -= offsetZ;
		}

		if (i - 1 == 0 && !_isStakeBroken) continue;
		_chainPos[i - 1].x += offsetX;
		_chainPos[i - 1].y += offsetY;
		_chainPos[i - 1].z += offsetZ;
	}
}

// ������ԂɈڍs����
// ������ԂɑJ�ڂ��\�ȃ^�C�~���O�ɌĂяo���A���߂�_changeEnhanced��true�ɂȂ����Ƃ��ɋ�����ԂɈڍs����i���̏ꍇ��_isEnhanced�͕K��false�j
void BossIronBall::CheckChangeEnhanced()
{
	if (_changeEnhanced && !_isEnhanced) {
		_isEnhanced = true;
		SetRotation();
	}
}

// �v���C���[���S���̍��G�͈͓��ɂ��邩���擾����
// ���G�͈͓��ɂ���ꍇ�͍��G�͈͂̃C���f�b�N�X��Ԃ�
// @return: -1: �T���͈͓��ɂ��Ȃ�, 0: �T���͈�0�ɂ���, 1: �T���͈�1�ɂ���
int BossIronBall::CheckPlayerInSearchRange()
{
	int rangeIndex = -1;
	float squareDistance = VSquareSize(VSub(_player->GetPosition(), *_stakePos));
	if (squareDistance < SEARCH_RANGE[0] * SEARCH_RANGE[0]) {
		rangeIndex = 0;
	}
	else if (squareDistance < SEARCH_RANGE[1] * SEARCH_RANGE[1]) {
		rangeIndex = 1;
	}
	return rangeIndex;
}

// �m�b�N�o�b�N�̐ݒ���s��
// @param vDir: �m�b�N�o�b�N����
// @param speed: �m�b�N�o�b�N���x
void BossIronBall::SetKnockBack(VECTOR vDir, float speed)
{
	_isInvincible = true;
	_isKnockBack = true;

	// �d����ԂłȂ��ꍇ�A���A�Y���j�󂳂�Ă���ꍇ�̓m�b�N�o�b�N��ԂɑJ��
	if (_ibState != IB_STATE::STIFFEN || _isStakeBroken) {
		_ibState = IB_STATE::KNOCK_BACK;

		_knockBackDir = VNorm(vDir);
		_knockBackSpeed = speed;

		// �K���X��Ԃ̏ꍇ�̓m�b�N�o�b�N���x�𑝉�
		if (_isGlass) _knockBackSpeed *= KB_SPEED_RATE_GLASS;

		_knockBackCnt = 30;
		_gravity = 80.0f;
	}
	else {
		// �d����Ԃ̏ꍇ�̓n�[�h�m�b�N�o�b�N��ԂɑJ��
		// �i�d�����Ƀv���C���[�̍U�������������ꍇ�̏��, �Y�܂Œ��ڔ��ł����A�Y�Ƀ_���[�W������j

		ResetPhase();
		_ibState = IB_STATE::HARD_KNOCK_BACK;

		// �ړ��O�̍��W��ۑ�
		_posBeforeMoving = _ibPos;
		// �ړ���̍��W��ݒ�i�Y�܂Ŕ��ł����j
		_targetPos = *_stakePos;

		// �n�ʁiy���W0�j����Ƃ��A�S���̔��a�̑傫���̕�����y���W���v���X����
		_targetPos.y = _ibSphereCol.r;
	}
}

// �m�b�N�o�b�N��Ԃ̏���
void BossIronBall::KnockBackProcess()
{
	if (_isKnockBack) {
		_ibPos = VAdd(_ibPos, VScale(_knockBackDir, _knockBackSpeed));
		GravityProcess();
		_knockBackCnt--;

		// �m�b�N�o�b�N���I��������d����ԂɑJ��
		if (_knockBackCnt < 0) {
			_knockBackCnt = 0;
			_isKnockBack = false;

			// �d����ԂɑJ��
			if (_isStakeBroken) {
				// �Y���j�󂳂�Ă���ꍇ�͍d�����ɖ��G�����Ȃ�
				SetStiffen(30);
			}
			else {
				// �Y���j�󂳂�Ă��Ȃ��ꍇ�͍d�����ɖ��G�����ێ�����
				SetStiffen(10, true, false);
			}
		}
	}
}

// �n�[�h�m�b�N�o�b�N��Ԃ̏���
void BossIronBall::HardKnockBackProcess()
{
	switch (_phase)
	{
	// �t�F�[�Y0: �Y�܂Ŕ��ł���
	case 0:
		// �Y�ɓ��������玟�̃t�F�[�Y��
		if (_isHitStake) {
			// ���̃t�F�[�Y�̐ݒ�

			// �����蔻��𖳌���
			_useCollision = false;

			// �ړ��O�̍��W��ۑ�
			_posBeforeMoving = _ibPos;

			// �ړ���̍��W��ݒ�i���˕Ԃ��̍��W�j
			VECTOR vDir = VGet(0.0f, 0.0f, -1.0f);
			// �����_���ȕ����֒��˕Ԃ���s��
			int angle = rand() % 360;
			vDir = VTransform(vDir, MGetRotY(angle));
			VECTOR vBase = *_stakePos;
			vBase.y = _ibSphereCol.r;
			_targetPos = VAdd(vBase, VScale(vDir, HK_BOUNCE_DISTANCE));

			_phaseCnt = 0;
			_phase++;
			break;
		}

		// �ړ�����
		// ���������͐��`��ԁA���������̓T�C���g�ňړ�
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, HK_REACH_STAKE_CNT);
		v.y = _ibSphereCol.r + 800.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(HK_REACH_STAKE_CNT)));
		v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, HK_REACH_STAKE_CNT);
		_ibPos = v;

		_phaseCnt++;
		// �Y�ɓ��B�����玟�̃t�F�[�Y��
		if (_phaseCnt > HK_REACH_STAKE_CNT) {
			// ���̃t�F�[�Y�̐ݒ�

			// �����蔻��𖳌���
			_useCollision = false;

			// �ړ��O�̍��W��ۑ�
			_posBeforeMoving = _ibPos;

			// �ړ���̍��W��ݒ�i���˕Ԃ��̍��W�j
			VECTOR vDir = VGet(0.0f, 0.0f, -1.0f);
			// �����_���ȕ����֒��˕Ԃ���s��
			int angle = rand() % 360;
			vDir = VTransform(vDir, MGetRotY(angle));
			VECTOR vBase = *_stakePos;
			vBase.y = _ibSphereCol.r;
			_targetPos = VAdd(vBase, VScale(vDir, HK_BOUNCE_DISTANCE));

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// �t�F�[�Y1: ���˕Ԃ�
	case 1:
		{
			// �ړ�����
			// ���������͐��`��ԁA���������̓T�C���g�ňړ�
			VECTOR v = VGet(0.0f, _ibPos.y, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, HK_BOUNCE_CNT);
			v.y = _posBeforeMoving.y + 800.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(HK_BOUNCE_CNT)));
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, HK_BOUNCE_CNT);
			_ibPos = v;
		}
		_phaseCnt++;
		// ���˕Ԃ肪�I�������玟�̃t�F�[�Y��
		if (_phaseCnt > HK_BOUNCE_CNT) {
			// ���̃t�F�[�Y�̐ݒ�

			// �����蔻���L����
			_useCollision = true;

			_phaseCnt = 0;
			_phase++;
			CheckChangeEnhanced();
		}
		break;
	case 2: // �d��
		_phaseCnt++;
		// �d�����Ԃ��o�߂�����ҋ@��ԂɑJ��
		if (_phaseCnt > HK_STIFFEN_CNT) {
			_isKnockBack = false;
			ResetPhase();
			SetIdle();
		}
		break;
	}
}

// �A�j���[�V�����̍X�V����
void BossIronBall::AnimationProcess()
{
	MV1SetAttachAnimTime(_ibModelHandle, _animIndex, _playTime);

	_playTime += 1.0f;
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}

// �f�o�b�O���̕`��
void BossIronBall::DrawDebugInfo()
{
	auto color = _isInvincible ? COLOR_WHITE : COLOR_RED;
	_ibSphereCol.Render(color);
	color = _isRotationAttack ? COLOR_RED : COLOR_WHITE;
	_chainCapsuleCol.Render(color);

	for (int i = 0; i < 1; i++) {
		Sphere s = { *_stakePos, SEARCH_RANGE[i] };
		s.Render(COLOR_RED);
	}

	int x = 0;
	int y = 500;
	int line = 0;
	DrawBox(x, y, x + 300, y + 500, GetColor(0, 0, 0), TRUE);
	std::array<std::string, 7> stateStr = { "IDLE", "STIFFEN", "ATTACK_RUSH", "ATTACK_DROP", "ATTACK_ROTATION", "KnockBack", "HardKnockBack" };
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "State:%s", stateStr[static_cast<int>(_ibState)].c_str()); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "pos: x %3.2f, y %3.2f, z %3.2f", _ibPos.x, _ibPos.y, _ibPos.z); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "_phase: %d, _phaseCnt: %d", _phase, _phaseCnt); line++;

	//line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCnt:%d", _ibIdleCnt); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCntMax:%d", debugFrameMax); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "���ɐݒ肷��t���[����:%d", debugFrame); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "���ɐݒ肷��ړ�����:%3.2f cm", debugValue); line++;
}