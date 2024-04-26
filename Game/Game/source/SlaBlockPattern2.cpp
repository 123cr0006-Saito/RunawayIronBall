//----------------------------------------------------------------------
// @filename SlaBlockPattern2.cpp
// @date: 2024/01/26
// @author: saito ko
// @explanation
// �X���u���b�N�̍U���p�^�[��2(�K���X,�S)�̃N���X 1�x�����v���C���[�̐^�ォ�痎���U�����s���܂�
//----------------------------------------------------------------------
#include "SlaBlockPattern2.h"

int SlaBlockPattern2::_collisionFrame = -1;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return ����
//----------------------------------------------------------------------
SlaBlockPattern2::SlaBlockPattern2() :EnemyBase::EnemyBase(){};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
SlaBlockPattern2::~SlaBlockPattern2() {
	delete _animManager;
	delete _frameData;
};
//----------------------------------------------------------------------
// @brief �G�l�~�[�̌ŗL�ϐ��̏�����
// @return �������Ă��邩
//----------------------------------------------------------------------
void SlaBlockPattern2::InheritanceInit() {
	_fallCount = 0;
};
//----------------------------------------------------------------------
// @brief �A�j���[�V�����E�t���[���f�[�^�̏�����
// @return ����
//----------------------------------------------------------------------
void SlaBlockPattern2::AnimInit() {
	//// ���[�V�������X�g�̃��[�h
	MotionList::Load("Slablock", "MotionList_Slablock.csv");
	auto motionList = MotionList::GetMotionList("Slablock");
	// �A�j���[�V�����}�l�[�W���[�̏�����
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Slablock", _model, *motionList);
	// �t���[���f�[�^�̏�����
	_frameData = NEW FrameData();
	_frameData->LoadData("Slablock", *motionList);

	if (_collisionFrame == -1) {
		_collisionFrame = MV1SearchFrame(_model, "face1");
	}
}
//----------------------------------------------------------------------
// @brief  �t���[���f�[�^�ł̃R�}���h����
// @return ����
//----------------------------------------------------------------------
void SlaBlockPattern2::CommandProcess() {
	std::vector<CommandParam> commandParam = _frameData->GetCommandData();

	for (auto itr = commandParam.begin(); itr != commandParam.end(); ++itr) {
		// �R�}���h���擾
		int command = itr->first;
		// �p�����[�^���擾
		float param = itr->second;

		switch (command) {
		case EN_MOTION_CHANGE:
			_animState = static_cast<ANIMSTATE>(param);
			break;
		}
	}
};
//----------------------------------------------------------------------
// @brief  �T�[�`��Ԃ̏���
// @param plAttack : �v���C���[���U�����Ă��邩�ǂ���
// @return �������Ă��邩�ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeSearch(bool plAttack){
	switch (_searchState) {
	case SEARCHTYPE::MOVE:
		ModeSearchToMove();
		_animState = ANIMSTATE::WALK;
		break;
	case SEARCHTYPE::TURN:
		ModeSearchToTurn();
		_animState = ANIMSTATE::WALK;
		break;
	case SEARCHTYPE::COOLTIME:
		ModeSearchToCoolTime();
		_animState = ANIMSTATE::IDLE;
		break;
	}

		//���G����
	VECTOR dirVec = VSub(_player->GetCollision().down_pos, _pos);
	float length = VSquareSize(dirVec);
	if (plAttack) {
		// �v���C���[���U�����͒��o�͈͂ŒT��
		if (length <= _hearingRangeSize * _hearingRangeSize) {
			_modeState = ENEMYTYPE::DISCOVER;//��Ԃ𔭌��ɂ���
			_currentTime = GetNowCount();
		}
	}
	else {
		// �v���C���[���U�����Ă��Ȃ��Ƃ��͎��E�ł̌���
		if (length <= _searchRange * _searchRange) {

			MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
			VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
			VECTOR pla_dir = VNorm(dirVec);
			float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

			if (range_dir <= _flontAngle) {
				_modeState = ENEMYTYPE::DISCOVER;//��Ԃ𔭌��ɂ���
				_currentTime = GetNowCount();
			}
		}
	}


	return true;
};
//----------------------------------------------------------------------
// @brief �ǐՏ�Ԃ̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeDisCover() {
	//�ړ�����
	VECTOR move = VSub(_player->GetCollision().down_pos, _pos); move.y = 0.0f;//������I���ɂ����y���̈ړ����Ȃ��Ȃ�
	move = VNorm(move);
	move = VScale(move, _speed);
	_pos = VAdd(_pos, move);

	//�ړ������Ɍ�����ς���
	VECTOR dirVec = VScale(move, -1);//�����x�N�g�����烂�f���������������v�Z
	_rotation.y = atan2(dirVec.x, dirVec.z);

	//�G�ƃv���C���[�̋������Z�o
	move = VSub(_player->GetCollision().down_pos, _pos);
	float pl_distance = VSquareSize(move);//�G�ƃv���C���[�̋���

	//���G����
	if (pl_distance >= _searchRange * _searchRange) {
		_modeState = ENEMYTYPE::SEARCH;//��Ԃ����G�ɂ���
		_animState = ANIMSTATE::IDLE;
		_orignPos = _nextMovePoint = _pos;
	}

	//�U������
	if (pl_distance <= _attackRangeSize * _attackRangeSize) {
		_modeState = ENEMYTYPE::ATTACK;//��Ԃ����G�ɂ���
		_animState = ANIMSTATE::DROP;
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 500, 0));
		_savePos = _pos;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �U����Ԃ̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	int enemyRigidityTime = 1.0f * 1000; //�U�����[�V�����ɓ����Ă̍d������
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//�G���v���C���[�̈ʒu�ɂ��܂ł̎���
	int fallTime = 1.0f * 1000;//��������܂ł̎���
	float fallSpeed = 30.0f;

	// 1�b�҂��Ă���20�t���[���Ńv���C���[�̓���ɓ���
	if (_fallCount == 0) {
		if (enemyRigidityTime <= nowTime && nowTime < enemyRigidityTime + enemyToPlayerPosFrame) {
			_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
			_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
			_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
		}
	}
	else {
		fallTime /= 2;
		enemyRigidityTime /= 2;
		if (enemyRigidityTime <= nowTime && nowTime <= enemyRigidityTime + enemyToPlayerPosFrame) {
			_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
			_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
			_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
		}
	}

	//�P�b�҂��Ă��痎��
	if (nowTime >= enemyRigidityTime + enemyToPlayerPosFrame + fallTime) {
		_pos.y -= fallSpeed;
		//�Ƃ肠����y��0�ɂȂ�܂ŗ���
		if (_pos.y <= 0.0f) {
			_fallCount++;
			_pos.y = 0.0f;
			_easingFrame = 0;
			_currentTime = GetNowCount();
			VECTOR dirVec = VSub(_pos, _player->GetPosition());
			dirVec = VNorm(dirVec);
			_rotation.y = atan2(dirVec.x, dirVec.z);
			_savePos = _pos;
			_saveNextPoint = VAdd(_player->GetPosition(), VGet(0, 200, 0));
			_animState = ANIMSTATE::DROP;
			ScreenVibration::GetInstance()->SetVibration(0, 5*(_fallCount+1), 10);
			if (_fallCount > 2) {
				_fallCount = 0;
				_animState = ANIMSTATE::STAN;
				_modeState = ENEMYTYPE::COOLTIME;
			}
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �N�[���^�C�����̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeCoolTime() {

	if (GetNowCount() - _currentTime >= _coolTime) {
		_currentTime = 0;
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �m�b�N�o�b�N��Ԃ̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeKnockBack() {
	int nowTime = GetNowCount() - _currentTime;
	float CoolTime = 3.0f * 1000; //�d������
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	if (_knockBackSpeedFrame > 0) {
		_knockBackSpeedFrame--;
	}
	if (_knockBackSpeedFrame <= 0 && nowTime > CoolTime) {
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �d�͂̐ݒ�
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::SetGravity() {
	//�d�͏���
	if (_modeState != ENEMYTYPE::ATTACK) {
		if (_pos.y > 0) {
			_gravity++;
			_pos.y -= _gravity;
			if (_pos.y < 0) {
				_gravity = 0;
				_pos.y = 0;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------
// @brief ���̃N���X�̌ŗL�̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::IndividualProcessing(){
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
};
//----------------------------------------------------------------------
// @brief ���̃N���X�̌ŗL�̕`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::IndividualRendering() {
	return true;
};
//----------------------------------------------------------------------
// @brief �f�o�b�O�p�̕`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool SlaBlockPattern2::DebugRender() {
	DrawSphere3D(MV1GetFramePosition(_model, _collisionFrame), _r, 8, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	return true;
}