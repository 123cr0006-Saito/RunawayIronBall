#include "Player.h"
#include "MotionList.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

namespace {
	// �ő僌�x��
	constexpr int LEVEL_MAX = 10;
	// �ő�HP
	constexpr int HP_MAX = 4;
	// �ő喳�G����
	constexpr int INVINCIBLE_CNT_MAX = 90;

	// �ړ����x�i�ʏ��ԁj
	// �ő�l
	constexpr float MOVE_SPEED_MAX = 8.0f;
	// �ŏ��l
	constexpr float MOVE_SPEED_MIN = 1.0f;
	
	// �ړ����x�i����ԁj
	// �ő�l
	constexpr float MOVE_SPEED_TIRED_MAX = 3.0f;
	// �ŏ��l
	constexpr float MOVE_SPEED_TIRED_MIN = 0.5f;

	// �X�e�B�b�N���̓x�N�g���̑傫���̍ő�l
	// ���̒l���ő�l�Ƃ��āA���͂̑傫�����������v�Z����i������0~1�͈̔͂ɃN�����v����j
	constexpr float MOVE_INPUT_VALUE_MAX = 0.8f;

	// �u����v��ԂƁu�����v��Ԃ�؂�ւ���臒l
	// �u�X�e�B�b�N���̓x�N�g���̑傫���v�����̒l�𒴂�����u����v��ԂɂȂ�
	constexpr float MOVE_RUN_THRESHOLD = 0.6f;


	// �X�^�~�i�̍ő�l
	constexpr float STAMINA_MAX = 480.0f;
	// ��]�U����1�t���[��������̃X�^�~�i�����
	constexpr float ROTAION_SWING_STAMINA_DECREASE = 1.0f;
	// �X�^�~�i��0�ɂȂ��Ă���ő�l�܂ŉ񕜂���̂ɂ����鎞��
	constexpr float STANIMA_RECOVERY_TIME = 120.0f;

	// �t���[���f�[�^�̃R�}���h
	constexpr unsigned int	C_P_CHANGE_MOTION							= 0;
	constexpr unsigned int 	C_P_ENABLE_MOVE								= 1;
	constexpr unsigned int 	C_P_MOVE_FORWARD							= 2;
	constexpr unsigned int 	C_P_ACCEPT_COMBO_INPUT					= 3;
	constexpr unsigned int 	C_P_CHECK_CHANGE_COMBO				= 4;
	constexpr unsigned int 	C_P_CHECK_CHANGE_ATTACK_STATE		= 5;
	constexpr unsigned int 	C_P_ENACLE_MOTION_CANCEL				= 6;

	constexpr unsigned int 	C_P_ENABLE_IB_ATTACK_COLLISION		= 100;
	constexpr unsigned int 	C_P_ENABLE_IB_FOLLOWING_MODE		= 101;
	constexpr unsigned int 	C_P_ENABLE_IB_INTERPOLATION			= 102;
}

Player::Player(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_input = XInput::GetInstance();
	_stickDir = VGet(0, 0, -1);

	_hp = HP_MAX;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;

	_stamina = STAMINA_MAX;
	_staminaMax = STAMINA_MAX;
	_isConsumingStamina = false;
	_isTired = false;
	_staminaRecoverySpeed = _staminaMax / STANIMA_RECOVERY_TIME;

	_canMove = true;
	_moveSpeed = 8.0f;
	_moveSpeedFWD = 0.0f;

	_capsuleCollision.r = 30.0f;
	_capsuleCollision.up = 65.0f;
	UpdateCollision();

	_isAttackState = false;

	_canMotionCancel = true;
	_playNextComboAnim = true;

	// �S���̏�����
	_chain = NEW Chain();
	_chain->Init();
	_chain->SetPlayerModelHandle(_modelHandle);
	// �S���̈ړ���Ԃ��u�Ǐ]�v�ɐݒ�
	_chain->SetMoveState(IB_MOVE_STATE::FOLLOWING);

	// �X�e�[�g���u�ҋ@�v�ɐݒ�
	_animStatus = ANIM_STATE::IDLE;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	SetBone();

	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;


	_instance = this;

	_idleFightingRemainingCnt = 0;

	_nowExp = 0;
	_nowLevel = 0;
	SetNextExp("res/JsonFile/ExpList.json");
	SetPowerScale("res/JsonFile/IronState.json");
	UpdateLevel();

	// ���[�V�������X�g�̓ǂݍ���
	MotionList::Load("Player", "MotionList_Player.csv");
	auto motionList = MotionList::GetMotionList("Player");

	// �A�j���[�V�����}�l�[�W���[�̏����ݒ�
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Player", _modelHandle, *motionList);

	// �t���[���f�[�^�̏����ݒ�
	_frameData = NEW FrameData();
	_frameData->LoadData("Player", *motionList);

	_modelColor = NEW ModelColor();
	_modelColor->Init(_modelHandle);
}

Player::~Player()
{
	_instance = nullptr;
	_input = nullptr;
	delete _animManager;
	delete _frameData;
	delete _modelColor;
	delete _chain;

	for (int i = 0; i < 2; i++) {
		delete _bone[i];
	}

}

// ���G��Ԃ̍X�V
void Player::ChangeIsInvincible(bool b, int frame)
{
	if (b) {
		if (!_isInvincible) {
			_invincibleRemainingCnt = frame;
		}
	}
	else {
		_invincibleRemainingCnt = 0;
	}
	_isInvincible = b;
}

void Player::SetDamage()
{
	_hp -= 1;
	if (_hp < 0) {
		_hp = 0;
	}
	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;
	_animStatus = ANIM_STATE::HIT;
	ChangeIsInvincible(true, INVINCIBLE_CNT_MAX);
	// �_���[�W�T�E���h�̍Đ�
	switch (_hp) {
	case 0:
		global._soundServer->DirectPlay("PL_GameOver");
		break;
	case 1:
		global._soundServer->DirectPlay("PL_HealthAlert");
		break;
	case 2:
	case 3:
		int voiceNum = rand() % 2 + 1;
		std::string voiceName = "PL_Damage0" + std::to_string(voiceNum);
		global._soundServer->DirectPlay(voiceName);
		break;
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
	_bone[0] = NEW bone(&_modelHandle, bone_left_list, bone_left_list.size() - 2, "res/JsonFile/hair_parameters.json");
	//�E��
	std::vector<int> bone_right_list(6);
	bone_right_list[0] = MV1SearchFrame(_modelHandle,"Right_mitsuami1");
	bone_right_list[1] = MV1SearchFrame(_modelHandle,"Right_mitsuami2");
	bone_right_list[2] = MV1SearchFrame(_modelHandle,"Right_mitsuami3");
	bone_right_list[3] = MV1SearchFrame(_modelHandle,"Right_mitsuami4");
	bone_right_list[4] = MV1SearchFrame(_modelHandle,"Right_mitsuami5");
	bone_right_list[5] = MV1SearchFrame(_modelHandle,"Right_mitsuami6");
	_bone[1] = NEW bone(&_modelHandle, bone_right_list, bone_right_list.size() - 2, "res/JsonFile/hair_parameters.json");
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
			UpdateLevel();
		}
	}
	return true;
};

bool Player::HealHp() {
	// hp ���񕜂ł���Ƃ� true : hp �� MAX �̎��񕜂����� false
	if (_hp < HP_MAX) {
		_hp++;
		global._soundServer->DirectPlay("PL_Heal");
		return true;
	}
	return false;
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
		if (_canMove || _canMotionCancel) {
			float size = VSize(vMoveDir);
			// ���X�e�B�b�N�̓��͂���������
			if (size > 0.000000f) {

				float rate = size / MOVE_INPUT_VALUE_MAX;
				rate = Math::Clamp(0.0f, 1.0f, rate);

				float speedMax = _isTired ? MOVE_SPEED_TIRED_MAX : MOVE_SPEED_MAX;
				float speedMin  = _isTired ? MOVE_SPEED_TIRED_MIN : MOVE_SPEED_MIN;

				_moveSpeed = Easing::Linear(rate, speedMin, speedMax, 1.0f);
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

		if (_moveSpeedFWD != 0.f) {
			_pos = VAdd(_pos, VScale(VNorm(_forwardDir), _moveSpeedFWD));
			_moveSpeedFWD = 0.f;
		}

		if (_canMotionCancel) {
			if (_isMoved) {
				if (_isTired) {
					_animStatus = ANIM_STATE::WALK_TIRED;
				}
				else {
					if (_isRunnning) {
						_animStatus = ANIM_STATE::RUN;
					}
					else {
						_animStatus = ANIM_STATE::WALK;
					}
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
			else if(!_isAttackState) {
				if (_isTired) {
					_animStatus = ANIM_STATE::IDLE_TIRED;
					_idleFightingRemainingCnt = 0;
				}
				else {
					if (_idleFightingRemainingCnt > 0) {
						_animStatus = ANIM_STATE::IDLE_FIGHTING;
						_idleFightingRemainingCnt -= 1;
					}
					else {
						_animStatus = ANIM_STATE::IDLE;
					}
				}
			}
		}
	}

	// �X�^�~�i�̍X�V
	if (!_isConsumingStamina) {
		_staminaRecoverySpeed = STAMINA_MAX / STANIMA_RECOVERY_TIME;
		_stamina += _staminaRecoverySpeed;
		if (_stamina > STAMINA_MAX) {
			_stamina = STAMINA_MAX;
			_isTired = false;
			//_animStatus = ANIM_STATE::IDLE;
		}
	}

	if (_animStatus == ANIM_STATE::ROTATION_SWING) {
		_stamina -= ROTAION_SWING_STAMINA_DECREASE;
		_isConsumingStamina = true;
		if (_stamina < 0.0f) {
			_stamina = 0.0f;
			_isTired = true;
			_isRotationSwinging = false;
			_rotationCnt = 0;
			_forwardDir = _stickDir;
			_animStatus = ANIM_STATE::HORISONTAL_SWING_03;
		}
	}

	if(!_isAttackState ){
		_isConsumingStamina = false;
	}

	// �U����Ԃ̍X�V
	if (_isTired == false && _animStatus != ANIM_STATE::AVOIDANCE && _animStatus != ANIM_STATE::HIT) {
		// ��]�U��
		if (_rotationCnt > 90) {
			if (!_isRotationSwinging) {
				_animStatus = ANIM_STATE::TO_ROTATION_SWING;
			}
		}
		// �ʏ�U��
		else if (_input->GetRel(XINPUT_BUTTON_X) != 0) {
			_playNextComboAnim = true;
			if (!_isAttackState) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_01;
			}
		}

		if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
			_rotationCnt++;
		}
		else {
			_rotationCnt = 0;
			if (_isRotationSwinging) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_03;

				// ���f���̐��ʕ������X�V����
				_forwardDir = _stickDir;
			}
		}
	}

	if (_canMotionCancel) {
		// ���
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			if (!_isSwinging || _isRotationSwinging) {
				_animStatus = ANIM_STATE::AVOIDANCE;
				// ���f���̐��ʕ������X�V����
				_forwardDir = _stickDir;
				_rotationCnt = 0;
				_idleFightingRemainingCnt = 240;
			}
		}
	}

	if (_isRotationSwinging) {
		float angle = _animStatus == ANIM_STATE::TO_ROTATION_SWING ? -(2.0f * DX_PI_F) / 80.0f : -(2.0f * DX_PI_F) / 30.0f;
		_forwardDir = VTransform(_forwardDir, MGetRotY(angle));
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

	


	_chain->Process();



	return true;
}

bool Player::AnimationProcess()
{
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
	_chain->Render();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = VAdd(_pos, VGet(0, _capsuleCollision.r, 0));
	_capsuleCollision.Update();
}

void Player::SetPowerScale(std::string FileName)
{
	myJson json(FileName);
	int level = 0;
	int power = 0;
	float scale = 0;
	for (auto& list : json._json) {
		list.at("Level").get_to(level);
		list.at("Power").get_to(power);
		list.at("Magnification").get_to(scale);
		_powerAndScale[level] = std::make_pair(power, scale);
	}
}

bool Player::UpdateLevel()
{
	_power = _powerAndScale[_nowLevel].first;
	_chain->UpdateLevel(_powerAndScale[_nowLevel].second);
	if (_nowLevel > 0) {
		// ���x���A�b�v�G�t�F�N�g
		float size = 5.0f * _powerAndScale[_nowLevel].second;
		VECTOR* pos = GetIBPosPtr();
		int effectHandle = ResourceServer::Load("FX_3D_Level_Up", "res/Effekseer/FX_3D_Level_Up/FX_3D_Level_Up.efkefc");
		EffekseerPosSynchro* effect = new EffekseerPosSynchro(effectHandle, pos, size);
		EffectManeger::GetInstance()->LoadEffect(effect);
		// ���x���A�b�v�{�C�X
		int randomNum = rand() % 2 + 1; // �����_���ŉ������Đ��@1~2
		std::string voiceName = "PL_LevelUp0" + std::to_string(randomNum);
		global._soundServer->DirectPlay(voiceName);
	}
	return true;
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
			_moveSpeedFWD = param;
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
				if (_animStatus == ANIM_STATE::TO_ROTATION_SWING || _animStatus == ANIM_STATE::ROTATION_SWING) {
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
		case C_P_ENACLE_MOTION_CANCEL:
			_canMotionCancel = static_cast<bool>(param);
			break;

		case C_P_ENABLE_IB_ATTACK_COLLISION:
			_chain->SetEnabledAttackCollision(static_cast<bool>(param));
			break;
		case C_P_ENABLE_IB_FOLLOWING_MODE:
		{
			IB_MOVE_STATE nextState = static_cast<int>(param) == 0 ? IB_MOVE_STATE::PUTTING_ON_SOCKET : IB_MOVE_STATE::FOLLOWING;
			_chain->SetMoveState(nextState);
			break;
		}
		case C_P_ENABLE_IB_INTERPOLATION:
			_chain->SetMoveState(IB_MOVE_STATE::INTERPOLATION);
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

	//int x = 0;
	//int y = 100;
	//int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "HP:%d", _hp); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "isInvincible:%d", _isInvincible); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "invincibleCnt:%d", _invincibleRemainingCnt); line++;

	//DrawFormatString(x, y + line * 16, COLOR_RED, "ANIM:%d", _animStatus); line++;
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);


	_animManager->DrawDebugInfo();
	_chain->DrawDebugInfo();
}
