#include "CrystarPattern3.h"

CrystarPattern3::CrystarPattern3() :EnemyBase::EnemyBase() {

};

CrystarPattern3::~CrystarPattern3() {
	EnemyBase::~EnemyBase();
};

void CrystarPattern3::InheritanceInit() {
	//�ʂŃZ�b�g�������
	_animState = ANIMSTATE::IDLE;
	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
};

void CrystarPattern3::AnimInit() {

	_roof = new CrystarRoof(ResourceServer::MV1LoadModel("CrystarRoof","res/Enemy/Crystar/cg_crystar_roof.mv1"), _model);

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

void CrystarPattern3::CommandProcess() {
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

void CrystarPattern3::Init(VECTOR pos) {
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
	MV1SetScale(_model, VScale(VGet(1.0f, 1.0f, 1.0f), randSize));

	_diffeToCenter = VGet(0, 20.0f * randSize, 0);
	_r = 25.0f * randSize;
	_weightExp = _weightExp * randSize;

	InheritanceInit();
};

bool CrystarPattern3::ModeSearch() {
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
			_modeState = ENEMYTYPE::ATTACK;//��Ԃ𔭌��ɂ���
			_animState = ANIMSTATE::HANDSTAND;
			_currentTime = GetNowCount();
			_stopTime = 0;
		}
	}

	return true;
}

bool CrystarPattern3::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

	// �U���J�n����2�b�ȓ��͓����Ȃ�
	if (nowTime <= 1.5f * 1000)return true;

	//���̖��@�v���C���[�̂ق�����Ɍ����̂ŋ}���Ɋp�x���ω������Ƃ��t�ʒu�Ɉړ�����

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	int rottime = 1.0f * 1000.0f;//�U���܂ł̉�]����
	int oneLoundTime = 3.0f * 1000.0f;//���E�̗h��ɑ΂��������鎞��
	int _rotAttackMaxDistance = 10.0f;//�U�����̉��Ɉړ�����ő勗��

	//VECTOR move = VSub(_player->GetPos(),VAdd( _pos, _attackPos)); move.y = 0.0f;//������I���ɂ����y���̈ړ����Ȃ��Ȃ�
	VECTOR move = VSub(_player->GetCollision().down_pos,_pos); move.y = 0.0f;//������I���ɂ����y���̈ړ����Ȃ��Ȃ�
	move = VNorm(move);
	move = VScale(move, _speed);
	_pos = VAdd(_pos, move);

	//�ړ������Ɍ�����ς���
	VECTOR dirVec = VScale(move, -1);//�����x�N�g�����烂�f���������������v�Z
	_rotation.y = atan2(dirVec.x, dirVec.z);

	_attackDir += Math::DegToRad(30.0f);//���t���[��30�x���₷

	MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
	VECTOR xDirVec = Math::MatrixToVector(matrix, 0);
	_attackPos = VScale(xDirVec, _rotAttackMaxDistance * sin(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime)));
	_pos = VAdd(_pos, _attackPos);
	//�G�ƃv���C���[�̋������Z�o
	//move = VSub(_player->GetPos(), VAdd(_pos, _attackPos));
	move = VSub(_player->GetCollision().down_pos, _pos);
	float p_distance = VSize(move);//�G�ƃv���C���[�̋���

	//���G����
	if (p_distance >= _discoverRangeSize) {
		_modeState = ENEMYTYPE::SEARCH;//��Ԃ����G�ɂ���
		_orignPos = _nextMovePoint = _pos;
		_attackDir = 0.0f;
		_attackPos = VGet(0, 0, 0);
	}

	return true;
};

bool CrystarPattern3::ModeCoolTime() {
	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 2.0f * 1000; //�U�����Ă���̃N�[���^�C��   
	
	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
	}
	return true;
};

bool CrystarPattern3::ModeKnockBack() {
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

bool CrystarPattern3::IndividualProcessing() {
	_roof->Updata();
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
}

bool CrystarPattern3::SetState() {
	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y + _attackDir, 0.0f));
		//MV1SetPosition(_model, VAdd(_pos, _attackPos));
		MV1SetPosition(_model, _pos);
	}
	return true;
};

bool CrystarPattern3::IndividualRendering() {
	_roof->Render();
	return true;
};

bool CrystarPattern3::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 8, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};