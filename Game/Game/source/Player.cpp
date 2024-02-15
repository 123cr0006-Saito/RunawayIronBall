#include "Player.h"
#include "appframe.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

namespace {
	// �ő�HP
	constexpr int HP_MAX = 4;
	// �ő喳�G����
	constexpr int INVINCIBLE_CNT_MAX = 90;

	// �ړ����x
	// �ő�l
	constexpr float MOVE_SPEED_MAX = 8.0f;
	// �ŏ��l
	constexpr float MOVE_SPEED_MIN = 1.0f;

	// �X�e�B�b�N���̓x�N�g���̑傫���̍ő�l
	// ���̒l���ő�l�Ƃ��āA���͂̑傫�����������v�Z����i������0~1�͈̔͂ɃN�����v����j
	constexpr float MOVE_DIR_SIZE_MAX = 0.8f;

	// �u����v��ԂƁu�����v��Ԃ�؂�ւ���臒l
	// �u�X�e�B�b�N���̓x�N�g���̑傫���v�����̒l�𒴂�����u����v��ԂɂȂ�
	constexpr float MOVE_RUN_THRESHOLD = 0.6f;
}

Player::Player(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_input = XInput::GetInstance();
	_stickDir = VGet(0, 0, -1);

	_hp = HP_MAX;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;

	_canMove = true;
	_moveSpeed = 8.0f;
	_moveSpeedFD = 0.0f;

	_capsuleCollision.r = 30.0f;
	_capsuleCollision.up = 65.0f;
	UpdateCollision();

	// �S���̈ړ���Ԃ��u�Ǐ]�v�ɐݒ�
	_ibMoveState = IB_MOVE_STATE::FOLLOWING;

	_isAttackState = false;

	_playNextComboAnim = true;


	// �X�e�[�g���u�ҋ@�v�ɐݒ�
	_animStatus = ANIM_STATE::IDLE;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	SetBone();

	_isSwinging = false;
	_isRotationSwinging = false;
	_spinCnt = 0;


	_instance = this;

	_idleFightingRemainingCnt = 0;
	// �A�j���[�V�����}�l�[�W���[�̏����ݒ�
	_animManager = new AnimationManager();
	_animManager->InitMap("Player", _modelHandle, "MotionList_Player.csv");


	_frameData = new FrameData();
	std::vector<std::pair<int, std::string>> fdFileName;
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::HORISONTAL_SWING_01), "FD_MO_PL_Horizontal_first.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::HORISONTAL_SWING_02), "FD_MO_PL_Horizontal_second.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::HORISONTAL_SWING_03), "FD_MO_PL_Horizontal_third.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::IDLE), "FD_MO_PL_Idle.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::IDLE_FIGHTING), "FD_MO_PL_Idle_Fighting.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::ROTATION_SWING), "FD_MO_PL_Rotate_Loop.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::AVOIDANCE), "FD_MO_PL_Avoidance.csv"));
	_frameData->LoadData("Player", fdFileName);

	//_animManager->InitMap(&_animMap);

	//// _animMap����̏ꍇ�A�A�j���[�V��������ݒ肷��
	//// _animMap�͐ÓI�����o�ϐ��Ȃ̂ŁA�C���X�^���X�Ԃŋ��L����
	//if (_animMap.empty() ){
	//	// _animMap�̏����ݒ�
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::WAIT), MV1GetAnimIndex(_modelHandle, "MO_PL_Stay"), 0);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::RUN), MV1GetAnimIndex(_modelHandle, "MO_PL_Run"), 0);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::HORISONTAL_SWING_01), MV1GetAnimIndex(_modelHandle, "MO_PL_Horizontal_first"), 1);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::HORISONTAL_SWING_02), MV1GetAnimIndex(_modelHandle, "MO_PL_Horizontal_second"), 1);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::SPIN_SWING), MV1GetAnimIndex(_modelHandle, "MO_PL_roteate_loop"), 1);
	//}

	_modelColor = new ModelColor();
	_modelColor->Init(_modelHandle);
}

Player::~Player()
{
	_instance = nullptr;
	delete _animManager;
}

// ���G��Ԃ̍X�V
void Player::ChangeIsInvincible(bool b)
{
	_isInvincible = b;
	if (b) {
		_invincibleRemainingCnt = INVINCIBLE_CNT_MAX;
	}
	else {
		_invincibleRemainingCnt = 0;
	}
}

void Player::SetBone() {
	//����
	std::vector<int> bone_left_list(6);
	bone_left_list[0] = MV1SearchFrame(_modelHandle,"Left_mitsuami1");
	bone_left_list[1] = MV1SearchFrame(_modelHandle,"Left_mitsuami2");
	bone_left_list[2] = MV1SearchFrame(_modelHandle,"Left_mitsuami3");
	bone_left_list[3] = MV1SearchFrame(_modelHandle,"Left_mitsuami4");
	bone_left_list[4] = MV1SearchFrame(_modelHandle,"Left_mitsuami5");
	bone_left_list[5] = MV1SearchFrame(_modelHandle,"Left_mitsuami6");
	_bone[0] = new bone(&_modelHandle, bone_left_list, bone_left_list.size() - 2, "res/JsonFile/hair_parameters.json");
	//�E��
	std::vector<int> bone_right_list(6);
	bone_right_list[0] = MV1SearchFrame(_modelHandle,"Right_mitsuami1");
	bone_right_list[1] = MV1SearchFrame(_modelHandle,"Right_mitsuami2");
	bone_right_list[2] = MV1SearchFrame(_modelHandle,"Right_mitsuami3");
	bone_right_list[3] = MV1SearchFrame(_modelHandle,"Right_mitsuami4");
	bone_right_list[4] = MV1SearchFrame(_modelHandle,"Right_mitsuami5");
	bone_right_list[5] = MV1SearchFrame(_modelHandle,"Right_mitsuami6");
	_bone[1] = new bone(&_modelHandle, bone_right_list, bone_right_list.size() - 2, "res/JsonFile/hair_parameters.json");
};

void Player::SetNextExp(std::string FileName) {
	myJson json(FileName);
	_maxLevel = json._size - 1;
	for(auto& expList : json._json) {
		int nowLevel = 0;
		int exp = 0;
		expList.at("Level").get_to(nowLevel);
		expList.at("Exp").get_to(exp);
		_nextLevel[nowLevel] = exp;
	}
};

bool Player::UpdateExp() {
	if (_nowLevel < _maxLevel) {
		if (_nowExp >= _nextLevel[_nowLevel]) {
			_nowExp -= _nextLevel[_nowLevel];
			_nowLevel++;
		}
	}

	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		if (_nowLevel <= _maxLevel) {
			_nowLevel--;
		}
	}
	if (_input->GetTrg(XINPUT_BUTTON_B)) {
		if (_nowLevel < _maxLevel) {
			_nowLevel++;
		}
	}
	return true;
};

bool Player::Process(float camAngleY)
{
	// �t���[���f�[�^�̎��s�R�}���h���`�F�b�N����
	CheckFrameDataCommand();


	// ���G��Ԋ֘A�̏���
	if (_isInvincible) {
		int cnt = 10;

		bool b = (INVINCIBLE_CNT_MAX - _invincibleRemainingCnt) % (cnt * 2) < cnt;
		_modelColor->ChangeFlickerTexture(b);

		// ���G���Ԃ����炷
		_invincibleRemainingCnt -= 1;
		// ���G���Ԃ�0�ȉ��ɂȂ����疳�G��Ԃ���������
		if (_invincibleRemainingCnt < 0) {
			_isInvincible = false;
			_modelColor->ChangeFlickerTexture(false);
		}
	}

	{
		// �ړ�����

		// �ړ��t���O
		bool _isMoved = false;

		bool _isRunnning = false;

		// ���X�e�B�b�N�̓��͏����擾����
		auto lStick = _input->GetAdjustedStick_L();
		VECTOR vMoveDir = VGet(lStick.x, 0, lStick.y);
		if (_canMove) {
			// ���X�e�B�b�N�̓��͂���������
			if (VSize(vMoveDir) > 0.000000f) {

				float size = VSize(vMoveDir);

				float rate = size / MOVE_DIR_SIZE_MAX;
				_moveSpeed = MOVE_SPEED_MAX * rate;
				rate = Math::Clamp(0.0f, 1.0f, rate);
				_moveSpeed = Easing::Linear(rate, MOVE_SPEED_MIN, MOVE_SPEED_MAX, 1.0f);
				if(rate > MOVE_RUN_THRESHOLD) _isRunnning = true;

				// ���͕����x�N�g���𐳋K������
				vMoveDir = VScale(vMoveDir, 1.0f / size);
				// ���͕����x�N�g���ɃJ�����̌����̕␳�������Ĉړ����������肷��
				MATRIX mRot = MGetRotY(camAngleY);
				vMoveDir = VTransform(vMoveDir, mRot);

				_pos = VAdd(_pos, VScale(vMoveDir, _moveSpeed));

				_stickDir = vMoveDir;
				_isMoved = true;
			}
		}

		if (_moveSpeedFD != 0.f) {
			_pos = VAdd(_pos, VScale(VNorm(_forwardDir), _moveSpeedFD));
		}

		if (!_isAttackState && _animStatus != ANIM_STATE::AVOIDANCE) {
			if (_isMoved) {
				if (_isRunnning) {
					_animStatus = ANIM_STATE::RUN;
				}
				else {
					_animStatus = ANIM_STATE::WALK;
				}

				// �L�����N�^�[�����炩�ɉ�]������
				float angle = Math::CalcVectorAngle(_forwardDir, vMoveDir);
				float rotRad = (2.0f * DX_PI_F) / 30.0f;
				if (rotRad > angle) {
					_forwardDir = vMoveDir;
				}
				else {
					VECTOR vN = VCross(_forwardDir, vMoveDir);
					_forwardDir = VTransform(_forwardDir, MGetRotAxis(vN, rotRad));
				}
			}
			else {
				if (_idleFightingRemainingCnt > 0) {
					_animStatus = ANIM_STATE::IDLE_FIGHTING;
					_idleFightingRemainingCnt -= 1;
				}
				else {
					_animStatus = ANIM_STATE::IDLE_FIGHTING;
				}
			}
		}
	}

	if (_animStatus != ANIM_STATE::AVOIDANCE) {
		// ��]�U��
		if (_spinCnt > 90) {
			_animStatus = ANIM_STATE::ROTATION_SWING;
		}
		// �ʏ�U��
		else if (_input->GetRel(XINPUT_BUTTON_X) != 0) {
			_playNextComboAnim = true;
			if (!_isAttackState) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_01;
			}
		}

		if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
			_spinCnt++;
		}
		else {
			_spinCnt = 0;
			if (_isRotationSwinging) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_03;

				// ���f���̐��ʕ������X�V����
				_forwardDir = _stickDir;
			}
		}

		// ���
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			if (!_isSwinging || _isRotationSwinging) {
				_animStatus = ANIM_STATE::AVOIDANCE;
				// ���f���̐��ʕ������X�V����
				_forwardDir = _stickDir;
				_spinCnt = 0;
			}
		}
	}


	if (_isRotationSwinging) {
		_forwardDir = VTransform(_forwardDir, MGetRotY(-(2.0f * DX_PI_F) / 30.0f));
	}

	// ��]����
	Math::SetModelForward_RotationY(_modelHandle, _forwardDir);

	BlastOffProcess();

	MV1SetPosition(_modelHandle, _pos);
	UpdateCollision();
	//-------------------
	//�V�����쐬�����֐��ł��B
	UpdateExp();
	UpdateBone();
	//-------------------

	_animManager->Process(static_cast<int>(_animStatus));
	_frameData->Process(static_cast<int>(_animStatus), _animManager->GetPlayTime());





	return true;
}


bool Player::BlastOffProcess()
{
	if (_blastOffPower > 0.0f) {
		VECTOR tmp = VNorm(_blastOffDir);
		if (VSize(tmp) > 0.0f) {
			_pos = VAdd(_pos, VScale(tmp, _blastOffPower));
		}
		_blastOffPower -= 1.0f;
	}
	return true;
}

bool Player::Render()
{
	CharacterBase::Render();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = VAdd(_pos, VGet(0, _capsuleCollision.r, 0));
	_capsuleCollision.Update();
}

void Player::UpdateBone() {
	for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
		_bone[i]->Process();
		_bone[i]->SetMain(_bone[i]->_massPosList);
	}
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
			_bone[i]->SetDebugDraw();
			_bone[i]->DebugProcess(12);
		}
	}
};

VECTOR Player::GetRightHandPos()
{
	VECTOR handPos = VGet(0.0f, 0.0f, 0.0f);
	MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, _rightHandFrameIndex);
	handPos = VTransform(handPos, m);
	return handPos;
}

void Player::CheckFrameDataCommand()
{
	// ���s�R�}���h���X�g���擾����
	std::vector<CommandParam> commandList = _frameData->GetCommandData();

	for (auto itr = commandList.begin(); itr != commandList.end(); ++itr) {
		// �R�}���h���擾����
		int command = itr->first;
		// �p�����[�^���擾����
		float param = itr->second;

		// �R�}���h�ɂ���ď����𕪊򂷂�
		switch (command)
		{
		// ���[�V������ύX����
		case C_P_CHANGE_MOTION:
			_animStatus = static_cast<ANIM_STATE>(param);
			break;
		// �ړ��\��Ԃ�ύX����
		case C_P_ENABLE_MOVE:
			_canMove = static_cast<bool>(param);
			break;
		case C_P_MOVE_FORWARD:
			_moveSpeedFD = param;
			break;
		// �R���{�̓��͎�t���J�n����
		// C_P_CHECK_CHANGE_COMBO�����s�����^�C�~���O��_playNextComboAnim��true�̏ꍇ�Ɏ��̃R���{���[�V�������Đ�����
		case C_P_ACCEPT_COMBO_INPUT:
			_playNextComboAnim = false;
			break;
		// �R���{���[�V�����̕ύX���`�F�b�N����
		case C_P_CHECK_CHANGE_COMBO:
			if (_playNextComboAnim) {
				switch (_animStatus)
				{
				case Player::ANIM_STATE::HORISONTAL_SWING_01:
					_animStatus = ANIM_STATE::HORISONTAL_SWING_02;
					break;
				case Player::ANIM_STATE::HORISONTAL_SWING_02:
					_animStatus = ANIM_STATE::HORISONTAL_SWING_03;
					break;
				}
			}
			break;
		// �U����Ԃ̕ύX���`�F�b�N����	
		case C_P_CHECK_CHANGE_ATTACK_STATE:
		{
			// ���̍U����Ԃ��擾����
			bool nextState = static_cast<bool>(param);
			// ���̍U����Ԃ�false�̏ꍇ�A�U����Ԃ���������
			if (!nextState){
				_isSwinging = false;
				_isRotationSwinging = false;
				// �O�̍U����Ԃ�true�̏ꍇ�A�퓬�ҋ@��ԂɑJ�ڂ���
				if (_isAttackState) {
					_idleFightingRemainingCnt = 240;
				}
			}
			else {
				// ��]�U���̏ꍇ�A��]�U���t���O�𗧂Ă�
				if (_animStatus == ANIM_STATE::ROTATION_SWING) {
					_isRotationSwinging = true;
					_isSwinging = false;
				}
				// �ʏ�U���̏ꍇ�A�ʏ�U���t���O�𗧂Ă�
				else {
					_isRotationSwinging = false;
					_isSwinging = true;
				}
			}
			// �U����Ԃ��X�V����
			_isAttackState = nextState;
			break;
		}
		case C_P_ENABLE_IB_ATTACK_COLLISION:
			break;
		case C_P_ENABLE_IB_FOLLOWING_MODE:
			_ibMoveState = static_cast<int>(param) == 0 ? IB_MOVE_STATE::PUTTING_ON_SOCKET : IB_MOVE_STATE::FOLLOWING;
			break;
		case C_P_ENABLE_IB_INTERPOLATION:
			_ibMoveState = IB_MOVE_STATE::INTERPOLATION;
			break;
		}
	}
}

void Player::DrawDebugInfo()
{
	for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
		_bone[i]->DebugRender();
	}

	DrawCapsule3D(_capsuleCollision.up_pos, _capsuleCollision.down_pos, _capsuleCollision.r, 16, COLOR_RED, COLOR_RED, false);

	int x = 0;
	int y = 100;
	int line = 0;
	DrawFormatString(x, y + line * 16, COLOR_RED, "HP:%d", _hp); line++;
	DrawFormatString(x, y + line * 16, COLOR_RED, "isInvincible:%d", _isInvincible); line++;
	DrawFormatString(x, y + line * 16, COLOR_RED, "invincibleCnt:%d", _invincibleRemainingCnt); line++;
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);
}
