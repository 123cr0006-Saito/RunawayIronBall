#include "EnemyBase.h"

EnemyBase::EnemyBase() {
	_player = nullptr;

	_stopTime = 0.0f;
	_currentTime = 0;

	_nextDir = 0.0f;
	_oldDir = 0.0f;

	_easingFrame = 0;
	_saveNextPoint = VGet(0, 0, 0);
};

EnemyBase::~EnemyBase() {
	MV1DeleteModel(_model);
};

bool EnemyBase::Create(int model, VECTOR pos, EnemyParam param) {
	_model = model;

	Init(pos);
	DebugSnail();
	//Param------------------
	_hp = param._hp;
	_exp = param._exp;
	_speed = param._speed;
	_coolTime = param._coolTime;

	_flontAngle = param._flontAngle;
	_hearingRangeSize = param._hearingRangeSize;
	_moveRange = param._moveRange;
	_sartchRange = param._sartchRange;
	_discoverRangeSize = param._discoverRangeSize;
	_attackRangeSize = param._attackRangeSize;
	return true;
};

void EnemyBase::Init(VECTOR pos, float scale) {
	//�X�P�[���l�͖���
};

void EnemyBase::Init(VECTOR pos) {
	_IsUse = true;

	SetPos(pos);
	InheritanceInit();

	_state = ENEMYTYPE::SEARCH;
	_direction = 0.0f;
};

void EnemyBase::InheritanceInit() {

};

void  EnemyBase::DebugSnail() {
	//���̃��f���ɓ\��t���Ă���e�N�X�`��
	MV1SetTextureGraphHandle(_model, 0, ResourceServer::LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, ResourceServer::LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true);
	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//�����Ă������f�����傫����������1/10�ɐݒ�

	//---------------------------------------------
	//�ʂŐݒ�ł���悤�ɂ���
	_diffeToCenter = VGet(0, 125, 0);
	_r = 150.0f;
	//--------------------------------------------

};

void EnemyBase::SetPos(VECTOR pos) {
	_pos = pos;
	_orignPos = pos;
	_savePos = pos;
	_nextMovePoint = pos;
};


bool EnemyBase::StopPos() {
	if (_pos.x >= _nextMovePoint.x - _speed && _pos.x <= _nextMovePoint.x + _speed &&
		_pos.y >= _nextMovePoint.y - _speed && _pos.y <= _nextMovePoint.y + _speed &&
		_pos.z >= _nextMovePoint.z - _speed && _pos.z <= _nextMovePoint.z + _speed)
	{
		return true;
	}
	return false;
};

bool EnemyBase::ModeSearch() {
	//�ړ������̐ݒ�
	if (StopPos()) {
		if (_stopTime == 0) {
			_stopTime = (float)(rand() % 200) / 100.0f + 2.0f;//1�b����3�b�܂Ŏ~�܂�@�����_�Q���܂ł̃����_��
			_currentTime = GetNowCount();
		}

		if ((float)(GetNowCount() - _currentTime) / 1000 >= _stopTime) {
			if (_nextDir == 0.0f) {

				VECTOR vArrow = VGet((float)(rand() % 20 / 10.0f) - 1.0f, 1.0f, (float)(rand() % 20 / 10.0f) - 1.0f);//�����_���ȕ����x�N�g�������
				vArrow = VScale(vArrow, rand() % (int)_moveRange); vArrow.y = 0.0f;//��_����̔��a���������_���Ŋ|���A���ɐi�ރ|�C���g�����߂�
				_saveNextPoint = VAdd(vArrow, _orignPos);//��_�ɕ��s�ړ�

				//atan2�^�C�v
				//VECTOR dirVec = VSub(_nextMovePoint, _pos);//�����x�N�g�����烂�f���������������v�Z
				//_direction = atan2(dirVec.x, dirVec.z) + 180 * 3.14 / 180;//-���Ȃ�������Rad��180�𑫂��Ă���

				//�t�H���[�h�x�N�g���^�C�v
				//VECTOR dirVec = VSub(_nextMovePoint, _pos);//�����x�N�g�����烂�f���������������v�Z
				//dirVec = VNorm(dirVec);
				//MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
				//VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
				//float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
				//VECTOR arrow = VCross(ene_dir, dirVec);
				//if (arrow.y < 0) {
				//	range_dir *= -1;
				//}
				//_direction = _direction + range_dir;

				VECTOR dirVec = VSub(_saveNextPoint, _pos);//�����x�N�g�����烂�f���������������v�Z
				dirVec = VNorm(dirVec);
				MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
				VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
				float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
				VECTOR arrow = VCross(ene_dir, dirVec);
				if (arrow.y < 0) {
					range_dir *= -1;
				}
				_nextDir = _direction + range_dir;
				_oldDir = _direction;
			}
			else {
				_easingFrame++;
				_direction = Easing::Linear(_easingFrame, _oldDir, _nextDir, 60);
				if (_easingFrame >= 60) {
					_easingFrame = 0;
					_nextDir = 0.0f;
					_stopTime = 0;//���Ԃ̏�����
					_nextMovePoint = _saveNextPoint;
				}
			}
		}
	}
	else {
		//�ړ�����
		VECTOR move = VSub(_nextMovePoint, _pos);
		move = VNorm(move);
		move = VScale(move, _speed);
		_pos = VAdd(_pos, move);
	}

	//���G����
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_state = ENEMYTYPE::DISCOVER;//��Ԃ𔭌��ɂ���
			_sartchRange = _discoverRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
			_currentTime = 0;
		}
	}

	return true;
};

bool EnemyBase::ModeDisCover() {
	//�ړ�����
	VECTOR move = VSub(_player->GetCollision().down_pos, _pos); move.y = 0.0f;//������I���ɂ����y���̈ړ����Ȃ��Ȃ�
	move = VNorm(move);
	move = VScale(move, _speed);
	_pos = VAdd(_pos, move);

	//�ړ������Ɍ�����ς���
	VECTOR dirVec = VScale(move, -1);//�����x�N�g�����烂�f���������������v�Z
	_direction = atan2(dirVec.x, dirVec.z);

	//�G�ƃv���C���[�̋������Z�o
	move = VSub(_player->GetCollision().down_pos, _pos);
	float p_distance = VSize(move);//�G�ƃv���C���[�̋���

	//���G����
	if (p_distance >= _sartchRange) {
		_state = ENEMYTYPE::SEARCH;//��Ԃ����G�ɂ���
		_sartchRange = _hearingRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
		_orignPos = _nextMovePoint = _pos;
	}

	//�U������
	if (p_distance <= _attackRangeSize) {
		_state = ENEMYTYPE::ATTACK;//��Ԃ����G�ɂ���
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 500, 0));
		_savePos = _pos;
	}
	return true;
};

bool EnemyBase::ModeAttack() {
	return true;
};

bool EnemyBase::ModeCoolTime() {
	return true;
};

bool EnemyBase::SetState() {
	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _direction, 0.0f));
		MV1SetPosition(_model, _pos);
	}
	return true;
};

bool EnemyBase::Process() {

	switch (_state) {
	case ENEMYTYPE::SEARCH:
		ModeSearch();
		break;
	case ENEMYTYPE::DISCOVER:
		ModeDisCover();
		break;
	case ENEMYTYPE::ATTACK:
		ModeAttack();
		break;
	case ENEMYTYPE::COOLTIME:
		ModeCoolTime();
		break;
	}

	SetState();

	return true;
};

bool  EnemyBase::DebugRender() {
	DrawSphere3D(VAdd(_pos, _diffeToCenter), _r, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), false);

	//�f�o�b�O�p
	//���G�͈͂Ȃǂ̕`��
	// MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
	// VECTOR now_dir = VScale(Math::MatrixToVector(matrix, 2), -1);//�t�H���[�h�x�N�g��
	////------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////���E
	//DrawLine3D(_pos, VAdd(_pos, VScale(now_dir, 3000)),GetColor(255,0,0));
	//DrawLine3D(_pos, VAdd(_pos,VTransform( VScale(now_dir, 3000),MGetRotY(45*3.14/180))), GetColor(0, 255, 0));
	//DrawLine3D(_pos, VAdd(_pos, VTransform(VScale(now_dir, 3000), MGetRotY(-45 * 3.14 / 180))), GetColor(0, 255, 0));
	////------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////���o
	//TDD::ThrDimColOfCircleDraw(_pos, 2000, 0, false);
	////-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////�U�����̍��G�͈�
	//TDD::ThrDimColOfCircleDraw(_pos, 10000, 0, false);
	////-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	return true;
};

bool EnemyBase::Render() {
	if (_model != 0) {
		DebugRender();
		MV1DrawModel(_model);
	}
	return true;
};

