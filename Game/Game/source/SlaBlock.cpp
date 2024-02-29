#include "SlaBlock.h"

SlaBlock::SlaBlock() :EnemyBase::EnemyBase()
{
	//�f�o�b�N���o�^
	//_model = ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1");
};

SlaBlock::~SlaBlock() {
	EnemyBase::~EnemyBase();
};

void SlaBlock::InheritanceInit() {
	_animState = ANIMSTATE::IDLE;
};

void SlaBlock::AnimInit() {
	//// ���[�V�������X�g�̃��[�h
	MotionList::Load("Slablock", "MotionList_Slablock.csv");
	auto motionList = MotionList::GetMotionList("Slablock");
	// �A�j���[�V�����}�l�[�W���[�̏�����
	_animManager = new AnimationManager();
	_animManager->InitMap("Slablock", _model, *motionList);
	// �t���[���f�[�^�̏�����
	_frameData = new FrameData();
	_frameData->LoadData("Slablock", *motionList);
}

void SlaBlock::CommandProcess() {
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

bool SlaBlock::ModeSearch() {
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
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_modeState = ENEMYTYPE::DISCOVER;//��Ԃ𔭌��ɂ���
			_sartchRange = _discoverRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
			_currentTime = 0;
		}
	}

	return true;
}

bool SlaBlock::ModeDisCover() {
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
	float p_distance = VSize(move);//�G�ƃv���C���[�̋���

	//���G����
	if (p_distance >= _sartchRange) {
		_animState = ANIMSTATE::IDLE;
		_modeState = ENEMYTYPE::SEARCH;//��Ԃ����G�ɂ���
		_sartchRange = _hearingRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
		_orignPos = _nextMovePoint = _pos;
	}

	//�U������
	if (p_distance <= _attackRangeSize) {
		_animState = ANIMSTATE::DROP;
		_modeState = ENEMYTYPE::ATTACK;//��Ԃ����G�ɂ���
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 200, 0));
		_savePos = _pos;
	}
	return true;
};

bool SlaBlock::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	int enemyRigidityTime = 1.0f * 1000; //�U�����[�V�����ɓ����Ă̍d������
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//�G���v���C���[�̈ʒu�ɂ��܂ł̎���
	int fallTime = 1.0f * 1000;//��������܂ł̎���

	// 1�b�҂��Ă���20�t���[���Ńv���C���[�̓���ɓ���
	if (enemyRigidityTime <= nowTime && nowTime < enemyRigidityTime + enemyToPlayerPosFrame) {
		_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
		_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
		_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
	}

	//�P�b�҂��Ă��痎��
	if (nowTime >= enemyRigidityTime + enemyToPlayerPosFrame + fallTime) {
		_pos.y -= _speed * 10;
		//�Ƃ肠����y��0�ɂȂ�܂ŗ���
		if (_pos.y <= 0.0f) {
			_pos.y = 0.0f;
			_easingFrame = 0;
			ScreenVibration::GetInstance()->SetVibration(0, 10, 10);
			_currentTime = GetNowCount();
			_animState = ANIMSTATE::STAN;
			_modeState = ENEMYTYPE::COOLTIME;
		}
	}
	return true;
};

bool SlaBlock::ModeCoolTime() {

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 3.0f * 1000; //�U�����Ă���̃N�[���^�C��   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool SlaBlock::ModeKnockBack() {
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	_knockBackSpeedFrame--;
	if (_knockBackSpeedFrame <= 0) {
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
}

bool SlaBlock::SetGravity() {
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

bool SlaBlock::IndividualProcessing(){
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
};

bool SlaBlock::IndividualRendering() {
	return true;
};