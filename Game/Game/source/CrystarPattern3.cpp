//----------------------------------------------------------------------
// @filename CrystarPattern3.h
// ��date: 2024/01/26
// ��author: saito ko
// @explanation
// ��ԋ����N���C�X�^�[�̃p�^�[��3(�S)�̃N���X ��]���Ȃ���ǔ��U�������܂��B
//----------------------------------------------------------------------
#include "CrystarPattern3.h"

int CrystarPattern3::_collisionFrame = -1;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
CrystarPattern3::CrystarPattern3() :EnemyBase::EnemyBase() {};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
CrystarPattern3::~CrystarPattern3() {
	delete _frameData;
	delete _animManager;
	for (int i = 0; i < 2; i++) {
		delete _roof[i];
	}
};
//----------------------------------------------------------------------
// @brief �G�l�~�[�̌ŗL�ϐ��̏�����
// @return �Ȃ�
//----------------------------------------------------------------------
void CrystarPattern3::InheritanceInit() {
	//�ʂŃZ�b�g�������
	_animState = ANIMSTATE::IDLE;
	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
};
//----------------------------------------------------------------------
// @brief �A�j���[�V�����}�l�[�W���[�ƃt���[���f�[�^�E�N���C�X�^�[�̉����̏�����
// @return �Ȃ�
//----------------------------------------------------------------------
void CrystarPattern3::AnimInit() {

	_roof[0] = NEW CrystarRoof(ResourceServer::MV1LoadModel("CrystarRoof_Glass", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Crystar_Roof_Iron.mv1"), _model,"joint1");
	_roof[1] = NEW CrystarRoof(ResourceServer::MV1LoadModel("CrystarRoof_Iron", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Helmet_Enemy_Crystar_Iron.mv1"), _model, "joint2");

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
//----------------------------------------------------------------------
// @brief �t���[���f�[�^�ł̃R�}���h����
// @return ����
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @brief ����������
// @param pos �ʒu
// @return ����
//----------------------------------------------------------------------
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
	MV1SetScale(_model, VScale(VGet(1.0f, 1.0f, 1.0f), 2.0f * randSize));

	_diffeToCenter = VGet(0, 20.0f * 2.0f * randSize, 0);
	_r = 25.0f * 2.0f * randSize;
	_weightExp = _weightExp * 2.0f * randSize;

	InheritanceInit();
};
//----------------------------------------------------------------------
// @brief �T�[�`��Ԃ̍X�V����
// ��param plAttack �v���C���[���U�����Ă��邩�ǂ���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::ModeSearch(bool plAttack) {
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
			_modeState = ENEMYTYPE::ATTACK;//��Ԃ𔭌��ɂ���
			_animState = ANIMSTATE::HANDSTAND;
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
			// �v���C���[�����E�ɓ������甭��
			if (range_dir <= _flontAngle) {
				_modeState = ENEMYTYPE::ATTACK;//��Ԃ𔭌��ɂ���
				_animState = ANIMSTATE::HANDSTAND;
				_currentTime = GetNowCount();
			}
		}
	}


	return true;
}
//----------------------------------------------------------------------
// @brief �U����Ԃ̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
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
		_animState = ANIMSTATE::IDLE;
		_orignPos = _nextMovePoint = _pos;
		_attackDir = 0.0f;
		_attackPos = VGet(0, 0, 0);
	}

	return true;
};
//----------------------------------------------------------------------
// @brief �N�[���^�C�����̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::ModeCoolTime() {
	
	if (GetNowCount() - _currentTime >= _coolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �m�b�N�o�b�N��Ԃ̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::ModeKnockBack() {
	int nowTime = GetNowCount() - _currentTime;
	float CoolTime = 3.0f * 1000; //�d������
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	if (_knockBackSpeedFrame > 0) {
		_knockBackSpeedFrame--;
	}
	if (_knockBackSpeedFrame <= 0 && nowTime > CoolTime) {
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief ���̃N���X�̌ŗL�̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::IndividualProcessing() {
	for(int i = 0; i < 2; i++){
	_roof[i]->Update();
	}
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
}
//----------------------------------------------------------------------
// @brief ���f���̍X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::SetState() {
	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y + _attackDir, 0.0f));
		MV1SetPosition(_model, _pos);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �ŗL�̕`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::IndividualRendering() {
	for (int i = 0; i < 2; i++) {
		_roof[i]->Render();
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �f�o�b�N�p�̕`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool CrystarPattern3::DebugRender() {
	DrawSphere3D(MV1GetFramePosition(_model, _collisionFrame), _r, 8, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};