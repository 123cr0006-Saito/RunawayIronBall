#include "Player.h"
#include "MotionList.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

namespace {
	// �ő僌�x��
	constexpr int LEVEL_MAX = 9;
	// �ő�HP
	constexpr int HP_MAX = 4;
	// �ő喳�G����
	constexpr int INVINCIBLE_CNT_MAX = 240;

	// �ړ����x�i�ʏ��ԁj
	// �ő�l
	constexpr float MOVE_SPEED_MAX = 14.0f;
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

	// ��]�U���̃J�E���g
	// ���̃t���[�����ȏ�̊ԁA�{�^��������������Ɖ�]�U������������
	constexpr int ROTAION_SWING_CNT_MAX = 25;

	// �X�^�~�i�̍ő�l
	constexpr float STAMINA_MAX = 480.0f;
	// ��]�U����1�t���[��������̃X�^�~�i�����
	constexpr float ROTAION_SWING_STAMINA_DECREASE = 1.0f;
	// ����s���̃X�^�~�i�����
	constexpr float AVOIDANCE_STAMINA_DECREASE = STAMINA_MAX / 6.0f;
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
	constexpr unsigned int 	C_P_SET_INVINCIBLE_CNT						= 7;

	constexpr unsigned int 	C_P_ENABLE_IB_ATTACK_COLLISION		= 100;
	constexpr unsigned int 	C_P_ENABLE_IB_FOLLOWING_MODE		= 101;
	constexpr unsigned int 	C_P_ENABLE_IB_INTERPOLATION			= 102;
}

Player::Player()
{
	_input = nullptr;
	_inputWorldDir = VGet(0.0f, 0.0f, -1.0f);

	_hp = 0;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;
	_idleFightingRemainingCnt = 0;

	_stamina = 0;
	_staminaMax = 0;
	_isRecoveringStamina = true;
	_cntToStartRecoveryStamina = 0;
	_isTired = false;
	_staminaRecoverySpeed = 0;

	_canMove = false;
	_moveSpeed = 0.0f;
	_moveSpeedFWD = 0.0f;

	_isAttackState = false;
	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;

	_canMotionCancel = false;
	_playNextComboAnim = false;

	_animManager = nullptr;
	_animStatus = ANIM_STATE::IDLE;
	_frameData = nullptr;

	_ironBall = nullptr;

	_capsuleCollision.r = 0.0f;
	_capsuleCollision.up = 0.0f;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = -1;

	global._oldExp = global.GetAllExp();


	_nowLevel = 0;
	_maxLevel = 0;
	global.SetOldExp(global.GetAllExp());
	_power = 0;

	_nextLevel.clear();
	_animManager = nullptr;
	_frameData = nullptr;
	_modelColor = nullptr;



	_instance = nullptr;
}

Player::~Player()
{
	_instance = nullptr;
	_input = nullptr;
	delete _animManager;
	delete _frameData;
	delete _modelColor;
	delete _ironBall;

	for (auto&& bone : _bone) {
		delete bone;
	}
	_bone.clear();

	global.SetAllExpAdd(_nowExp);
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
	std::vector<int> Left_mitsuami(6);
	Left_mitsuami[0] = MV1SearchFrame(_modelHandle, "Left_mitsuami1");
	Left_mitsuami[1] = MV1SearchFrame(_modelHandle, "Left_mitsuami2");
	Left_mitsuami[2] = MV1SearchFrame(_modelHandle, "Left_mitsuami3");
	Left_mitsuami[3] = MV1SearchFrame(_modelHandle, "Left_mitsuami4");
	Left_mitsuami[4] = MV1SearchFrame(_modelHandle, "Left_mitsuami5");
	Left_mitsuami[5] = MV1SearchFrame(_modelHandle, "Left_mitsuami6");
	_bone.push_back(NEW bone(&_modelHandle, Left_mitsuami, Left_mitsuami.size() - 2, "Data/BoneParam/Hear.json"));
	//�E��
	std::vector<int> Right_mitsuami(6);
	Right_mitsuami[0] = MV1SearchFrame(_modelHandle, "Right_mitsuami1");
	Right_mitsuami[1] = MV1SearchFrame(_modelHandle, "Right_mitsuami2");
	Right_mitsuami[2] = MV1SearchFrame(_modelHandle, "Right_mitsuami3");
	Right_mitsuami[3] = MV1SearchFrame(_modelHandle, "Right_mitsuami4");
	Right_mitsuami[4] = MV1SearchFrame(_modelHandle, "Right_mitsuami5");
	Right_mitsuami[5] = MV1SearchFrame(_modelHandle, "Right_mitsuami6");
	_bone.push_back(NEW bone(&_modelHandle, Right_mitsuami, Right_mitsuami.size() - 2, "Data/BoneParam/Hear.json"));
	//�X�J�[�t������
	std::vector<int> Scarf_Underf(3);
	Scarf_Underf[0] = MV1SearchFrame(_modelHandle, "Under_sukafu1");
	Scarf_Underf[1] = MV1SearchFrame(_modelHandle, "Under_sukafu2");
	Scarf_Underf[2] = MV1SearchFrame(_modelHandle, "Under_sukafu3");
	_bone.push_back(NEW bone(&_modelHandle, Scarf_Underf, Scarf_Underf.size() - 2, "Data/BoneParam/Scarf.json"));
	//���{������
	std::vector<int> Left_Sarope(3);
	Left_Sarope[0] = MV1SearchFrame(_modelHandle, "Left_sarope1");
	Left_Sarope[1] = MV1SearchFrame(_modelHandle, "Left_sarope2");
	Left_Sarope[2] = MV1SearchFrame(_modelHandle, "Left_sarope3");
	_bone.push_back(NEW bone(&_modelHandle, Left_Sarope, Left_Sarope.size() - 2, "Data/BoneParam/Sarope.json"));
	//���{���E��
	std::vector<int> Right_Sarope(3);
	Right_Sarope[0] = MV1SearchFrame(_modelHandle, "Right_sarope1");
	Right_Sarope[1] = MV1SearchFrame(_modelHandle, "Right_sarope2");
	Right_Sarope[2] = MV1SearchFrame(_modelHandle, "Right_sarope3");
	_bone.push_back(NEW bone(&_modelHandle, Right_Sarope, Right_Sarope.size() - 2, "Data/BoneParam/Sarope.json"));
	//���{������
	std::vector<int> Left_ribbon(4);
	Left_ribbon[0] = MV1SearchFrame(_modelHandle, "Left_ribbon1");
	Left_ribbon[1] = MV1SearchFrame(_modelHandle, "Left_ribbon2");
	Left_ribbon[2] = MV1SearchFrame(_modelHandle, "Left_ribbon3");
	Left_ribbon[3] = MV1SearchFrame(_modelHandle, "Left_ribbon4");
	_bone.push_back(NEW bone(&_modelHandle, Left_ribbon, Left_ribbon.size() - 2, "Data/BoneParam/Ribbon.json"));
	//�X�J�[�g���{���E��
	std::vector<int> Right_ribbon(4);
	Right_ribbon[0] = MV1SearchFrame(_modelHandle, "Right_ribbon1");
	Right_ribbon[1] = MV1SearchFrame(_modelHandle, "Right_ribbon2");
	Right_ribbon[2] = MV1SearchFrame(_modelHandle, "Right_ribbon3");
	Right_ribbon[3] = MV1SearchFrame(_modelHandle, "Right_ribbon4");
	_bone.push_back(NEW bone(&_modelHandle, Right_ribbon, Right_ribbon.size() - 2, "Data/BoneParam/Ribbon.json"));
	//�A�z��
	std::vector<int> Ahoge(3);
	Ahoge[0] = MV1SearchFrame(_modelHandle, "Ahoge1");
	Ahoge[1] = MV1SearchFrame(_modelHandle, "Ahoge2");
	Ahoge[2] = MV1SearchFrame(_modelHandle, "Ahoge3");
	_bone.push_back(NEW bone(&_modelHandle, Ahoge, Ahoge.size() - 2, "Data/BoneParam/Ahoge.json"));
};

bool Player::HealHp(){
	if(_hp < HP_MAX){
		_hp++;
		global._soundServer->DirectPlay("SE_Heal");
		global._soundServer->DirectPlay("PL_Heal");
		return true;
	}
	return false;
};

bool Player::UpdateExp() {
	if (_nowLevel < _maxLevel) {
		if (_nowExp >= _nextLevel[_nowLevel]) {
			_nowExp -= _nextLevel[_nowLevel];
			global.SetAllExpAdd(_nextLevel[_nowLevel]);
			_nowLevel++;
			UpdateLevel();
		}
	}
	return true;
};

bool Player::Init(int modelHandle, VECTOR pos)
{
	CharacterBase::Init(modelHandle, pos);

	_input = XInput::GetInstance();
	_inputWorldDir = VGet(0.0f, 0.0f, -1.0f);

	_hp = HP_MAX;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;

	_stamina = STAMINA_MAX;
	_staminaMax = STAMINA_MAX;
	_isRecoveringStamina = true;
	_isTired = false;
	_staminaRecoverySpeed = _staminaMax / STANIMA_RECOVERY_TIME;

	_canMove = true;
	_moveSpeed = 8.0f;
	_moveSpeedFWD = 0.0f;

	_isAttackState = false;
	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;

	_canMotionCancel = true;
	_playNextComboAnim = true;

	// ���[�V�������X�g�̓ǂݍ���
	MotionList::Load("Player", "MotionList_Player.csv");
	auto motionList = MotionList::GetMotionList("Player");
	// �A�j���[�V�����}�l�[�W���[�̏����ݒ�
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Player", _modelHandle, *motionList);
	// �t���[���f�[�^�̏����ݒ�
	_frameData = NEW FrameData();
	_frameData->LoadData("Player", *motionList);
	// �X�e�[�g���u�ҋ@�v�ɐݒ�
	_animStatus = ANIM_STATE::IDLE;


	// �S���̏�����
	_ironBall = NEW IronBall();
	_ironBall->Init();
	_ironBall->SetParentInstance(this);
	_ironBall->SetParentPosPtr(&_pos);
	_ironBall->SetPlayerModelHandle(_modelHandle);
	// �S���̈ړ���Ԃ��u�Ǐ]�v�ɐݒ�
	_ironBall->SetMoveState(IB_MOVE_STATE::FOLLOWING);

	// �����蔻��̏����ݒ�
	_capsuleCollision.r = 30.0f;
	_capsuleCollision.up = 65.0f;
	UpdateCollision();

	_cell->_objType = OBJ_TYPE::PL;

	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	_modelColor = NEW ModelColor();
	_modelColor->Init(_modelHandle);

	SetBone();




	_instance = this;

	_idleFightingRemainingCnt = 0;

	_nowLevel = 0;

	SetLevelParam("res/JsonFile/IronState.json");
	UpdateLevel();












	return true;
}

bool Player::Process(float camAngleY)
{
	// �t���[���f�[�^�̎��s�R�}���h���`�F�b�N����
	CheckFrameDataCommand();

	if(_hp <= 0){
		_animStatus = ANIM_STATE::GAMEOVER;
	}

	if (_animStatus != ANIM_STATE::GAMEOVER) {

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
					float speedMin = _isTired ? MOVE_SPEED_TIRED_MIN : MOVE_SPEED_MIN;

					_moveSpeed = Easing::Linear(rate, speedMin, speedMax, 1.0f);
					if (rate > MOVE_RUN_THRESHOLD) _isRunnning = true;

					// ���͕����x�N�g���𐳋K������
					vMoveDir = VScale(vMoveDir, 1.0f / size);
					// ���͕����x�N�g���ɃJ�����̌����̕␳�������Ĉړ����������肷��
					MATRIX mRot = MGetRotY(camAngleY);
					vMoveDir = VTransform(vMoveDir, mRot);

					_pos = VAdd(_pos, VScale(vMoveDir, _moveSpeed));

					_inputWorldDir = vMoveDir;
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
				else if (!_isAttackState) {
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
		if (_isRecoveringStamina) {
			_staminaRecoverySpeed = _staminaMax / STANIMA_RECOVERY_TIME;
			_stamina += _staminaRecoverySpeed;
			if (_stamina > _staminaMax) {
				_stamina = _staminaMax;
				_isTired = false;
				//_animStatus = ANIM_STATE::IDLE;
			}
		}

		if (_animStatus == ANIM_STATE::ROTATION_SWING) {
			_isRecoveringStamina = false;
			_stamina -= ROTAION_SWING_STAMINA_DECREASE;
			_cntToStartRecoveryStamina = 90;
			if (_stamina < 0.0f) {
				_stamina = 0.0f;
				_isTired = true;
				_isRotationSwinging = false;
				_rotationCnt = 0;
				_forwardDir = _inputWorldDir;
				_animStatus = ANIM_STATE::HORISONTAL_SWING_03;
			}
		}

		if (_cntToStartRecoveryStamina > 0) {
			_cntToStartRecoveryStamina -= 1;
			if (_cntToStartRecoveryStamina <= 0) {
				_isRecoveringStamina = true;
			}
		}

		// �U����Ԃ̍X�V
		if (_isTired == false && _animStatus != ANIM_STATE::AVOIDANCE && _animStatus != ANIM_STATE::HIT) {
			// ��]�U�����������邩�ǂ����̔���
			if (_rotationCnt > ROTAION_SWING_CNT_MAX) {
				if (!_isRotationSwinging) {
					_animStatus = ANIM_STATE::TO_ROTATION_SWING;
				}
			}
			// �R���{�U��1�i�ڂ̓���
			else if (!_isAttackState) {
				if (_input->GetRel(XINPUT_BUTTON_X) != 0) { // �����[�X����
					_playNextComboAnim = true;
					if (!_isAttackState) {
						_animStatus = ANIM_STATE::HORISONTAL_SWING_01;
					}
				}
			}
			// �R���{�U��2�i�ځE3�i�ڂ̓���
			else if (_animStatus == ANIM_STATE::HORISONTAL_SWING_01 || _animStatus == ANIM_STATE::HORISONTAL_SWING_02) {
				if (_input->GetTrg(XINPUT_BUTTON_X) != 0) { // �g���K����
					_playNextComboAnim = true;
				}
			}

			// ��]�U���̓���
			if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
				_rotationCnt++;
			}
			else {
				_rotationCnt = 0;
				if (_isRotationSwinging) {
					_animStatus = ANIM_STATE::HORISONTAL_SWING_03;

					// ���f���̐��ʕ������X�V����
					_forwardDir = _inputWorldDir;
				}
			}
		}

		if (!_isRotationSwinging && _isAttackState) {
			_rotationCnt = 0;
		}

		if (!_isTired && _canMotionCancel) {
			// ���
			if (_input->GetTrg(XINPUT_BUTTON_A)) {
				if (!_isSwinging || _isRotationSwinging) {
					_animStatus = ANIM_STATE::AVOIDANCE;
					// ���f���̐��ʕ������X�V����
					_forwardDir = _inputWorldDir;
					_rotationCnt = 0;
					_idleFightingRemainingCnt = 240;

					_isRecoveringStamina = false;
					_cntToStartRecoveryStamina = 90;
					_stamina -= AVOIDANCE_STAMINA_DECREASE;
					if (_stamina < 0.0f) {
						_stamina = 0.0f;
						_isTired = true;
					}
				}
			}
		}

		if (_isRotationSwinging) {
			float angle = _animStatus == ANIM_STATE::TO_ROTATION_SWING ? -(2.0f * DX_PI_F) / 80.0f : -(2.0f * DX_PI_F) / 30.0f;
			_forwardDir = VTransform(_forwardDir, MGetRotY(angle));
		}

		// ��]����
		Math::SetModelForward_RotationY(_modelHandle, _forwardDir);

	}

	BlastOffProcess();

	UpdateCollision();
	//-------------------
	//�V�����쐬�����֐��ł��B
	UpdateExp();
	UpdateBone();
	//-------------------

	


	_ironBall->Process();

	_collisionManager->UpdateCell(_cell);

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
	_ironBall->Render();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = VAdd(_pos, VGet(0, _capsuleCollision.r, 0));
	_capsuleCollision.Update();
}

void Player::SetLevel(int allExp){
	int exp = allExp;
	while(1){
		if (_nowLevel >= LEVEL_MAX)break;// �ő僌�x��

		if(exp >= _nextLevel[_nowLevel]){
			exp -= _nextLevel[_nowLevel];
			_nowLevel++;
		}else{
			_nowExp = exp;
			break;
		}
	}
	_power = _levelParam[_nowLevel].power;
	_ironBall->UpdateLevel(_levelParam[_nowLevel].magnification);
	_staminaMax = _levelParam[_nowLevel].stamina;
};

void Player::SetLevelParam(std::string FileName)
{
	myJson json(FileName);
	_maxLevel = json._size - 1;
	int level = 0;
	int exp = 0;
	LevelData data;
	for (auto& list : json._json) {
		// ���x���ƌo���l���擾
		list.at("Level").get_to(level);
		list.at("Exp").get_to(exp);
		_nextLevel[level] = exp;
		// �U���͂Ɗg�嗦�E�X�^�~�i���擾
		list.at("Power").get_to(data.power);
		list.at("Magnification").get_to(data.magnification);
		list.at("Stamina").get_to(data.stamina);
		_levelParam[level] = data;
	}
}

bool Player::UpdateLevel()
{
	_power = _levelParam[_nowLevel].power;
	_ironBall->UpdateLevel(_levelParam[_nowLevel].magnification);
	_staminaMax =  _levelParam[_nowLevel].stamina;
	if (_nowLevel > 0) {
		// ���x���A�b�v�G�t�F�N�g
		float size = 5.0f * _levelParam[_nowLevel].magnification;
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
	for (int i = 0; i < 2; i++) {
		// ���̖т̏d�͂�ύX
		_bone[i]->SetGravity("Character1_Spine", "Character1_Head");
	}
	for (auto&& bone : _bone) {
		bone->Process();
		bone->SetMain(bone->_massPosList);
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
		case C_P_SET_INVINCIBLE_CNT:
			ChangeIsInvincible(true, static_cast<int>(param));
			break;

		case C_P_ENABLE_IB_ATTACK_COLLISION:
			_ironBall->SetEnabledAttackCollision(static_cast<bool>(param));
			break;
		case C_P_ENABLE_IB_FOLLOWING_MODE:
		{
			IB_MOVE_STATE nextState = static_cast<int>(param) == 0 ? IB_MOVE_STATE::PUTTING_ON_SOCKET : IB_MOVE_STATE::FOLLOWING;
			_ironBall->SetMoveState(nextState);
			break;
		}
		case C_P_ENABLE_IB_INTERPOLATION:
			_ironBall->SetMoveState(IB_MOVE_STATE::INTERPOLATION);
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
	int y = 500;
	int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "HP:%d", _hp); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "isInvincible:%d", _isInvincible); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "invincibleCnt:%d", _invincibleRemainingCnt); line++;
	DrawFormatString(x, y + line * 16, COLOR_RED, "_rotationCnt:%d", _rotationCnt); line++;

	//DrawFormatString(x, y + line * 16, COLOR_RED, "ANIM:%d", _animStatus); line++;
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);


	_animManager->DrawDebugInfo();
	_ironBall->DrawDebugInfo();
}
