#include "Crystarl.h"

Crystarl::Crystarl() :EnemyBase::EnemyBase() {

};

Crystarl::~Crystarl() {
	EnemyBase::~EnemyBase();
};

void Crystarl::InheritanceInit() {
	//�ʂŃZ�b�g�������
	_player = Player::GetInstance();
	_r = 100.0f;
	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
};

bool Crystarl::ModeSearch() {
	//-------------------------------
	// ���̓G�̓T�[�`���[�h�̌�ǐՏ�Ԃɓ��炸�A�U���ɓ���
	//------------------------------
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

				VECTOR dirVec = VSub(_saveNextPoint, _pos);//�����x�N�g�����烂�f���������������v�Z
				dirVec = VNorm(dirVec);
				MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
				VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
				float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
				VECTOR arrow = VCross(ene_dir, dirVec);
				if (arrow.y < 0) {
					range_dir *= -1;
				}
				_nextDir = _rotation.y + range_dir;
				_oldDir = _rotation.y;
			}
			else {
				_easingFrame++;
				_rotation.y = Easing::Linear(_easingFrame, _oldDir, _nextDir, 60);
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

		MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_state = ENEMYTYPE::ATTACK;//��Ԃ𔭌��ɂ���
			_sartchRange = _discoverRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
			_currentTime = GetNowCount();
			_stopTime = 0;
		}
	}

	return true;
}

bool Crystarl::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

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
	if (p_distance >= _sartchRange) {
		_state = ENEMYTYPE::SEARCH;//��Ԃ����G�ɂ���
		_sartchRange = _hearingRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
	//	_orignPos = _nextMovePoint = VAdd(_pos, _attackPos);
		_orignPos = _nextMovePoint = _pos;
		_attackDir = 0.0f;
		_attackPos = VGet(0, 0, 0);
	}

	return true;
};

bool Crystarl::ModeCoolTime() {
	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 2.0f * 1000; //�U�����Ă���̃N�[���^�C��   
	
	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_state = ENEMYTYPE::ATTACK;
	}
	return true;
};

bool Crystarl::ModeKnockBack() {
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	_knockBackSpeedFrame--;
	if (_knockBackSpeedFrame <= 0) {
		_state = ENEMYTYPE::ATTACK;
	}
	return true;
};

bool Crystarl::SetState() {
	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y + _attackDir, 0.0f));
		//MV1SetPosition(_model, VAdd(_pos, _attackPos));
		MV1SetPosition(_model, _pos);
	}
	return true;
};

bool Crystarl::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 32, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};