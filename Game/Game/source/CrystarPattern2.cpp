#include "CrystarPattern2.h"

int CrystarPattern2::_collisionFrame = -1;

CrystarPattern2::CrystarPattern2() :EnemyBase::EnemyBase() {

};

CrystarPattern2::~CrystarPattern2() {
	delete _frameData;
	delete _animManager;
	delete _roof;
};

void CrystarPattern2::InheritanceInit() {
	//�ʂŃZ�b�g�������
	_animState = ANIMSTATE::IDLE;
};

void CrystarPattern2::AnimInit() {

	_roof = NEW CrystarRoof(ResourceServer::MV1LoadModel("CrystarRoof","res/Enemy/Crystar/cg_crystar_roof.mv1"), _model);

	//// ���[�V�������X�g�̃��[�h
	MotionList::Load("Crystarl", "MotionList_Crystarl.csv");
	auto motionList = MotionList::GetMotionList("Crystarl");
	// �A�j���[�V�����}�l�[�W���[�̏�����
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Crystarl", _model, *motionList);
	// �t���[���f�[�^�̏�����
	_frameData = NEW FrameData();
	_frameData->LoadData("Crystarl", *motionList);

	if (_collisionFrame == -1) {
		_collisionFrame = MV1SearchFrame(_model, "Hip");
	}
}

void CrystarPattern2::CommandProcess() {
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

void CrystarPattern2::Init(VECTOR pos) {
	_IsUse = true;

	SetKindPos(pos);
	_hp = _maxHp;
	_knockBackSpeedFrame = 0;
	_gravity = 0;
	_animState = ANIMSTATE::IDLE;
	_modeState = ENEMYTYPE::SEARCH;
	_searchState = SEARCHTYPE::COOLTIME;
	_rotation = VGet(0, 0, 0);

	float randSize = (float)(rand() % 75) / 100 + 0.75;// 1 + 0.0 ~ 0.5
	MV1SetScale(_model, VScale(VGet(1.0f, 1.0f, 1.0f), 2.0f * randSize));

	_diffeToCenter = VGet(0, 20.0f * 2.0f * randSize, 0);
	_r = 25.0f * 2.0f * randSize;
	_weightExp = _weightExp * 2.0f * randSize;

	InheritanceInit();
};

bool CrystarPattern2::ModeSearch(bool plAttack) {
	switch (_searchState) {
	case SEARCHTYPE::MOVE:
		ModeSearchToMove();
		_animState = ANIMSTATE::RUN;
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
}

bool CrystarPattern2::ModeDisCover() {
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
		_animState = ANIMSTATE::HANDSTAND;
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 500, 0));
		_savePos = _pos;
	}
	return true;
};

bool CrystarPattern2::ModeAttack() {
	float attackTime = 54.0f / 60.0f * 1000; // �U�����[�V��������
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H
	float speed = 2 * -1; //���f���̕�����-z�����Ȃ̂�*-1
	if (nowTime > attackTime) { speed *= -1; _animState = ANIMSTATE::HANDBUTT; }

	VECTOR dirvec = Math::MatrixToVector(MGetRotY(_rotation.y), 2);
	_pos = VAdd(_pos, VScale(dirvec, speed));

	// �U���I������
	if (nowTime > attackTime * 2) {
		_modeState = ENEMYTYPE::COOLTIME;
		_animState = ANIMSTATE::IDLE;
	}

	return true;
};

bool CrystarPattern2::ModeCoolTime() {

	if (GetNowCount() - _currentTime >= _coolTime) {
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool CrystarPattern2::ModeKnockBack() {
	int nowTime = GetNowCount() - _currentTime;
	float CoolTime = 3.0f * 1000; //�d������
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	if (_knockBackSpeedFrame > 0) {
		_knockBackSpeedFrame--;
	}
	if (_knockBackSpeedFrame <= 0 && nowTime > CoolTime) {
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool CrystarPattern2::IndividualProcessing() {
	_roof->Updata();
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
}

bool CrystarPattern2::SetState() {
	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y , 0.0f));
		MV1SetPosition(_model, _pos);
	}
	return true;
};

bool CrystarPattern2::IndividualRendering() {
	_roof->Render();
	return true;
};

bool CrystarPattern2::DebugRender() {
	DrawSphere3D(MV1GetFramePosition(_model, _collisionFrame), _r, 8, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};