#include "CrystarPattern4.h"
CrystarPattern4::CrystarPattern4() :EnemyBase::EnemyBase() {

};

CrystarPattern4::~CrystarPattern4() {
	EnemyBase::~EnemyBase();
};

void CrystarPattern4::InheritanceInit() {

	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
	_nowAttackDistance = 0;
	_attackDistanceSpeed = 3;
};

bool CrystarPattern4::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	int rottime = 1.0f * 1000.0f;//�U���܂ł̉�]����
	int oneLoundTime = 0.7f * 1000.0f;//���E�̗h��ɑ΂��������鎞��
	int attackRange = 750;//�U���͈͂̔��a cm

	_attackDir -= Math::DegToRad(30.0f);//���t���[��30�x���₷

	if (nowTime >= rottime) {
		//1������O�ɔ��a�̐L�т� - �ɂȂ�ƃA���L���f�X�̖@������O���̂ŗ����ł͂Ȃ����ʂɔ�яo���`�ɂȂ�
		_nowAttackDistance += _attackDistanceSpeed;
		if (_nowAttackDistance <= 0 || attackRange <= _nowAttackDistance) {
			_attackDistanceSpeed = -_attackDistanceSpeed;
			if (_nowAttackDistance <= 0) {
				_modeState = ENEMYTYPE::COOLTIME;
				_currentTime = GetNowCount();
				_nowAttackDistance = 0;
			}
		}
	}
	_attackPos.z = _nowAttackDistance * sin(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime));
	_attackPos.x = _nowAttackDistance * cos(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime));

	return true;
};

bool CrystarPattern4::ModeCoolTime() {
	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 2.0f * 1000; //�U�����Ă���̃N�[���^�C��   
	_attackDir += Math::DegToRad(15.0f);//���t���[��30�x���₷
	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool CrystarPattern4::SetState() {
	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(_rotation.x, _rotation.y + _attackDir, 0.0f));
		MV1SetPosition(_model, VAdd(_pos, _attackPos));
	}
	return true;
};

bool CrystarPattern4::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	return true;
};