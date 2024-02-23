#include "CrystarPattern2.h"

CrystarPattern2::CrystarPattern2() :EnemyBase::EnemyBase() {

};

CrystarPattern2::~CrystarPattern2() {
	EnemyBase::~EnemyBase();
};

void CrystarPattern2::InheritanceInit() {
	//�ʂŃZ�b�g�������
	_animState = ANIMSTATE::IDLE;
};

void CrystarPattern2::AnimInit() {

	_roof = new CrystarRoof(ResourceServer::MV1LoadModel("res/Enemy/Crystar/cg_crystar_roof.mv1"), _model);

	//// ���[�V�������X�g�̃��[�h
	MotionList::Load("Crystarl", "MotionList_Crystarl.csv");
	auto motionList = MotionList::GetMotionList("Crystarl");
	// �A�j���[�V�����}�l�[�W���[�̏�����
	_animManager = new AnimationManager();
	_animManager->InitMap("Crystarl", _model, *motionList);
	// �t���[���f�[�^�̏�����
	_frameData = new FrameData();
	_frameData->LoadData("Crystarl", *motionList);

}

void CrystarPattern2::Init(VECTOR pos) {
	_IsUse = true;

	SetPos(pos);
	_hp = _maxHp;
	_knockBackSpeedFrame = 0;
	_gravity = 0;
	_animState = ANIMSTATE::IDLE;
	_modeState = ENEMYTYPE::SEARCH;
	_searchState = SEARCHTYPE::COOLTIME;
	_rotation = VGet(0, 0, 0);

	float randSize = (float)(rand() % 75) / 100 + 0.75;// 1 + 0.0 ~ 0.5
	MV1SetScale(_model, VScale(VGet(1.0f, 1.0f, 1.0f), randSize));

	_diffeToCenter = VGet(0, 20.0f * randSize, 0);
	_r = 25.0f * randSize;
	_weightExp = _weightExp * randSize;

	InheritanceInit();
};

bool CrystarPattern2::ModeSearch() {
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
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_modeState = ENEMYTYPE::DISCOVER;//��Ԃ𔭌��ɂ���
			_animState = ANIMSTATE::WALK;
			_currentTime = GetNowCount();
			_stopTime = 0;
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
	float p_distance = VSize(move);//�G�ƃv���C���[�̋���

	//���G����
	if (p_distance >= _sartchRange) {
		_modeState = ENEMYTYPE::SEARCH;//��Ԃ����G�ɂ���
		_sartchRange = _hearingRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
		_orignPos = _nextMovePoint = _pos;
	}

	//�U������
	if (p_distance <= _attackRangeSize) {
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
	int speed = 5;
	if (nowTime > attackTime) { speed *= -1; }

	VECTOR dirvec = Math::MatrixToVector(MGetRotY(_rotation.y), 1);
	_pos = VAdd(_pos, VScale(dirvec, speed));

	return true;
};

bool CrystarPattern2::ModeCoolTime() {
	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 2.0f * 1000; //�U�����Ă���̃N�[���^�C��   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
	}
	return true;
};

bool CrystarPattern2::ModeKnockBack() {
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	_knockBackSpeedFrame--;
	if (_knockBackSpeedFrame <= 0) {
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
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
	DrawSphere3D(VAdd(_pos, _diffeToCenter), _r, 32, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};