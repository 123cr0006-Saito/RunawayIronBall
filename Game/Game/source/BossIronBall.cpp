#include "BossIronBall.h"
#include "BossIronBall.h"

namespace {
	constexpr int BOSS_CHAIN_MAX = 12;
	constexpr float BOSS_CHAIN_TOTAL_LENGTH = 3000.0f;


	constexpr float SEARCH_RANGE[2] = { 2000.0f, 3000.0f };

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
	constexpr int RU_ATTACK_CNT = 60;
	// �ːi�ł̈ړ�����
	constexpr float RU_MOVE_DISTANCE = 3500.0f;
	// �ːi��̍d�����ԁi�ʏ펞�j
	constexpr int RU_STIFFEN_CNT = 120;
	// �ːi��̍d�����ԁi�������j
	constexpr int RU_STIFFEN_CNT_ENHANCED = 180;

	// �����U��
	// ��яオ���Ă���A�ō��n�_�ɓ��B����܂ł̃t���[����
	constexpr int DR_REACH_HIGHEST_CNT = 90;
	// �ō��n�_�ɓ��B���Ă���A�n�ʂɒ��n����܂ł̃t���[����
	constexpr int DR_REACH_GROUND_CNT = 30;
	// �n�ʂɒ��n��̍d�����ԁi�ʏ펞�j
	constexpr int DR_STIFFEN_CNT = 60;
	// �n�ʂɒ��n��̍d�����ԁi������Ԏ��j
	constexpr int DR_STIFFEN_CNT_ENHANCED = 180;

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
	constexpr float RO_ROTAION_RADIUS_MAX = 2800.0f;

	// �ő呬�x�ɓ��B����܂ł̃t���[����
	constexpr int RO_ACCELERATION_CNT_MAX = 180;
	// �ő呬�x���ێ�����t���[����
	constexpr int RO_MAINTAIN_MAXSPEED_CNT = 210;
	// �ő呬�x����A���x0�܂Ō���������܂ł̃t���[����
	constexpr int RO_DECELERATION_CNT_MAX = 90;
	// ��]�U����̍d������
	constexpr int RO_STIFFEN_CNT = 300;

	// �n�[�h�m�b�N�o�b�N
	// �Y�ɓ��B����܂ł̃t���[����
	constexpr int HK_REACH_STAKE_CNT = 45;
	// �Y�ɂ����蒵�˕Ԃ���s���t���[����
	constexpr int HK_BOUNCE_CNT = 45;
	// ���˕Ԃ苗��
	constexpr float HK_BOUNCE_DISTANCE = 2000.0f;
	// ���˕Ԃ��̍d������
	constexpr int HK_STIFFEN_CNT = 60;

	// ---------

	// ��
	constexpr float SCALE = 12.8f;
}

BossIronBall::BossIronBall()
{
	_ibModelHandle = -1;
	_ibPos = VGet(0.0f, 0.0f, 0.0f);
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 0.0f;
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


	_player = nullptr;
}

BossIronBall::~BossIronBall()
{
	_stakePos = nullptr;
	_player = nullptr;
}

void BossIronBall::LoadModel()
{
	_ibModelHandle = ResourceServer::MV1LoadModel("Bossnake", "res/Enemy/Cg_Enemy_Bossnake/Cg_Enemy_Bossnake.mv1");
	_chainModelHandle = ResourceServer::MV1LoadModel("Chain", "res/Chain/Cg_Chain.mv1");
}

void BossIronBall::Init(VECTOR* stakePos)
{
	_stakePos = stakePos;
	_chainDistance = BOSS_CHAIN_TOTAL_LENGTH / BOSS_CHAIN_MAX;
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		_chainPos.push_back(VAdd(*_stakePos, VGet(0.0f, 500.0f, -_chainDistance * 2 * i)));
	}


	_ibPos = _chainPos[BOSS_CHAIN_MAX - 1];
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 20.0f * SCALE;
	MV1SetScale(_ibModelHandle, VScale(VGet(1.0f, 1.0f, 1.0f), SCALE));

	_ibState = IB_STATE::IDLE;


	_mStakePos = MGetTranslate(*_stakePos);
	_mStakePosInv = MInverse(_mStakePos);


	_rotAngle = 0.0f;


	_player = Player::GetInstance();
}

void BossIronBall::Process()
{
	if (CheckHitKey(KEY_INPUT_Z) != 0) {
		SetRotation();
	}

	switch (_ibState)
	{
	case BossIronBall::IB_STATE::IDLE:
		IdleProcess();
		CheckChangeEnhanced();
		break;
	case BossIronBall::IB_STATE::STIFFEN:
		StiffenProcess();
		CheckChangeEnhanced();
		break;
	case BossIronBall::IB_STATE::ATTACK_RUSH:
		RushProcess();
		break;
	case BossIronBall::IB_STATE::ATTACK_DROP:
		DropProcess();
		break;
	case BossIronBall::IB_STATE::ATTACK_ROTATION:
		RotationProcess();
		break;
	case BossIronBall::IB_STATE::KNOCK_BACK:
		KnockBackProcess();
		break;
	case BossIronBall::IB_STATE::HARD_KNOCK_BACK:
		HardKnockBackProcess();
		break;
	}

	//_ibPos = _chainPos[CHAIN_MAX - 1];
	_ibPos.y -= 16.0f;
	if (_ibPos.y - _ibSphereCol.r < 0.0f) _ibPos.y = _ibSphereCol.r;
	UpdateIBCollision();

	_isHitStake = false;

	// ��
	if (1 == CheckHitKey(KEY_INPUT_UP)) {
		debugFrame += 1;
	}
	else if (1 == CheckHitKey(KEY_INPUT_DOWN)) {
		debugFrame -= 1;
		if (debugFrame < 1) debugFrame = 1;
	}

	if (1 == CheckHitKey(KEY_INPUT_RIGHT)) {
		debugValue += 1.0f;
	}
	else if (1 == CheckHitKey(KEY_INPUT_LEFT)) {
		debugValue -= 1.0f;
	}

	ChainProcess();
}

void BossIronBall::Render()
{
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		MV1SetPosition(_chainModelHandle, _chainPos[i]);
		MV1DrawModel(_chainModelHandle);
	}

	MV1SetPosition(_ibModelHandle, _ibPos);
	MV1DrawModel(_ibModelHandle);
}

void BossIronBall::UpdateIBCollision()
{
	_ibSphereCol.centerPos = _ibPos;
}

void BossIronBall::CheckState()
{
	int searchRangeIndex = -1;
	if (!_isStakeBroken) {
		searchRangeIndex = CheckPlayerInSearchRange();
		if (searchRangeIndex == -1) {
			_ibState = IB_STATE::ATTACK_RUSH;
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

	switch (_ibState)
	{
	case BossIronBall::IB_STATE::IDLE:
		break;
	case BossIronBall::IB_STATE::STIFFEN:
		break;
	case BossIronBall::IB_STATE::ATTACK_RUSH:
		// ������Ԃ̏ꍇ�̘A���U����
		_enhancedAttackCnt = 3;
		SetRush();
		break;
	case BossIronBall::IB_STATE::ATTACK_DROP:
		// ������Ԃ̏ꍇ�̘A���U����
		_enhancedAttackCnt = 3;
		SetDrop();
		break;
	case BossIronBall::IB_STATE::ATTACK_ROTATION:
		SetRotation();
		break;
	}
}

void BossIronBall::ResetPhase()
{
	_phase = 0;
	_phaseCnt = 0;
}

void BossIronBall::IdleProcess()
{
	switch (_phase)
	{
	case 0:
		_ibPos = VAdd(_ibPos, VScale(_ibMoveDir, IDLE_MOVE_SPEED));
		_ibPos.y = sinf(_phaseCnt / 30.0f * DX_PI) * 100.0f + _ibSphereCol.r;

		_phaseCnt++;
		if (_phaseCnt > IDLE_CNT_MAX) {
			_ibIdleCnt = IDLE_INTERVAL_BASE + (rand() % IDLE_INTERVAL_ADD_MAX);

			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 1:
		_phaseCnt++;
		if (_phaseCnt > _ibIdleCnt) {
			ResetPhase();
			CheckState();
		}
		break;
	}
}

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
}

void BossIronBall::StiffenProcess()
{
	_ibStiffenCnt--;
	if (_ibStiffenCnt < 0) {
		_ibStiffenCnt = 0;
		SetIdle();
	}
}

// �d����ԂɑJ�ڂ���
// isInvincible: ���G��Ԃɂ��邩�ǂ��� �f�t�H���g��false
void BossIronBall::SetStiffen(int cnt, bool isInvincible)
{
	_ibState = IB_STATE::STIFFEN;
	_ibStiffenCnt = cnt;
	_isInvincible = isInvincible;
}

void BossIronBall::RushProcess()
{
	switch (_phase)
	{
	case 0: 			// �Y�̕t�߂ֈړ�
		{
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RU_REACH_STAKE_CNT);
			v.y = _ibSphereCol.r + 500.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(RU_REACH_STAKE_CNT)));
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RU_REACH_STAKE_CNT);
			_ibPos = v;
		}
		_phaseCnt++;
		if (_phaseCnt > RU_REACH_STAKE_CNT) {

			VECTOR vDir = VSub(_player->GetPosition(), _ibPos);
			vDir.y = 0.0f;
			vDir = VNorm(vDir);
			_ibMoveDir = vDir;

			_posBeforeMoving = _ibPos;
			_targetPos = VScale(_ibMoveDir, RU_MOVE_DISTANCE);

			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 1:			// ����
		_phaseCnt++;
		if (_phaseCnt > RU_CHARGE_CNT) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 2:			// �ːi
		{
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RU_ATTACK_CNT);
			v.y = _ibSphereCol.r;
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RU_ATTACK_CNT);
			_ibPos = v;
		}
		_phaseCnt++;
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
				SetStiffen(RU_STIFFEN_CNT);
			}
		}
		break;
	}
}

void BossIronBall::SetRush()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_RUSH;
	_posBeforeMoving = _ibPos;

	_targetPos = VGet(_stakePos->x, _ibSphereCol.r, _stakePos->z);
	VECTOR vDir = VSub(_posBeforeMoving, _targetPos);
	vDir.y = 0.0f;
	vDir = VNorm(vDir);
	_targetPos = VAdd(_targetPos, VScale(vDir, 500.0f));
}

void BossIronBall::DropProcess()
{
	switch (_phase)
	{
	case 0:				// ��яオ��
	{
		VECTOR highestPos = VAdd(_targetPos, VGet(0.0f, 500.0f, 0.0f));
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::OutExpo(_phaseCnt, _posBeforeMoving.x, highestPos.x, DR_REACH_HIGHEST_CNT);
		v.y = Easing::OutExpo(_phaseCnt, _posBeforeMoving.y, highestPos.y, DR_REACH_HIGHEST_CNT);
		v.z = Easing::OutExpo(_phaseCnt, _posBeforeMoving.z, highestPos.z, DR_REACH_HIGHEST_CNT);
		_ibPos = v;
		_phaseCnt++;
		if (_phaseCnt > DR_REACH_HIGHEST_CNT) {
			_posBeforeMoving = _ibPos;
			_phaseCnt = 0;
			_phase++;
		}
	}
	break;

	case 1:				// ���n
	{
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.x, _targetPos.x, DR_REACH_GROUND_CNT);
		v.y = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.y, _targetPos.y, DR_REACH_GROUND_CNT);
		v.z = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.z, _targetPos.z, DR_REACH_GROUND_CNT);


		_ibPos = v;
		_phaseCnt++;
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
				SetStiffen(DR_STIFFEN_CNT);
			}
		}
	}
	break;
	}
}

void BossIronBall::SetDrop()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_DROP;
	_posBeforeMoving = _ibPos;
	_targetPos = _player->GetPosition();
	_targetPos.y = _ibSphereCol.r;
}

void BossIronBall::RotationProcess()
{
	if (_phase != 0) {
		// ��]����
		float angle = _rotAngle * RO_ROTATION_DIR;
		VECTOR v = VScale(_rotBaseDir, _rotRadius);
		v.y = _ibSphereCol.r;
		MATRIX mR = MGetRotY(angle);
		MATRIX m = MMult(_mStakePosInv, mR); // �Y�̍��W�𒆐S�ɉ�]����
		m = MMult(m, _mStakePos);
		_ibPos = VTransform(v, m);
		MV1SetRotationXYZ(_ibModelHandle, VGet(0.0f, angle, 0.0f));
	}

	// ��]���x�̍X�V����
	switch (_phase)
	{
	case 0: // �Y�̕t�߂܂Ŗ߂�

		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RO_REACH_STAKE_CNT);
		v.y = _ibSphereCol.r + 500.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(RO_REACH_STAKE_CNT)));
		v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RO_REACH_STAKE_CNT);
		_ibPos = v;

		_phaseCnt++;
		if (_phaseCnt > RU_REACH_STAKE_CNT) {
			_rotRadius = RO_ROTAION_RADIUS_MIN;

			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 1: // ����
		_rotAngularVelocity = Easing::Linear(_phaseCnt, RO_ANGULAR_VELOCITY_MIN, RO_ANGULAR_VELOCITY_MAX, RO_ACCELERATION_CNT_MAX);
		_rotRadius = Easing::Linear(_phaseCnt, RO_ROTAION_RADIUS_MIN, RO_ROTAION_RADIUS_MAX, RO_ACCELERATION_CNT_MAX);

		_phaseCnt++;
		if (_phaseCnt > RO_ACCELERATION_CNT_MAX) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 2: // �ő呬�x�ێ�
		_phaseCnt++;
		if (_phaseCnt > RO_MAINTAIN_MAXSPEED_CNT) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 3: // ����
		_rotAngularVelocity = Easing::Linear(_phaseCnt, RO_ANGULAR_VELOCITY_MAX, 0.0f, RO_DECELERATION_CNT_MAX);

		_phaseCnt++;
		if (_phaseCnt > RO_DECELERATION_CNT_MAX) {
			ResetPhase();
			SetStiffen(RO_STIFFEN_CNT);
			return;
		}
		break;
	}

	// ���̃t���[���ł̉�]�p�x���X�V
	_rotAngle += _rotAngularVelocity;
}

void BossIronBall::SetRotation()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_ROTATION;

	_posBeforeMoving = _ibPos;

	_rotBaseDir = VSub(_ibPos, *_stakePos);
	_rotBaseDir.y = 0.0f;
	_rotBaseDir = VNorm(_rotBaseDir);

	_targetPos = VGet(_stakePos->x, _ibSphereCol.r, _stakePos->z);
	_targetPos = VAdd(_targetPos, VScale(_rotBaseDir, RO_ROTAION_RADIUS_MIN));

	_rotAngularVelocity = 0;
	_rotAngle = RO_ANGULAR_VELOCITY_MIN;
	_rotRadius = 0.0f;
}


void BossIronBall::GravityProcess()
{
	_ibPos.y += _gravity;
	if (_ibPos.y - _ibSphereCol.r < 0.0f) {
		_gravity = 40.0f;
	}
	else {
		_gravity -= 6.0f;
	}
}

void BossIronBall::ChainProcess()
{
	_chainPos[BOSS_CHAIN_MAX - 1] = _ibPos;
	for (int i = 0; i < BOSS_CHAIN_MAX - 1; i++) {
		if (i == 0 && !_isStakeBroken) continue;
		_chainPos[i].y -= 16.0f;
		if (_chainPos[i].y - 20.0f < 0.0f) _chainPos[i].y = 20.0f;
	}

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

void BossIronBall::SetKnockBack(VECTOR vDir, float speed)
{
	_isInvincible = true;
	_isKnockBack = true;
	if (_ibState != IB_STATE::STIFFEN || _isStakeBroken) {
		_ibState = IB_STATE::KNOCK_BACK;
		_knockBackDir = VNorm(vDir);
		_knockBackSpeed = speed;
		_knockBackCnt = 30;
		_gravity = 80.0f;
	}
	else {
		ResetPhase();
		_ibState = IB_STATE::HARD_KNOCK_BACK;

		_posBeforeMoving = _ibPos;
		_targetPos = *_stakePos;
		// �n�ʁiy���W0�j����Ƃ��A�S���̔��a�̑傫���̕�����y���W���v���X����
		_targetPos.y = _ibSphereCol.r;
	}
}

void BossIronBall::DrawDebugInfo()
{
	auto color = _isInvincible ? COLOR_WHITE : COLOR_RED;
	_ibSphereCol.Render(color);

	for (int i = 0; i < 1; i++) {
		Sphere s = { *_stakePos, SEARCH_RANGE[i] };
		s.Render(COLOR_RED);
	}

	int x = 0;
	int y = 500;
	int line = 0;
	DrawBox(x, y, x + 300, y + 500, GetColor(0, 0, 0), TRUE);
	std::array<std::string, 7> stateStr = { "IDLE", "STIFFEN", "ATTACK_RUSH", "ATTACK_DROP", "ATTACK_ROTATION", "KnockBack", "HardKnockBack"};
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "State:%s", stateStr[static_cast<int>(_ibState)].c_str()); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "pos: x %3.2f, y %3.2f, z %3.2f", _ibPos.x, _ibPos.y, _ibPos.z); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "_phase: %d, _phaseCnt: %d", _phase, _phaseCnt); line++;

	//line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCnt:%d", _ibIdleCnt); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCntMax:%d", debugFrameMax); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "���ɐݒ肷��t���[����:%d", debugFrame); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "���ɐݒ肷��ړ�����:%3.2f cm", debugValue); line++;
}

void BossIronBall::KnockBackProcess()
{
	if (_isKnockBack) {
		_ibPos = VAdd(_ibPos, VScale(_knockBackDir, _knockBackSpeed));
		GravityProcess();
		_knockBackCnt--;
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
				SetStiffen(10, true);
			}
		}
	}
}

void BossIronBall::HardKnockBackProcess()
{
	switch (_phase)
	{
	case 0: // �Y�ɓ��B����܂�
		// �Y�ɓ��������玟�̃t�F�[�Y��
		if (_isHitStake) {
			_useCollision = false;
			_posBeforeMoving = _ibPos;

			VECTOR vDir = VGet(0.0f, 0.0f, -1.0f);
			int angle = rand() % 360;
			vDir = VTransform(vDir, MGetRotY(angle));
			VECTOR vBase = *_stakePos;
			vBase.y = _ibSphereCol.r;
			_targetPos = VAdd(vBase, VScale(vDir, HK_BOUNCE_DISTANCE));

			_phaseCnt = 0;
			_phase++;
		}
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, HK_REACH_STAKE_CNT);
		v.y = _ibSphereCol.r + 800.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(HK_REACH_STAKE_CNT)));
		v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, HK_REACH_STAKE_CNT);
		_ibPos = v;

		_phaseCnt++;
		if (_phaseCnt > HK_REACH_STAKE_CNT) {
			_useCollision = false;
			_posBeforeMoving = _ibPos;

			VECTOR vDir = VGet(0.0f, 0.0f, -1.0f);
			int angle = rand() % 360;
			vDir = VTransform(vDir, MGetRotY(angle));
			VECTOR vBase = *_stakePos;
			vBase.y = _ibSphereCol.r;
			_targetPos = VAdd(vBase, VScale(vDir, HK_BOUNCE_DISTANCE));

			_phaseCnt = 0;
			_phase++;
		}
		break;
	case 1: // ���˕Ԃ�
		{
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, HK_BOUNCE_CNT);
			v.y = _posBeforeMoving.y + 800.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(HK_BOUNCE_CNT)));
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, HK_BOUNCE_CNT);
			_ibPos = v;
		}
		_phaseCnt++;
		if (_phaseCnt > HK_BOUNCE_CNT) {
			_useCollision = true;
			_phaseCnt = 0;
			_phase++;
			CheckChangeEnhanced();
		}
		break;
	case 2: // �d��
		_phaseCnt++;
		if (_phaseCnt > HK_STIFFEN_CNT) {
			_isKnockBack = false;
			ResetPhase();
			SetIdle();
		}
		break;
	}
}