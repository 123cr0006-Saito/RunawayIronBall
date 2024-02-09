#include "Player.h"
#include "appframe.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

Player::Player(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_input = XInput::GetInstance();

	_canMove = true;
	_speed = 8.0f;
	UpdateCollision();

	_ibFollowingMode = true;
	_isAttackState = false;

	_playNextComboAnim = true;
	_nextComboAnim = STATUS::NONE;

	// �A�j���[�V�����A�^�b�`�͂���Ă��Ȃ�
	_attach_index = -1;
	// �X�e�[�^�X���u�����v�ɐݒ�
	_animStatus = STATUS::NONE;
	// �Đ����Ԃ̏�����
	_total_time = 0.f;
	_play_time = 0.0f;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	SetBone();

	_isSwinging = false;
	_isSpinning = false;
	_spinCnt = 0;


	_instance = this;

	// �A�j���[�V�����}�l�[�W���[�̏����ݒ�
	_animManager = new AnimationManager();
	_animManager->InitMap("Player", _modelHandle, "MotionList_Player.csv");
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

}

Player::~Player()
{
	_instance = nullptr;
	delete _animManager;
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
	// �����O�̃X�e�[�^�X��ۑ����Ă���
	STATUS oldStatus = _animStatus;
	//_animStatus = STATUS::NONE;



	// ��]�U��
	if (_spinCnt > 90) {
		_animStatus = STATUS::SPIN_SWING;
		_isSpinning = true;
		_canMove = true;
	}
	// �ʏ�U��
	else if (_input->GetRel(XINPUT_BUTTON_X) != 0) {
		if (_nextComboAnim != STATUS::NONE) {
			_playNextComboAnim = true;

			_isSwinging = true;
			_canMove = false;
		}
	}

	if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
		_spinCnt++;
		//_canMove = false;
	}
	else {
		_spinCnt = 0;
		_isSpinning = false;
	}


	if (!_isSwinging && !_isSpinning) {
		_animStatus = STATUS::WAIT;


		_canMove = true;

		_ibFollowingMode = true;
		_isAttackState = false;
	}
	else {
		_ibFollowingMode = false;
		_isAttackState = true;
	}

	if (_playNextComboAnim) {
		if (_animStatus == STATUS::HORISONTAL_SWING_01 || _animStatus == STATUS::HORISONTAL_SWING_02) {
			if (_play_time >= 94.0f) {
				_animStatus = _nextComboAnim;
				_playNextComboAnim = false;
			}
		}
		else {
			_animStatus = _nextComboAnim;
			_playNextComboAnim = false;
		}
		
	}

	bool _isMoved = false;
	// ���X�e�B�b�N�̓��͏����擾����
	auto lStick = _input->GetAdjustedStick_L();
	VECTOR vDir = VGet(lStick.x, 0, lStick.y);
	if (_canMove) {
		// ���X�e�B�b�N�̓��͂���������
		if (VSize(vDir) > 0.000000f) {
			// �ړ�����
			vDir = VNorm(vDir);

			MATRIX mRot = MGetRotY(camAngleY);
			// �ړ������x�N�g������]������
			vDir = VTransform(vDir, mRot);
			_pos = VAdd(_pos, VScale(vDir, _speed));

			_isMoved = true;

			
		}
	}

	if (!_isAttackState) {
		if (_isMoved) {
			_animStatus = STATUS::RUN;

			// �L�����N�^�[�����炩�ɉ�]������
			float angle = Math::CalcVectorAngle(_forwardDir, vDir);
			float rotRad = (2.0f * DX_PI_F) / 30.0f;
			if (rotRad > angle) {
				_forwardDir = vDir;
			}
			else {
				VECTOR vN = VCross(_forwardDir, vDir);
				_forwardDir = VTransform(_forwardDir, MGetRotAxis(vN, rotRad));
			}
		}
		else {
			_animStatus = STATUS::WAIT;
		}
	}

	if (_isSpinning) {
		_forwardDir = VTransform(_forwardDir, MGetRotY((2.0f * DX_PI_F) / 30.0f));
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
	//AnimProcess(oldStatus);
	_animManager->Process(static_cast<int>(_animStatus));
	UpdateNextComboAnim();
	return true;
}

bool Player::AnimProcess(STATUS oldStatus)
{
	if (oldStatus == _animStatus) {
		// �Đ����Ԃ�i�߂�
		_play_time += 1.0f;
	}
	else {
		// �A�j���[�V�������A�^�b�`����Ă�����A�f�^�b�`����
		if (_attach_index != -1) {
			MV1DetachAnim(_modelHandle, _attach_index);
			_attach_index = -1;
		}
		// �X�e�[�^�X�ɍ��킹�ăA�j���[�V�����̃A�^�b�`
		switch (_animStatus) {

		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_modelHandle, 0, -1, FALSE);
			break;
		case STATUS::RUN:
			_attach_index = MV1AttachAnim(_modelHandle, 1, -1, FALSE);
			break;

		case STATUS::HORISONTAL_SWING_01:
			_attach_index = MV1AttachAnim(_modelHandle, 2, -1, FALSE);
			break;
		case STATUS::HORISONTAL_SWING_02:
			_attach_index = MV1AttachAnim(_modelHandle, 3, -1, FALSE);
			break;
		case STATUS::SPIN_SWING:
			_attach_index = MV1AttachAnim(_modelHandle, 4, -1, FALSE);
			break;

		}
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_total_time = MV1GetAttachAnimTotalTime(_modelHandle, _attach_index);
		// �Đ����Ԃ�������
		_play_time = 0.0f;
	}

	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (_play_time >= _total_time) {
		_play_time = 0.0f;

		if (_animStatus == STATUS::HORISONTAL_SWING_01 || _animStatus == STATUS::HORISONTAL_SWING_02) {
			_isSwinging = false;
		}
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(_modelHandle, _attach_index, _play_time);
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

	DrawDebugInfo();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = _pos;
	_capsuleCollision.r = 35.0f;
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

bool Player::UpdateNextComboAnim()
{
	switch (_animStatus)
	{
	case STATUS::HORISONTAL_SWING_01:
		_nextComboAnim = STATUS::HORISONTAL_SWING_02;
		break;
	case STATUS::HORISONTAL_SWING_02:
		_nextComboAnim = STATUS::NONE;
		break;

	default:
		_nextComboAnim = STATUS::HORISONTAL_SWING_01;
		break;
	}
	return true;
}

VECTOR Player::GetRightHandPos()
{
	VECTOR handPos = VGet(0.0f, 0.0f, 0.0f);
	MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, _rightHandFrameIndex);
	handPos = VTransform(handPos, m);
	return handPos;
}

void Player::DrawDebugInfo()
{
	for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
		_bone[i]->DebugRender();
	}
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);
}
