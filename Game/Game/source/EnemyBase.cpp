#include "EnemyBase.h"

const float EnemyBase::_flontAngle = Math::DegToRad(45);//�Ƃ肠�����S�L��������45�x

EnemyBase::EnemyBase(int model, VECTOR pos) {
	_player = nullptr;

	_pos = pos;

	_orignPos = _pos;
	_savePos = _pos;
	_nextMovePoint = _pos;

	_sartchRangeSize = 0.0f;
	_discoverRangeSize = 0.0f;

	_model = MV1DuplicateModel(model);
	_speed = 0;
	_sartchRange = 0.0f;
	_state = TYPE::search;
	_direction = 0.0f;

	_moveRange = 0.0f;

	_r = 0.0f;

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
			_state = TYPE::discover;//��Ԃ𔭌��ɂ���
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
		_state = TYPE::search;//��Ԃ����G�ɂ���
		_sartchRange = _sartchRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
		_orignPos = _nextMovePoint = _pos;
	}

	//�U������
	if (p_distance <= _attackRangeSize) {
		_state = TYPE::attack;//��Ԃ����G�ɂ���
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
	case TYPE::search:
		ModeSearch();
		break;
	case TYPE::discover:
		ModeDisCover();
		break;
	case TYPE::attack:
		ModeAttack();
		break;
	case TYPE::cooltime:
		ModeCoolTime();
		break;
	}

	SetState();

	return true;
};

bool EnemyBase::Render() {
	if (_model != 0) {
		MV1DrawModel(_model);

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
	}
	return true;
};