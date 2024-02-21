#include "SlaBlockPattern2.h"
SlaBlockPattern2::SlaBlockPattern2() :EnemyBase::EnemyBase()
{
	//�f�o�b�N���o�^
	//_model = ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1");
};

SlaBlockPattern2::~SlaBlockPattern2() {
	EnemyBase::~EnemyBase();
};

void SlaBlockPattern2::InheritanceInit() {
	_fallCount = 0;
};

bool SlaBlockPattern2::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	int enemyRigidityTime = 1.0f * 1000; //�U�����[�V�����ɓ����Ă̍d������
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//�G���v���C���[�̈ʒu�ɂ��܂ł̎���
	int fallTime = 1.0f * 1000;//��������܂ł̎���

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
		_pos.y -= _speed * 10;
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
			_saveNextPoint = VAdd(_player->GetPosition(), VGet(0, 500, 0));
			ScreenVibration::GetInstance()->SetVibration(0, 20*(_fallCount+1), 20);
			if (_fallCount > 2) {
				_fallCount = 0;
				_modeState = ENEMYTYPE::COOLTIME;
			}
		}
	}
	return true;
};

bool SlaBlockPattern2::ModeCoolTime() {

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 3.0f * 1000; //�U�����Ă���̃N�[���^�C��   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

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

bool SlaBlockPattern2::DebugRender() {
	DrawSphere3D(VAdd(_pos, _diffeToCenter), _r, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	return true;
}