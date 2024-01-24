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
			ScreenVibration::GetInstance()->SetVibration(0, 20, 20);
			_currentTime = GetNowCount();
			_state = ENEMYTYPE::COOLTIME;
		}
	}
	return true;
};

bool SlaBlock::ModeCoolTime() {

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 3.0f * 1000; //�U�����Ă���̃N�[���^�C��   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_state = ENEMYTYPE::DISCOVER;
	}
	return true;
};